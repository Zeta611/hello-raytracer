CXX = clang++
CXXFLAGS = -std=c++2a -Wall -Wextra -Werror -Wpedantic -pedantic-errors
OBJS = color.o material.o light.o sphere.o render.o

%.o : %.c
	$(CXX) -MMD -c -o $@ $< $(CXXFLAGS) -include *.d

run : $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY : clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) run
