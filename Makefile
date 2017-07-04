CXX = g++
CPPFLAGS = -Wall -O3 -std=c++11 -I include/
LDFLAGS = -lGL -lGLU -lglut -lm
    
.cpp:
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

all: main

main: main.cpp algebra3.cpp utilitarios.cpp cubo.cpp

.PHONY: clean realclean 

clean:
	-rm -f *.o *~ 
	-rm main


