#include <iostream>
#include <string>

#include "parser.hpp"
#include "tree.hpp"
#include "interpreter.hpp"

int main(int argc, char **argv)
{
    parser p;
    interpreter i;

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
