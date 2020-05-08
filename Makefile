CXX = clang++
IDIR = /usr/local/include/lua/
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Wpedantic -pedantic-errors -I$(IDIR)
LFLAGS = -llua
OBJS = color.o material.o light.o sphere.o render.o lua_helpers.o

%.o : %.c
	$(CXX) -MMD -c -o $@ $< $(CXXFLAGS) -include *.d

run : $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LFLAGS)

.PHONY : clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) run
