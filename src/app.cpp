#include <nusys.h>
#include <os_internal.h>

#include "app.hpp"
#include "pad.hpp"
#include "math.hpp"
#include "exception.hpp"

TBlockHeap * CApp::sCurrentHeap;

// -------------------------------------------------------------------------- //

static OSMesgQueue sChMessageQueue;
static OSMesg sChMessageBuf;
static OSThread sChThread;
static u64 sChStack[kCrashStackSize / sizeof(u64)];

// -------------------------------------------------------------------------- //

void CApp::init()
{   
    nuGfxInit();

    //osViSetEvent(&nusched.retraceMQ, (OSMesg)666, 2);
    
    TPad::init();
    TSine::startup(10);
    
    // instantiate main game
    mGame = new TGame;

    osCreateThread(&sChThread,
            kCrashId, 
            appError, 
            0, 
            (sChStack + kCrashStackSize / sizeof(u64)),
            OS_PRIORITY_APPMAX
    );
    
    osStartThread(&sChThread);
    
    /*
    // initialize VI manager
    //osCreateViManager(OS_PRIORITY_VIMGR);
    //osViSetMode(&osViModeTable[mode]);
    
    // initialize PI manager for PI access
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &mPiMessageQ, mPiMessages, 
		      kMaxPiMsg);

    // set framebuffer pointers
    mFrameBuffers[0] = cfb_16_a;
    mFrameBuffers[1] = cfb_16_b;

    // create DMA message queue
    osCreateMesgQueue(&DmaMessageQ, &DmaMessageBuffer, 1);

    // create RDP message queue and interrupt
    osCreateMesgQueue(&mRdpMessageQ, &mRdpMessageBuffer, 1);
    osSetEventMesg(OS_EVENT_DP, &mRdpMessageQ, mDummyMessage);

    // create and set v-blank event 
    osCreateMesgQueue(&mVblankMessageQ, &mVblankMessageBuffer, 1);
    osViSetEvent(&mVblankMessageQ, mDummyMessage, 1);

    // VI features
    //osViSetSpecialFeatures(feature);
    
    mClearColor = color;
    mFifo = useFifo;
    */
}

// -------------------------------------------------------------------------- //

void CApp::setupStaticSegment(void * dest, u32 const & src)
{
    /*
    OSIoMesg dmaIOMessageBuf{};

    dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIOMessageBuf.hdr.retQueue = &DmaMessageQ;
    dmaIOMessageBuf.dramAddr     = dest;
    dmaIOMessageBuf.devAddr      = src;
    dmaIOMessageBuf.size         = _staticSegmentRomEnd-_staticSegmentRomStart;

    osEPiStartDma(gHandler, &dmaIOMessageBuf, OS_READ);
    
    // wait for DMA to finish
    osRecvMesg(&DmaMessageQ, &mDummyMessage, OS_MESG_BLOCK);
    */
}

// -------------------------------------------------------------------------- //

void CApp::run()
{   
    mGame->init();

    nuGfxFuncSet(TGame::testRender);
    nuGfxDisplayOn();

    while(1) {}
    
}

// -------------------------------------------------------------------------- //

void CApp::appError(void * arg)
{
    OSMesg msg;
    OSThread *curr;

    // Create the message queue for the fault message
    osCreateMesgQueue(&sChMessageQueue, &sChMessageBuf, 1);
    osSetEventMesg(OS_EVENT_FAULT, &sChMessageQueue, (OSMesg)kCrashMsg);

    while(true) {
        
        // Wait for a fault message to arrive
        osRecvMesg(&sChMessageQueue, (OSMesg *)&msg, OS_MESG_BLOCK);

        u16* buffer = (u16 *)osViGetNextFramebuffer();
        osViSetMode(&osViModeNtscLpn1);
        osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
        osViBlack(FALSE);
        osViSwapBuffer(buffer);

        // Get the faulted thread
        curr = (OSThread *)__osGetCurrFaultedThread();
        if(curr != NULL) {
            hmm:
            __OSThreadContext* context = &curr->context;

            // Print the basic info
            char str[128];
            sprintf(str, "something fucked up!!!! Fault in thread: %d", curr->id);
            debug_draw_string(buffer, 5, 5, str);

            sprintf(str, "pc    | %08x", (u32)context->pc);
            debug_draw_string(buffer, 5, 15, str);

            debug_printreg(buffer, 5, 25, (u8)context->cause, const_cast<char *>("cause"), const_cast<reg_desc *>(causeDesc));
            debug_printreg(buffer, 5, 35, (u32)context->sr, const_cast<char *>("sr   "), const_cast<reg_desc *>(srDesc));

            sprintf(str, "badvaddr | %08x", (u32)context->badvaddr);
            debug_draw_string(buffer, 5, 45, str);
                    
            // Print the registers
            sprintf(str, "at %08x | v0 %08x | v1 %08x", (u32)context->at, (u32)context->v0, (u32)context->v1);
            debug_draw_string(buffer, 5, 55, str);            
            sprintf(str, "a0 %08x | a1 %08x | a2 %08x", (u32)context->a0, (u32)context->a1, (u32)context->a2);
            debug_draw_string(buffer, 5, 65, str);
            sprintf(str, "a3 %08x | t0 %08x | t1 %08x", (u32)context->a3, (u32)context->t0, (u32)context->t1);
            debug_draw_string(buffer, 5, 75, str);
            sprintf(str, "t2 %08x | t3 %08x | t4 %08x", (u32)context->t2, (u32)context->t3, (u32)context->t4);
            debug_draw_string(buffer, 5, 85, str);
            sprintf(str, "t5 %08x | t6 %08x | t7 %08x", (u32)context->t5, (u32)context->t6, (u32)context->t7);
            debug_draw_string(buffer, 5, 95, str);
            sprintf(str, "s0 %08x | s1 %08x | s2 %08x", (u32)context->s0, (u32)context->s1, (u32)context->s2);
            debug_draw_string(buffer, 5, 105, str);
            sprintf(str, "s3 %08x | s4 %08x | s5 %08x", (u32)context->s3, (u32)context->s4, (u32)context->s5);
            debug_draw_string(buffer, 5, 115, str);
            sprintf(str, "s6 %08x | s7 %08x | t8 %08x", (u32)context->s6, (u32)context->s7, (u32)context->t8);
            debug_draw_string(buffer, 5, 125, str);
            sprintf(str, "t9 %08x | gp %08x | sp %08x", (u32)context->t9, (u32)context->gp, (u32)context->sp);
            debug_draw_string(buffer, 5, 135, str);
            sprintf(str, "s8 %08x | ra %08x", (u32)context->s8, (u32)context->ra);
            debug_draw_string(buffer, 5, 145, str);

            // Print the floating point registers
            debug_printreg(buffer, 5, 165, (u32)context->fpcsr, const_cast<char *>("fpcsr"), const_cast<reg_desc *>(fpcsrDesc));
            sprintf(str, "d0  %.4e | d2  %.4e | d4  %.4e", context->fp0.d,  context->fp2.d, context->fp4.d);
            debug_draw_string(buffer, 5, 175, str);
            debug_draw_string(buffer, 5, 175, str);
            sprintf(str, "d6  %.4e | d8  %.4e | d10 %.4e", context->fp6.d,  context->fp8.d, context->fp10.d);
            debug_draw_string(buffer, 5, 185, str);
            sprintf(str, "d12 %.4e | d14 %.4e | d16 %.4e", context->fp12.d,  context->fp14.d, context->fp16.d);
            debug_draw_string(buffer, 5, 195, str);
            sprintf(str, "d18 %.4e | d20 %.4e | d22 %.4e", context->fp18.d, context->fp20.d, context->fp22.d);
            debug_draw_string(buffer, 5, 205, str);
            sprintf(str, "d24 %.4e | d26 %.4e | d28 %.4e", context->fp24.d, context->fp26.d, context->fp28.d);
            debug_draw_string(buffer, 5, 215, str);
            sprintf(str, "d30 %.4e", context->fp30.d);
            debug_draw_string(buffer, 5, 225, str);

            osWritebackDCache(buffer, 2 * 320 * 32);
            goto hmm;
        }
    }
}