# KRONOS

## Overview

The integration methodology can significantly affect the performance of dedicated accelerators. This work undertakes an exploration of this aspect, considering Keccak, a pivotal hashing standard in Post-Quantum Cryptography (PQC), as a case of study. The paper presents three versions of KRONOS (Keccak RISC-V Optimized eNgine fOr haShing): a loosely-coupled memory-mapped accelerator, a tightly-coupled approach, and an Instruction Set Extension (ISE). The latter two versions leverage the CV-X-IF interface, with and without, respectively, an additional register file to store the Keccak state. Results show that the tightly approach is the most efficient integration method, achieving a balance between resource consumption and throughput.

## Branches

- loosely
- tightly
- coprocessor



## Getting started

Once you have cloned the repository:
```
make mcu-gen
make x_heep-sync
make questasim-sim
```

Then, depending on the applications you want to run, you need to do:
```
make app-optimized-KECCAK-SHA3-384 SCHEME=SHA3-384 ALG=KECCAK ACC=optimized
make run-optimized-KECCAK-SHA3-384 SCHEME=SHA3-384 ALG=KECCAK ACC=optimized
```
```
make app-original-KECCAK-SHA3-384 SCHEME=SHA3-384 ALG=KECCAK ACC=original
make run-original-KECCAK-SHA3-384 SCHEME=SHA3-384 ALG=KECCAK ACC=original
```
