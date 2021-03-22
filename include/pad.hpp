#pragma once

#include <nusys.h>

// -------------------------------------------------------------------------- //

s32 constexpr kMaxControllers = 1;

// -------------------------------------------------------------------------- //

enum EButton : u16
{
    START = START_BUTTON,
    A = A_BUTTON,
    B = B_BUTTON,
    C_UP = U_CBUTTONS,
    C_DOWN = D_CBUTTONS,
    C_LEFT = L_CBUTTONS,
    C_RIGHT = R_CBUTTONS,
    UP = U_JPAD,
    DOWN = D_JPAD,
    LEFT = L_JPAD,
    RIGHT = R_JPAD,
    Z = Z_TRIG,
    L = L_TRIG,
    R = R_TRIG,
};

// -------------------------------------------------------------------------- //

class TPad
{
    public:

    TPad(s32 contNum);
    ~TPad() = default;

    static void init();
    void read();

    bool isPressed(EButton button) const;
    bool isHeld(EButton button) const;
    bool isReleased(EButton button) const;

    s8 getAnalogX() const;
    s8 getAnalogY() const;

    private:

    s32 mPadNum{0};
    u16 mPrevButton{0};
    s8 mAnalogX{0};
    s8 mAnalogY{0}; 

    static NUContData sConts[kMaxControllers];
};

// -------------------------------------------------------------------------- //