
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

#include "global.h"

scalar_t global_prior = 0.01;
scalar_t global_mu_image_density = 0.0;
scalar_t global_sigma_image_density = 0.01;
scalar_t global_smoothing_coefficient = 0.8;
int global_max_nb_solver_iterations = 100;
scalar_t global_error_max = 1e-4;
int global_nb_stable_error_for_convergence = 5;
scalar_t global_proba_ignored = 1.00;

