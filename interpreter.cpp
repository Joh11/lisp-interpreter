#include <list>
#include <map>
#include <iostream>
#include <stack>
#include <regex>
#include <algorithm>

#include "utils.hpp"
#include "interpreter.hpp"

interpreter::interpreter()
{
    // Fill the function map
    /*
    _functions["+"] = [](std::list<parser::syntax_tree> args)
	{
	    int value = 0;
	    for(auto const& arg : args)
		value += std::stoi(arg.value);
	    return parser::syntax_tree{std::to_string(value)};
	};
    */

    _functions["+"] = numeric_function([](std::list<int> args)
				       {
					   int value = 0;
					   for(auto const& n : args)
					       value += n;
					   return value;
				       });

    _functions["*"] = numeric_function([](std::list<int> args)
				       {
					   int value = 1;
					   for(auto const& n : args)
					       value *= n;
					   return value;
				       });
}

parser::syntax_tree interpreter::interpret(parser::syntax_tree tree)
{
    // If the tree is a leaf we cannot do anything so quit
    if(tree.is_leaf())
    {
	return tree;
    }


    // Two choices : the first child is a subtree or a leaf
    if(tree.subtree_begin()->is_leaf())
    {
	std::string name{tree.subtree_begin()->value};
	// Use the first arg as a function
	// If it cannot then error
	if(!is_function(name))
	    throw interpreter_exception("The object '" + name + "' is not applicable. Error");

	// Build the argument list
	auto beginArgs{tree.subtree_begin()};
	++beginArgs;
	// We unfortunately have to specify the template types explicitely
	std::list<parser::syntax_tree> args{sub_list<decltype(beginArgs), std::list<parser::syntax_tree> >(beginArgs, tree.subtree_end())};

	// We then have to evaluate all the arguments
	std::for_each(args.begin(), args.end(), [this](parser::syntax_tree & t){t = interpret(t);});

	// We apply !
	tree = apply(name, args);
    }
    
    return tree;
}


bool interpreter::is_number(std::string const& str)
{
    return std::regex_match(str, std::regex("\\d+"));
}

bool interpreter::is_number(parser::syntax_tree const& t)
{
    return t.is_leaf() && is_number(t.value);
}

bool interpreter::is_function(std::string const& str) const
{
    return _functions.find(str) != _functions.end();
}

parser::syntax_tree interpreter::apply(std::string const& op, std::list<parser::syntax_tree> arguments)
{
    lambda const& l{_functions[op]};

    return l(arguments);
}

interpreter::lambda interpreter::numeric_function(std::function<int(std::list<int>)> f)
{
    return [f](std::list<parser::syntax_tree> args)
    {
	// Convert all arguments to int
	std::list<int> numArgs{};

	for(auto const& arg : args)
	{
	    if(!interpreter::is_number(arg))
	    {
		// The given argument is not a number : throw exception
		throw interpreter_exception(" The given argument is not a number. Error");
	    }

	    numArgs.push_back(std::stoi(arg.value));
	}

	return std::to_string(f(numArgs));
    };
}
