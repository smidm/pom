
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

#include <cmath>

#include "room.h"
#include "misc.h"

Room::Room(int view_width, int view_height, int nb_cameras, int nb_positions) {
  _view_width = view_width;
  _view_height = view_height;
  _nb_cameras = nb_cameras;
  _nb_positions = nb_positions;
  _rectangles = new Rectangle[_nb_cameras * _nb_positions];
}

Room::~Room() {
  delete[] _rectangles;
}

void Room::save_stochastic_view(char *name,
                                int n_camera,
                                const ProbaView *view,
                                const Vector<scalar_t> *proba_presence) const  {

  RGBImage image(view->get_width(), view->get_height());

  Array<scalar_t> proba_pixel_off(_view_width, _view_height);

  for(int px = 0; px < _view_width; px++) for(int py = 0; py < _view_height; py++)
    proba_pixel_off(px, py) = 1.0;

  Array<bool> dots(_view_width, _view_height);
  dots.clear();

  for(int n = 0; n < nb_positions(); n++) {
    Rectangle *r = avatar(n_camera, n);
    if(r->visible) {
      for(int py = r->ymin; py < r->ymax; py++)
        for(int px = r->xmin; px < r->xmax; px++)
          proba_pixel_off(px, py) *= (1 - (*proba_presence)[n]);
      if(r->xmin > 0 && r->xmax < _view_width-1 && r->ymax < _view_height-1)
        dots((r->xmax + r->xmin)/2, r->ymax) = true;
    }
  }

  for(int py = 0; py < _view_height; py++) for(int px = 0; px < _view_width; px++) {
    scalar_t r, g, b;
    scalar_t a = proba_pixel_off(px, py);

    if(dots(px, py)) { r = 0.0; g = 0.0; b = 0.0; }
    else {
      if(a < 0.5) { r = 0; g = 0; b = 2*a; }
      else        { r = (a - 0.5) * 2; g = (a - 0.5) * 2; b = 1.0; }
    }

    scalar_t c = (*view)(px, py);

    r = c * 0.0 + (1 - c) * r;
    g = c * 0.8 + (1 - c) * g;
    b = c * 0.6 + (1 - c) * b;

    image.set_pixel(px, py, (unsigned char) (255 * r), (unsigned char) (255 * g), (unsigned char) (255 * b));
  }

  image.write_png(name);
}
