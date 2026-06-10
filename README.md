# KRONOS — Keccak RISC-V Optimized eNgine fOr haShing

## Overview

Integration methodology can significantly affect the performance of dedicated hardware accelerators. This work explores this aspect using **Keccak** — a pivotal hashing standard in Post-Quantum Cryptography (PQC) — as a case study.

KRONOS presents three integration variants of the Keccak accelerator on the open-source **32-bit X-HEEP** RISC-V SoC:

| Variant | Branch | Interface | Description |
|---------|--------|-----------|-------------|
| Loosely coupled | `loosely` | Memory-mapped AXI peripheral | Accelerator accessed via load/store instructions |
| Tightly coupled | `tightly` | CV-X-IF (with extra register file) | Keccak state stored in a dedicated register file alongside the core |
| Coprocessor (ISE) | `coprocessor` | CV-X-IF (no extra register file) | Instruction Set Extension, Keccak state lives in the main register file |

Results show that the **tightly coupled** approach is the most efficient integration method, achieving the best balance between resource consumption and throughput.

The reference publication for this work is:

> Valeria Piscopo, Alessandra Dolmeta, Mattia Mirigaldi, Maurizio Martina, and Guido Masera. 2025. **A Deep Dive into Integration Methodologies in RISC-V.** In *Proceedings of the 22nd ACM International Conference on Computing Frontiers: Workshops and Special Sessions (CF '25 Companion)*. Association for Computing Machinery, New York, NY, USA, 30–33. https://doi.org/10.1145/3706594.3726969

---

## Getting Started

Clone the repository and run the initial setup:

```bash
make mcu-gen
make x_heep-sync
make questasim-sim
```

Then build and run an application. The Makefile targets follow this pattern, where `ACC` selects the accelerator variant and `VERSION` selects the implementation version:

```bash
# Build the application
make app-$(ACC)-LWC-ASCON-$(VERSION) SCHEME=LWC ALG=ASCON VERSION=$(VERSION) ACC=$(ACC)

# Run the simulation
make run-$(ACC)-LWC-ASCON-$(VERSION) SCHEME=LWC ALG=ASCON VERSION=$(VERSION) ACC=$(ACC)
```

For example, to run the loosely coupled variant:

```bash
make app-loosely-LWC-ASCON-v1 SCHEME=LWC ALG=ASCON VERSION=v1 ACC=loosely
make run-loosely-LWC-ASCON-v1 SCHEME=LWC ALG=ASCON VERSION=v1 ACC=loosely
```

---

## Authors

- Alessandra Dolmeta
- Valeria Piscopo

---

## License

This project is licensed under the Apache License 2.0.

---

## Acknowledgement

This work is supported by the EU TRISTAN project with GA 101095947, which has received funding from the Chips Joint Undertaking and its members.
