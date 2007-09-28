
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

#ifndef MISC_H
#define MISC_H

#ifdef DEBUG
#define ASSERT(x, s) if(!(x)) { std::cerr << "ASSERT FAILED IN " << __FILE__ << ":" << __LINE__ << " [" << (s) << "]\n"; abort(); }
#else
#define ASSERT(x, s)
#endif

typedef double scalar_t;

template <class T> T sq(T x) { return x*x; }

const int buffer_size = 1024;

char *next_word(char *buffer, char *r, int buffer_size);

int pomsprintf(char *buffer, int buffer_length, char *format, int n_camera, int n_frame, int n_iteration);

#endif
