
#include <nusys.h>

#include "heap.hpp"
#include "linklist.hpp"
#include "modelkeeper.hpp"

// -------------------------------------------------------------------------- //

TModelKeeper * TModelKeeper::sInstance;

// -------------------------------------------------------------------------- //

void TModelKeeper::startup(
  THeap * heap
) {
  if (heap == nullptr) {
    heap = THeap::getCurrentHeap();
  }

  sInstance = new(heap) TModelKeeper {};
  sInstance->mHeap = heap;
}

// -------------------------------------------------------------------------- //

void TModelKeeper::shutdown() {
  sInstance->unloadAll();
  delete sInstance;
  sInstance = nullptr;
}

// -------------------------------------------------------------------------- //

bool TModelKeeper::isModelLoaded(u32 id) const {
  return lookupID(id, nullptr);
}

// -------------------------------------------------------------------------- //

TModelData const *
TModelKeeper::getModel(u32 id) const {
  TModelData * data;

  if (!lookupID(id, &data)) {
    return nullptr;
  }

  return data;
}

// -------------------------------------------------------------------------- //

TModelData const *
TModelKeeper::loadModel(u32 id) {
  TModelData * existing;

  if (lookupID(id, &existing)) {
    ++existing->load_num;
    return existing;
  }

  if (id == MDL_DUMMY) {
    return nullptr;
  }

  auto mdl_data = new(mHeap) TModelData { id };
  loadModelData(mdl_data);
  mModelList.link(mModelList.end(), &mdl_data->node);
  return mdl_data;
}

// -------------------------------------------------------------------------- //

bool TModelKeeper::unloadModel(u32 id) {
  TModelData * data;

  if (!lookupID(id, &data)) {
    return false;
  }

  if (data->load_num != 0) {
    --data->load_num;
  }

  if (data->load_num == 0) {
    unloadModelData(data);
    mModelList.unlink(&data->node);
    delete data;
  }

  return true;
}

// -------------------------------------------------------------------------- //

void TModelKeeper::unloadAll() {
  auto node = mModelList.begin();

  while (node != nullptr) {
    auto next = node->next;
    unloadModelData(node->data);
    delete node->data;
    node = next;
  }

  mModelList.clear();
}

// -------------------------------------------------------------------------- //

bool TModelKeeper::lookupID(
  u32 id, TModelData ** data
) const {
  if (id == MDL_DUMMY) {
    return false;
  }

  auto node = mModelList.begin();

  while (node != mModelList.end()) {
    if (node->data->id == id) {
      if (data != nullptr) {
        *data = node->data;
      }

      return true;
    }

    node = node->next;
  }

  return false;
}

// -------------------------------------------------------------------------- //

void TModelKeeper::loadModelData(
  TModelData * data
) {
  // load model data here based on data->id
}

// -------------------------------------------------------------------------- //

void TModelKeeper::unloadModelData(
  TModelData * data
) {
  // unload model data here
}

// -------------------------------------------------------------------------- //
