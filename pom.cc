
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
// Modified by Matej Smid <smidm@cmp.felk.cvut.cz>							               	//
//////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

using namespace std;

#include "misc.h"
#include "global.h"
#include "vector.h"
#include "room.h"
#include "pom_solver.h"

void check_parameter(char *s, int line_number, char *buffer) {
  if(!s) {
    cerr << "Missing parameter line " << line_number << ":" << endl;
    cerr << buffer << endl;
    exit(1);
  }
}

int main(int argc, char **argv) {

  if(argc > 2) {
    cerr << argv[0] << " [-h | --help | <configuration file>]" << endl;
    exit(1);
  }

  ifstream *configuration_file = 0;
  istream *input_stream;

  if(argc > 1) {
    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
      cout << argv[0] << " [-h | --help | <configuration file>]" << endl
           << endl
           << "  If a configuration file name is provided, the program processes it" << endl
           << "  and prints information about the files it generates. Otherwise, it" << endl
           << "  reads the standard input and does not produce any printout on the" << endl
           << "  standard output unless an error occurs." << endl
           << endl;
      exit(0);
    }
    configuration_file = new ifstream(argv[1]);
    if(configuration_file->fail()) {
      cerr << "Can not open " << argv[1] << " for reading." << endl;
      exit(1);
    }
    input_stream = configuration_file;
  } else input_stream = &cin;

  char input_view_format[buffer_size] = "";
  char result_format[buffer_size] = "";
  char result_view_format[buffer_size] = "";
  char convergence_view_format[buffer_size] = "";

  char buffer[buffer_size], token[buffer_size];

  int line_number = 0;
  Vector<ProbaView *> *proba_views = NULL;
  int nb_positions = -1;
  int nb_cameras = -1;
  Vector<int> *camera_names = NULL;


  Room *room = NULL;

  while(!input_stream->eof()) {

    input_stream->getline(buffer, buffer_size);
    line_number++;

    char *s = buffer;
    s = next_word(token, s, buffer_size);

    if(strcmp(token, "ROOM") == 0) {
      int view_width = -1, view_height = -1;

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      view_width = atoi(token);

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      view_height = atoi(token);

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      nb_cameras = atoi(token);

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      nb_positions = atoi(token);

      if(proba_views != NULL) {
        cerr << "Room already defined, line" << line_number << "." << endl;
        exit(1);
      }

      proba_views = new Vector<ProbaView *>(nb_cameras);
      if((view_width == -1) || (view_height == -1)) {
        // view dimension inconsistent
        for(int c = 0; c < proba_views->length(); c++)
          (*proba_views)[c] = NULL;
      } else {        
        // view dimensions consistent for the all cameras
        for(int c = 0; c < proba_views->length(); c++)
          (*proba_views)[c] = new ProbaView(view_width, view_height);
      }
      camera_names = new Vector<int>(nb_cameras);
      for(int c = 0; c < nb_cameras; c++) {
        (*camera_names)[c] = -1;
      }
    }

    else if(strcmp(token, "CAMERA") == 0) {
      int view_width = -1, view_height = -1;
      int camera_name = -1;

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      camera_name = atoi(token);

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      view_width = atoi(token);

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      view_height = atoi(token);

      if(proba_views == NULL) {
        cerr << "Room not defined, you must specify room before CAMERA lines." << endl;
        exit(1);
      }
      int free_idx = camera_names->find(-1);
      if(free_idx == -1) {
        cerr << "More CAMERA definitions as specified using the ROOM keyword <number of cameras> parameter." << endl;
        exit(1);
      }
      (*camera_names)[free_idx] = camera_name;
      (*proba_views)[free_idx] = new ProbaView(view_width, view_height);
    }

    else if(strcmp(token, "CONVERGENCE_VIEW_FORMAT") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(convergence_view_format, s, buffer_size);
    }

    else if(strcmp(token, "INPUT_VIEW_FORMAT") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(input_view_format, s, buffer_size);
    }

    else if(strcmp(token, "RESULT_VIEW_FORMAT") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(result_view_format, s, buffer_size);
    }

    else if(strcmp(token, "RESULT_FORMAT") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(result_format, s, buffer_size);
    }

    else if(strcmp(token, "PROCESS") == 0) {
      RGBImage tmp;
      int first_frame, nb_frames;

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      first_frame = atoi(token);

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      nb_frames = atoi(token);
      
      if(proba_views == NULL) {
        cerr << "Room not defined, you must specify room before PROCESS line." << endl;
        exit(1);
      }

      POMSolver solver(room);

      Vector<scalar_t> prior(room->nb_positions());
      Vector<scalar_t> proba_presence(room->nb_positions());
      for(int i = 0; i < room->nb_positions(); i++) prior[i] = global_prior;

      if(strcmp(input_view_format, "") == 0) {
        cerr << "You must specify the input view format." << endl;
        exit(1);
      }

      for(int f = first_frame; f < first_frame + nb_frames; f++) {

        if(configuration_file)
          cout << "Processing frame " << f << endl;

        for(int c = 0; c < room->nb_cameras(); c++) {
          pomsprintf(buffer, buffer_size, input_view_format, (*camera_names)[c], f, 0);
          tmp.read_png(buffer);
          (*proba_views)[c]->from_image(&tmp);
        }

        if(strcmp(convergence_view_format, "") != 0)
          solver.solve(room, &prior, &proba_presence, f, convergence_view_format);
        else
          solver.solve(room, &prior, &proba_presence, f, 0);

        if(strcmp(result_view_format, "") != 0) {
          for(int c = 0; c < room->nb_cameras(); c++) {
            pomsprintf(buffer, buffer_size, result_view_format, (*camera_names)[c], f, 0);
            if(configuration_file)
              cout << "Saving " << buffer << endl;
            room->save_stochastic_view(buffer, c, &proba_presence);
          }
        }

        if(strcmp(result_format, "") != 0) {
          pomsprintf(buffer, buffer_size, result_format, 0, f, 0);
          ofstream result(buffer);
          if(result.fail()) {
            cerr << "Can not open " << token << " for writing." << endl;
            exit(1);
          }
          if(configuration_file)
            cout << "Saving " << buffer << endl;
          for(int i = 0; i < room->nb_positions(); i++)
            result << i << " " << proba_presence[i] << endl;
          result.flush();
        }
      }
    }

    else if(strcmp(token, "RECTANGLE") == 0) {
      int camera_name, n_position;

      if(proba_views == NULL) {
        cerr << "You must define a room before adding rectangles, line" << line_number << "." << endl;
        exit(1);
      }
      if(room == NULL) {
        if (-1 != proba_views->find(NULL)) {
          cerr << "You must define all CAMERA views dimensions before adding rectangles." << endl;
          exit(1);
        }
        room = new Room(nb_cameras, nb_positions, *proba_views);
      }


      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      camera_name = atoi(token);

      int n_camera = camera_names->find(camera_name);
      if(-1 == n_camera) {
        cerr << "Camera " << camera_name << " not defined, line " << line_number << "." << endl;
        exit(1);
      }

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      n_position = atoi(token);

      if(n_position < 0 || n_position >= room->nb_positions()) {
        cerr << "Out of range position number line " << line_number << "." << endl;
        exit(1);
      }

      Rectangle *current = room->avatar(n_camera, n_position);

      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      if(strcmp(token, "notvisible") == 0) {
        current->visible = false;
        current->xmin = -1;
        current->ymin = -1;
        current->xmax = -1;
        current->ymax = -1;
      } else {
        current->visible = true;
        current->xmin = atoi(token);
        check_parameter(s, line_number, buffer);
        s = next_word(token, s, buffer_size);
        current->ymin = atoi(token);
        check_parameter(s, line_number, buffer);
        s = next_word(token, s, buffer_size);
        current->xmax = atoi(token);
        check_parameter(s, line_number, buffer);
        s = next_word(token, s, buffer_size);
        current->ymax = atoi(token);

        if(current->xmin < 0 || current->xmax >= room->view_width(n_camera) ||
           current->ymin < 0 || current->ymax >= room->view_height(n_camera)) {
          cerr << "Rectangle out of bounds, line " << line_number << endl;
          exit(1);
        }
      }
    }

    else if(strcmp(token, "PRIOR") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      global_prior = atof(token);
    }

    else if(strcmp(token, "SIGMA_IMAGE_DENSITY") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      global_sigma_image_density = atof(token);
    }

    else if(strcmp(token, "SMOOTHING_COEFFICIENT") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      global_smoothing_coefficient = atof(token);
    }

    else if(strcmp(token, "MAX_NB_SOLVER_ITERATIONS") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      global_max_nb_solver_iterations = atoi(token);
    }

    else if(strcmp(token, "ERROR_MAX") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      global_error_max = atof(token);
    }

    else if(strcmp(token, "NB_STABLE_ERROR_FOR_CONVERGENCE") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      global_nb_stable_error_for_convergence = atoi(token);
    }

    else if(strcmp(token, "PROBA_IGNORED") == 0) {
      check_parameter(s, line_number, buffer);
      s = next_word(token, s, buffer_size);
      global_proba_ignored = atof(token);
    }

    else if(strcmp(buffer, "") == 0 || buffer[0] == '#') { }

    else {
      cerr << "Unknown token " << token << "." << endl;
      exit(1);
    }
  }

  if(proba_views)
    for(int c = 0; c < proba_views->length(); c++) delete (*proba_views)[c];

  delete proba_views;
  delete room;

  delete configuration_file;
}
