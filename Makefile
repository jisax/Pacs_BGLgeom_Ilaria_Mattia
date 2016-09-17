SRCS=$(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)
CPPFLAGS ?= -I$(mkBGLInc) -I/home/pacs_student/Mattia/Progetto/Pacs_project_Ilaria_Mattia/include
LDFLAGS ?= 
LIBS = 
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

TARGET_DIR = bin
BUILD_DIR = objs
SOURCE_DIR = src

-include Makefile.inc

# $(EXEC) : $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

# $(OBJS) : $(SRCS) $(HEADERS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: all clean distclean Zunino Formaggia

all: Zunino Formaggia

Zunino: main_Zunino

main_Zunino: main_Zunino.o
	$(CXX) $(LDFLAGS) $(BUILD_DIR)/main_Zunino.o -o $(TARGET_DIR)/$@ $(LIBS)
main_Zunino.o: ./$(SOURCE_DIR)/main_Zunino.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@
	
Formaggia: main_Formaggia

main_Formaggia: main_Formaggia.o
	$(CXX)
main_Formaggia.o: main_Formaggia.cpp
	$(CXX)
	
doc:
	doxygen $(DOXYFILE)



clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) $(EXEC)
