
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
// Modified by Matej Smid <smidm@cmp.felk.cvut.cz>                              //
//////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "room.h"
#include "misc.h"

Room::Room(int nb_cameras, int nb_positions, Vector<ProbaView *> proba_views) {
  _proba_views = proba_views;
  _nb_cameras = nb_cameras;
  _nb_positions = nb_positions;
  _rectangles = new Rectangle[_nb_cameras * _nb_positions];
}

Room::~Room() {
  delete[] _rectangles;
}

void Room::save_stochastic_view(char *name,
                                int n_camera,
                                const Vector<scalar_t> *proba_presence) const  {

  int view_width = _proba_views[n_camera]->get_width();
  int view_height = _proba_views[n_camera]->get_height();

  RGBImage image(view_width, view_height);
  Array<scalar_t> proba_pixel_off(view_width, view_height);

  for(int px = 0; px < view_width; px++) for(int py = 0; py < view_height; py++)
    proba_pixel_off(px, py) = 1.0;

  Array<bool> dots(view_width, view_height);
  dots.clear();

  for(int n = 0; n < nb_positions(); n++) {
    Rectangle *r = avatar(n_camera, n);
    if(r->visible) {
      for(int py = r->ymin; py < r->ymax; py++)
        for(int px = r->xmin; px < r->xmax; px++)
          proba_pixel_off(px, py) *= (1 - (*proba_presence)[n]);
      if(r->xmin > 0 && r->xmax < view_width-1 && r->ymax < view_height-1)
        dots((r->xmax + r->xmin)/2, r->ymax) = true;
    }
  }

  for(int py = 0; py < view_height; py++) for(int px = 0; px < view_width; px++) {
    scalar_t r, g, b;
    scalar_t a = proba_pixel_off(px, py);

    if(dots(px, py)) { r = 0.0; g = 0.0; b = 0.0; }
    else {
      if(a < 0.5) { r = 0; g = 0; b = 2*a; }
      else        { r = (a - 0.5) * 2; g = (a - 0.5) * 2; b = 1.0; }
    }

    scalar_t c = (*_proba_views[n_camera])(px, py);

    r = c * 0.0 + (1 - c) * r;
    g = c * 0.8 + (1 - c) * g;
    b = c * 0.6 + (1 - c) * b;

    image.set_pixel(px, py, (unsigned char) (255 * r), (unsigned char) (255 * g), (unsigned char) (255 * b));
  }

  image.write_png(name);
}
