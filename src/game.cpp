#include <nusys.h>

#include "app.hpp"
#include "sprite.hpp"
#include "pad.hpp"
#include "animator.hpp"
#include "math.h"
#include "graphic.h"
#include "audio.hpp"
#include "title.hpp"

// -------------------------------------------------------------------------- //

TGame * TGame::sGameInstance{nullptr};

extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

// -------------------------------------------------------------------------- //

static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

Gfx rspinit_dl[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD ),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx rdpinit_spr_dl[] = {
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetBlendMask(0xff),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx rdpinit_dl[] = {
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetBlendMask(0xff),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

// -------------------------------------------------------------------------- //

TGame::TGame()
{
    sGameInstance = this;
}

void TGame::init()
{
    TAudio::init();

    // allocate dynamic display list
    mDynList = new TDynList2(2048, nullptr);

    //mCamera = new TCamera(mDynList);

    mBlockHeap = (TBlockHeap *)THeap::getCurrentHeap();
    mScene = new TLogoScene("logo", mDynList);
    mBlockHeap->setGroupID(69);

    //initAudio();

    //TTask::build(ETaskCode::F3DEX2, true);

    //Set up model animator
    //Vtx* birdMeshes[] = {bird_Bird_mesh_vtx_0, bird_Bird_mesh_vtx_1, bird_Bird_mesh_vtx_2, bird_Bird_mesh_vtx_3};
    //Vtx** birdAnims[] = {bird_Bird_Walk_0, bird_Bird_Walk_1, bird_Bird_Walk_2, bird_Bird_Walk_3};
    //int meshSizes[] = {265, 75, 59, 8};

    //mBirdAnim = new TAnimator(4, birdMeshes, meshSizes);
    //mBirdAnim->setAnimation(bird_Bird_Walk_Length, birdAnims);
}

// -------------------------------------------------------------------------- //

void TGame::update()
{   
    mDynList->reset();

    initRcpSegment();
    initZBuffer();
    initFrameBuffer();

    gDPPipelineMode(mDynList->pushDL(), G_PM_NPRIMITIVE);
    gDPSetScissor(mDynList->pushDL(), G_SC_NON_INTERLACE, 10, 10, kResWidth - 10, kResHeight - 10);
    //gDPSetScissor(mDynList->pushDL(), G_SC_NON_INTERLACE, -80, -80, kResWidth + 80, kResHeight + 80);

    auto scene = getCurrentScene();

    // check for null scene or scene that has not yet been initialized
    if (scene != nullptr && scene->isInitialized()) {
        scene->draw();
    }

    // that's a wrap for this frame. finalize
    gDPFullSync(mDynList->pushDL());
	gSPEndDisplayList(mDynList->pushDL());

    osWritebackDCache(mDynList->getHead(), 2048);
    
    nuGfxTaskStart(mDynList->getHead(),
        (s32)(mDynList->fetchCmdIndex()) * sizeof (Gfx),
	    NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);

    // split our tasks
    
    mDynList->reset();
    mDynList->flip();

    gSPDisplayList(mDynList->pushDL(), rdpinit_spr_dl);
    TSprite::init(mDynList);

    // check for null scene or scene that has not yet been initialized
    if (scene != nullptr && scene->isInitialized()) {
        scene->draw2D();
    }

    gSPDisplayList(mDynList->pushDL(), rdpinit_dl);
	gSPDisplayList(mDynList->pushDL(), rspinit_dl);
    
    // that's a wrap for this frame. finalize
    gDPFullSync(mDynList->pushDL());
	gSPEndDisplayList(mDynList->pushDL());

    osWritebackDCache(mDynList->getHead(), 2048);
    
    nuGfxTaskStart(mDynList->getHead(),
        (s32)(mDynList->fetchCmdIndex()) * sizeof (Gfx),
	    NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);


    char conbuff[64];
    nuDebConTextColor(0, NU_DEB_CON_TEXT_RED);
    nuDebConTextPos(0,3,3);
    sprintf(conbuff,"%x", THeap::getCurrentHeap()->getUsedSize());
    nuDebConCPuts(0, conbuff);
    
    nuDebConDisp(NU_SC_SWAPBUFFER);

    mDynList->flip();
}

// -------------------------------------------------------------------------- //

void TGame::draw()
{
    //getCurrentScene()->update();
    
    /*
    u16 perspNorm{0};
    
    guPerspective(&dyn.projection, &perspNorm,
		      mFov, 320.0/240.0, 100, 8000, 1.0);
    guLookAtReflect(&dyn.viewing, &(dyn.lookat[0]),
		       50, 0, 400,
		       0, 0, 0,
		       0, 1, 0);
	
    gSPLookAt(mDynList->pushDL(), &(dyn.lookat[0]));
    gSPPerspNormalize(mDynList->pushDL(), perspNorm);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&(dyn.projection)),
		  G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&(dyn.viewing)),
		  G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);
	
    guTranslate(&dyn.letters_trans, -15.0f, 0.0f, -1000.0f);
    guScale(&dyn.letters_scale, 0.5f, 0.5f, 0.5f);
    guRotate(&dyn.letters_rotate, mTheta, 0.2, mTheta, 0.0);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&(dyn.letters_trans)),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&(dyn.letters_scale)),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&(dyn.letters_rotate)),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    gSPDisplayList(mDynList->pushDL(), letters_setup_dl);

    mBirdAnim->update();

    gSPDisplayList(mDynList->pushDL(), bird_Bird_mesh);

    TSprite::init();
    
    TSprite bo(&white_sprite, 0, 0);
    bo.mScale = {320.0f,240.0f};
    bo.mColor = {255,255,255,mWhiteAlpha};
    
    TSprite wo(&black_sprite, 0, 0);
    wo.mScale = {320.0f,240.0f};
    wo.mColor = {255,255,255,mBlackAlpha};
    
    bo.render();
    wo.render();
   
    TSprite::finalize();

    if (mCurrentFrame >= 80) {
        if (mBlackAlpha > 0) {
            mBlackAlpha -= 4;
        }
        if (mBlackAlpha < 4) {
            mBlackAlpha = 0;
        }
    }

    if (mCurrentFrame >= 110) {
        if (mWhiteAlpha > 0) {
            mWhiteAlpha -= 4;
        }
        if (mWhiteAlpha < 4) {
            mWhiteAlpha = 0;
        }
    }

    if (mCurrentFrame >= 115) {
            mFov -= 0.5f;
    }

    if (mFov < 40.0f) mFov = 40.0f;

    mCurrentFrame++;
    */
}

// -------------------------------------------------------------------------- //

void TGame::exit() {
    TScene * next;

    // make sure the next TScene pointer is allocated in group 255
    mBlockHeap->setGroupID(255);
    next = mScene->exit();

    // free all objects from the old scene (group 69)
    mBlockHeap->setGroupID(69);
    mBlockHeap->freeGroup(69);

    delete mScene;
    mScene = next;
}

// -------------------------------------------------------------------------- //

// tell the RCP where each segment is
void TGame::initRcpSegment()
{
    /* Setting the RSP segment register  */
    gSPSegment(mDynList->pushDL(), 0, 0x0);  /* For the CPU virtual address  */

    gSPDisplayList(mDynList->pushDL(), rdpinit_dl);
	gSPDisplayList(mDynList->pushDL(), rspinit_dl);
    
    /*
    gSPSegment(mDynList->pushDL(), 0, 0x0);	// K0 (physical) address segment
	gSPSegment(
        mDynList->pushDL(), STATIC_SEGMENT,
        osVirtualToPhysical(_codeSegmentEnd));
	
	gSPDisplayList(mDynList->pushDL(), rdpinit_dl);
	gSPDisplayList(mDynList->pushDL(), rspinit_dl);
	gDPSetDepthImage(mDynList->pushDL(), OS_K0_TO_PHYSICAL(zbuffer));
    */
}

// -------------------------------------------------------------------------- //

void TGame::initZBuffer()
{
    gDPSetDepthImage(mDynList->pushDL(), OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetCycleType(mDynList->pushDL(), G_CYC_FILL);
    gDPSetColorImage(mDynList->pushDL(), G_IM_FMT_RGBA, G_IM_SIZ_16b,kResWidth,
        OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(mDynList->pushDL(),(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
        GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(mDynList->pushDL(), 0, 0, kResWidth-1, kResHeight-1);
    gDPPipeSync(mDynList->pushDL());
}

// -------------------------------------------------------------------------- //

void TGame::initFrameBuffer()
{
    gDPSetColorImage(mDynList->pushDL(), G_IM_FMT_RGBA, G_IM_SIZ_16b, kResWidth,
        osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(mDynList->pushDL(), mClearColor);
    gDPFillRectangle(mDynList->pushDL(), 0, 0, kResWidth-1, kResHeight-1);
}

// -------------------------------------------------------------------------- //

static int frameState = 0;

void TGame::testRender(u32 taskNum)
{
    auto game = TGame::getInstance();
    auto scene = game->getCurrentScene();

    frameState++;

    TAudio::update();

    switch(scene->getState())
    {
        case ESceneState::IDLE:
            scene->init();
            break;
        case ESceneState::RUNNING:
            scene->update();
            break;
        case ESceneState::EXITING:
            game->exit();
            break;
    }

    if (taskNum > 4){   //2 graphics tasks per framebuffer
        return;
    }

    game->update();
    game->draw();

    nuGfxRetraceWait(2);

    if (scene->getName() == "game") {
        //nuGfxRetraceWait(1);
    } else {
       // nuGfxRetraceWait(2);
    }
}

// -------------------------------------------------------------------------- //
