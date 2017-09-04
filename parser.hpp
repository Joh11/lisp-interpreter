#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <stdexcept>

#include "constants.hpp"
#include "tree.hpp"

// An exception class used to specify errors that occured during parse
class syntax_error : public std::runtime_error
{
public:
    syntax_error(std::string const& what_arg) : runtime_error(what_arg) {}
    syntax_error(const char* what_arg) : runtime_error(what_arg) {}
};


// A class that will parse the given LISP input
class parser
{
public:
    // An alias to simplify
    using syntax_tree = tree_node<std::string>;
    
    // Tokenize a string
    token_table tokenize(std::string input);

    // Turn the token table into a tree of expressions
    syntax_tree parenthesize(token_table const& tokens);

    // Combine the two previous methods
    syntax_tree parse(std::string input);
};

#endif
