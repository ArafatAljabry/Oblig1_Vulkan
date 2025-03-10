#ifndef INPUT_H
#define INPUT_H

struct Input{
    //WASD
    bool W{false},
         A{false},
         S{false},
         D{false};

    //Arrow keys
    bool UP{false},
         DOWN{false},
         LEFT{false},
         RIGHT{false};

    bool Q{false},
         E{false},
         C{false},
         LCTRL{false},
         LSHIFT{false};

    // Mouse buttons
    bool LMB{false},
         RMB{false},
         MMB{false};
    float MWHEEL{0.0f};

    int MOUSEX{0},
        MOUSEY{0};


};
#endif // INPUT_H
