//===-- KInstIterator.h -----------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_KINSTITERATOR_H
#define KLEE_KINSTITERATOR_H
#include  "klee/Internal/Module/KInstruction.h"

namespace klee {
  struct KInstruction;

  class KInstIterator {
    KInstruction **it;

  public:
    KInstIterator() : it(0){}
    KInstIterator(KInstruction **_it) : it(_it) {}
    KInstIterator(const KInstIterator &b) : it(b.it) {}
    
    bool operator==(const KInstIterator &b) const {
      return it==b.it;
    }

    bool operator==(int* a) const {
      if(a == nullptr){
        return it ? 0 : 1;
      }
    }

    bool operator!=(const KInstIterator &b) const {
      return !(*this == b);
    }

    bool operator!=(int* a) const {
      if(a == nullptr){
        return it ? 1 : 0;
      }
    }

    //KInstIterator &operator=(const KInstIterator &b) {
    //    it = b.it;
    //    assert((*it)->inst);
    //    return *this;
    //}

    KInstIterator &operator++() {
      ++it;
      //assert((*it)->inst);
      return *this;
    }

    operator KInstruction*() const { return it ? *it : 0;}
    operator bool() const { return it != 0; }

    KInstruction *operator ->() const { return *it; }
  };
} // End klee namespace

#endif
