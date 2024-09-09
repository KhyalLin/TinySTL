#pragma once

// waiting for pair<>, swap()

#include "tiny_construct.h"
#include "tiny_alloc.h"
#include "tiny_iterator.h"
#include "tiny_functional.h"
#include <iso646.h>
#include <utility>

namespace Tiny
{

using __rb_tree_color_type = bool;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base
{
    using color_type = __rb_tree_color_type;
    using base_ptr = __rb_tree_node_base*;

    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;
    
    static base_ptr minimum(base_ptr x) {
        while (x->left != 0) x = x->left;
        return x;
    }
    static base_ptr maximum(base_ptr x) {
        while (x->right != 0) x = x->right;
        return x;
    }
};

template <typename Value>
struct __rb_tree_node : public __rb_tree_node_base
{
    using link_type = __rb_tree_node<Value>*;
    Value value_field;
};

struct __rb_tree_base_iterator
{
    using base_ptr = __rb_tree_node_base::base_ptr;
    using iterator_category = bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;

    base_ptr node;
    void increment()
    {
        if (node->right != 0)
        {
            node = node->right;
            while (node->left != nullptr)
                node = node->left;
        }
        else
        {
            base_ptr y = node->parent;
            while (node == y->right)
                node = y, y = y->parent;
            if (node->right != y)
                node = y;
        }
    }

    void decrement()
    {
        if (node->color == __rb_tree_red and
            node->parent->parent == node)
        {
            node = node->right;
            return;
        }
        if (node->left != nullptr)
        {
            base_ptr y = node->left;
            while (y->right != nullptr)
                y = y->right;
            node = y;
        }
        else
        {
            base_ptr y = node->parent;
            while (node == y->left)
                node = y, y = y->parent;
            node = y;
        }
    }
};

template <typename Value, typename Ref, typename Ptr>
struct __rb_tree_iterator : public __rb_tree_base_iterator
{
    using value_type = Value;
    using reference = Ref;
    using pointer = Ptr;
    using iterator = __rb_tree_iterator<Value, Value&, Value*>;
    using const_iterator = __rb_tree_iterator<Value, const Value&, const Value*>;
    using self = __rb_tree_iterator<Value, Ref, Ptr>;
    using link_type = __rb_tree_node<Value>*;

    __rb_tree_iterator() = default;
    __rb_tree_iterator(link_type x) { node = x; }
    operator const_iterator() { return const_iterator((link_type)node); }

    reference operator*() { return link_type(node)->value_field; }
    pointer operator->() { return &(operator*()); }
    bool operator==(const __rb_tree_iterator& x) const
        { return node == x.node; }
    bool operator!=(const __rb_tree_iterator& x) const
        { return node != x.node; }

    self& operator++() { 
        increment(); 
        return *this; 
    }
    self operator++(int) { 
        self tmp = *this;
        increment();
        return tmp;
    }
    self& operator--() {
        decrement();
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        decrement();
        return tmp;
    }
};

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
class rb_tree
{
protected:
    using void_pointer = void*;
    using base_ptr = __rb_tree_node_base*;
    using rb_tree_node = __rb_tree_node<Value>;
    using rb_tree_node_allocator = simple_alloc<rb_tree_node, Alloc>;
    using color_type = __rb_tree_color_type;

public:
    using key_type = Key;
    using value_type = Value;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using link_type = rb_tree_node*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using self = rb_tree<Key, Value, KeyOfValue, Compare, Alloc>;

protected:
    link_type get_node() { return rb_tree_node_allocator::allocate(); }
    void put_node(link_type p) { return rb_tree_node_allocator::deallocate(p); }

    link_type create_node(const value_type& x)
    {
        link_type tmp = get_node();
        try {
            construct(&tmp->value_field, x);
        }
        catch (...) {
            put_node(tmp);
            throw;
        }
        return tmp;
    }

    link_type clone_node(link_type x)
    {
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = nullptr;
        tmp->right = nullptr;
        return tmp;
    }

    void destroy_node(link_type p)
    {
        destroy(&p->value_field);
        put_node(p);
    }

protected:
    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type& root() const { return (link_type&)header->parent; }
    link_type& leftmost() const { return (link_type&)header->left; }
    link_type& rightmost() const { return (link_type&)header->right; }
    
    static link_type& left(link_type x) { return (link_type&)x->left; }
    static link_type& right(link_type x) { return (link_type&)x->right; }
    static link_type& parent(link_type x) { return (link_type&)x->parent; }
    static reference value(link_type x) { return x->value_field; }
    static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
    static color_type& color(link_type x) { return x->color; }

    static link_type& left(base_ptr x) { return (link_type&)x->left; }
    static link_type& right(base_ptr x) { return (link_type&)x->right; }
    static link_type& parent(base_ptr x) { return (link_type&)x->parent; }
    static reference value(base_ptr x) { return link_type(x)->value_field; }
    static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
    static color_type& color(base_ptr x) { return link_type(x)->color; }

    static link_type minimum(link_type x) {
        return (link_type)__rb_tree_node_base::minimum(x);
    }
    static link_type maximum(link_type x) {
        return (link_type)__rb_tree_node_base::maximum(x);
    }

    void __rb_tree_rotate_left(base_ptr, base_ptr& root);
    void __rb_tree_rotate_right(base_ptr, base_ptr& root);
    void __rb_tree_rebalance_insert(base_ptr, base_ptr& root);
    base_ptr __rb_tree_rebalance_erase(base_ptr, base_ptr& root);
    
public:
    using iterator = __rb_tree_iterator<value_type, reference, pointer>;
    using const_iterator = __rb_tree_iterator<value_type, const_reference, const_pointer>;

private:
    iterator __insert(base_ptr x, base_ptr y, const value_type& v);
    link_type __copy(link_type x, link_type p);
    void __erase(link_type x);
    void init() {
        header = get_node();
        color(header) = __rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }

public:
    rb_tree(const Compare& comp = Compare())
        : node_count(0), key_compare(comp) { init(); }
    rb_tree(const self&);
    rb_tree(self&&);
    ~rb_tree() { clear(), put_node(header); }

    self& operator=(const self& x);
    void swap(self& x);
    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost(); }
    iterator end() { return header; }
    const_iterator begin() const { return leftmost(); }
    const_iterator end() const { return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    static size_type max_size() { return size_type(-1); }

    iterator find(const Key&);
    const_iterator find(const Key&) const;
    iterator lower_bound(const Key&);
    iterator upper_bound(const Key&);
    const_iterator lower_bound(const Key&) const;
    const_iterator upper_bound(const Key&) const;
    size_type count(const key_type&) const;
    std::pair<iterator, bool> insert_unique(const value_type&);
    iterator insert_equal(const value_type&);
    void erase(iterator);
    size_type erase(const Key&);
    void clear();
};


template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p) -> link_type
{
    if (x == nullptr) return nullptr;

    link_type top = clone_node(x);
    top->parent = p;
    try {
        top->right = __copy(x->right);
        top->left = __copy(x->left);
    }
    catch (...) {
        __erase(top);
        throw;
    }
    return top;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const self& x)
    : node_count(x.node_count), key_compare(x.key_comp)
{
    init();
    try {
        root() = __copy(x.root(), header);
    }
    catch (...) {
        put_node(root());
        throw;
    }
    leftmost() = minimum(root());
    rightmost() = maximum(root());
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(self&& x)
    : node_count(x.node_count), header(x.header), key_compare(x.key_compare)
{
    x.header = nullptr;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const self& x) -> self&
{
    node_count = x.node_count;
    key_compare = x.key_compare;
    clear();
    try {
        root() = __copy(x.root(), header);
    }
    catch (...) {
        put_node(root());
        throw;
    }
    leftmost() = minimum(root());
    rightmost() = maximum(root());
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::swap(self& x)
{
    swap(node_count, x.node_count);
    swap(header, x.header);
    swap(key_compare, x.key_compare);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) -> iterator
{
    link_type y = header;
    link_type x = root();
    while (x != nullptr) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    return __insert(x, y, v);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) -> std::pair<iterator, bool>
{
    link_type y = header;
    link_type x = root();
    bool comp = true;

    while (x != nullptr) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
    }

    iterator j = iterator(y);
    if (comp)
    {
        if (j == begin())
            return { __insert(x, y, v), true };
        j--;
    }
    if (key_compare(key(j.node), KeyOfValue()(v)))
        return { __insert(x, y, v), true };
    return std::pair<iterator, bool>(j, false);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const Value& v) -> iterator
{
    link_type x = (link_type)x_;
    link_type y = (link_type)y_;
    link_type z;

    if (y == header or x != nullptr or key_compare(KeyOfValue()(v), key(y)))
    {
        z = create_node(v);
        left(y) = z;
        if (y == header) {
            root() = z;
            rightmost() = z;
        }
        else if (y == leftmost())
            leftmost() = z;
    }
    else
    {
        z = create_node(v);
        right(y) = z;
    }

    parent(z) = y;
    left(z) = nullptr;
    right(z) = nullptr;
    __rb_tree_rebalance_insert(z, header->parent);
    node_count++;
    return iterator(z);
}


template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_rebalance_insert(base_ptr x, base_ptr& root)
{
    x->color = __rb_tree_red;
    while (x != root and x->parent->color == __rb_tree_red)
    {
        if (x->parent == x->parent->parent->left)
        {
            base_ptr y = x->parent->parent->right;
            if (y and y->color == __rb_tree_red) {
                x->parent->color = __rb_tree_black;
                y->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->right) {
                    x = x->parent;
                    __rb_tree_rotate_left(x, root);
                }
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root);
            }
        }
        else
        {
            base_ptr y = x->parent->parent->left;
            if (y and y->color == __rb_tree_red) {
                x->parent->color = __rb_tree_black;
                y->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    x = x->parent;
                    __rb_tree_rotate_right(x, root);
                }
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    root->color = __rb_tree_black;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_rotate_left(base_ptr x, base_ptr& root)
{
    base_ptr y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_rotate_right(base_ptr x, base_ptr& root)
{
    base_ptr y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
        y->right->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) -> iterator
{
    link_type y = header;
    link_type x = root();

    while (x != nullptr)
    {
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    }
    iterator j = iterator(y);
    return (j == end() or key_compare(k, key(j.node)) ? end() : j);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) const -> const_iterator
{
    link_type y = header;
    link_type x = root();

    while (x != nullptr)
    {
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    }
    iterator j = const_iterator(y);
    return (j == end() or key_compare(k, key(j.node)) ? end() : j);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x)
{
    if (x == nullptr) return;
    __erase(left(x));
    __erase(right(x));
    destroy_node(x);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
{
    __erase(root());
    left(header) = header;
    right(header) = header;
    parent(header) = nullptr;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
{
	base_ptr to_be_delete = __rb_tree_rebalance_erase(position.node, (base_ptr&)root());
	destroy_node((link_type)to_be_delete);
	node_count--;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& val) -> size_type
{
	iterator first = lower_bound(val);
    iterator last = upper_bound(val);
    size_type len = distance(first, last);

    while (first != last)
        erase(first++);
    return len;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_rebalance_erase(base_ptr z, base_ptr& root) -> base_ptr
{
	base_ptr del_node = z;
	base_ptr replace_node = nullptr;
	base_ptr replace_node_parent;

	if (del_node->left == nullptr)
		replace_node = del_node->right;
	else if (del_node->right == nullptr)
		replace_node = del_node->left;
	else
	{
		del_node = del_node->right;
		while (del_node->left != nullptr)
			del_node = del_node->left;
		replace_node = del_node->right;
	}

	if (del_node == z)
	{
		replace_node_parent = del_node->parent;
		if (z == root)
			root = replace_node;
		else if (z == z->parent->left)
			z->parent->left = replace_node;
		else
			z->parent->right = replace_node;

		if (replace_node != nullptr)
            replace_node->parent = z->parent;
        
        if (z == leftmost())
		{
			if (z->right == nullptr)
				leftmost() = (link_type)z->parent;
			else
				leftmost() = minimum((link_type)replace_node);
		}
		if (z == rightmost())
		{
			if (z->left == 0)
				rightmost() = (link_type)z->parent;
			else
				rightmost() = maximum((link_type)replace_node);
		}
    }
	else
	{
		del_node->left = z->left;
		z->left->parent = del_node;

		if (z->right != del_node)
		{
			replace_node_parent = del_node->parent;
			if (replace_node != nullptr) 
				replace_node->parent = del_node->parent;
			del_node->parent->left = replace_node;

			del_node->right = z->right;
			z->right->parent = del_node;
		}
		else
			replace_node_parent = del_node;

		if (z == root)
			root = del_node;
		else if (z == z->parent->left)
			z->parent->left = del_node;
		else
			z->parent->right = del_node;
		del_node->parent = z->parent;

		std::swap(del_node->color, z->color);
		del_node = z;	
	}
    if (del_node->color == __rb_tree_red)
        return del_node;

	while (replace_node != root and (!replace_node or replace_node->color == __rb_tree_black))  
	{
		if (replace_node == replace_node_parent->left)
		{
			base_ptr s = replace_node_parent->right;
			if (s->color == __rb_tree_red)
			{
				s->color = __rb_tree_black;
				replace_node_parent->color = __rb_tree_red;
				__rb_tree_rotate_left(replace_node_parent, root);
				s = replace_node_parent->right;
			}
			if ((!s->left or s->left->color == __rb_tree_black) and
				(!s->right or s->right->color == __rb_tree_black))
			{
				s->color = __rb_tree_red;
				replace_node = replace_node_parent;
				replace_node_parent = replace_node_parent->parent;
			}
			else
			{
				if (!s->right or s->right->color == __rb_tree_black)
				{
					if (s->left != nullptr)
						s->left->color = __rb_tree_black;
					s->right->color = __rb_tree_red;
                    __rb_tree_rotate_right(s, root);
					s = replace_node_parent->right;
				}

				s->color = replace_node_parent->color;
				replace_node_parent->color = __rb_tree_black;
				if (s->right != nullptr)
					s->right->color = __rb_tree_black;
				__rb_tree_rotate_left(replace_node_parent, root);
				break;
			}
        }
		else
		{
			base_ptr s = replace_node_parent->left;
			if (s->color == __rb_tree_red)
			{
				s->color = __rb_tree_black;
				replace_node_parent->color = __rb_tree_red;
				__rb_tree_rotate_right(replace_node_parent, root);
				s = replace_node_parent->left;
			}

			if ((!s->left or s->left->color == __rb_tree_black) and
				(!s->right or s->right->color == __rb_tree_black))
			{
				s->color = __rb_tree_red;
				replace_node = replace_node_parent;
				replace_node_parent = replace_node_parent->parent;
			}
			else
			{
				if (!s->left or s->left->color == __rb_tree_black)
				{
					if (s->right != nullptr)
						s->right->color = __rb_tree_black;
					s->color = __rb_tree_red;
					__rb_tree_rotate_left(s, root);
					s = replace_node_parent->left;
				}
				s->color = replace_node_parent->color;
				replace_node_parent->color = __rb_tree_black;
				if (s->left != nullptr)
					s->left->color = __rb_tree_black;
				__rb_tree_rotate_right(replace_node_parent, root);
			    break;
			}
		}
	}
	if (replace_node != nullptr) 
        replace_node->color = __rb_tree_black;
	return del_node;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const -> size_type
{
    const_iterator first = lower_bound(k);
    const_iterator last = upper_bound(k);
    return distance(first, last);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) -> iterator
{
    link_type y = header;
    link_type x = root();

    while (x != nullptr) 
    {
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    }

    return iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) const -> const_iterator
{
    link_type y = header;
    link_type x = root();

    while (x != nullptr) 
    {
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    }

    return const_iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) -> iterator
{
    link_type y = header;
    link_type x = root();

    while (x != 0) 
    {
        if (key_compare(k, key(x)))
            y = x, x = left(x);
        else
            x = right(x);
    }

   return iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) const -> const_iterator
{
    link_type y = header;
    link_type x = root();

    while (x != 0)
    { 
        if (key_compare(k, key(x)))
            y = x, x = left(x);
        else
            x = right(x);
    }

    return const_iterator(y);
}

}