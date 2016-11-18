#ifeq "$(shell uname)" "Darwin"
    LIBGL=  -framework OpenGL
#else
    LIBGL= -lGLU -lGL
#endif

CXXFLAGS += `pkg-config glew --cflags` `sdl2-config --cflags` -g -W -Wall -Wno-unused-parameter -Wno-deprecated-declarations
LDFLAGS += `pkg-config glew --libs` `sdl2-config --libs` $(LIBGL)

all : main.exe
run : main.exe
	./main.exe
main.exe : main.cpp *.h
	$(CXX) main.cpp $(CXXFLAGS) $(LDFLAGS) -o$@

sol : main_solution.exe
runs : main_solution.exe
	./main_solution.exe
main_solution.exe : main_solution.cpp *.h
	$(CXX) main_solution.cpp $(CXXFLAGS) $(LDFLAGS) -o $@

clean :
	rm -f *.o *.exe
