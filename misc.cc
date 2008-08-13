
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
#include <stdlib.h>

using namespace std;

#include "misc.h"

char *next_word(char *buffer, char *r, int buffer_size) {
  char *s;
  s = buffer;
  if(r != 0) {
    while((*r == ' ') || (*r == '\t') || (*r == ',')) r++;
    if(*r == '"') {
      r++;
      while((*r != '"') && (*r != '\0') &&
            (s<buffer+buffer_size-1))
        *s++ = *r++;
      if(*r == '"') r++;
    } else {
      while((*r != '\r') && (*r != '\n') && (*r != '\0') &&
            (*r != '\t') && (*r != ' ') && (*r != ',')) {
        if(s == buffer + buffer_size) {
          cerr << "Buffer overflow in next_word." << endl;
          exit(1);
        }
        *s++ = *r++;
      }
    }

    while((*r == ' ') || (*r == '\t') || (*r == ',')) r++;
    if((*r == '\0') || (*r=='\r') || (*r=='\n')) r = 0;
  }
  *s = '\0';

  return r;
}

int pomsprintf(char *buffer, int buffer_length, char *format, int n_camera, int n_frame, int n_iteration) {
  char *s = buffer, *t = format;

  while(*t && s < buffer + buffer_length - 1) {
    if(*t == '%') {
      t++;
      int v;
      switch(*t) {
      case 'c':
        v = n_camera;
        t++;
        break;
      case 'f':
        v = n_frame;
        t++;
        break;
      case 'i':
        v = n_iteration;
        t++;
        break;
      default:
        cerr << "Unknown format type in " << format << "." << endl;
        exit(1);
      }

      s += snprintf(s, buffer + buffer_length - s, "%d", v);
    } else *(s++) = *(t++);
  }

  *(s++) = '\0';

  return s - buffer;
}
