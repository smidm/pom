
#==============================================================================#
# This program is free software: you can redistribute it and/or modify	       #
# it under the terms of the version 3 of the GNU General Public License	       #
# as published by the Free Software Foundation.				       #
# 									       #
# This program is distributed in the hope that it will be useful, but	       #
# WITHOUT ANY WARRANTY; without even the implied warranty of		       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU	       #
# General Public License for more details.				       #
# 									       #
# You should have received a copy of the GNU General Public License	       #
# along with this program. If not, see <http://www.gnu.org/licenses/>.	       #
# 									       #
# Written by Francois Fleuret					               #
# (C) Ecole Polytechnique Federale de Lausanne                                 #
# Contact <pom@epfl.ch> for comments & bug reports	                       #
#==============================================================================#

ifeq ($(STATIC),yes)
  LDFLAGS=-static -lm -lpng -lz
else
  LDFLAGS=-lm -lpng
endif

ifeq ($(DEBUG),yes)
  OPTIMIZE_FLAG = -g -DDEBUG
else
  OPTIMIZE_FLAG = -g -O3
endif

ifeq ($(PROFILE),yes)
  PROFILE_FLAG = -pg
endif

CXXFLAGS = -Wall $(OPTIMIZE_FLAG) $(PROFILE_FLAG)

all: pom

pom.tgz:
	cd .. ; tar zcvf pom/pom.tgz pom/*.cc pom/*.h pom/Makefile pom/images/*.png pom/test.pom pom/*.txt

pom: pom.o global.o pom_solver.o proba_view.o normal_law.o rectangle.o room.o misc.o rgb_image.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Makefile.depend: *.h *.cc Makefile
	$(CC) -M *.cc > Makefile.depend

clean:
	\rm -f pom *.o Makefile.depend

-include Makefile.depend
