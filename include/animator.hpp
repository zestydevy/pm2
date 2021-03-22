#pragma once

#include <ultra64.h>

class TAnimator
{
    public:
    TAnimator(int meshCount, Vtx* meshes[], int sizes[]);
    ~TAnimator();

    void update();
    void draw();
    
    void setAnimation(int length, Vtx** animation[], bool loop = true, float speed = 0.25f);
    void setTimescale(float dt);
    void setFrame(float frame);

    int getEndFrame();

    bool isAnimationCompleted();
    bool isAnimationLooping();

    protected:
    int mMeshCount;
    int* mMeshSizes;
    Vtx** mMeshes;
    Vtx*** mAnim;

    bool loop;

    int mLength;
    float mTime;
    float mTimescale;
    
    void setVertexPos(int size, Vtx vtx[], Vtx* anim[], int frame);
    void lerpVertexPos(int size, Vtx vtx[], Vtx* anim[], float frame);
};

// -------------------------------------------------------------------------- //
