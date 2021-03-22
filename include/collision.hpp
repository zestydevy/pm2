
#ifndef INCLUDE_COLLISION_HPP
#define INCLUDE_COLLISION_HPP

#include <nusys.h>

#include "heap.hpp"
#include "math.hpp"

// -------------------------------------------------------------------------- //

class TCollision {

  public:

  struct TFace {

    TVec3F vtx[3];
    TVec3F nrm;
    float d;

    void calc();

    bool isPassThru() const;
    void setPassThru(bool);

    bool isGround() const;

    float minX() const;
    float maxX() const;

    float minY() const;
    float maxY() const;

    float minZ() const;
    float maxZ() const;

    bool isXZInside(TVec2F const & pt) const;
    bool isXYZInside(TVec3F const & pt) const;

    float calcYAt(TVec2F const & xz) const;
    float calcDist(TVec3F const & pt) const; // distance to PLANE, not POLYGON

    void project(TVec3F const & src, TVec3F * dst) const;
    void calcClosestPt(TVec3F const & src, TVec3F * dst) const;

    private:

    static bool calcNrm(
      TVec3F const & v0,
      TVec3F const & v1,
      TVec3F const & v2,
      TVec3F * nrm
    );

  };

  struct TPacket {

    TPacket * next;
    TFace const * face;

  };

  static bool startup(
    TFace data[], u32 size, THeap * heap,
    u32 num_pkt, u16 num_blk, float blk_sz
  );

  static void shutdown();

  static bool calc();
  static bool calc(u32 start, u32 count);

  static u32 checkRadius(
    TVec3F const & pt, float r,
    TFace const * faces[] = nullptr, u32 limit = 0
  );

  static TFace const * findClosest(
    TVec3F const & pt, float r = 0.0F
  );

  // pt: 3D point to survey
  //  b: bias to add to pt.y()
  //  d: if > 0, limit y distance
  static TFace const * findGroundBelow(
    TVec3F const & pt, float b = 0.0F, float d = 0.0F
  );

  static void getBlkBox(u32 x, u32 y,
    float * min_x, float * min_z,
    float * max_x, float * max_z
  );

  static void getSphereBlk(
    TVec3F const & pt, float rd,
    u32 * l, u32 * b, u32 * r, u32 * t
  );

  static u32 blkMapCount(u32 x, u32 y) {
    u32 count = 0;
    TPacket * list = sBlkMap[y * sNumBlkMap + x];

    while (list != nullptr) {
      list = list->next;
      ++count;
    }

    return count;
  }

  private:

  static TFace * sCollFaceAry;
  static u32 sNumCollFace;

  static TPacket * sCollPktAry;
  static u32 sMaxNumCollPkt;
  static u32 sFreeCollPkt;
  static u32 sNumCollPkt;

  static TPacket ** sBlkMap;
  static float sBlkMapSz;
  static u32 sNumBlkMap;

  static TPacket * fetchPktFast(TFace const *);
  static TPacket * fetchPktSlow(TFace const *);

  static void blkMapLink(TPacket **, TPacket *);
  static void blkMapUnlink(TPacket **, TPacket *);

  static bool blkMapAdd(TFace *, TPacket * (*)(TFace const *));
  static bool blkMapRemove(TPacket **, TFace const &);

  static bool isSphereInBlk(
    TVec3F const & pt, float rd, u32 x, u32 y
  );

  static void getFaceBlk(
    TFace const * face,
    u32 * l, u32 * b, u32 * r, u32 * t
  );

  static bool isFaceInBlk(
    TFace const * face, u32 x, u32 y
  );

  static u32 calcBlkIdx(float, float, float);

};

// -------------------------------------------------------------------------- //

using TCollFace = TCollision::TFace;
using TCollPacket = TCollision::TPacket;

// -------------------------------------------------------------------------- //

#endif
