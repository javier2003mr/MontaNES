#include "KeyConfig.hpp"
#include <cstdio>
#include <Path.h>
#include <File.h>
#include <interface/InterfaceDefs.h> // Provides key_info struct and get_key_info function
#include <ios>
#include <Catalog.h>
#include <Locale.h>
#include <Key.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "KeyNES2"

bool capturing = false;
int kindex = 0;

unsigned char keys[8];

BString getButtonNames(int index){

    BString name;

    switch (index)
    {
    case 1:
        name = "A";
        break;
    case 2:
        name = "B";
        break;
    case 3:
        name = "Start";
        break;
    case 4:
        name = "Select";
        break;
    case 5:
        name = "Up";
        break;
    case 6:
        name = "Down";
        break;
    case 7:
        name = "Left";
        break;
    case 8:
        name = "Right";
        break;
    default:
        name = "Unknown Button";
        break;
    }

    return name;
}

BString GetKeyName(uint32 keycode)
{

    key_map* map = NULL;
    char* keyBuffer = NULL;
    
    // Obtiene el mapa y el búfer actual del sistema
    // Nota: Esta función reserva memoria que debemos liberar después.
    get_key_map(&map, &keyBuffer);
    
    if (map == NULL || keyBuffer == NULL)
        return BString("[Error]");

    // El rango estándar de keycodes en Haiku es de 0 a 127
    if (keycode >= 128) {
        free(map);
        free(keyBuffer);
        return BString("[Desconocido]");
    }

    // Obtenemos el desplazamiento (offset) en el búfer para el modo normal
    int32 offset = map->normal_map[keycode];
    
    // El primer byte en la posición del offset es la longitud de la cadena UTF-8
    int length = keyBuffer[offset];
    
    BString result;
    if (length > 0) {
        // Copiamos los caracteres saltándonos el byte de longitud (offset + 1)
        result.SetTo(&keyBuffer[offset + 1], length);
    }

    // ¡Importante! Liberar la memoria asignada por get_key_map
    free(map);
    free(keyBuffer);
    
    return result;
}

BString getWorkingDirectory() {
    image_info info;
    int32 cookie = 0;

    while (get_next_image_info(B_CURRENT_TEAM, &cookie, &info) >= B_OK) {
        if (info.type == B_APP_IMAGE) {
            BString path(info.name);
            int32 lastSlash = path.FindLast('/');
            if (lastSlash >= 0){
                path.Truncate(lastSlash);
                int32 lastSlash = path.FindLast('/');
                if (lastSlash >= 0){
                    path.Truncate(lastSlash);
                }
            }

            return path;
        }
    }
    return BString();
}

void loadKeys(const char * path){

    //unsigned char keys[8];
    FILE * fkeys;

    fkeys = fopen(path, "rb");
    if (fkeys != NULL){
        fread(keys, sizeof(unsigned char), 8, fkeys);
    }else{
        keys[0] = 0x42;
        keys[1] = 0x43;
        keys[2] = 0x30;
        keys[3] = 0x47;
        keys[4] = 0x28;
        keys[5] = 0x3d;
        keys[6] = 0x3c;
        keys[7] = 0x3e;
    }
    fclose(fkeys);
    printf("Configuracion de teclas:\n");
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
        label << B_TRANSLATE("Button") << " " << getButtonNames(i + 1) << ": " << GetKeyName(fKeySettings[i]);
        BRect rect(20, 20 + i * 40, 160, 50 + i * 40);
        fKeyButtons[i] = new BButton(rect, nullptr, label.String(), msg);
        AddChild(fKeyButtons[i]);
    }

    // Crea los botones de "Guardar" y "Cancelar"
    BRect saveRect(180, 20, 280, 50);
    fSaveButton = new BButton(saveRect, nullptr, B_TRANSLATE("Save"), new BMessage(MSG_SAVE));
    AddChild(fSaveButton);

    BRect cancelRect(180, 70, 280, 100);
    fCancelButton = new BButton(cancelRect, nullptr, B_TRANSLATE("Cancel"), new BMessage(MSG_CANCEL));
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

    BString dir;
    char * c;

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
            
            dir = getWorkingDirectory();
            dir += "/keyconfig";
            c = dir.LockBuffer(MAX_PATH_LENGTH);
            storeKeys(c);
            //fSavePanel->Show();
            break;
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
    label << B_TRANSLATE("Capturing Key") << " " << getButtonNames(index+1) << "...";
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
                //keys[kindex] = captured_key; // Store in the global keys array
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
        label << B_TRANSLATE("Button") << " " << getButtonNames(i + 1) << ": " << GetKeyName(fKeySettings[i]);
        fKeyButtons[i]->SetLabel(label.String());
    }
    capturing = false;
    fActiveKey = 0;
}