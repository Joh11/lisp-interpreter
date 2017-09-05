#include <list>
#include <map>
#include <iostream>
#include <stack>
#include <regex>
#include <algorithm>

#include "utils.hpp"
#include "interpreter.hpp"
#include "dot.hpp"

interpreter::interpreter()
{
    // Fill predefined stuff
    fill_special();
    fill_function();
}

void interpreter::fill_function()
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

    // Even though eq? is a special form, we have to evaluate its arguments first so I placed it here
    _functions["eq?"] = lambda{[](std::list<parser::syntax_tree> args)
			      {
				  if(args.size() != 2)
				      throw interpreter_exception("eq? takes exactly two arguments");
				  // Just check if the two trees are equal
				  auto second{args.begin()};
				  ++second;
				  int value{*args.begin() == *second ? 1 : 0};
				  return parser::syntax_tree{std::to_string(value)};
			      }};

    // Same for cons
    _functions["cons"] = lambda{[](std::list<parser::syntax_tree> args)
				{
				    if(args.size() != 2)
					throw interpreter_exception("cons takes exactly two arguments");

				    auto second{args.begin()};
				    auto first{second++};

				    return build_dot(*first, *second);
				}};

    // And car
    _functions["car"] = lambda{[](std::list<parser::syntax_tree> args)
			       {
				   if(args.size() != 1)
				       throw interpreter_exception("car takes only one argument");
				   return car(*args.begin());
			       }};
    // And cdr
    _functions["cdr"] = lambda{[](std::list<parser::syntax_tree> args)
			       {
				   if(args.size() != 1)
				       throw interpreter_exception("cdr takes only one argument");
				   return cdr(*args.begin());
			       }};

    // And atom
    _functions["atom?"] = lambda{[](std::list<parser::syntax_tree> args)
				 {
				     if(args.size() != 1)
					 throw interpreter_exception("atom? takes only one argument");

				     int value{args.begin()->is_leaf() ? 1 : 0};

				     return parser::syntax_tree{std::to_string(value)};
				 }};
}

void interpreter::fill_special()
{
    _specials["quote"] = lambda{[](std::list<parser::syntax_tree> args)
				{
				    if(args.size() != 1)
					throw interpreter_exception("quote takes only one argument");
				    return *args.begin();
				}};

    _specials["define"] = lambda{[this](std::list<parser::syntax_tree> args)
				 {
				     if(args.size() != 2)
					 throw interpreter_exception{"define takes exactly two arguments"};

				     if(!args.begin()->is_leaf())
					 throw interpreter_exception{"The first argument of define must be a symbol"};
				     
				     // Bind the name of the first argument to the expression given in the second argument
				     auto it{args.begin()};
				     bind(args.begin()->value, *(++it));

				     return *args.begin();
				 }};
}

parser::syntax_tree interpreter::interpret(parser::syntax_tree tree)
{
    // If the tree is a leaf we cannot do anything so quit
    if(tree.is_leaf())
    {
	// Unless the leaf is a variable
	if(is_variable(tree.value))
	{
	    return interpret(_variables[tree.value]);
	}
	
	return tree;
    }


    // Two choices : the first child is a subtree or a leaf
    if(tree.subtree_begin()->is_leaf())
    {
	std::string name{tree.subtree_begin()->value};

	// We build the args
	auto args{build_args(tree)};


	// First check for special forms
	// They are different from functions because we can't evaluate the arguments before
	if(is_special(name))
	{
	    // Just apply
	    tree = _specials[name](args);
	}
	else if(is_function(name))
	{
	    // First evaluate the arguments
	    std::for_each(args.begin(), args.end(), [this](parser::syntax_tree & t){t = interpret(t);});
	    // Then apply
	    tree = _functions[name](args);
	}
	else if(is_variable(name))
	{
	    // Replace the name with its value
	    tree.erase(tree.subtree_begin());
	    tree.push_front(_variables[name]);

	    // And rerun the interpreter
	    tree = interpret(tree);
	}
	else
	{
	    // The leaf was not recognized as special form nor function, so there is an error
	    throw interpreter_exception("The object '" + name + "' is not applicable. Error");
	}
    }
    
    return tree;
}

bool interpreter::is_leaf_rational(parser::syntax_tree const& t)
{
    bool val1{t.is_leaf()};
    bool val2{is_rational(t.value)};
    
    return val1 && val2;
}

bool interpreter::is_special(std::string const& str) const
{
    return _specials.find(str) != _specials.end();
}

bool interpreter::is_function(std::string const& str) const
{
    return _functions.find(str) != _functions.end();
}

bool interpreter::is_variable(std::string const& str) const
{
    return _variables.find(str) != _variables.end();
}

void interpreter::bind(std::string const& name, parser::syntax_tree const& tree)
{
    _variables[name] = parser::syntax_tree{tree};
}

std::list<parser::syntax_tree> interpreter::build_args(parser::syntax_tree const& t)
{
    auto beginArgs{t.subtree_begin()};
    ++beginArgs;

    return sub_list<decltype(beginArgs), std::list<parser::syntax_tree> >(beginArgs, t.subtree_end());
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
