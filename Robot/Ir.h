#ifndef IR_H
#define IR_H

class Ir {
    private:
        int pinLO;
        bool lo;

        int pinLI;
        bool li;

        int pinMM;
        bool mm;

        int pinRI;
        bool ri;

        int pinRO;
        bool ro;

    public:
        Ir(int pinLO, int pinLI, int pinMM, int pinRI, int pinRO);
        void initialize();
        void update();
        void getActiveSensorsByInt();
};

#endif
