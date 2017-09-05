CXX := g++
CXXFLAGS = -std=c++11 -g

EXEC_NAME = app

OBJ = main.o parser.o interpreter.o number.o dot.o tree.o
OBJDIR = build/

OBJ_WITH_DIR = $(addprefix $(OBJDIR), $(OBJ))

all: debug

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJDIR)$@

debug: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ_WITH_DIR) -o $(EXEC_NAME)

clean:
	rm -f $(EXEC_NAME) $(OBJDIR)*.o
