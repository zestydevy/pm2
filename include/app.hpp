#pragma once

#include <nusys.h>

#include "game.hpp"
#include "heap.hpp"

// -------------------------------------------------------------------------- //

size_t constexpr kStackSize = 0x2000;
size_t constexpr kMaxPiMsg = 8;
size_t constexpr kHeapLocation = 0x8028F800;
size_t constexpr kHeapSize     = 1024*512;      // 0.5 MB

size_t constexpr kResWidth = 320;
size_t constexpr kResHeight = 240;

size_t constexpr kCrashStackSize = 0x2000;
size_t constexpr kCrashId = 14;
size_t constexpr kCrashMsg = 16;


void idle(void * arg);
void bootApp();

// -------------------------------------------------------------------------- //

class CApp
{
    public:
    
    CApp() = default;
    ~CApp() = default;
    void init();
    void setupStaticSegment(void * dest, u32 const & src);
    void run();

    private:

    static void appError(void * arg);
    
    u32 mClearColor{0};
    bool mFifo{false}; 

    static TBlockHeap * sCurrentHeap;
    TGame * mGame{nullptr};
};

// -------------------------------------------------------------------------- //