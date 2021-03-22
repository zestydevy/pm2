#pragma once

#include <nusys.h>
#include <nualstl_n.h>

#include "array.hpp"

// -------------------------------------------------------------------------- //

enum ESfxType : musHandle
{
    SFX_BIRD_FLAP            = 0,
    SFX_BIRD_WALL_HIT        = 1,
    SFX_BORGAR               = 2,
    SFX_COUNTDOWN_GO         = 3,
    SFX_COUNTDOWN_NUMBER     = 4,
    SFX_CAW                  = 5,
    SFX_FREEDOM              = 6,
    BGM_FUNK_COMP_L          = 7,
    BGM_FUNK_COMP_R          = 8,
    SFX_GRAB                 = 9,
    SFX_MENU_BACK            = 10,
    SFX_MENU_PAGE            = 11,
    SFX_MENU_SELECT          = 12,
    SFX_POP                  = 13,
    SFX_REALEAGLE1           = 14,
    BGM_SONG_COMP_L          = 15,
    BGM_SONG_COMP_R          = 16,
    SFX_TIMEOUT              = 17,
    SFX_WOOSH                = 18,
    SFX_STAMP                = 19,
};

enum EBgm : musHandle
{
    BGM_TEST = 0,
    BGM_FUNK = BGM_FUNK_COMP_L,
    BGM_FLY = BGM_SONG_COMP_L,
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