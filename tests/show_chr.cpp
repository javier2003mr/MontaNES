#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <stdio.h>
#include "../include/Cartridge.hpp"
#include "../include/PaletteColors.hpp"

class CHRView : public BView {
public:
    CHRView(BRect frame, Cartridge* cartridge)
        : BView(frame, "chr_view", B_FOLLOW_ALL, B_WILL_DRAW)
    {
        fCartridge = cartridge;
    }

    virtual void Draw(BRect updateRect)
    {
        if (fCartridge == NULL)
            return;

        unsigned char* chr_rom = fCartridge->getCHRROM();
        if (chr_rom == NULL)
            return;

        // Each tile is 8x8 pixels, and each pixel is 2 bits.
        // A tile is 16 bytes.
        int tile_x = 0;
        int tile_y = 0;

        for (int i = 0; i < 512; i++) {
            tile_x = i % 32;
            tile_y = i / 32;
            unsigned char* tile = &chr_rom[i * 16];
            for (int y = 0; y < 8; y++) {
                unsigned char low_byte = tile[y];
                unsigned char high_byte = tile[y + 8];
                for (int x = 0; x < 8; x++) {
                    int bit0 = (low_byte >> (7 - x)) & 1;
                    int bit1 = (high_byte >> (7 - x)) & 1;
                    int palette_index = (bit1 << 1) | bit0;

                    rgb_color color;
                    if (palette_index == 0) {
                        color = {0, 0, 0, 255}; // Black
                    } else if (palette_index == 1) {
                        color = {85, 85, 85, 255}; // Dark Gray
                    } else if (palette_index == 2) {
                        color = {170, 170, 170, 255}; // Light Gray
                    } else {
                        color = {255, 255, 255, 255}; // White
                    }
                    SetHighColor(color);

                    FillRect(BRect(tile_x * 8 * 3 + x * 3, tile_y * 8 * 3 + y * 3, tile_x * 8 * 3 + x * 3 + 2, tile_y * 8 * 3 + y * 3 + 2));
                }
            }
            tile_x++;
            if (tile_x >= 32) {
                tile_x = 0;
                tile_y++;
            }
        }
    }

private:
    Cartridge* fCartridge;
};

class CHRWindow : public BWindow {
public:
    CHRWindow(Cartridge* cartridge)
        : BWindow(BRect(100, 100, 100 + 768, 100 + 384), "CHR ROM", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
    {
        fCHRView = new CHRView(Bounds(), cartridge);
        AddChild(fCHRView);
    }

    virtual bool QuitRequested()
    {
        be_app->PostMessage(B_QUIT_REQUESTED);
        return true;
    }

private:
    CHRView* fCHRView;
};

class CHRApplication : public BApplication {
public:
    CHRApplication(char* signature, char* nes_file_path)
        : BApplication(signature)
    {
        fCartridge = new Cartridge();
        fCartridge->loadROM(nes_file_path);
        fWindow = new CHRWindow(fCartridge);
        fWindow->Show();
    }

    ~CHRApplication()
    {
        delete fCartridge;
    }

private:
    CHRWindow* fWindow;
    Cartridge* fCartridge;
};

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: %s <nes_file>\n", argv[0]);
        return 1;
    }

    char signature[100];
    sprintf(signature, "application/x-vnd.nes-chr-viewer-%s", argv[1]);

    CHRApplication app(signature, argv[1]);
    app.Run();

    return 0;
}
