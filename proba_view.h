
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

#ifndef PROBA_VIEW_H
#define PROBA_VIEW_H

#include "misc.h"
#include "array.h"
#include "rgb_image.h"

class ProbaView : public Array<scalar_t> {
public:
  ProbaView(int view_width, int view_height);

  void from_image(const RGBImage *picture);

  inline void fill(const scalar_t &t) {
    register scalar_t *s = content;
    register int i = 0;
    for(i = 0; i < width * height - 7; i += 8) {
      *(s++) = t; *(s++) = t; *(s++) = t; *(s++) = t;
      *(s++) = t; *(s++) = t; *(s++) = t; *(s++) = t;
    }
    for(; i < width * height; i++) *(s++) = t;
  }

  inline void multiply_subarray(int xmin, int ymin, int xmax, int ymax, scalar_t k) {
    register scalar_t *s = content + xmin * height + ymin;
    for(register int x = 0; x < xmax - xmin; x++) {
      for(register int y = 0; y < ymax - ymin; y++) *(s++) *= k;
      s += height - ymax + ymin;
    }
  }
};

#endif
