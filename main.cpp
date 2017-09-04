#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

#include "parser.hpp"
#include "tree.hpp"
#include "interpreter.hpp"

// Load a file by evaluating all the lines in it.
void load_file(std::string const& path, parser & p, interpreter & i);

int main(int argc, char *argv[])
{
    parser p;
    interpreter i;

    for(int file = 1 ; file < argc ; ++file)
	load_file(argv[file], p, i); // Load all the files given as arguments

    std::cout << "LISP Interpreter" << std::endl;

    while(true)
    {
	std::string str;
	std::getline(std::cin, str);

	if(str == "QUIT")
	    break;

	std::cout << "==> "  << i.interpret(p.parse(str)) << std::endl;
    }

    return 0;
}

void load_file(std::string const& path, parser & p, interpreter & i)
{
    // Load the file
    std::ifstream file(path);

    if(!file.is_open())
	throw std::runtime_error("No such file : " + path);

    std::cout << "LOAD FILE : " << path << std::endl;

    std::string expr;
    while(std::getline(file, expr))
    {
	std::cout << "==> " << i.interpret(p.parse(expr)) << std::endl;
    }
}
