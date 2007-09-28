
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "misc.h"

// We used the same prior at every location, but POMSolver::solve
// takes as parameter a vector of priors if you want
extern scalar_t global_prior;

// The parameters of the Normal law for the conditional image density,
// given the true state. Mu does not appear in the paper, sigma does.
extern scalar_t global_mu_image_density;
extern scalar_t global_sigma_image_density;

// When we iterate the solver, we smooth the estimate to prevent from
// oscillating effects that tend to appear naturally
extern scalar_t global_smoothing_coefficient;

// Ugly parameters for defining the convergence of the solver. I guess
// there are far better ways to do

// Hard bound on the number of iterations
extern int global_max_nb_solver_iterations;

// What error is considered acceptable
extern scalar_t global_error_max;

// How many steps under the global_error_max defines convergence
extern int global_nb_stable_error_for_convergence;

// If the probability of absence is greated than that, ignore the
// avatar in the computation of the average image to save computation
extern scalar_t global_proba_ignored;

#endif
