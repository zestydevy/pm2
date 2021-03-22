
#ifndef INCLUDE_MENU_HPP
#define INCLUDE_MENU_HPP

#include <nusys.h>

#include "dynlist.hpp"
#include "hud.hpp"
#include "pad.hpp"
#include "scene.hpp"
#include "sprite.hpp"

// -------------------------------------------------------------------------- //

enum : u32 {

  UNLOCK_FREEDOM = 0x0000'0001,

};

// -------------------------------------------------------------------------- //

class TMenuScene :
  public TScene
{

  public:

  explicit TMenuScene(TDynList2 *);
  virtual ~TMenuScene() = default;

  virtual void init() override;
  virtual void update() override;
  virtual void draw() override;
  virtual void draw2D() override;
  virtual TScene * exit() override;

  static bool isFreedomMode();
  static void unlockFreedomMode();

  static u32 getTimeLimit();

  private:

  enum {

    ST_FADE_OUT,
    ST_BIRD_IN,
    ST_FLASH_IN,
    ST_FLASH_OUT,
    ST_TITLE,
    ST_MENU_IN,
    ST_MENU_WAIT,
    ST_MENU_LEFT,
    ST_MENU_RIGHT,
    ST_MENU_FLASH,
    ST_SUBMENU_IN,
    ST_SUBMENU_WAIT,
    ST_SUBMENU_LEFT,
    ST_SUBMENU_RIGHT,
    ST_SUBMENU_OUT,
    ST_SUBMENU_FLASH,
    ST_START_IN,
    ST_START_WAIT,
    ST_START_OUT,
    ST_START_CANCEL,
    ST_UNLOCK_IN,
    ST_UNLOCK_WAIT,
    ST_UNLOCK_OUT,

  };

  enum : u32 {

    SPR_FILL,
    SPR_BG,
    SPR_BIRD,
    SPR_LOGO,
    SPR_MENU_SWOOP,
    SPR_MENU_0,
    SPR_MENU_1,
    SPR_MENU_LEFT,
    SPR_MENU_RIGHT,
    SPR_SUBMENU_0,
    SPR_SUBMENU_1,
    SPR_FLASH,
    SPR_UNLOCK_1,
    SPR_UNLOCK_0,
    SPR_START,
    SPR_OK,

    NUM_SPRITES

  };

  enum {

    OPT_PRACTICE,
    OPT_TIME,
    OPT_FREEDOM,

    NUM_OPTIONS

  };

  enum {

    SUBOPT_300,
    SUBOPT_500,
    SUBOPT_700,

    NUM_SUBOPTIONS

  };

  int mState { ST_FADE_OUT };
  THudAlarm mStateTimer;
  TSprite mSprite[NUM_SPRITES];
  u32 mSpriteMask { 0 };
  TPad * mPad { nullptr };
  float mMenuTimer { 0.0F };
  int mMenuList[NUM_OPTIONS];
  int mNumMenuOpts { 0 };
  int mMenuOpt { 0 };
  int mOldMenuOpt { 0 };
  int mSubMenuList[NUM_SUBOPTIONS];
  int mNumSubMenuOpts { 0 };
  int mSubMenuOpt { 0 };
  int mOldSubMenuOpt { 0 };

  bool isPressLeft() const;
  bool isPressRight() const;
  bool isPressSelect() const;
  bool isPressCancel() const;

  void setOnSprite(u32 i) { mSpriteMask &= ~(1U << i); }
  void setOffSprite(u32 i) { mSpriteMask |= (1U << i); }

  static Sprite const & getOptSprite(int);
  static s16 getOptWidth(int);

  static Sprite const & getSubOptSprite(int);
  static s16 getSubOptWidth(int);

};

// -------------------------------------------------------------------------- //

#endif
