
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

#ifndef RGB_IMAGE_H
#define RGB_IMAGE_H

#include "misc.h"

using namespace std;

class RGBImage {
protected:
  int _width, _height;
  unsigned char ***_bit_plans, **_bit_lines, *_bit_map;
  static const int RED = 0;
  static const int GREEN = 1;
  static const int BLUE = 2;
  static const int RGB_DEPTH = 3;

  void allocate();
  void deallocate();

public:

  RGBImage();
  RGBImage(int width, int height);
  virtual ~RGBImage();

  inline int width() const { return _width; }
  inline int height() const { return _height; }

  inline void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    ASSERT(x >= 0 && x < _width && y >= 0 && y < _height, "Out of bounds.");
    _bit_plans[RED][y][x] = r;
    _bit_plans[GREEN][y][x] = g;
    _bit_plans[BLUE][y][x] = b;
  }

  inline unsigned char pixel(int x, int y, int d) const {
    ASSERT(x >= 0 && x < _width && y >= 0 && y < _height && d >= 0 && d < RGB_DEPTH, "Out of bounds.");
    return _bit_plans[d][y][x];
  }

  virtual void read_ppm(const char *filename);
  virtual void write_ppm(const char *filename);

  virtual void read_png(const char *filename);
  virtual void write_png(const char *filename);
};

#endif
