#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <MessageRunner.h>
#include <LayoutBuilder.h>
#include <Alert.h>
#include <stdio.h>
#include <stdlib.h>
#include <map> // For std::map
#include <app/AppDefs.h>             // ADDED: Provides B_KEY_* constants
#include <interface/InterfaceDefs.h> // Provides key_info struct and get_key_info function

// Emulator core headers
#include "NES.h"
#include "PPU.hpp"
#include "Cartridge.hpp"
#include "CPU.h"

// --- Emulator View ---
// This view is responsible for drawing the main game screen.
class EmulatorView : public BView {
public:
    EmulatorView(PPU* ppu)
        : BView("emulator_view", B_WILL_DRAW), fPPU(ppu) {
        fBitmap = new BBitmap(BRect(0, 0, 255, 239), B_RGB32, true);
    }

    ~EmulatorView() {
        delete fBitmap;
    }

    virtual void Draw(BRect updateRect) {
        if (!fPPU) return;

        uint32_t* nesFrameBuffer = (uint32_t*)fPPU->getFrameBuffer();
        if (nesFrameBuffer) {
            memcpy(fBitmap->Bits(), nesFrameBuffer, fBitmap->BitsLength());
        }
        DrawBitmap(fBitmap, Bounds());
    }

private:
    PPU*      fPPU;
    BBitmap*  fBitmap;
};

// --- Pattern Table View ---
// This view is responsible for drawing the PPU's pattern tables.
class PatternTableView : public BView {
public:
    PatternTableView(PPU* ppu, Cartridge* cart)
        : BView("pattern_table_view", B_WILL_DRAW), fPPU(ppu), fCartridge(cart) {
        fBitmap = new BBitmap(BRect(0, 0, 255, 127), B_RGB32, true);
    }

    ~PatternTableView() {
        delete fBitmap;
    }

    virtual void Draw(BRect updateRect) {
        if (!fPPU || !fCartridge) return;
        
        DrawPatternTable(0, 0, 0);
        DrawPatternTable(1, 128, 0);
        DrawBitmap(fBitmap, Bounds());
    }

private:
    void DrawPatternTable(int table, int startX, int startY) {
        unsigned char* chr_rom = fCartridge->getCHRROM();
        if (!chr_rom) return;

        uint32_t* bits = (uint32_t*)fBitmap->Bits();
        uint32_t bpr = fBitmap->BytesPerRow() / 4;

        // Define a simple, fixed grayscale palette for visualization
        uint32_t palette[4] = {
            0xFF000000, // Black
            0xFF555555, // Dark Gray
            0xFFAAAAAA, // Light Gray
            0xFFFFFFFF  // White
        };

        for (int tileY = 0; tileY < 16; ++tileY) {
            for (int tileX = 0; tileX < 16; ++tileX) {
                int tileIndex = tileY * 16 + tileX;
                int tileOffset = (table * 4096) + (tileIndex * 16);

                for (int y = 0; y < 8; ++y) {
                    unsigned char low = chr_rom[tileOffset + y];
                    unsigned char high = chr_rom[tileOffset + y + 8];

                    for (int x = 0; x < 8; ++x) {
                        int bit0 = (low >> (7 - x)) & 1;
                        int bit1 = (high >> (7 - x)) & 1;
                        int colorIndex = (bit1 << 1) | bit0;
                        
                        // Use the fixed grayscale palette
                        uint32_t color = palette[colorIndex];

                        int pixelX = startX + tileX * 8 + x;
                        int pixelY = startY + tileY * 8 + y;
                        if (pixelX < 256 && pixelY < 128) {
                           bits[pixelY * bpr + pixelX] = color;
                        }
                    }
                }
            }
        }
    }

    PPU*        fPPU;
    Cartridge*  fCartridge;
    BBitmap*    fBitmap;
};


// --- Main Emulator Window ---
class EmulatorWindow : public BWindow {
public:
    EmulatorWindow(PPU* ppu)
        : BWindow(BRect(100, 100, 100 + 256 * 3, 100 + 240 * 3), "NES Emulator", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS) {
        fEmulatorView = new EmulatorView(ppu);
        BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
            .Add(fEmulatorView)
            .End();
    }

    void FrameRendered() {
        if (Lock()) {
            fEmulatorView->Invalidate();
            Unlock();
        }
    }
private:
    EmulatorView* fEmulatorView;
};

// --- Pattern Table Window ---
class PatternTableWindow : public BWindow {
public:
    PatternTableWindow(PPU* ppu, Cartridge* cart)
        : BWindow(BRect(650, 100, 650 + 512, 100 + 256), "Pattern Tables", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS) {
        fPatternView = new PatternTableView(ppu, cart);
        BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
            .Add(fPatternView)
            .End();
    }

    void FrameRendered() {
        if (Lock()) {
            fPatternView->Invalidate();
            Unlock();
        }
    }
private:
    PatternTableView* fPatternView;
};


// --- Haiku Application Class ---
class EmulatorApp : public BApplication {
public:
    EmulatorApp() : BApplication("application/x-vnd.nes-emulator") {
        // 1. Initialize Emulator using the NES class
        fNES = new NES();
        // IMPORTANT: Change this path if your .nes file is elsewhere.
        if (!fNES->loadCartridge("nestest.nes")) {
            BAlert* alert = new BAlert("Error", "Failed to load cartridge.\nMake sure 'marioBros.nes' is in the same directory as the executable.", "OK");
            alert->Go();
            be_app->PostMessage(B_QUIT_REQUESTED);
            return;
        }

        // 2. Create Windows, passing components from the NES object
        fEmulatorWindow = new EmulatorWindow(fNES->getPPU());
        fPatternTableWindow = new PatternTableWindow(fNES->getPPU(), fNES->getCartridge());

        fEmulatorWindow->Show();
        fPatternTableWindow->Show();

        // 3. Set up the emulator loop
        BMessage tickMsg(kClockTick);
        fRunner = new BMessageRunner(BMessenger(this), &tickMsg, 1000000/60); // ~60 FPS
    }

    ~EmulatorApp() {
        delete fRunner;
        delete fNES; // This will delete cpu, ppu, and cartridge
    }

    virtual void MessageReceived(BMessage* message) {
        switch (message->what) {
            case kClockTick:
                _ClockTick();
                break;
            default:
                BApplication::MessageReceived(message);
                break;
        }
    }

private:
    void _ClockTick() {
        if (fNES && fNES->isCartridgeLoaded()) {
            // Map of Haiku Raw Key Codes to NES buttons.
            // The key codes are defined in <interface/InterfaceDefs.h>
            // and are used by Haiku's key polling API.
            // NES buttons: A, B, Select, Start, Up, Down, Left, Right
            static const std::map<uint32_t, int> nes_keymap = {
                //{'a', 0}, // NES A
                //{'s', 1}, // NES B
                //{'p', 2}, // NES Select
                //{B_ENTER, 3}, // NES Start
                //{B_UP_ARROW, 4}, // NES Up
                //{B_DOWN_ARROW, 5}, // NES Down
                //{B_LEFT_ARROW, 6}, // NES Left
                //{B_RIGHT_ARROW, 7} // NES Right
                {0x3c, 0}, // NES A
                {0x3d, 1}, // NES B
                {0x30, 2}, // NES Select
                {0x47, 3}, // NES Start
                {0x57, 4}, // NES Up
                {0x62, 5}, // NES Down
                {0x61, 6}, // NES Left
                {0x63, 7} // NES Right
            };
            
            key_info keyInfo;
            get_key_info(&keyInfo); // Get the current state of all keys
            
            bool nes_buttons[8] = {false}; // Initialize all to false
            
            // Check our mapped keys
            for (auto const& [haikuKey, nesButtonIndex] : nes_keymap) {
                // Haiku stores key states in a bitmask.
                // The expression checks if the bit corresponding to the key code is set.
                if (keyInfo.key_states[haikuKey / 8] & (1 << (7 - (haikuKey % 8)))) {
                    nes_buttons[nesButtonIndex] = true;
                }
            }

            // Load the button states into the Joypad
            fNES->getJoypad()->loadButtons(
                nes_buttons[0], nes_buttons[1], nes_buttons[2], nes_buttons[3],
                nes_buttons[4], nes_buttons[5], nes_buttons[6], nes_buttons[7]
            );

            // The NES class now handles the details of running a frame.
            fNES->runFrame();

            // After a frame is run, tell the windows to redraw.
            fEmulatorWindow->FrameRendered();
            fPatternTableWindow->FrameRendered();
        }
    }

    static const uint32_t kClockTick = 'clck';

    NES*                fNES;
    EmulatorWindow*     fEmulatorWindow;
    PatternTableWindow* fPatternTableWindow;
    BMessageRunner*     fRunner;
};

// --- Main Entry Point ---
int main() {
    EmulatorApp app;
    app.Run();
    return 0;
}
