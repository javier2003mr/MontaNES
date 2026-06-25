# MontaNES

MontaNES is a NES (Nintendo Entertainment System) emulator for the Haiku operating system.

## Building

The build method is very simple: just run `make` in the project root directory.  
Make sure you have the basic Haiku development tools installed (g++, make, and xres).

```bash
make
```

## Compatibility

Currently, MontaNES supports running ROMs that use the following mappers:

- **NROM** (mapper 0)
- **UxROM** (mapper 2)
- **CNROM** (mapper 3)

Support for additional mappers is planned for future releases.

Please note that the emulator currently only supports **single‑player** gameplay. Two‑player (second controller) input is not yet implemented.

