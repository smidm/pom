
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

#ifndef INTEGRAL_PROBA_VIEW_H
#define INTEGRAL_PROBA_VIEW_H

#include "proba_view.h"
#include "integral_array.h"

class IntegralProbaView : public IntegralArray<scalar_t> {
public:
  IntegralProbaView(int view_width, int view_height) : IntegralArray<scalar_t>(view_width, view_height) {};

  // Computes the integral image and returns the sum of all the
  // original image pixels

  inline scalar_t compute_sum(const ProbaView *m) {
    scalar_t *p = content, *pm = m->content;
    for(int x = 0; x < height; x++) *(p++) = 0;

    register scalar_t st = 0;
    register scalar_t sl;
    for(register int y = 1; y < width; y++) {
      sl = 0; *(p++) = sl;
      for(register int x = 0; x < height - 1; x++) {
        sl += *(pm++);
        *p = sl + *(p - height);
        p++;
      }
      st += sl;
    }

    return st;
  }

  // Computes the integral image and returns the sum of (2m-1)*b

  inline scalar_t compute_sum(const ProbaView *m, const ProbaView *b) {
    scalar_t *p = content, *pm = m->content, *pb = b->content;

    for(int x = 0; x < height; x++) *(p++) = 0;

    scalar_t st = 0;
    register scalar_t sl;
    for(int y = 1; y < width; y++) {
      sl = 0; *(p++) = 0;
      for(int x = 0; x < height - 1; x++) {
        st -= *pb;
        sl += *(pm++) * *(pb++);
        *p = sl + *(p - height);
        p++;
      }
      st += 2 * sl;
    }

    return st;
  }
};
#endif
