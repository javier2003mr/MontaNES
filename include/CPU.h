class CPU {
    private:
        
        // Contador de programa
        unsigned short PC;

        // Acumulador
        unsigned char A;

        // Indices
        unsigned char X, Y;

        // Puntero de pila
        unsigned char SP;

        // Registros de estado
        // Mapa de bits:
        // 0: Carry
        // 1: Zero
        // 2: Interrupt Disable
        // 3: Decimal
        // 4: B flag
        // 5: 1
        // 6: Overflow
        // 7: Negative
        unsigned char P;

    public:

        void emulationCycle();
};