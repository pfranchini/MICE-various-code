CC = g++
CFLAGS = -DDEBUG -ggdb -Wall

all: TrackerTof

TrackerTof: TrackerTof.c
	$(CC) $(CFLAGS) TrackerTof.c -o TrackerTof \
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
			      -fdiagnostics-color \
			      -Wno-error
clean:
	rm TrackerTof

