
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

#ifndef INTEGRAL_ARRAY_H
#define INTEGRAL_ARRAY_H

#include <iostream>

using namespace std;

#include "array.h"

template <class T>
class IntegralArray: public Array<T> {
public:

  void compute(const Array<T> *m) {
    T *v = Array<T>::content, *w = m->Array<T>::content;
    for(int x = 0; x < Array<T>::height; x++) *(v++) = 0;

    register T sl;
    for(int y = 1; y < Array<T>::width; y++) {
      sl = 0; *(v++) = 0;
      for(int x = 0; x < Array<T>::height - 1; x++) {
        sl += *(w++);
        *(v++) = sl + *(v - Array<T>::height);
      }
    }
  }

  IntegralArray(int w, int h) : Array<T>(w+1, h+1) { }

  IntegralArray(const Array<T> &m) : Array<T>(m->get_width() + 1, m->get_height() + 1) {
    compute(m);
  }

  // Integral on xmin <= x < xmax, ymin <= y < ymax
  // Thus, xmax and ymax can go up to m->width+1 and m->height+1 respectively

  inline T integral(int xmin, int ymin, int xmax, int ymax) const {
    ASSERT(xmin <= xmax && ymin <= ymax, "Inconsistent bounds for integral");
    ASSERT(xmin >= 0 && xmax < Array<T>::width &&
           ymin >= 0 && ymax < Array<T>::height, "Index out of bounds in Array::operator () const");
    return Array<T>::heads[xmax][ymax] + Array<T>::heads[xmin][ymin]
      - Array<T>::heads[xmax][ymin] - Array<T>::heads[xmin][ymax];
  }
};

#endif
