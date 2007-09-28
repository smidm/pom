
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

#ifndef POM_SOLVER_H
#define POM_SOLVER_H

#include "misc.h"
#include "integral_proba_view.h"
#include "normal_law.h"
#include "room.h"

class POMSolver {

  // At each pixel the proba for the pixel to be off

  ProbaView neg;

  // At each pixel, 0 if the view is 0, and the proba for the pixel to
  // be off if the view is 1 (or, more mathematically: neg * view)

  ProbaView neg_view;

  // Integral images to speed-up computation

  IntegralProbaView ii_neg, ii_neg_view;

  // Distribution of surface_difference / surface_synthetic

  NormalLaw global_difference;

  void compute_average_images(int camera,
                              Room *room,
                              Vector<scalar_t> *proba_absence);

  // Adds to every sum[i] the value log(P(X_i = 1 | V_camera) / P(X_i
  // = 0 | V_camera)), given the other P(X_j = 1 | V)

  void add_log_ratio(int camera,
                     Room *room,
                     ProbaView *view,
                     Vector<scalar_t> *proba_absence,
                     Vector<scalar_t> *sum);

public:

  POMSolver(Room *room);

  // Uses the computation above for the various cameras and the prior
  // to refresh proba_absence. Iterates as many times as specified.

  void solve(Room *room,
             Vector<scalar_t> *prior,
             Vector<ProbaView *> *views,
             Vector<scalar_t> *proba_presence,
             int nb_frame,
             char *convergence_file_format);
};

#endif
