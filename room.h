
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

#ifndef ROOM_H
#define ROOM_H

#include "rectangle.h"
#include "proba_view.h"
#include "vector.h"

using namespace std;

class Room {

  int _nb_cameras, _nb_positions;
  int _view_width, _view_height;

  Rectangle *_rectangles;

public:

  Room(int view_width, int view_height, int nb_cameras, int nb_positions);
  ~Room();

  inline int nb_positions() const { return _nb_positions; }
  inline int nb_cameras() const { return _nb_cameras; }
  inline int view_width() const { return _view_width; }
  inline int view_height() const { return _view_height; }

  inline Rectangle *avatar(int n_camera, int n_position) const {
    ASSERT(n_camera >= 0 && n_camera < _nb_cameras &&
           n_position >= 0 && n_position < _nb_positions,
           "Index out of bounds");
    return _rectangles + n_camera * _nb_positions + n_position;
  }

  void save_stochastic_view(char *name, int ncam, const ProbaView *view,
                            const Vector<scalar_t> *proba_presence) const;
};

#endif
