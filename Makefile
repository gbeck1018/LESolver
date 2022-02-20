CXX		:= clang++
CXXFLAGS:= -std=c++17 -Wall -Wextra -Iinc/ -pthread
SOURCES	:= $(wildcard src/*.cpp)
OBJECTS	:= $(patsubst src/%,obj/%,$(patsubst %.cpp,%.o,$(SOURCES)))
DEPENDS	:= $(patsubst src/%,obj/%,$(patsubst %.cpp,%.d,$(SOURCES)))
TARGET	:= lesolver

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(DEPENDS)

obj/%.o: src/%.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
