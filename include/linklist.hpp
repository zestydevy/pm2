
#ifndef INCLUDE_LINKLIST_HPP
#define INCLUDE_LINKLIST_HPP

#include <nusys.h>

template<typename T>
class TDoubleLinkList {

  public:

  struct TNode {

    TNode * next { nullptr };
    TNode * prev { nullptr };
    T * data { nullptr };

    TNode() = default;

    explicit inline TNode(T * data) :
      data(data)
    {}

  };

  inline TDoubleLinkList() { clear(); }
  inline ~TDoubleLinkList() = default;

  inline bool empty() const {
    return (mSize == 0);
  }

  inline u32 size() const {
    return mSize;
  }

  inline TNode * begin() { return mRootNode.next; }
  inline TNode const * begin() const { return mRootNode.next; }
  inline TNode const * cbegin() const { return mRootNode.next; }

  inline TNode * end() { return &mRootNode; }
  inline TNode const * end() const { return &mRootNode; }
  inline TNode const * cend() const { return &mRootNode; }

  inline void clear() {
    mSize = 0;
    mRootNode.next = &mRootNode;
    mRootNode.prev = &mRootNode;
  }

  void link(TNode * at, TNode * node) {
    if (at == nullptr || node == nullptr || node == &mRootNode) {
      return;
    }

    node->prev = at->prev;
    node->next = at;

    if (at->prev != nullptr) {
      at->prev->next = node;
    }

    at->prev = node;
    ++mSize;
  }

  void unlink(TNode * node) {
    if (node == nullptr || node == &mRootNode) {
      return;
    }

    if (node->next != nullptr) {
      node->next->prev = node->prev;
    }

    if (node->prev != nullptr) {
      node->prev->next = node->next;
    }

    --mSize;
  }

  private:

  u32 mSize { 0 };
  TNode mRootNode;

};

template<typename T>
using TDoubleLinkListNode = typename TDoubleLinkList<T>::TNode;

#endif
