
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

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

#include "pom_solver.h"
#include "global.h"

//////////////////////////////////////////////////////////////////////

POMSolver::POMSolver(Room *room) : neg(room->view_width(), room->view_height()),
                                   neg_view(room->view_width(), room->view_height()),
                                   ii_neg(room->view_width(), room->view_height()),
                                   ii_neg_view(room->view_width(), room->view_height()) {
  global_difference.set(global_mu_image_density, global_sigma_image_density);
}

//////////////////////////////////////////////////////////////////////

void POMSolver::compute_average_images(int camera,
                                       Room *room,
                                       Vector<scalar_t> *proba_absence) {
  neg.fill(1.0);

  for(int n = 0; n < room->nb_positions(); n++) if((*proba_absence)[n] <= global_proba_ignored) {
    Rectangle *r = room->avatar(camera, n);
    if(r->visible)
      neg.multiply_subarray(r->xmin, r->ymin, r->xmax + 1, r->ymax + 1, (*proba_absence)[n]);
  }
}

//////////////////////////////////////////////////////////////////////

void POMSolver::add_log_ratio(int camera,
                              Room *room,
                              ProbaView *view,
                              Vector<scalar_t> *proba_absence,
                              Vector<scalar_t> *sum) {

  // Computes the average on the complete picture

  compute_average_images(camera, room, proba_absence);

  double s = ii_neg.compute_sum(&neg);
  double sv = ii_neg_view.compute_sum(&neg, view);

  scalar_t noise_proba = 0.01; // 1% of the scene can remain unexplained
  scalar_t average_surface = room->view_width() * room->view_height() * (1 + noise_proba) - s;
  scalar_t average_diff = average_surface + sv;

  // Cycles throw all positions and adds the log likelihood ratio to
  // the total sum for each

  for(int i = 0; i < room->nb_positions(); i++) {
    Rectangle *r = room->avatar(camera, i);
    if(r->visible) {
      scalar_t lambda = 1 - 1/(*proba_absence)[i];

      scalar_t integral_neg = ii_neg.integral(r->xmin, r->ymin, r->xmax + 1, r->ymax + 1);
      scalar_t average_surface_givpre = average_surface +          integral_neg;
      scalar_t average_surface_givabs = average_surface + lambda * integral_neg;

      scalar_t integral_neg_view = ii_neg_view.integral(r->xmin, r->ymin, r->xmax + 1, r->ymax + 1);
      scalar_t average_diff_givpre = average_diff +           integral_neg - 2 * integral_neg_view;
      scalar_t average_diff_givabs = average_diff + lambda * (integral_neg - 2 * integral_neg_view);

      scalar_t log_mu0 = global_difference.log_proba(average_diff_givabs / average_surface_givabs);
      scalar_t log_mu1 = global_difference.log_proba(average_diff_givpre / average_surface_givpre);

      (*sum)[i] += log_mu1 - log_mu0;

    }
  }
}

void POMSolver::solve(Room *room,
                      Vector<scalar_t> *prior,
                      Vector<ProbaView *> *views,
                      Vector<scalar_t> *proba_presence,
                      int nb_frame,
                      char *convergence_file_format) {

  Vector<scalar_t> log_prior_ratio(prior->length());

  Vector<scalar_t> sum(room->nb_positions());
  Vector<scalar_t> proba_absence(room->nb_positions());

  for(int i  = 0; i < room->nb_positions(); i++) {
    log_prior_ratio[i] = log((*prior)[i]/(1 - (*prior)[i]));
    proba_absence[i] = 1 - (*prior)[i];
  }

  int nb_stab = 0;

  for(int it = 0; (nb_stab < global_nb_stable_error_for_convergence) && (it < global_max_nb_solver_iterations); it++) {

    sum.clear();
    for(int c = 0; c < room->nb_cameras(); c++)
      add_log_ratio(c, room, (*views)[c], &proba_absence, &sum);

    scalar_t e = 0;
    for(int i = 0; i < room->nb_positions(); i++) {
      scalar_t np = global_smoothing_coefficient * proba_absence[i] + (1 - global_smoothing_coefficient) / (1 + exp(log_prior_ratio[i] + sum[i]));
      if(abs(proba_absence[i] - np) > e) e = abs(proba_absence[i] - np);
      proba_absence[i] = np;
    }

    if(e < global_error_max) nb_stab++; else nb_stab = 0;

    if(convergence_file_format) {
      char buffer[buffer_size];
      for(int p = 0; p < room->nb_positions(); p++) (*proba_presence)[p] = 1 - proba_absence[p];
      for(int c = 0; c < room->nb_cameras(); c++) {
        pomsprintf(buffer, buffer_size, convergence_file_format, c, nb_frame, it);
        cout << "Saving " << buffer << "\n"; cout.flush();
        room->save_stochastic_view(buffer, c, (*views)[c], proba_presence);
      }
    }

  }

  for(int p = 0; p < room->nb_positions(); p++) (*proba_presence)[p] = 1 - proba_absence[p];
}
