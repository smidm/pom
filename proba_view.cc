
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

#include "proba_view.h"

ProbaView::ProbaView(int view_width, int view_height) : Array<scalar_t>(view_width, view_height) { }

void ProbaView::from_image(const RGBImage *image) {
  ASSERT(image->width() == width && image->height() == height,
         "Image size missmatch");
  int k = 0;
  for(int x = 0; x < width; x++) for(int y = 0; y < height; y++) {
    content[k++] = scalar_t(image->pixel(x, y, 0) +
                            image->pixel(x, y, 1) +
                            image->pixel(x, y, 2))/(3 * 255);
  }
}
