#ifndef NES_H
#define NES_H

#include <string>
#include <memory>
#include "Joypad.hpp" // Include Joypad header

// Forward-declare the classes to avoid circular dependencies
// and to keep the header clean.
class CPU;
class PPU;
class Cartridge;

class NES {
public:
    NES();
    ~NES();

    // Load a .nes file into the emulator.
    // Returns true on success, false on failure.
    bool loadCartridge(const std::string& path);

    // Reset the emulator to its power-on state.
    void reset();

    // Execute enough cycles to render a single video frame.
    void runFrame();

    // Accessors for the GUI or other external tools to interact with.
    PPU* getPPU() const;
    CPU* getCPU() const;
    Cartridge* getCartridge() const;
    Joypad* getJoypad() const; // New accessor for Joypad

    bool isCartridgeLoaded() const;

private:
    //std::unique_ptr<Cartridge> cartridge;
    //std::unique_ptr<CPU> cpu;
    //std::unique_ptr<PPU> ppu;

    Cartridge * cartridge;
    CPU * cpu;
    PPU * ppu;
    Joypad * joypad; // Add Joypad member

    bool cartridgeLoaded = false;
};

#endif // NES_H
