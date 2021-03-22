#include <nusys.h>

#include "pad.hpp"

// -------------------------------------------------------------------------- //

NUContData TPad::sConts[kMaxControllers];

#define DEADZONE 4

// -------------------------------------------------------------------------- //

TPad::TPad(s32 contNum)
{
    mPadNum = contNum;
}

// -------------------------------------------------------------------------- //

void TPad::init()
{
    auto pattern = nuContInit();
}

// -------------------------------------------------------------------------- //

void TPad::read()
{
    mPrevButton = sConts[mPadNum].button;
    nuContDataGetEx(sConts, mPadNum);
    mAnalogX = sConts[mPadNum].stick_x;
    mAnalogY = sConts[mPadNum].stick_y;
}

// -------------------------------------------------------------------------- //

bool TPad::isPressed(EButton const button) const
{
    return (sConts[mPadNum].trigger & (static_cast<u16>(button)));
}

// -------------------------------------------------------------------------- //

bool TPad::isHeld(EButton const button) const
{
    return (sConts[mPadNum].button & static_cast<u16>(button));
}

// -------------------------------------------------------------------------- //

bool TPad::isReleased(EButton const button) const
{
    return (!(sConts[mPadNum].button & static_cast<u16>(button)) && 
        (mPrevButton & static_cast<u16>(button)));
}

// -------------------------------------------------------------------------- //

s8 TPad::getAnalogX() const
{
    if (mAnalogX > -DEADZONE && mAnalogX < DEADZONE)
        return 0;
    return mAnalogX;
}

// -------------------------------------------------------------------------- //

s8 TPad::getAnalogY() const
{
    if (mAnalogY > -DEADZONE && mAnalogY < DEADZONE)
        return 0;
    return mAnalogY;
}

// -------------------------------------------------------------------------- //