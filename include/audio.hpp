#pragma once

#include <nusys.h>
#include <nualstl_n.h>

#include "array.hpp"

// -------------------------------------------------------------------------- //

enum ESfxType : musHandle
{
    SFX_INTRON = 0,
    SFX_KARASU,
    SFX_SELECT,
};

enum EBgm : musHandle
{
    BGM_INTRO = 0,
};

class TAudio
{
    public:

    static void init();
    static void update();

    static void playSound(ESfxType sound);

    static void playMusic(EBgm music);
    static void fadeMusic(float to, float time = 0.0F);
    static void stopMusic();

    private:

    static u8 * sBankBuffer;
    static u8 * sMusicBuffer;
    static u8 * sSfxBuffer;

    static musHandle mBgmL;
    static musHandle mBgmR;

    static float sBgmCurVol;
    static float sBgmToVol;
    static float sBgmFadeVol;

    static TArray<musHandle> sHandleList;

    static void setVolume();

};

// -------------------------------------------------------------------------- //