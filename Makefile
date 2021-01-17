CXX = g++-10
IDIR = /usr/include/lua5.3
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -Werror -Wpedantic -pedantic-errors -I$(IDIR)
LFLAGS = -llua5.3
OBJS = color.o canvas.o material.o light.o sphere.o render.o lua_helpers.o lodepng/lodepng.o

%.o : %.c
	$(CXX) -MMD -c -o $@ $< $(CXXFLAGS) -include *.d

run : $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LFLAGS)

.PHONY : clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) run
