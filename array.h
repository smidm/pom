
//////////////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify         //
// it under the terms of the version 3 of the GNU General Public License        //
// as published by the Free Software Foundation.                                //
//                                                                              //
// This program is distributed in the hope that it will be useful, but          //
// WITHOUT ANY WARRANTY; without even the implied warranty of                   //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU             //
// General Public License for more details.                                     //
//                                                                              //
// You should have received a copy of the GNU General Public License            //
// along with this program. If not, see <http://www.gnu.org/licenses/>.         //
//                                                                              //
// Written by Francois Fleuret                                                  //
// (C) Ecole Polytechnique Federale de Lausanne                                 //
// Contact <pom@epfl.ch> for comments & bug reports                             //
//////////////////////////////////////////////////////////////////////////////////

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

#include "misc.h"

template<class T>
class Array {
public:
  int width, height;
  // [i][j] is faster on i386 CPU than [i + j * width]
  T *content;
  T **heads;

  inline void resize(int w, int h) {
    delete[] content;
    delete[] heads;
    width = w; height = h;
    content = new T[width * height];
    heads = new T *[width];
    for(int i = 0; i < width; i++) heads[i] = content + i * height;
  }

  inline int get_width() const { return width; }
  inline int get_height() const { return height; }

  inline Array() : width(0), height(0), content(0), heads(0) { }

  inline Array(int w, int h) : width(w), height(h) {
    content = new T[width * height];
    heads = new T *[width];
    for(int i = 0; i < width; i++) heads[i] = content + i * height;
  }

  inline Array(const Array &a) : width(a.width), height(a.height) {
    content = new T[width * height];
    heads = new T *[width];
    for(int i = 0; i < width; i++) heads[i] = content + i * height;
    memcpy(content, a.content, height * width * sizeof(T));
  }

  inline ~Array() { delete[] content; delete[] heads; }

  inline Array &operator = (const Array &a) {
    if(this != &a) {
      if(a.width != width || a.height != height) resize(a.width, a.height);
      if(width > 0 && height > 0) memcpy(content, a.content, height * width * sizeof(T));
    }
    return *this;
  }

  inline Array& clear() {
    memset(content, 0, height * width * sizeof(T));
    return *this;
  }

  inline T dot(const Array &a) {
    ASSERT(width == a.width && height == a.height, "Size mismatch in Array::dot");
    T *u1 = content, *u2 = a.content;
    T s = 0;
    for(int i = 0; i < width * height; i++) s += *(u1++) * *(u2++);
    return s;
  }

  inline T sum_square() {
    T *u = content;
    T s = 0;
    for(int i = 0; i < width * height; i++) { s += *u * *u; u++; }
    return s;
  }

  inline T sum() {
    T *u = content;
    T s = 0;
    for(int i = 0; i < width * height; i++) s += *(u++);
    return s;
  }

  inline T &operator () (int i, int j) {
    ASSERT(i >= 0 && i < width && j >= 0 && j < height, "Index out of bounds in Array::operator ()");
    return heads[i][j];
  }

  inline T operator () (int i, int j) const {
    ASSERT(i >= 0 && i < width && j >= 0 && j < height, "Index out of bounds in Array::operator () const");
    return heads[i][j];
  }

  inline void print(std::ostream &os) const {
    for(int i = 0; i < width; i++) for(int j = 0; j < height; j++)
      os << heads[i][j] << ((i < width-1) ? ' ' : '\n');
  }

  inline void print_for_gnuplot(std::ostream &os) const {
    for(int i = 0; i < width; i++) {
      for(int j = 0; j < height; j++)
        os << i << " " << j << " " << heads[i][j] << "\n";
      os << "\n";
    }
  }

  inline T l2distance(const Array<T> &m) {
    ASSERT(m.width == width && m.height == height, "Array size mismatch");
    T r = 0;
    for(int i = 0; i < width * height; i++) r += (m.content[i] - content[i]) * (m.content[i] - content[i]);
    return r;
  }
};

template<class T>
std::ostream &operator << (std::ostream &os, const Array<T> &v) { v.print(os); return os; }

#endif
