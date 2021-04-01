#include <nusys.h>
#include <nualstl_n.h>

#include "audio.hpp"
#include "heap.hpp"
#include "util.hpp"
#include "array.hpp"

#include "segment.h"

// -------------------------------------------------------------------------- //

u8 * TAudio::sBankBuffer{nullptr};
u8 * TAudio::sMusicBuffer{nullptr};
u8 * TAudio::sSfxBuffer{nullptr};

musHandle TAudio::mBgmL;
musHandle TAudio::mBgmR;

float TAudio::sBgmCurVol = { 0.0F };
float TAudio::sBgmToVol = { 0.0F };
float TAudio::sBgmFadeVol = { 0.0F };

TArray<musHandle> TAudio::sHandleList;

// -------------------------------------------------------------------------- //

void TAudio::init()
{
    musConfig c;

    c.control_flag = 0; // Set to MUSCONTROL_RAM if wbk file also placed in RAM
    c.channels = NU_AU_CHANNELS; // Maximum total number of channels
    c.sched = NULL; // The address of the Scheduler structure. NuSystem uses NULL
    c.thread_priority = NU_AU_MGR_THREAD_PRI; // Thread priority (highest)
    c.heap = (unsigned char*)NU_AU_HEAP_ADDR; // Heap address
    c.heap_length = NU_AU_HEAP_SIZE; // Heap size
    c.ptr = NULL; // Allows you to set a default ptr file
    c.wbk = NULL; // Allows you to set a default wbk file
    c.default_fxbank = NULL; // Allows you to set a default bfx file
    c.fifo_length = NU_AU_FIFO_LENGTH; // The size of the library's FIFO buffer
    c.syn_updates = NU_AU_SYN_UPDATE_MAX; // Number of updates for the synthesizer.
    c.syn_output_rate = 44100; // Audio output rate. The higher, the better quality
    c.syn_rsp_cmds = NU_AU_CLIST_LEN; // Maximum length of the audio command list.
    c.syn_retraceCount = 1; // The number of frames per retrace message
    
    // Number of buffers the Audio Manager can use for DMA from ROM transfer.
    c.syn_num_dma_bufs = NU_AU_DMA_BUFFER_NUM; 
    c.syn_dma_buf_size = NU_AU_DMA_BUFFER_SIZE; // The length of each DMA buffer

    // Initialize the Audio Manager.
    nuAuStlMgrInit(&c);

    // Register the PRENMI function.
    nuAuPreNMIFuncSet(nuAuPreNMIProc);

    // allocate audio buffers (not heap)
    sBankBuffer = new u8[NU_AU_SAMPLE_SIZE];
    sMusicBuffer = new u8[NU_AU_SONG_SIZE];
    sSfxBuffer = new u8[NU_AU_SE_SIZE];

    // register sample bank
    TUtil::toMemory(
        reinterpret_cast<void *>(sBankBuffer),
        reinterpret_cast<void *>(_miditableSegmentRomStart),
        _miditableSegmentRomEnd-_miditableSegmentRomStart);

    MusPtrBankInitialize(sBankBuffer, _midibankSegmentRomStart);

    // register sound effects
    TUtil::toMemory(
        reinterpret_cast<void *>(sSfxBuffer),
        reinterpret_cast<void *>(_sfxSegmentRomStart),
        _sfxSegmentRomEnd-_sfxSegmentRomStart);

	MusFxBankInitialize(sSfxBuffer);

    // set heap for handles
    sHandleList.setHeap(THeap::getCurrentHeap());
    
    // play bgm
    //sHandleList.push(MusStartEffect(ESfxType::SFX_CAW));
    //MusHandleSetReverb(sHandleList[0], 10000>>8);

};

// -------------------------------------------------------------------------- //

void TAudio::update() {
    setVolume();

    if (sBgmCurVol > sBgmToVol) {
        sBgmCurVol += sBgmFadeVol;

        if (sBgmCurVol <= sBgmToVol) {
            sBgmCurVol = sBgmToVol;
            sBgmFadeVol = 0.0F;
        }
    } else {
        sBgmCurVol += sBgmFadeVol;

        if (sBgmCurVol >= sBgmToVol) {
            sBgmCurVol = sBgmToVol;
            sBgmFadeVol = 0.0F;
        }
    }
}

// -------------------------------------------------------------------------- //

void TAudio::playSound(ESfxType const sound)
{
    MusStartEffect(sound);
}

// -------------------------------------------------------------------------- //

void TAudio::playMusic(EBgm const music)
{
    mBgmL = MusStartEffect(music);
    mBgmR = MusStartEffect(music+1);
    setVolume();
}

void TAudio::playMusicMono(EBgm const music)
{
    mBgmL = MusStartEffect(music);
    setVolume();
}

// -------------------------------------------------------------------------- //

void TAudio::fadeMusic(float to, float time) {
    sBgmToVol = to;

    if (time <= 0.001F) {
        sBgmCurVol = to;
        sBgmFadeVol = 0.0F;
    } else {
        sBgmFadeVol = ((to - sBgmCurVol) / (kFrameRate * time));
    }
}

// -------------------------------------------------------------------------- //

void TAudio::stopMusic()
{
    MusHandleStop(mBgmL, 0);
    //MusHandleStop(mBgmR, 0);
}

// -------------------------------------------------------------------------- //

void TAudio::setVolume() {
    int volume = (int)(sBgmCurVol * 0.5F * 128.0F);

    MusHandleSetVolume(mBgmL, volume);
    //MusHandleSetVolume(mBgmR, volume);
}

// -------------------------------------------------------------------------- //

bool TAudio::isPlaying()
{
    return MusAsk(MUSFLAG_EFFECTS);
}