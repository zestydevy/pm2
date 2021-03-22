
#ifndef INCLUDE_MODELKEEPER_HPP
#define INCLUDE_MODELKEEPER_HPP

#include <nusys.h>

#include "heap.hpp"
#include "linklist.hpp"

// -------------------------------------------------------------------------- //

struct TModelData {

  u32 id;
  u32 load_num { 1 };
  // add model data here

  explicit TModelData(u32 id) : id { id } {}

  TDoubleLinkListNode<TModelData> node { this };

};

// -------------------------------------------------------------------------- //

enum TModelID {

  MDL_DUMMY = 0,
  // add model IDs here

};

// -------------------------------------------------------------------------- //

class TModelKeeper {

  public:

  TModelKeeper() = default;
  ~TModelKeeper() = default;

  static void startup(THeap * heap = nullptr);
  static void shutdown();

  bool isModelLoaded(u32 id) const;
  TModelData const * getModel(u32 id) const;
  TModelData const * loadModel(u32 id);
  bool unloadModel(u32 id);
  void unloadAll();

  static TModelKeeper * getInstance() {
    return sInstance;
  }

  private:

  static TModelKeeper * sInstance;
  TDoubleLinkList<TModelData> mModelList;
  THeap * mHeap { nullptr };

  bool lookupID(u32, TModelData **) const;

  void loadModelData(TModelData *);
  void unloadModelData(TModelData *);

};

// -------------------------------------------------------------------------- //

#endif
