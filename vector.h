
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

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

#include "misc.h"

template<class T>
class Vector {
protected:
  int size;
  T *content;
public:
  inline void resize(int s) {
    delete[] content;
    size = s;
    content = new T[size];
  }

  inline int length() const { return size; }

  inline Vector(std::istream &is) {
    is.read((char *) &size, sizeof(size));
    content = new T[size];
    is.read((char *) content, sizeof(T)*size);
  }

  inline Vector() : size(0), content(0) { }
  inline Vector(int s) : size(s), content(new T[size]) {}
  inline Vector(const Vector &v) : size(v.size), content(new T[size]) {
    if(size > 0) memcpy(content, v.content, size * sizeof(T));
  }
  inline ~Vector() {
    delete[] content;
#ifdef DEBUG
    content = 0;
#endif
  }

  inline void load(std::istream &is) {
    is.read((char *) &size, sizeof(size));
    resize(size);
    is.read((char *) content, sizeof(T) * size);
  }

  inline void save(std::ostream &os) const {
    os.write((char *) &size, sizeof(size));
    os.write((char *) content, sizeof(T) * size);
  }

//   inline void fill(const T &t) {
//     T *s = content;
//     for(int i = 0; i < size; i++) *(s++) = t;
//   }

  inline Vector &operator = (const Vector &v) {
    if(this != &v) {
      if(v.size != size) {
	delete[] content;
	size = v.size;
	content = new T[size];
      }
      if(size > 0) memcpy(content, v.content, size * sizeof(T));
    }
    return *this;
  }

  inline bool operator == (const Vector &v) {
    if(this != &v) {
      if(v.size != size) return false;
      return memcmp(content, v.content, size * sizeof(T)) == 0;
    } else return true;
  }

  inline bool operator != (const Vector &v) {
    if(this != &v) {
      if(v.size != size) return true;
      return memcmp(content, v.content, size * sizeof(T)) != 0;
    } else return false;
  }

  inline Vector & clear() {
    if(size > 0) memset(content, 0, size * sizeof(T));
    return *this;
  }

  inline T &operator [] (int k) {
    ASSERT(k >= 0 && k < size, "Index out of bound in Vector::operator []");
    return content[k];
  }

  inline T operator [] (int k) const {
    ASSERT(k >= 0 && k < size, "Index out of bound in Vector::operator [] const");
    return content[k];
  }

  inline T norme() const {
    T s = 0;
    for(int i = 0; i<size; i++) s += content[i] * content[i];
    return sqrt(s);
  }

  inline void print(std::ostream &os) const {
    for(int i = 0; i < size; i++) os << content[i] << ((i < size - 1) ? " " : "\n");
  }

  inline void print_for_gnuplot(std::ostream &os) const {
    for(int i = 0; i < size; i++) os << content[i] << "\n";
  }

};

template<class T>
std::ostream &operator << (std::ostream &os, const Vector<T> &v) { v.print(os); return os; }

#endif
