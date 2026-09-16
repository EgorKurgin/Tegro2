# Tegro2

**Tegro** is a tool for analyzing the **chemical kinetics of conjugated chemical reactions** under diffusion control. It accounts for multi-particle interactions using advanced theoretical frameworks.

**Tegro2** is a cross-platform fork of the original Tegro program, completely rewritten in modern C++ using the Qt framework. It preserves the theoretical foundation and computational core of the original, while enhancing its architecture, interface, and functionality.

**Original work:** https://gitlab.com/krissinel/tegro.git

## Core Methodologies
Tegro implements the following theories to describe interactions:
* **ET**: Encounter Theory
* **MET**: Modified Encounter Theory
* **CMET**: Complete Modified Encounter Theory
* 
## Status

🔧 **Active development.** Currently migrating from the legacy
code base (1997–2026) to a modern C++23 implementation.

## Building

Requirements:
- MSYS2 with UCRT64 toolchain (Windows) / GCC ≥ 13 / Clang ≥ 17
- CMake ≥ 3.24
- Ninja

```bash
pacman -S mingw-w64-ucrt-x86_64-{gcc,cmake,ninja,git}

git clone https://github.com/EgorKurgin.git
cd tegro
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## Usage Terms & License
Using this program is subject to compliance with the **CC BY-NC 4.0** (Attribution-NonCommercial) license.

### Academic Attribution
If you use Tegro in your research, please cite the following publication:

> P.A. Frantsuzov, O.A. Igoshin, E.B. Krissinel (2000), *Chem.Phys.Lett.* 317, 481-489.
> [https://doi.org/10.1016/S0009-2614(99)01440-2](https://doi.org/10.1016/S0009-2614(99)01440-2)

---
*(C) E.B. Krissinel, P.A. Frantsuzov, O.A. Igoshin, 1997-2026; E.B. Kurgin, 2026*

