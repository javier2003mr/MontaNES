#ifndef KEYCONFIG
#define KEYCONFIG

#include <Window.h>
#include <View.h>
#include <Button.h>
#include <FilePanel.h>
#include <MessageRunner.h>
#include <Message.h>
#include <Application.h>
#include <LayoutBuilder.h>
#include <cstdio>
#include <Catalog.h>
#include <Locale.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "KeyNES"
#define MAX_PATH_LENGTH 10000

extern bool capturing;
extern int kindex;
extern unsigned char keys[8];

void loadKeys(const char * path);
void storeKeys(const char * path);
int CaptureKey(void * p);
BString getWorkingDirectory();

// Constantes para los mensajes que usarán los botones
const uint32_t MSG_KEY1 = 'key1';
const uint32_t MSG_KEY2 = 'key2';
const uint32_t MSG_KEY3 = 'key3';
const uint32_t MSG_KEY4 = 'key4';
const uint32_t MSG_KEY5 = 'key5';
const uint32_t MSG_KEY6 = 'key6';
const uint32_t MSG_KEY7 = 'key7';
const uint32_t MSG_KEY8 = 'key8';

const uint32_t MSG_SAVE = 'save';
const uint32_t MSG_CANCEL = 'cncl';
const uint32_t MSG_TIMER = 'tmr_';
const uint32_t MSG_SAVE_PANEL = 'svpn';


class KeyConfView : public BView {
public:
    KeyConfView(BRect frame); // Constructor
    virtual void MessageReceived(BMessage* msg) override; // Recibe y procesa mensajes
    virtual void AttachedToWindow() override; // Se ejecuta al mostrar la ventana
    void setKeySettings(int index, unsigned char code);
    ~KeyConfView(); // Destructor

private:
    // Métodos privados para la lógica interna
    void StartCapture(uint32_t keyMsgWhat); // Inicia el temporizador de captura para un botón
    void StopCapture(); // Detiene el temporizador

    // Miembros de datos
    BButton* fKeyButtons[8]; // Array para los 8 botones de teclas
    BButton* fSaveButton;    // Botón para guardar
    BButton* fCancelButton;  // Botón para cancelar
    BFilePanel* fSavePanel;  // Panel del sistema para elegir archivo

    BMessageRunner* fTimer;  // Temporizador para la captura de 5 segundos
    uint32_t fActiveKey;       // Mensaje de la tecla que se está configurando (0 si ninguna)

    char fKeySettings[8];    // Array para almacenar las teclas capturadas
};

// --- Ventana Principal ---
class KeyConfWindow : public BWindow {
public:
    KeyConfWindow()
        : BWindow(BRect (100, 100, 420, 500), B_TRANSLATE("Key Configuration"), B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
    {
        KeyConfView* view = new KeyConfView(Bounds());
        AddChild(view);
    }
};

#endif