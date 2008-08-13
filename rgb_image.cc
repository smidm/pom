
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
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include <libpng/png.h>

#include "rgb_image.h"

void RGBImage::allocate() {
  _bit_plans = new unsigned char **[RGB_DEPTH];
  _bit_lines = new unsigned char *[RGB_DEPTH * _height];
  _bit_map = new unsigned char [_width * _height * RGB_DEPTH];
  for(int k = 0; k < RGB_DEPTH; k++) _bit_plans[k] = _bit_lines + k * _height;
  for(int k = 0; k < RGB_DEPTH * _height; k++) _bit_lines[k] = _bit_map + k * _width;
}

void RGBImage::deallocate() {
  delete[] _bit_plans;
  delete[] _bit_lines;
  delete[] _bit_map;
}

RGBImage::RGBImage() : _bit_plans(0), _bit_lines(0), _bit_map(0) { }

RGBImage::RGBImage(int width, int height) : _width(width), _height(height) {
  allocate();
  memset(_bit_map, 0, _width * _height * RGB_DEPTH * sizeof(unsigned char));
}

RGBImage::~RGBImage() {
  deallocate();
}

void RGBImage::write_ppm(const char *filename) {
  FILE *outfile;

  if ((outfile = fopen (filename, "wb")) == 0) {
    fprintf (stderr, "Can't open %s for reading\n", filename);
    exit(1);
  }

  fprintf(outfile, "P6\n%d %d\n255\n", _width, _height);

  char *raw = new char[_width * _height * 3];

  int k = 0;
  for(int y = 0; y < _height; y++) for(int x = 0; x < _width; x++) {
    raw[k++] = _bit_map[x + _width * (y + _height * RED)];
    raw[k++] = _bit_map[x + _width * (y + _height * GREEN)];
    raw[k++] = _bit_map[x + _width * (y + _height * BLUE)];
  }

  fwrite((void *) raw, sizeof(unsigned char), _width * _height * 3, outfile);
  fclose(outfile);

  delete[] raw;
}

void RGBImage::read_ppm(const char *filename) {
  const int buffer_size = 1024;
  FILE *infile;
  char buffer[buffer_size];
  int max;

  deallocate();

  if((infile = fopen (filename, "r")) == 0) {
    fprintf (stderr, "Can't open %s for reading\n", filename);
    exit(1);
  }

  fgets(buffer, buffer_size, infile);

  if(strncmp(buffer, "P6", 2) == 0) {

    do {
      fgets(buffer, buffer_size, infile);
    } while((buffer[0] < '0') || (buffer[0] > '9'));
    sscanf(buffer, "%d %d", &_width, &_height);
    fgets(buffer, buffer_size, infile);
    sscanf(buffer, "%d", &max);

    allocate();

    unsigned char *raw = new unsigned char[_width * _height * RGB_DEPTH];
    fread(raw, sizeof(unsigned char), _width * _height * RGB_DEPTH, infile);

    int k = 0;
    for(int y = 0; y < _height; y++) for(int x = 0; x < _width; x++) {
      _bit_plans[RED][y][x] = raw[k++];
      _bit_plans[GREEN][y][x] = raw[k++];
      _bit_plans[BLUE][y][x] = raw[k++];
    }

    delete[] raw;

  } else if(strncmp(buffer, "P5", 2) == 0) {

    do {
      fgets(buffer, buffer_size, infile);
    } while((buffer[0] < '0') || (buffer[0] > '9'));
    sscanf(buffer, "%d %d", &_width, &_height);
    fgets(buffer, buffer_size, infile);
    sscanf(buffer, "%d", &max);

    allocate();

    unsigned char *pixbuf = new unsigned char[_width * _height];
    fread(buffer, sizeof(unsigned char), _width * _height, infile);

    int k = 0, l = 0;
    for(int y = 0; y < _height; y++) for(int x = 0; x < _width; x++) {
      unsigned char c = pixbuf[k++];
      _bit_map[l++] = c;
      _bit_map[l++] = c;
      _bit_map[l++] = c;
    }

    delete[] pixbuf;

  } else {
    cerr << "Can not read ppm of type [" << buffer << "] from " << filename << ".\n";
    exit(1);
  }
}

void RGBImage::read_png(const char* filename) {
  // This is the number of bytes the read_png routine will read to
  // decide if the file is a PNG or not. According to the png
  // documentation, it can be 1 to 8 bytes, 8 being the max and the
  // best.

  const int header_size = 8;

  png_byte header[header_size];
  png_bytep *row_pointers;

  deallocate();

  // open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    cerr << "Unable to open file " << filename << " for reading.\n";
    exit(1);
  }

  // read header
  fread(header, 1, header_size, fp);
  if (png_sig_cmp(header, 0, header_size)) {
    cerr << "File " << filename << " does not look like PNG.\n";
    fclose(fp);
    exit(1);
  }

  // create png pointer
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png_ptr) {
    cerr << "png_create_read_struct failed\n";
    fclose(fp);
    exit(1);
  }

  // create png info struct
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, (png_infopp) 0, (png_infopp) 0);
    cerr << "png_create_info_struct failed\n";
    fclose(fp);
    exit(1);
  }

  // get image info
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, header_size);
  png_read_info(png_ptr, info_ptr);

  _width = info_ptr->width;
  _height = info_ptr->height;

  png_byte bit_depth, color_type, channels;
  color_type = info_ptr->color_type;
  bit_depth = info_ptr->bit_depth;
  channels = info_ptr->channels;

  if(bit_depth != 8) {
    cerr << "Can only read 8-bits PNG images." << endl;
    exit(1);
  }

  // allocate image pointer
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * _height);
  for (int y = 0; y < _height; y++)
    row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);

  allocate();

  // read image
  png_read_image(png_ptr, row_pointers);

  // send image to red, green and blue buffers
  switch (color_type) {
  case PNG_COLOR_TYPE_GRAY:
    {
      unsigned char pixel = 0;
      for (int y = 0; y < _height; y++) for (int x = 0; x < _width; x++) {
        pixel = row_pointers[y][x];
        _bit_plans[RED][y][x] = pixel;
        _bit_plans[GREEN][y][x] = pixel;
        _bit_plans[BLUE][y][x] = pixel;
      }
    }
    break;

  case PNG_COLOR_TYPE_GRAY_ALPHA:
    cerr << "PNG type GRAY_ALPHA not supported.\n";
    exit(1);
    break;

  case PNG_COLOR_TYPE_PALETTE:
    cerr << "PNG type PALETTE not supported.\n";
    exit(1);
    break;

  case PNG_COLOR_TYPE_RGB:
    {
      if(channels != RGB_DEPTH) {
        cerr << "Unsupported number of channels for RGB type\n";
        break;
      }
      int k;
      for (int y = 0; y < _height; y++) {
        k = 0;
        for (int x = 0; x < _width; x++) {
          _bit_plans[RED][y][x] = row_pointers[y][k++];
          _bit_plans[GREEN][y][x] = row_pointers[y][k++];
          _bit_plans[BLUE][y][x] = row_pointers[y][k++];
        }
      }
    }
    break;

  case PNG_COLOR_TYPE_RGB_ALPHA:
    cerr << "PNG type RGB_ALPHA not supported.\n";
    exit(1);
    break;

  default:
    cerr << "Unknown PNG type\n";
    exit(1);
  }

  // release memory
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);

  for (int y = 0; y < _height; y++) free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}

void RGBImage::write_png(const char *filename) {
  png_bytep *row_pointers;

  // create file
  FILE *fp = fopen(filename, "wb");

  if (!fp) {
    cerr << "Unable to create image '" << filename << "'\n";
    exit(1);
  }

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

  if (!png_ptr) {
    cerr << "png_create_write_struct failed\n";
    fclose(fp);
    exit(1);
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    cerr << "png_create_info_struct failed\n";
    fclose(fp);
    exit(1);
  }

  png_init_io(png_ptr, fp);

  png_set_IHDR(png_ptr, info_ptr, _width, _height,
               8, 2, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  // allocate memory
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * _height);
  for (int y = 0; y < _height; y++)
    row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);

  int k;
  for (int y = 0; y < _height; y++) {
    k = 0;
    for (int x = 0; x < _width; x++) {
      row_pointers[y][k++] = _bit_map[x + _width * (y + _height * RED)];
      row_pointers[y][k++] = _bit_map[x + _width * (y + _height * GREEN)];
      row_pointers[y][k++] = _bit_map[x + _width * (y + _height * BLUE)];
    }
  }

  png_write_image(png_ptr, row_pointers);
  png_write_end(png_ptr, 0);

  png_destroy_write_struct(&png_ptr, &info_ptr);

  // cleanup heap allocation
  for (int y = 0; y < _height; y++) free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}
