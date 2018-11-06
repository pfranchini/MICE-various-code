CC = g++
CFLAGS = -DDEBUG -ggdb -Wall

all : virtual

virtual : virtual.c
	$(CC) $(CFLAGS) virtual.c -o virtual \
	                      -I${MAUS_ROOT_DIR}/ \
	                      -I${MAUS_ROOT_DIR}/src/common_cpp \
	                      -I${MAUS_ROOT_DIR}/src/legacy \
	                      -I${MAUS_THIRD_PARTY}/third_party/install/include \
	                      -I${ROOTSYS}/include \
	                      -L${MAUS_ROOT_DIR}/build/ \
	                      `root-config --ldflags` \
	                      `${ROOTSYS}/bin/root-config --glibs` \
	                      -lMausCpp \
	                      -Wl,--no-as-needed \
			      -fdiagnostics-color

count_virtual : count_virtual.c
	$(CC) $(CFLAGS) count_virtual.c -o count_virtual \
	                      -I${MAUS_ROOT_DIR}/ \
	                      -I${MAUS_ROOT_DIR}/src/common_cpp \
	                      -I${MAUS_ROOT_DIR}/src/legacy \
	                      -I${MAUS_THIRD_PARTY}/third_party/install/include \
	                      -I${ROOTSYS}/include \
	                      -L${MAUS_ROOT_DIR}/build/ \
	                      `root-config --ldflags` \
	                      `${ROOTSYS}/bin/root-config --glibs` \
	                      -lMausCpp \
	                      -Wl,--no-as-needed \
			      -fdiagnostics-color




clean:
	rm virtual

