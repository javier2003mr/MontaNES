#ifndef JOYPAD
#define JOYPAD

class Joypad {

    private:
        bool strobe_active;
        unsigned char buttons;
        unsigned char shift_register;

    public:

        Joypad();

        void loadButtons (bool a, bool b, bool select, bool start, bool up, bool down, bool left, bool right);
        void write4016 (unsigned char value);
        unsigned char read4016 ();
};

#endif