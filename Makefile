CXX = g++
CXXFLAGS = -g
LIBS = -lm
LDFLAGS =
CPPFLAGS = -I. 

SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

# Various commands
CXX_COMMAND = ${CXX}

SRCS = GridBase.cpp GridData.cpp GridList.cpp GridNMC.cpp GridLaLo.cpp GridNMC4.cpp optlist.cpp
LIBOBJS = GridBase.o GridData.o GridList.o GridNMC.o GridLaLo.o GridNMC4.o

all: nc2mtc mtc2nc

.cpp.o: 
	$(CXX_COMMAND) -c $(CXXFLAGS) $(CPPFLAGS) $<

libgrid.a: $(LIBOBJS)
	rm -f libgrid.a
	ar cr libgrid.a $(LIBOBJS)
	ranlib libgrid.a

nc2mtc: nc2mtc.cpp libgrid.a optlist.o
	g++ -g -o nc2mtc nc2mtc.cpp optlist.o -L./ -lgrid -lnetcdf_c++

agradsa: agradsa.cpp	libgrid.a
	g++ -g -o agradsa agradsa.cpp -L./ -lgrid

mtc2nc: mtc2nc.cpp libgrid.a
	g++ -g -o mtc2nc mtc2nc.cpp -L./ -lgrid -lnetcdf_c++

mtc2cdl: mtc2cdl.cpp
	g++ -g -o mtc2cdl.cpp -L./ -lgrid

install: nc2mtc
	cp libgrid.a /usr/local/lib/
	cp *.h	/usr/local/include/
	cp nc2mtc /usr/local/bin/

uninstall:
	rm /usr/local/lib/libgrid.a
	rm /usr/local/bin/nc2mtc
	rm /usr/local/include/Grid.h

clean: 
	rm -f *.o *~ *.a agradsa libgrid.a nc2mtc mtc2nc mtc2cdl

