##################################################################
#
#	Makefile for hdf5ToRoot using gcc on Linux. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#	16-Dec-23       CBL     Original
#
#
######################################################################
# Machine specific stuff
#
#
TARGET = hdf5ToRoot
#
# Compile time resolution.
#
INCLUDE = -I$(ROOT_INC) -I$(DRIVE)/common/utility \
	-I/usr/include/hdf5/serial
LIBS = $(ROOT_LIBS) -lutility -lhdf5_cpp -lhdf5
LIBS += -L$(HDF5LIB) -lconfig++


# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp hdf5ToRoot.cpp UserSignals.cpp
SRCS    = $(SRC) $(SRCCPP)

HEADERS = hdf5ToRoot.hh UserSignals.hh Version.hh

# When we build all, what do we build?
all:      $(TARGET)

include $(DRIVE)/common/makefiles/makefile.inc


#dependencies
include make.depend 
# DO NOT DELETE
