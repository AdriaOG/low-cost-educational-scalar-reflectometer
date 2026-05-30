# Low-Cost Educational Scalar Reflectometer for Microwave Teaching

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![CERN-OHL](https://img.shields.io/badge/License-CERN%20OHL%20v2.0-blue.svg)](https://cern.ch/ohl)

**Low-cost PCB-based scalar reflectometer (50 MHz–1 GHz) for hands-on teaching of reflection coefficient measurement, antenna impedance matching and RF hardware in microwave engineering courses.**

This repository contains all the materials required to reproduce the educational platform presented in the paper:

> A. Ortiz, J. Navarro, J.L. Pijoan, and J. Anguera, “A Low-Cost Educational Reflectometer for Microwave Teaching,” *IEEE Transactions on Education*, 2026.

## Overview
The platform implements a complete **scalar reflectometer** on a single two-layer PCB using off-the-shelf components. It exposes the full measurement chain (directional coupler, passive envelope detectors, configurable analog conditioning, and embedded microcontroller) so that students can understand how |S₁₁| is obtained from RF signals in a transparent and modular way.

Designed specifically for undergraduate and graduate courses in microwave engineering, the system supports the complete 10-stage laboratory sequence described in the paper.

## Key Features
- Broadband directional coupler (directivity 8–15 dB)
- Passive Schottky envelope detectors with three configurable analog modes: passive, unity-gain buffer, and fixed-gain amplification
- Low-cost microcontroller with real-time |S₁₁| computation and serial streaming
- Full pedagogical transparency of the entire signal path
- 10-stage progressive laboratory sequence (theoretical foundations → PCB design → fabrication → automated measurements → open-ended projects)
- Multidisciplinary integration: microwave theory, analog electronics, embedded systems, and antennas
- Reproducible with standard two-layer PCB fabrication and widely available components

## Repository Contents

The repository is organized as follows:

```bash
low-cost-educational-scalar-reflectometer/
├── bom/               # Bill of Materials (Excel + CSV)
├── firmware/          # Microcontroller source code (C)
├── gerbers/           # Fabrication files (Gerber X2)
├── hardware/          # KiCad schematic + PCB layout (v1.0)
├── software/          # Python/MATLAB scripts for automated sweeps and post-processing
└── README.md
```
## Repository Contents
- Fabricate the PCB using the provided Gerber files (standard 2-layer process).
- Assemble the board following the BOM.
- Flash the firmware and connect to a PC via USB.
- Follow the 10-stage laboratory sequence (adaptable to different course lengths).

## Citation
If you use this platform or any of its materials in your teaching or research, please cite the original paper:
```bibtex
bibtex@article{ortiz2026reflectometer,
  author  = {A. Ortiz and J. Navarro and J.L. Pijoan and J. Anguera},
  title   = {A Low-Cost Educational Reflectometer for Microwave Teaching},
  journal = {IEEE Transactions on Education},
  year    = {2026},
  doi     = {10.1109/TE.2026.xxxxxxx}
}
