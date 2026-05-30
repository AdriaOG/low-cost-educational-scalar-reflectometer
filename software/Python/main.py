import pyvisa
import serial
import time
import csv
import re
import pandas as pd

#ADJUST THE CONFIGURATION ACCORDING TO YOUR NEEDS
RIGOL_IP = "192.168.1.100"
VISA_BACKEND = "@py"

SERIAL_PORT = "COM3"
SERIAL_BAUDRATE = 9600

START_FREQ_HZ = 50e6
STOP_FREQ_HZ = 2.1e9
STEP_HZ = 10e6

#POWERS_DBM = list(range(0, 21, 2))   # 0, 2, 4, ..., 20 dBm
POWERS_DBM = list(range(0, 21, 5))   # -30,-28,-60 ... 0, 2, 4, ..., 20 dBm
#POWERS_DBM = list(range(0, 21, 10))   # -30,-28,-60 ... 0, 2, 4, ..., 20 dBm

def transform_csv_to_excel(input_csv, output_xlsx):
    # 1. Read CSV
    df = pd.read_csv(input_csv)

    # 2. Convert frequency from Hz to MHz
    df["freq_Hz"] = df["freq_Hz"].astype(float) / 1_000_000

    # 3. Multiply RA0 and RA1 by the factor (5/1024)
    factor = 5 / 1024
    df["RA0"] = df["RA0"].astype(float) * factor
    df["RA1"] = df["RA1"].astype(float) * factor

    # 4. rename Columns
    df.rename(columns={
        "freq_Hz": "f",
        "RA0": "m1p_coupled",
        "RA1": "m1p_isolated"
    }, inplace=True)

    # 5. Save as Excel
    df.to_excel(output_xlsx, index=False)
    print(f"File generated correctly: {output_xlsx}")





def read_current_values(ser):
    """
    The PIC sends several messages. We only want the following block:

        Current values:
        -> RA0: XXXX <-
        -> RA1: YYYY <-

    Strategy:
      - Read line by line until finding: "Current values:"
      - Read the next two lines
      - Extract XXXX and YYYY
    """

    while True:
        linea = ser.readline().decode(errors="ignore").strip()

        if "->" in linea:
            l_ra0 = ser.readline().decode(errors="ignore").strip()
            l_ra1 = ser.readline().decode(errors="ignore").strip()

            m0 = re.search(r"RA0:\s*(\d+)", l_ra0)
            m1 = re.search(r"RA1:\s*(\d+)", l_ra1)

            if not m0 or not m1:
                print("[WARNING] Could not parse RA0/RA1:")
                print(repr(l_ra0))
                print(repr(l_ra1))
                return None

            ra0 = int(m0.group(1))
            ra1 = int(m1.group(1))

            return ra0, ra1
        # If it doesn't match, we continue reading (we ignore short messages)
    # END while


def power_sweep(gen, ser, power_dbm):
    """
    Performs a complete frequency sweep for a power level, returning a list of tuples (freq, RA0, RA1).
    """

    results = []

    print(f"\n========== POWER: {power_dbm} dBm ==========")
    gen.write(f"POW {power_dbm}dBm")
    time.sleep(0.2)
    """
    Time changed. Original-> 0.1
    """

    print("Confirmed on generator:", gen.query("POW?").strip())

    freq = START_FREQ_HZ
    while freq <= STOP_FREQ_HZ:

        # 1) Configuring frequency of the generator
        gen.write(f"FREQ {freq}Hz")

        # 2) Send trigger to PIC
        ser.reset_input_buffer()
        ser.write(b"R")
        ser.flush()

        # 3) read complete block from the PIC
        medida = read_current_values(ser)

        if medida is None:
            print(f"[WARNING] Invalid measure at {freq/1e6:.1f} MHz")
            freq += STEP_HZ
            continue

        ra0, ra1 = medida
        results.append((freq, ra0, ra1))
        print(f"OK @ {freq/1e6:.1f} MHz -> RA0={ra0}, RA1={ra1}")

        # 4) we wait for the PIC to process its 10 samples
        time.sleep(0.2)

        # Next frequency
        freq += STEP_HZ

    return results


def main():
    # ------------------------
    # connection to the RF generator
    # ------------------------
    rm = pyvisa.ResourceManager(VISA_BACKEND)
    gen = rm.open_resource(
        f"TCPIP::{RIGOL_IP}::INSTR",
        timeout=5000,
        write_termination='\n',
        read_termination='\n'
    )

    print("IDN generator:", gen.query("*IDN?").strip())

    # ------------------------
    # Serial connection to the micro
    # ------------------------
    ser = serial.Serial(
        port=SERIAL_PORT,
        baudrate=SERIAL_BAUDRATE,
        timeout=None
    )

    print(f"Serial port open: {SERIAL_PORT} @ {SERIAL_BAUDRATE} bps")

    # ------------------------
    # Sweeping for every power
    # ------------------------
    try:
        gen.write("OUTP ON")

        for power in POWERS_DBM:

            results = power_sweep(gen, ser, power)

            # Guardar CSV correspondiente
            filename = f"measures_{power}dBm.csv"

            with open(filename, "w", newline="") as f:
                wr = csv.writer(f)
                wr.writerow(["freq_Hz", "RA0", "RA1"])
                wr.writerows(results)

            print(f">>> Saved: {filename}")
            print(f"    Total samples: {len(results)}")

    finally:
        try:
            gen.write("OUTP OFF")
        except:
            pass

        ser.close()
        gen.close()
        rm.close()
print("\n>>> PROCESS COMPLETED FOR ALL POWER LEVELS <<<")


if __name__ == "__main__":
    main()
