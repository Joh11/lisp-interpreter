#ifndef TREE_HPP
#define TREE_HPP

#include <list>
#include <ostream>

template <typename T>
class tree_node
{
public:
    // Constructors and copy assignment operator
    tree_node(T value = T());
    tree_node(tree_node<T> const& obj);
    tree_node<T> & operator=(tree_node<T> const& other);
    
    // It is public because the exterior has to access it
    T value;

    // An alias to keep it simple
    using subtree_iterator = typename std::list<tree_node<T> >::iterator;
    using subtree_const_iterator = typename std::list<tree_node<T> >::const_iterator;

    subtree_iterator subtree_begin();
    subtree_iterator subtree_end();

    // For const correctness
    subtree_const_iterator subtree_begin() const;
    subtree_const_iterator subtree_end() const;

    // Add a substree
    void push_back(tree_node<T> const& subtree);

    // Return true if there is no subtree
    bool is_leaf() const;

    // Return the number of children
    size_t nb_children() const;

    // Return true if the node has no parent
    bool is_root() const;

    tree_node<T> & parent();

    tree_node<T> const& parent() const;

    // Replace recursively all the leafs with the value source with the tree_node dest
    void deep_replace_leafs(T const& source, tree_node<T> const& dest);
    
private:
    std::list<tree_node<T> > _subtrees;
    tree_node<T> *_parent; // The parent of the node ; null_ptr if it is a root
};

// Operator overload for streams
template <typename T>
std::ostream & operator<<(std::ostream & o, tree_node<T> const& tree);


// Implementation of the template class

// Constructors
template <typename T>
tree_node<T>::tree_node(T value) : value(value), _parent(nullptr) // We can write that because the compiler knows that the first value must be a member variable
{}

template <typename T>
tree_node<T>::tree_node(tree_node<T> const& obj) : value(obj.value), _subtrees(obj._subtrees), _parent(obj._parent)
{}

// Copy assignment operator
template <typename T>
tree_node<T> & tree_node<T>::operator=(tree_node<T> const& other)
{
    value = other.value;
    _parent = other._parent;
    _subtrees = other._subtrees; // Deep copy because everything is passed by value
}

// Iterators
template <typename T>
typename tree_node<T>::subtree_iterator tree_node<T>::subtree_begin()
{
    return _subtrees.begin();
}

template <typename T>
typename tree_node<T>::subtree_iterator tree_node<T>::subtree_end()
{
    return _subtrees.end();
}

template <typename T>
typename tree_node<T>::subtree_const_iterator tree_node<T>::subtree_begin() const
{
    return _subtrees.begin();
}

template <typename T>
typename tree_node<T>::subtree_const_iterator tree_node<T>::subtree_end() const
{
    return _subtrees.end();
}

// Other methods
template <typename T>
void tree_node<T>::push_back(tree_node<T> const& subtree)
{
    // Give it a pointer to this as parent
    tree_node<T> treeCopy{subtree};

    treeCopy._parent = this;
    
    _subtrees.push_back(treeCopy);
}

template <typename T>
bool tree_node<T>::is_leaf() const
{
    return _subtrees.empty();
}

template <typename T>
size_t tree_node<T>::nb_children() const
{
    return _subtrees.size();
}

template <typename T>
bool tree_node<T>::is_root() const
{
    return _parent == nullptr;
}

template <typename T>
tree_node<T> & tree_node<T>::parent()
{
    return *_parent;
}

template <typename T>
tree_node<T> const& tree_node<T>::parent() const
{
    return *_parent;
}

template <typename T>
void tree_node<T>::deep_replace_leafs(T const& source, tree_node<T> const& dest)
{
    // Do this for all subtrees
    for(auto & subtree : _subtrees)
    {
	if(subtree.is_leaf())
	{
	    if(subtree.value == source)
		subtree = dest;
	}
	else
	    subtree.deep_replace_leafs(source, dest);
    }
}

// Operator overload
template <typename T>
std::ostream & operator<<(std::ostream & o, tree_node<T> const& tree)
{
    if(!tree.is_leaf())
	o << "(";
    o << tree.value;

    for(auto it = tree.subtree_begin() ; it != tree.subtree_end() ; ++it)
	o << *it;

    if(!tree.is_leaf())
	o << ")";
    
    return o;
}

#endif