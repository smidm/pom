
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

#include "normal_law.h"
#include <cmath>

NormalLaw::~NormalLaw() { }

scalar_t NormalLaw::log_proba(scalar_t x) {
  return normalizer - sq(x - expectation)/(2*variance);
}

scalar_t NormalLaw::proba(scalar_t x) {
  return exp(normalizer - sq(x - expectation)/(2*variance));
}

void NormalLaw::set(scalar_t e, scalar_t v) {
  expectation = e;
  variance = v;
  normalizer = - 0.5 * log(variance * 2 * M_PI);
}
