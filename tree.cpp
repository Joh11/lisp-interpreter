#include "tree.hpp"
#include "dot.hpp"

// A file used because full template specialization require to put implementation in a cpp file ...

// We specialize the template to handle dotted notation (a . b)
template <>
std::ostream & operator<<(std::ostream & o, tree_node<std::string> const& tree)
{
    // There are three cases : leaf, standard branch and dot

    if(tree.is_leaf())
    {
	return o << tree.value;
    }
    else if(is_dot(tree))
    {
	o << "(";

	auto it{tree.subtree_begin()};

	o << *it;

	o << " . ";

	o << *(++it);
	
	return o << ")";
    }
    else
    {
	o << "(";

	auto it{tree.subtree_begin()};
	o << *it;

	for(++it ; it != tree.subtree_end() ; ++it)
	{
	    o << " ";
	    o << *it;
	}
	
	return o << ")";
    }
}
