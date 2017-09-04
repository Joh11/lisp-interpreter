#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <list>
#include <map>
#include <functional>
#include <stdexcept>

#include "parser.hpp"

class interpreter_exception : public std::runtime_error
{
public:
    interpreter_exception(std::string const& what_arg) : std::runtime_error(what_arg) {}
    interpreter_exception(const char* what_arg) : std::runtime_error(what_arg) {};
};


// The class that will interprets and evaluate the given syntax tree
class interpreter
{
public:
    interpreter();
    
    // The method that will interpret the syntax tree
    parser::syntax_tree interpret(parser::syntax_tree tree);
private:
    // Alias to keep it simple

    // A lambda will take in input a variable number of subtrees, and return a syntax_tree
    using lambda = std::function<parser::syntax_tree(std::list<parser::syntax_tree> )>;

    
    // Return true if the string contains only [0 - 9] digits
    static bool is_number(std::string const& str);

    // Same but check for leaf numbers
    static bool is_number(parser::syntax_tree const& t);

    // Look in _functions
    bool is_function(std::string const& str) const;

    // Apply using lambdas in _functions
    parser::syntax_tree apply(std::string const& op, std::list<parser::syntax_tree> arguments);

    // Simplify the creation of numeric functions
    lambda numeric_function(std::function<int(std::list<int>)> f);
    
    // Private member variables

    // A map of functions
    std::map<std::string, lambda> _functions;

};

#endif
