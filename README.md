# Low-Cost Educational Scalar Reflectometer for Microwave Teaching

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.xxxxxxx.svg)](https://doi.org/10.5281/zenodo.xxxxxxx)  
*(Reemplaza el DOI una vez que lo generes en Zenodo)*

## Overview
This repository contains all the materials required to reproduce the **low-cost, PCB-based scalar reflectometer** presented in the paper:

> A. Ortiz, J. Navarro, J.L. Pijoan, and J. Anguera, “A Low-Cost Educational Reflectometer for Microwave Teaching,” *IEEE Transactions on Education*, 2026.

The platform enables hands-on teaching of reflection coefficient measurement (|S₁₁|), antenna impedance matching, directional coupler operation, square-law detection, analog signal conditioning, and embedded RF systems in the frequency range **50 MHz to 1 GHz** (sub-GHz IoT band).

Designed for undergraduate and graduate courses in microwave engineering, the system is fully transparent, modular, and reproducible using standard two-layer PCB fabrication and off-the-shelf components.

## Key Features
- Broadband directional coupler (8–15 dB directivity)
- Passive Schottky envelope detectors with configurable analog conditioning (passive / unity-gain buffer / fixed-gain amplification)
- Low-cost microcontroller with real-time |S₁₁| computation
- Complete measurement chain exposed for pedagogical purposes
- 10-stage progressive laboratory sequence (theoretical foundations → PCB design → fabrication → automated measurements → open-ended projects)
- Multidisciplinary integration: RF/microwave, analog electronics, embedded systems, and antennas

## Repository Contents
├── hardware/          # KiCad schematic + PCB layout (v1.0)
├── gerbers/           # Fabrication files (Gerber X2)
├── bom/               # Bill of Materials (Excel + CSV)
├── firmware/          # Microcontroller source code (C)
├── software/          # Python/MATLAB scripts for automated sweeps and post-processing
├── lab-guides/        # Ten-stage laboratory sequence
├── figures/           # High-resolution figures from the paper
└── README.md
text## How to Use
1. Fabricate the PCB using the provided Gerber files (standard 2-layer process).
2. Assemble the board following the BOM.
3. Flash the firmware and connect to a PC via USB.
4. Follow the 10-stage laboratory sequence (adaptable to different course lengths).

Detailed step-by-step instructions are available in `lab-guides/`.

## Citation
If you use this platform or any of its materials in your teaching or research, please cite the original paper:

```bibtex
@article{ortiz2026reflectometer,
  author  = {A. Ortiz and J. Navarro and J.L. Pijoan and J. Anguera},
  title   = {A Low-Cost Educational Reflectometer for Microwave Teaching},
  journal = {IEEE Transactions on Education},
  year    = {2026},
  doi     = {10.1109/TE.2026.xxxxxxx}
}
