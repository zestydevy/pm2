/* ----------------------------------------------------
 ######  ### ######  ######           #    #    #     # 
 #     #  #  #     # #     #          #   # #   ##   ## 
 #     #  #  #     # #     #          #  #   #  # # # # 
 ######   #  ######  #     #          # #     # #  #  # 
 #     #  #  #   #   #     #    #     # ####### #     # 
 #     #  #  #    #  #     #    #     # #     # #     # 
 ######  ### #     # ######      #####  #     # #     # 
-----------------------------------------------------*/

// -------------------------------------------------------------------------- //

#include <nusys.h>

#include "app.hpp"
#include "collider.hpp"
#include "heap.hpp"
#include "modelkeeper.hpp"
#include "reloc.h"
#include "segment.h"

// -------------------------------------------------------------------------- //

extern "C" {

void mainproc(void)
{
  bootApp();
}

}

// -------------------------------------------------------------------------- //

void bootApp()
{
  THeap::init();

  CApp app = CApp();

  // 0.5 MB heap
  auto gameHeap = TBlockHeap(
  reinterpret_cast<void *>(kHeapLocation), kHeapSize);

  // set game heap as current heap
  THeap::setCurrentHeap(&gameHeap);

  //RelocSegLoad((u32)_testRelSegmentRomStart, (u32)_testRelSegmentRomEnd, _testRelSegmentStart, _testRelSegmentEnd,
  //_testSegmentReloc, _codeSegmentEnd);

  //bool (*FuncPtr)();
  //void * testfunc = RELOC_GET_RAM_ADDR(testRel, _testRelSegmentStart, _codeSegmentEnd);
  //FuncPtr = reinterpret_cast<bool (*)()>(testfunc);
  //FuncPtr();

  app.init();
  app.run();

  while (1) {}
}

// -------------------------------------------------------------------------- //