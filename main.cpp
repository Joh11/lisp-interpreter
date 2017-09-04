#include <iostream>
#include <string>

#include "parser.hpp"
#include "tree.hpp"
#include "interpreter.hpp"

int main(int argc, char **argv)
{
    parser p;
    interpreter i;

    std::string str;
    std::getline(std::cin, str);

    std::cout << "Bonjour" << std::endl;

    auto t = p.parse(str);
    std::cout << "Bonjour" << std::endl;
    
    std::cout << i.interpret(t) << std::endl;

    /*
    // Test for trees
    
    // Meant tree : (lambda (x y) x y)
    using t = tree_node<std::string>;

    t tree{};
    tree.push_back(t{"lambda"});

    t sub{};
    sub.push_back(t{"x"});
    sub.push_back(t{"y"});

    tree.push_back(sub);

    tree.push_back(t{"x"});
    tree.push_back(t{"y"});
    
    std::cout << tree;
    */
    return 0;
}
