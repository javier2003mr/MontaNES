#include "KeyConfig.hpp"
#include <cstdio>
#include <Path.h>
#include <File.h>
#include <interface/InterfaceDefs.h> // Provides key_info struct and get_key_info function

bool capturing = false;
int kindex = 0;

unsigned char keys[8];

void loadKeys(const char * path){

    //unsigned char keys[8];
    FILE * fkeys;

    fkeys = fopen(path, "rb");
    if (fkeys != NULL){
        fread(keys, sizeof(unsigned char), 8, fkeys);
    }
    fclose(fkeys);
    for (int i = 0; i < 8; ++i){
        printf("%x\n", keys[i]);
    }
    
    printf("Key settings loaded\n");
}

void storeKeys(const char * path){

    FILE * fkeys;

    fkeys = fopen(path, "wb");
    if (fkeys != NULL){
        fwrite(keys, sizeof(unsigned char), 8, fkeys);
    }
    fclose(fkeys);

    printf("Key settings saved\n");
}


// --- Implementación de KeyConfView ---
KeyConfView::KeyConfView(BRect frame)
    : BView(frame, "KeyConfView", B_FOLLOW_ALL, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
      fSavePanel(nullptr),
      fTimer(nullptr),
      fActiveKey(0)
{
    // Configura el color de fondo de la ventana
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

    // Inicializa la configuración por defecto (ej: teclas 'a', 'b', 'c'...)
    for (int i = 0; i < 8; i++) {
        fKeySettings[i] = keys[i];
    }

    // Crea los 8 botones para capturar teclas
    for (int i = 0; i < 8; i++) {
        BMessage* msg = new BMessage(MSG_KEY1 + i);
        BString label;
        label << "Capturar tecla " << i + 1;
        BRect rect(20, 20 + i * 40, 160, 50 + i * 40);
        fKeyButtons[i] = new BButton(rect, nullptr, label.String(), msg);
        AddChild(fKeyButtons[i]);
    }

    // Crea los botones de "Guardar" y "Cancelar"
    BRect saveRect(180, 20, 280, 50);
    fSaveButton = new BButton(saveRect, nullptr, "Guardar", new BMessage(MSG_SAVE));
    AddChild(fSaveButton);

    BRect cancelRect(180, 70, 280, 100);
    fCancelButton = new BButton(cancelRect, nullptr, "Cancelar", new BMessage(MSG_CANCEL));
    AddChild(fCancelButton);
}

KeyConfView::~KeyConfView()
{
    delete fSavePanel;
    StopCapture();
}

void KeyConfView::AttachedToWindow()
{
    BView::AttachedToWindow();
    // Establece esta vista como objetivo de los mensajes de los botones
    for (int i = 0; i < 8; i++) {
        fKeyButtons[i]->SetTarget(this);
    }
    fSaveButton->SetTarget(this);
    fCancelButton->SetTarget(this);
    // Habilita que la vista reciba eventos de teclado
    MakeFocus(true);
}

void KeyConfView::MessageReceived(BMessage* msg)
{
    switch (msg->what) {
        // Mensajes de los botones de captura de tecla
        case MSG_KEY1 ... MSG_KEY1 + 7:
            StartCapture(msg->what);
            break;
        // Mensaje del botón "Guardar"
        case MSG_SAVE:
            for (int i = 0; i < 8; ++i){
                keys[i] = fKeySettings[i];
            }
            storeKeys("./keyconfig");
            //fSavePanel->Show();
            break;
        // Mensaje de confirmación del panel de guardado
        case MSG_SAVE_PANEL:
        {
            /*
            entry_ref ref;
            if (msg->FindRef("directory", &ref) == B_OK) {
                BPath path(&ref);
                // Se obtiene el nombre del archivo
                const char* name = nullptr;
                if (msg->FindString("name", &name) == B_OK) {
                    path.Append(name);
                }
                _SaveToFile(path);
            }
            */
            //storeKeys("./keyconfig");
            break;
        }
        // Mensaje del botón "Cancelar"
        case MSG_CANCEL:
            //CancelSettings();
            Window()->PostMessage(B_QUIT_REQUESTED); // Request the window to close
            break;
        // Mensaje del temporizador para finalizar la captura
        case MSG_TIMER:
            StopCapture();
            break;
        default:
            BView::MessageReceived(msg);
            break;
    }
}

void KeyConfView::KeyDown(const char* bytes, uint32_t numBytes)
{
    // Solo actúa si hay una tecla activa y no se ha cancelado el temporizador
    if (fActiveKey != 0 && numBytes > 0) {
        int index = fActiveKey - MSG_KEY1;
        if (index >= 0 && index < 8) {
            fKeySettings[index] = bytes[0];
            // Actualiza la etiqueta del botón para mostrar la tecla capturada
            BString label;
            label << "Tecla " << index + 1 << ": " << bytes[0];
            fKeyButtons[index]->SetLabel(label.String());
        }
        StopCapture();
    }
    BView::KeyDown(bytes, numBytes);
}

void KeyConfView::StartCapture(uint32_t keyMsgWhat)
{
    StopCapture(); // Detiene cualquier captura en curso

    fActiveKey = keyMsgWhat;

    // Crea un temporizador de 3 segundos que enviará un mensaje
    BMessage timerMsg(MSG_TIMER);
    fTimer = new BMessageRunner(BMessenger(this), &timerMsg, 3000000LL, 1);
    if (fTimer->InitCheck() != B_OK) {
        delete fTimer;
        fTimer = nullptr;
        fActiveKey = 0;
        return;
    }
    // Cambia la etiqueta del botón para indicar que está en modo captura
    int index = keyMsgWhat - MSG_KEY1;
    BString label;
    label << "Capturando tecla " << index + 1 << "...";
    fKeyButtons[index]->SetLabel(label.String());

    kindex = index;
    capturing = true;
    resume_thread(spawn_thread(&CaptureKey, "Capture Thread", 100, this));
}

int CaptureKey(void * p){

    KeyConfView * view = static_cast<KeyConfView*>(p);

    unsigned char captured_key = 0; // Local variable for the captured key

    while(capturing){
        //printf("HOLA\n");
        key_info keyInfo;
        get_key_info(&keyInfo); // Get the current state of all keys

        // Iterate through all possible raw key codes (0-127, as key_states is 16 bytes * 8 bits/byte = 128 bits)
        // Haiku key codes are typically 0-127.
        for (int i = 0; i < 128; ++i) {
            if (keyInfo.key_states[i / 8] & (1 << (7 - (i % 8)))) {
                // Key 'i' is pressed
                captured_key = (unsigned char)i;
                keys[kindex] = captured_key; // Store in the global keys array
                capturing = false; // Stop capturing
                printf("Captured:%i %x\n",kindex, captured_key);
                view->setKeySettings(kindex, captured_key);
                break; // Exit the for loop
            }
        }
    }

    return 0;
}

void KeyConfView::setKeySettings(int index, unsigned char code){
    fKeySettings[index] = code;
}

void KeyConfView::StopCapture()
{
    if (fTimer) {
        delete fTimer;
        fTimer = nullptr;
    }
    // Restaura las etiquetas de los botones a su estado normal
    for (int i = 0; i < 8; i++) {
        BString label;
        label << "Tecla " << i + 1 << ": " << fKeySettings[i];
        fKeyButtons[i]->SetLabel(label.String());
    }
    capturing = false;
    fActiveKey = 0;
}

/*
void KeyConfView::_SaveToFile(const BPath& path)
{
    BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
    if (file.InitCheck() == B_OK) {
        // Escribe la configuración (formato simple, una letra por línea)
        for (int i = 0; i < 8; i++) {
            char line[4];
            sprintf(line, "%c\n", fKeySettings[i]);
            file.Write(line, 2);
        }
    }
}
*/
/*
void KeyConfView::CancelSettings()
{
    // Restablece los valores por defecto
    for (int i = 0; i < 8; i++) {
        fKeySettings[i] = 'a' + i;
    }
    StopCapture();
}
*/