CXXFLAGS += $(shell pkg-config --cflags glew)
CXXFLAGS += $(shell pkg-config --cflags sdl2)
#CXXFLAGS += $(shell pkg-config --cflags glfw3)
CXXFLAGS += -g -W -Wall -Wno-unused-parameter -Wno-deprecated-declarations

LDFLAGS += $(shell pkg-config --libs glew)
LDFLAGS += $(shell pkg-config --libs sdl2)
#LDFLAGS += $(shell pkg-config --libs glfw3)
ifeq "$(shell uname)" "Darwin"
    LDFLAGS += -framework OpenGL # -lobjc -framework Foundation -framework Cocoa 
endif

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
	rm -rf *.o *.exe *.exe.dSYM
