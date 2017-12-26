/*
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa
 * University Description : library for Assignment 2 - Data structures and
 * Algorithms - Fall 2017
 * =========================================================================================
 */
/**
 * This is done by nguyen anh khoa - 1611617
 *
 * Github repository will be public after online judge closes
 * Github: https://github.com/nganhkhoa/CTDL-ASS2.git
 * Docker:
 * https://cloud.docker.com/swarm/luibo/repository/docker/luibo/ctdl-ass2/general
 *
 * Library use with define so no affect on building online
 *
 * Libraries used:
 *    Spdlog: https://github.com/gabime/spdlog
 *    Googletest: https://github.com/google/googletest
 *
 * This is only in case teacher want to interview me
 */

#ifndef A02_DSALIB_H
#define A02_DSALIB_H

#include <string>
#include <math.h>
#include <vector>

using namespace std;


class DSAException {
      int    _error;
      string _text;

    public:
      DSAException() : _error(0), _text("Success") {}
      DSAException(int err) : _error(err), _text("Unknown Error") {}
      DSAException(int err, const char* text) : _error(err), _text(text) {}

      int getError() {
            return _error;
      }
      string& getErrorText() {
            return _text;
      }
};

template <class T>
struct L1Item
{
      T          data;
      L1Item<T>* pNext;
      L1Item() : pNext(nullptr) {}
      L1Item(T& a) : data(a), pNext(nullptr) {}
};

template <class T>
class L1List {
      L1Item<T>* _pHead;    // The head pointer of linked list
      size_t     _size;     // number of elements in this list
    public:
      L1List() : _pHead(nullptr), _size(0) {}
      ~L1List();

      void clean();

      inline bool isEmpty() {
            return _pHead == nullptr;
      }
      inline size_t getSize() const {
            return _size;
      }

      T& at(int i);
      T& operator[](int i);

      bool find(T& a, int& idx);
      T*   find(T& a);
      T*   findLast(T& a);
      int  insert(int i, T& a);
      int  remove(int i);
      int  removeAll(T& a);

      int push_back(T& a);
      int insertHead(T& a);

      int removeHead();
      int removeLast();

      void reverse();

      void traverse(void (*op)(T&)) {
            L1Item<T>* p = _pHead;
            while (p) {
                  op(p->data);
                  p = p->pNext;
            }
      }
      void traverse(void (*op)(T&, void*), void* pParam) {
            L1Item<T>* p = _pHead;
            while (p) {
                  op(p->data, pParam);
                  p = p->pNext;
            }
      }
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T& a) {
      if (_pHead == nullptr) {
            _pHead = new L1Item<T>(a);
      }
      else {
            L1Item<T>* p = _pHead;
            while (p->pNext)
                  p = p->pNext;
            p->pNext = new L1Item<T>(a);
      }

      _size++;
      return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T& a) {
      L1Item<T>* p = new L1Item<T>(a);
      p->pNext     = _pHead;
      _pHead       = p;
      _size++;
      return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
      if (_pHead) {
            L1Item<T>* p = _pHead;
            _pHead       = p->pNext;
            delete p;
            _size--;
            return 0;
      }
      return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
      if (_pHead) {
            if (_pHead->pNext) {
                  L1Item<T>* prev = _pHead;
                  L1Item<T>* pcur = prev->pNext;
                  while (pcur->pNext) {
                        prev = pcur;
                        pcur = pcur->pNext;
                  }
                  delete pcur;
                  prev->pNext = nullptr;
            }
            else {
                  delete _pHead;
                  _pHead = nullptr;
            }
            _size--;
            return 0;
      }
      return -1;
}

template <class T>
L1List<T>::~L1List() {
      clean();
      _pHead = nullptr;
}

template <class T>
void L1List<T>::clean() {
      while (_pHead)
            removeHead();
}

template <class T>
T& L1List<T>::at(int i) {
      if (i < 0 || i >= (int) getSize())
            throw new DSAException(1, "Index out of range");

      auto* temp = _pHead;
      while (i--)
            temp = temp->pNext;
      return temp->data;
}

template <class T>
T& L1List<T>::operator[](int i) {
      return at(i);
}

template <class T>
bool L1List<T>::find(T& a, int& idx) {
      if (isEmpty())
            return false;

      auto temp = _pHead;
      int  i    = 0;
      while (temp) {
            if (temp->data == a) {
                  idx = i;
                  return true;
            }
            i++;
            temp = temp->pNext;
      }
      idx = -1;
      return false;
}

template <class T>
T* L1List<T>::find(T& a) {
      if (isEmpty())
            return nullptr;

      auto temp = _pHead;
      while (temp) {
            if (temp->data == a) {
                  return &temp->data;
            }
            temp = temp->pNext;
      }
      return nullptr;
}

template <class T>
void L1List<T>::reverse() {
      if (isEmpty())
            return;

      L1Item<T>* prev = NULL;
      L1Item<T>* next = _pHead;

      while (_pHead) {
            next = next->pNext;

            _pHead->pNext = prev;

            prev   = _pHead;
            _pHead = next;
      }
      _pHead = prev;
}

/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
template <class T>
struct AVLNode
{
      T          _data;
      AVLNode<T>*_pLeft, *_pRight;
#ifdef AVL_USE_HEIGHT
      int _height;
      AVLNode(T& a) : _data(a), _pLeft(nullptr), _pRight(nullptr), _height(1) {}
#else
      int _bFactor;
      AVLNode(T& a)
          : _data(a), _pLeft(nullptr), _pRight(nullptr), _bFactor(0) {}
#endif

      void calibrate() {
            this->_height = [](int a, int b) { return a > b ? a : b; }(
                               (_pLeft ? _pLeft->_height : 0),
                               (_pRight ? _pRight->_height : 0)) +
                            1;
      }
};

template <class T>
class AVLTree {
      AVLNode<T>* _pRoot;
#if defined UNIT_TEST || defined DEBUGGING
      size_t size;
#endif

    public:
      AVLTree() : _pRoot(nullptr) {
#if defined UNIT_TEST || defined DEBUGGING
            size = 0;
#endif
      }
      ~AVLTree() {
            destroy(_pRoot);
      }

#if defined UNIT_TEST || defined DEBUGGING
      inline size_t getSize() const {
            return size;
      }
#endif

      inline bool isEmpty() const {
            return _pRoot == nullptr;
      }
      bool find(T& key, T*& ret) {
            return find(_pRoot, key, ret);
      }
      // a compare function
      // making sure of not insert same data
      bool insert(T& key, bool (*cmp)(T&, T&) = [](T& t1, T& t2) -> bool {
            return false;
      }) {
            return insert(_pRoot, key, cmp);
      }
      bool remove(T& key) {
            return remove(_pRoot, key);
      }
      void traverseNLR(void (*op)(T&)) {
            traverseNLR(_pRoot, op);
      }
      void traverseLNR(void (*op)(T&)) {
            traverseLNR(_pRoot, op);
      }
      void traverseLRN(void (*op)(T&)) {
            traverseLRN(_pRoot, op);
      }

    protected:
      void destroy(AVLNode<T>*& pR);
      bool find(AVLNode<T>* pR, T& key, T*& ret);
      bool insert(AVLNode<T>*& pR, T& a, bool (*cmp)(T&, T&));
      bool remove(AVLNode<T>*& pR, T& a);
      void traverseNLR(AVLNode<T>* pR, void (*op)(T&));
      void traverseLNR(AVLNode<T>* pR, void (*op)(T&));
      void traverseLRN(AVLNode<T>* pR, void (*op)(T&));

#if defined UNIT_TEST || defined DEBUGGING
    public:
      L1List<AVLNode<T>*>* getListNode() {
            auto list = new L1List<AVLNode<T>*>();
            _getListNode(_pRoot, list);
            list->reverse();
            return list;
      }
      void _getListNode(AVLNode<T>* pR, L1List<AVLNode<T>*>* list) {
            if (!pR)
                  return;
            _getListNode(pR->_pLeft, list);
            list->insertHead(pR);
            _getListNode(pR->_pRight, list);
      }
#endif

      void rotLeft(AVLNode<T>*& pR);
      void rotRight(AVLNode<T>*& pR);
      void rotLR(AVLNode<T>*& pR);
      void rotRL(AVLNode<T>*& pR);

      bool balanceLeft(AVLNode<T>*& pR);
      bool balanceRight(AVLNode<T>*& pR);
      bool balance(AVLNode<T>*& pR);
};

template <class T>
void AVLTree<T>::destroy(AVLNode<T>*& n) {
      if (n == nullptr)
            return;

      if (n->_pLeft == nullptr && n->_pRight == nullptr) {
            delete n;
            n = nullptr;
      }
      else {
            // destroy left right and itself
            destroy(n->_pLeft);
            destroy(n->_pRight);
            delete n;
            n = nullptr;
      }
}

template <class T>
void AVLTree<T>::rotLeft(AVLNode<T>*& n) {
      auto temp     = n;
      n             = n->_pRight;
      temp->_pRight = n->_pLeft;
      n->_pLeft     = temp;

      temp->calibrate();
      n->calibrate();
      temp = nullptr;
}

template <class T>
void AVLTree<T>::rotRight(AVLNode<T>*& n) {
      auto temp    = n;
      n            = n->_pLeft;
      temp->_pLeft = n->_pRight;
      n->_pRight   = temp;

      temp->calibrate();
      n->calibrate();
      temp = nullptr;
}

template <class T>
void AVLTree<T>::rotLR(AVLNode<T>*& n) {
      rotLeft(n->_pLeft);
      rotRight(n);
}

template <class T>
void AVLTree<T>::rotRL(AVLNode<T>*& n) {
      rotRight(n->_pRight);
      rotLeft(n);
}

template <class T>
bool AVLTree<T>::balanceLeft(AVLNode<T>*& n) {
      // right of left height
      int rlh = n->_pLeft->_pRight ? n->_pLeft->_pRight->_height : 0;
      // left of left height
      int llh = n->_pLeft->_pLeft ? n->_pLeft->_pLeft->_height : 0;

      switch (rlh - llh) {
            case 1:
                  rotLR(n);
                  return true;
            case -1:
                  rotRight(n);
                  return true;
            default:
                  return false;
      }
      return false;
}

template <class T>
bool AVLTree<T>::balanceRight(AVLNode<T>*& n) {
      // right of right height
      int rrh = n->_pRight->_pRight ? n->_pRight->_pRight->_height : 0;
      // left of right height
      int lrh = n->_pRight->_pLeft ? n->_pRight->_pLeft->_height : 0;

      switch (rrh - lrh) {
            case -1:
                  rotRL(n);
                  return true;
            case 1:
                  rotLeft(n);
                  return true;
            default:
                  return false;
      }
      return false;
}


template <class T>
bool AVLTree<T>::balance(AVLNode<T>*& n) {
      int leftHeight  = n->_pLeft ? n->_pLeft->_height : 0;
      int rightHeight = n->_pRight ? n->_pRight->_height : 0;

      switch (rightHeight - leftHeight) {
            case 2:
                  return balanceRight(n);
            case -2:
                  return balanceLeft(n);
            default:
                  return false;
      }
      return false;
}

template <class T>
bool AVLTree<T>::insert(AVLNode<T>*& n, T& t, bool (*cmp)(T&, T&)) {
      if (n == nullptr) {
            n = new AVLNode<T>(t);
#if defined UNIT_TEST || defined DEBUGGING
            size++;
#endif
            return true;
      }

      // if same then stop
      if (cmp(n->_data, t))
            return false;

      if (t < n->_data) {
            insert(n->_pLeft, t, cmp);
      }
      else {
            insert(n->_pRight, t, cmp);
      }

      n->calibrate();
      return balance(n);
}


template <class T>
void AVLTree<T>::traverseNLR(AVLNode<T>* pR, void (*op)(T&)) {
      if (!pR)
            return;

      op(pR->_data);
      traverseNLR(pR->_pLeft, op);
      traverseNLR(pR->_pRight, op);
}
template <class T>
void AVLTree<T>::traverseLNR(AVLNode<T>* pR, void (*op)(T&)) {
      if (!pR)
            return;

      traverseNLR(pR->_pLeft, op);
      op(pR->_data);
      traverseNLR(pR->_pRight, op);
}
template <class T>
void AVLTree<T>::traverseLRN(AVLNode<T>* pR, void (*op)(T&)) {
      if (!pR)
            return;

      traverseNLR(pR->_pLeft, op);
      traverseNLR(pR->_pRight, op);
      op(pR->_data);
}


template <class T>
bool AVLTree<T>::find(AVLNode<T>* pR, T& key, T*& ret) {
      if (!pR)
            return false;

      if (pR->_data == key) {
            ret = &pR->_data;
            return true;
      }

      if (pR->_data > key)
            return find(pR->_pLeft, key, ret);
      else
            return find(pR->_pRight, key, ret);
}

#endif    // A02_DSALIB_H
