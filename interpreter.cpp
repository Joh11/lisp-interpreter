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
    // Fill the function map with the four classical operations : + - * /
    
    _functions["+"] = numeric_function([](std::list<number> args)
				       {
					   number value{0};
					   for(auto const& n : args)
					       value += n;
					   return value;
				       });

    _functions["*"] = numeric_function([](std::list<number> args)
				       {
					   number value{1};
					   for(auto const& n : args)
					       value *= n;
					   return value;
				       });

    _functions["-"] = numeric_function([](std::list<number> args)
				       {
					   // If there is only one argument return the opposite
					   if(args.size() == 1) return (-1) * *args.begin();

					   auto it{args.begin()};
					   number value{*it};

					   for(++it ; it != args.end() ; ++it)
					       value -= *it;

					   return value;
				       });

    _functions["/"] = numeric_function([](std::list<number> args)
				       {

					   if(args.size() < 2) throw interpreter_exception(" You need at least two values for a division !");
					   
					   auto it{args.begin()};
					   number value{*it};

					   for(++it ; it != args.end() ; ++it)
					       value /= *it;

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

bool interpreter::is_leaf_rational(parser::syntax_tree const& t)
{
    bool val1{t.is_leaf()};
    bool val2{is_rational(t.value)};
    
    return val1 && val2;
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

interpreter::lambda interpreter::numeric_function(std::function<number(std::list<number>)> f)
{
    return [f](std::list<parser::syntax_tree> args)
    {
	// Convert all arguments to int
	std::list<number> numArgs{};

	for(auto const& arg : args)
	{
	    if(!is_leaf_rational(arg))
	    {
		// The given argument is not a number : throw exception
		throw interpreter_exception(" The given argument is not a number. Error");
	    }

	    numArgs.push_back(str_to_rational(arg.value));
	}

	return std::to_string(f(numArgs));
    };
}
