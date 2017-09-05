#include "dot.hpp"
#include "interpreter.hpp"

void parse_dot(parser::syntax_tree & tree)
{
    // We do not have to do anything if it is a leaf
    if(tree.is_leaf())
	return;

    if(tree.nb_children() == 3)
    {
	// We must check if the second subtree is a leaf with the value "."
	auto it{tree.subtree_begin()};
	++it;

	if(it->is_leaf() && it->value == ".")
	{
	    // We can now remove the dot subtree, and tag the tree with a dot
	    tree.erase(it);
	    tree.value = ".";
	}
    }

    // Do it recursively
    for(auto it{tree.subtree_begin()} ; it != tree.subtree_end() ; ++it)
	parse_dot(*it);
}

parser::syntax_tree build_dot(parser::syntax_tree const& a, parser::syntax_tree const& b)
{
    parser::syntax_tree t{"."};

    t.push_back(a);
    t.push_back(b);

    return t;
}

bool is_dot(parser::syntax_tree const& tree)
{
    return tree.value == "." && tree.nb_children() == 2;
}

parser::syntax_tree car(parser::syntax_tree const& tree)
{
    if(tree.is_leaf())
	throw interpreter_exception("car does not work on atoms");
    
    // Basically as the dot pairs and the lists are stored the same way we don't have to distinguish both cases
    return *tree.subtree_begin();
}

parser::syntax_tree cdr(parser::syntax_tree const& tree)
{
    if(tree.is_leaf())
	throw interpreter_exception("cdr does not work on atoms");

    if(is_dot(tree))
    {
	auto it{tree.subtree_begin()};
	return *(++it);
    }

    auto treeCopy{tree};
    treeCopy.erase(treeCopy.subtree_begin());

    return treeCopy;
}
