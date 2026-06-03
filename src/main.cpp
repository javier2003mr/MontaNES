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
#include <interface/InterfaceDefs.h> // Provides key_info struct and get_key_info function
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <FilePanel.h>
#include <Path.h>
#include <fstream>
#include <string>
#include <Catalog.h>
#include <Locale.h>

// Emulator core headers
#include "NES.h"
#include "PPU.hpp"
#include "Cartridge.hpp"
#include "CPU.h"

#include "KeyConfig.hpp"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MontaNES"

// Message constants
const uint32_t kMsgFileOpen = 'flop';
const uint32_t kMsgKeyConfOpen = 'kopn';
const uint32_t kMsgPttrnOpen = 'popn';

using namespace std;

bool previousTabState;
bool fastForward;



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

    void SetPPU(PPU* ppu) {
        fPPU = ppu;
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

    void SetPPU(PPU* ppu) {
        fPPU = ppu;
    }

    void SetCartridge(Cartridge* cart) {
        fCartridge = cart;
    }

private:
    void DrawPatternTable(int table, int startX, int startY) {
        
        unsigned char * chr_rom = fPPU->getVRAM();
        
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

// --- Pattern Table Window ---
class PatternTableWindow : public BWindow {
public:
    PatternTableWindow(PPU* ppu, Cartridge* cart)
        : BWindow(BRect(650, 100, 650 + 512, 100 + 256), B_TRANSLATE("Pattern Tables"), B_TITLED_WINDOW, B_NOT_CLOSABLE | B_ASYNCHRONOUS_CONTROLS) {
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

    //void Quit(){
    //    Hide();
    //}

    void SetPPU(PPU* ppu) {
        fPatternView->SetPPU(ppu);
    }

    void SetCartridge(Cartridge* cart) {
        fPatternView->SetCartridge(cart);
    }
private:
    PatternTableView* fPatternView;
};

// --- Main Emulator Window ---
class EmulatorWindow : public BWindow {
public:
    EmulatorWindow(PPU* ppu)
        : BWindow(BRect(100, 100, 100 + 256 * 3, 100 + 240 * 3), B_TRANSLATE("NES Emulator"), B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS) {
        
        fEmulatorView = new EmulatorView(ppu);

        // Create Menu Bar
        BMenuBar* menuBar = new BMenuBar("menuBar");

        BMenu* fileMenu = new BMenu(B_TRANSLATE("File"));
        fileMenu->AddItem(new BMenuItem(B_TRANSLATE("Open"), new BMessage(kMsgFileOpen), 'O'));

        BMenu* keysMenu = new BMenu(B_TRANSLATE("Config"));
        keysMenu->AddItem(new BMenuItem(B_TRANSLATE("Gamepad"), new BMessage(kMsgKeyConfOpen)));

        BMenu* pttrnMenu = new BMenu(B_TRANSLATE("Pattern Window"));
        pttrnMenu->AddItem(new BMenuItem(B_TRANSLATE("Show"), new BMessage(kMsgPttrnOpen)));

        menuBar->AddItem(fileMenu);
        menuBar->AddItem(keysMenu);
        menuBar->AddItem(pttrnMenu);

        BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
            .Add(menuBar)
            .Add(fEmulatorView)
            .End();

        fFilePanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(be_app));
        
        //BRect frame(100, 100, 420, 500);
        //fKeyConfWindow = new KeyConfWindow(frame);
    }

    ~EmulatorWindow() {
        if (fFilePanel) delete fFilePanel;
    }

    virtual void MessageReceived(BMessage* message) {
        switch (message->what) {
            case kMsgFileOpen:
                fFilePanel->Show();
                break;
            case kMsgKeyConfOpen:
                keyConfWindow = new KeyConfWindow();
                keyConfWindow->Show();
                keyConfWindow->SetWorkspaces(B_CURRENT_WORKSPACE);
                keyConfWindow->Activate();                
                break;
            case kMsgPttrnOpen:
                fPatternWindow->Show();
                break;
            default:
                BWindow::MessageReceived(message);
                break;
        }
    }

    virtual bool QuitRequested() {
        be_app->PostMessage(B_QUIT_REQUESTED);
        return true;
    }

    void FrameRendered() {
        if (Lock()) {
            fEmulatorView->Invalidate();
            Unlock();
        }
    }

    void SetPPU(PPU* ppu) {
        fEmulatorView->SetPPU(ppu);
    }

    void SetPatternWindow(PatternTableWindow * f){
        fPatternWindow = f;
    }

private:
    EmulatorView* fEmulatorView;
    BFilePanel*   fFilePanel;
    KeyConfWindow* keyConfWindow;
    PatternTableWindow* fPatternWindow;
};

// --- Haiku Application Class ---
class EmulatorApp : public BApplication {
public:
    EmulatorApp() : BApplication("application/x-vnd.Haiku-montaNES") {
        // 1. Initialize Emulator to nullptr
        fNES = nullptr;

        // 2. Create Windows with nullptr components
        fEmulatorWindow = new EmulatorWindow(nullptr);
        fPatternTableWindow = new PatternTableWindow(nullptr, nullptr);
        fEmulatorWindow->SetPatternWindow(fPatternTableWindow);

        fEmulatorWindow->Show();
        fPatternTableWindow->Show();

        // 3. Set up the emulator loop
        BMessage tickMsg(kClockTick);
        fRunner = new BMessageRunner(BMessenger(this), &tickMsg, 1000000 / 60); // ~60 FPS
        previousTabState = false;
        fastForward = false;
    }

    ~EmulatorApp() {
        delete fRunner;
        if (fNES) {
            delete fNES;
        }
    }

    virtual void RefsReceived(BMessage* message) {
        entry_ref ref;
        if (message->FindRef("refs", &ref) == B_OK) {
            BEntry entry(&ref, true);
            BPath path;
            entry.GetPath(&path);
            LoadNewRom(path.Path());
        }
    }

    virtual void MessageReceived(BMessage* message) {
        switch (message->what) {
            case kClockTick:
                _ClockTick();
                break;
            case kMsgFileOpen:
                // This is handled by the window, but we could add logic here if needed
                break;
            default:
                BApplication::MessageReceived(message);
                break;
        }
    }

    void LoadNewRom(const std::string& romPath) {
        if (fNES) {
            delete fNES;
            fNES = nullptr;
        }

        fNES = new NES();
        if (!fNES->loadCartridge(romPath)) {
            std::string errorMsg = "Failed to load cartridge.\nMake sure '" + romPath + "' is a valid .nes file.";
            BAlert* alert = new BAlert("Error", errorMsg.c_str(), "OK");
            alert->Go();
            delete fNES;
            fNES = nullptr;
            return;
        }

        // Update windows with new PPU and Cartridge
        fEmulatorWindow->SetPPU(fNES->getPPU());
        fPatternTableWindow->SetPPU(fNES->getPPU());
        fPatternTableWindow->SetCartridge(fNES->getCartridge());
    }

private:
    void _ClockTick() {
        if (fNES && fNES->isCartridgeLoaded()) {
            // Map of Haiku Raw Key Codes to NES buttons.
            // The key codes are defined in <interface/InterfaceDefs.h>
            // and are used by Haiku's key polling API.
            // NES buttons: A, B, Select, Start, Up, Down, Left, Right
            const std::map<uint32_t, int> nes_keymap = {
                {keys[0], 0}, // NES A ------> A key
                {keys[1], 1}, // NES B ------> S key
                {keys[2], 2}, // NES Select -> P key
                {keys[3], 3}, // NES Start --> Enter key
                {keys[4], 4}, // NES Up -----> Up row
                {keys[5], 5}, // NES Down ---> Down row
                {keys[6], 6}, // NES Left ---> Left row
                {keys[7], 7}, // NES Right ---> Right row
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

            // --- Fast Forward Toggle ---
            uint32_t tabKey = 0x26;
            bool tabState = (keyInfo.key_states[tabKey / 8] & (1 << (7 - (tabKey % 8))));

            if (tabState != previousTabState && tabState){
                if (fastForward){
                    fRunner->SetInterval(1000000 / 60);
                }else{
                    fRunner->SetInterval(1000000 / 120);
                }
                fastForward = !fastForward;
            }
            previousTabState = tabState;

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
    bool                fFastForward;
    bool                fTabKeyState;
};

// --- Main Entry Point ---
int main(int argc, char* argv[]) {

    const char * c = "./keyconfig";
    loadKeys(c);
    
    EmulatorApp app;

    if (argc == 2) {
        app.LoadNewRom(argv[1]);
    }

    app.Run();
    return 0;
}
