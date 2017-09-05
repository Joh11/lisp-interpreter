#include <algorithm>
#include <regex>
#include <stack>
#include <stdexcept>

#include "parser.hpp"

token_table parser::tokenize(std::string input)
{
    // Place spaces around parenthesis to not break anything else
    std::string str = std::regex_replace(input, std::regex("[()]"), " $& ");
    
    // Turn the input into a table of tokens
    
    token_table tokens;
    {
	std::string currentToken = "";
	
	for(auto c : str)
	{
	    // If c is not a whitespace we add it to the current token
	    if(!isspace(c))
		currentToken += c;
	    else // Else we add the current token to the token table if it is not empty
	    {
		if(!currentToken.empty())
		{
		    tokens.push_back(currentToken);
		    currentToken = "";
		}
	    }
	}

	// To take care of the case where there is no final space
	if(!currentToken.empty())
	    tokens.push_back(currentToken);
    }

    return tokens;
}

parser::syntax_tree parser::parenthesize(token_table const& tokens)
{
    // We won't do anything recursively

    // ( -> start a subtree
    // ) -> end the subtree and go up one level
    // anything else -> create a leaf to the current subtree

    // We need a stack to store the iterators of each level
    std::stack<syntax_tree> subtreeStack;

    // Create a first one
    subtreeStack.push(syntax_tree{});

    for(auto const& token : tokens)
    {
	if(token == "(")
	{
	    // Start a subtree
	    subtreeStack.push(syntax_tree{});
	}
	else if(token == ")")
	{
	    // Take the current subtree, add it to the end of the previous one
	    syntax_tree sub = subtreeStack.top(); // We have to take it by value because it will be destroyed otherwise

	    // Remove it from the stack
	    subtreeStack.pop();
	    
	    // Append it to the previous one
	    subtreeStack.top().push_back(sub);
	}
	else
	{
	    // Create a single leaf with the token as value
	    syntax_tree leaf{token};

	    // Add it to the current subtree
	    subtreeStack.top().push_back(leaf);
	}
    }

    // If the input was correct, we should have only one item in the stack
    // This tree should only have one subtree : our syntax tree
    // Throw exceptions if it is not the case

    if(subtreeStack.size() != 1)
	throw syntax_error("There are unbound subtrees in the parse stack. Check your parenthesis !");
    if(subtreeStack.top().nb_children() > 1)
	throw syntax_error("There are several roots in the syntactic tree. You should enter only one instruction at a time !");

    // Everything should be ok so return the syntax tree
    return *subtreeStack.top().subtree_begin();
}

parser::syntax_tree parser::parse(std::string input)
{
    return parenthesize(tokenize(input));
}
