#ifndef _TREE_NODE_HPP_
#define _TREE_NODE_HPP_

#include <cmath>
#include <memory>

#include "TreeNode.h"

#define EPS __DBL_EPSILON__ // 2.2204460492503131e-16

template <Comparable Type>
shared_ptr<Type> TreeNode<Type>::allocateData() 
{
    try 
    {
        shared_ptr<value_type> new_ptr(new value_type);
        return new_ptr;
    } 
    catch (bad_alloc &error) 
    {
        time_t cur_time = time(NULL);
        throw MemoryException(ctime(&cur_time), __FILE__, __LINE__,
                              typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
shared_ptr<TreeNode<Type>> TreeNode<Type>::allocateNode() 
{
    try 
    {
        shared_ptr<TreeNode<value_type>> new_ptr(new TreeNode<value_type>);
        return new_ptr;
    } 
    catch (bad_alloc &error) 
    {
        time_t cur_time = time(NULL);
        throw MemoryException(ctime(&cur_time), __FILE__, __LINE__,
                              typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
TreeNode<Type>::TreeNode() {}

template <Comparable Type>
TreeNode<Type>::TreeNode(const Type &data)
{
    set(data);
}

template <Comparable Type>
TreeNode<Type>::TreeNode(Type &&data)
{
    set(data);
}

template <Comparable Type>
template <typename OtherType>
TreeNode<Type>::TreeNode(const OtherType &data)
{
    set(data);
}

template <Comparable Type>
template <typename OtherType>
TreeNode<Type>::TreeNode(OtherType &&data)
{
    set(data);
}

template <Comparable Type>
TreeNode<Type>::TreeNode(const TreeNode<value_type> &other) = default;
template <Comparable Type>
TreeNode<Type>::TreeNode(TreeNode<value_type> &&other) noexcept = default;

template <Comparable Type>
void TreeNode<Type>::set(const Type &data) 
{ 
    data_ptr_ = allocateData();
    *data_ptr_ = data;
    empty_ = false;
}

template <Comparable Type>
void TreeNode<Type>::set(Type &&data) 
{ 
    data_ptr_ = allocateData();
    *data_ptr_ = data;
    empty_ = false;
}

template <Comparable Type>
template <typename OtherType>
void TreeNode<Type>::set(const OtherType &data) 
{ 
    data_ptr_ = allocateData();
    *data_ptr_ = data;
    empty_ = false;
}

template <Comparable Type>
template <typename OtherType>
void TreeNode<Type>::set(OtherType &&data) 
{ 
    data_ptr_ = allocateData();
    *data_ptr_ = data;
    empty_ = false;
}

template <Comparable Type>
bool TreeNode<Type>::isEmpty() 
{
    return empty_;
}

template <Comparable Type>
void TreeNode<Type>::set_left(shared_ptr<TreeNode<value_type>> next) 
{ 
    left_ = next; 
}

template <Comparable Type>
void TreeNode<Type>::set_left_null() 
{ 
    left_ = nullptr; 
}

template <Comparable Type>
void TreeNode<Type>::set_right(shared_ptr<TreeNode<value_type>> next) 
{ 
    right_ = next; 
}

template <Comparable Type>
void TreeNode<Type>::set_right_null() 
{ 
    right_ = nullptr; 
}

template <Comparable Type>
shared_ptr<TreeNode<Type>> TreeNode<Type>::left() 
{ 
    return left_; 
}

template <Comparable Type>
shared_ptr<TreeNode<Type>> TreeNode<Type>::right() 
{ 
    return right_; 
}

template <Comparable Type>
const Type &TreeNode<Type>::getData() const 
{ 
    return *data_ptr_; 
}

template <Comparable Type>
Type &TreeNode<Type>::getData() 
{ 
    return *data_ptr_; 
}

template <Comparable Type>
TreeNode<Type> &TreeNode<Type>::operator=(const TreeNode<value_type> &other) = default;

template <Comparable Type>
TreeNode<Type> &TreeNode<Type>::operator=(TreeNode<value_type> &&other) noexcept = default;

template <Comparable Type>
bool TreeNode<Type>::operator==(const TreeNode<value_type> &other) const 
{
    return this == addressof(other);
}

template <Comparable Type>
bool TreeNode<Type>::operator!=(const TreeNode<value_type> &other) const 
{
    return !(this == addressof(other));
}

template <Comparable Type>
TreeNode<Type>::~TreeNode() = default;

#endif // _TREE_NODE_HPP_
