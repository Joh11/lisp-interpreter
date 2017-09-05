#ifndef DOT_HPP
#define DOT_HPP

#include "parser.hpp"

// A file to handle everything about the dotted pair notation (a . b)


// A function that will recursively convert all {a, ., b} to .{a, b}
void parse_dot(parser::syntax_tree & tree);

// A function that will build a dot tree using the two given args
parser::syntax_tree build_dot(parser::syntax_tree const& a, parser::syntax_tree const& b);

// Return true if the tree is labeled ".", and has two subtrees
bool is_dot(parser::syntax_tree const& tree);

// If the given tree is a dot pair then return the first one
// If it is a list return the first element
// If it is a leaf then error
parser::syntax_tree car(parser::syntax_tree const& tree);

// If the given tree is a dot pair then return the second one
// If it is a list return the list without the first element
// If it is a leaf then error
parser::syntax_tree cdr(parser::syntax_tree const& tree);

#endif
