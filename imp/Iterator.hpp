#ifndef _ITERATOR_HPP_
#define _ITERATOR_HPP_
#include <memory>
#include <stack>

#include "Iterator.h"

#pragma region Constructors
template <Comparable Type>
Iterator<Type>::Iterator(const tree &other) noexcept: BaseIterator() 
{
    root_ = other.root();
    ptr_ = other.ptr();
    size = other.size();
}

template <Comparable Type>
Iterator<Type>::Iterator(const shared_ptr<node> root, shared_ptr<node> ptr, size_t idx, size_t size_): BaseIterator(), index_(idx) 
{
    root_ = root; 
    ptr_ = ptr; 
    size = size_;
}

template <Comparable Type>
Iterator<Type>::Iterator(const Iterator<value_type> &iterator) noexcept: BaseIterator(iterator),
               root_{iterator.root()}, index_{iterator.index()},
               ptr_{iterator.ptr()} { size = iterator.size; }
               
#pragma endregion Constructors

template <Comparable Type>
Iterator<Type>::~Iterator() = default;
BaseIterator::~BaseIterator() = default;

#pragma region Operators
template <Comparable Type>
Iterator<Type>::operator bool() const 
{
    return size && index_ < size;
}

template <Comparable Type>
Iterator<Type> &Iterator<Type>::operator=(const Iterator<value_type> &iterator) noexcept = default;

template <Comparable Type>
template <Comparable OtherType>
Iterator<Type> Iterator<Type>::operator+(const OtherType diff) const 
{
    Iterator<value_type> res(*this);

    for (size_t i = 0; i < diff; ++i) 
        ++res;

    return res;
}

template <Comparable Type>
template <Comparable OtherType>
Iterator<Type> Iterator<Type>::operator-(const OtherType diff) const
{
    Iterator<value_type> res(*this);

    for (size_t i = 0; i < diff; ++i) 
        --res;

    return res;
}

template <Comparable Type>
size_t Iterator<Type>::operator-(const Iterator<Type> diff) const 
{
    if (index_ >= diff.index_)
        return index_ - diff.index_;
    else
        return diff.index_ - index_;
}

template <Comparable Type>
template <typename OtherType>
Iterator<Type> &Iterator<Type>::operator+=(const OtherType diff) 
{
    for (size_t i = 0; i < diff; ++i) ++(*this);
    return this;
}

template <Comparable Type>
template <typename OtherType>
Iterator<Type> &Iterator<Type>::operator-=(const OtherType diff)
{
    for (size_t i = 0; i < diff; ++i) ++(*this);
    return this;
}

template <Comparable Type>
Iterator<Type> &Iterator<Type>::operator++ () 
{
    expiredCheckPtr(__LINE__);
    expiredCheckRoot(__LINE__);
    
    if (ptr_.lock() == nullptr)
        return *this;

    sub_stack_.push(ptr_);

    if (right() != nullptr)
        stack_.push(right());

    if (left() != nullptr) 
        stack_.push(left());

    if (!stack_.empty()) 
    {
        ptr_ = stack_.top();
        stack_.pop();
    }

    ++index_;
    return *this;
}

template <Comparable Type>
Iterator<Type> Iterator<Type>::operator++() const
{
    Iterator<value_type> tmp(*this);
    ++tmp;    

    return tmp;
}

template <Comparable Type>
Iterator<Type> &Iterator<Type>::operator--() 
{ 
    expiredCheckPtr(__LINE__);
    expiredCheckRoot(__LINE__);

    if (index_ == 0)
        return *this;

    if (index_ != sub_stack_.size() + 1)
    {
        size_t tmp_index = index_;

        while (!sub_stack_.empty())
            sub_stack_.pop();

        while (!stack_.empty())
            stack_.pop();

        ptr_ = root_;
        index_ = 0;

        for (size_t i = 0; i < tmp_index; ++i)
            ++(*this);
    }

    ptr_ = sub_stack_.top();
    sub_stack_.pop();

    --index_;

    return *this;
}

template <Comparable Type>
Iterator<Type> Iterator<Type>::operator--() const
{
    Iterator<value_type> tmp(*this);
    --tmp;
    return tmp;
}


template <Comparable Type>
Type &Iterator<Type>::operator[](const size_t offset)
{
    expiredCheckPtr(__LINE__);
    expiredCheckRoot(__LINE__);
    indexCheck(__LINE__, offset);

    return *(*this - index_ + offset);
}

template <Comparable Type>
const Type &Iterator<Type>::operator[](const size_t offset) const
{
    expiredCheckPtr(__LINE__);
    expiredCheckRoot(__LINE__);
    indexCheck(__LINE__, offset);

    return *(*this - index_ + offset);
}

template <Comparable Type>
bool Iterator<Type>::operator==(const Iterator<value_type> &iterator) const 
{
    return index() == iterator.index();
}

template <Comparable Type>
bool Iterator<Type>::operator!=(const Iterator<value_type> &iterator) const 
{
    return index() != iterator.index();
}

template <Comparable Type>
bool Iterator<Type>::operator>(const Iterator<value_type> &iterator) const 
{
    return index() > iterator.index();
}

template <Comparable Type>
bool Iterator<Type>::operator>=(const Iterator<value_type> &iterator) const 
{
    return index() >= iterator.index();
}

template <Comparable Type>
bool Iterator<Type>::operator<(const Iterator<value_type> &iterator) const 
{
    return index() < iterator.index();
}

template <Comparable Type>
bool Iterator<Type>::operator<=(const Iterator<value_type> &iterator) const 
{
    return index() <= iterator.index();
}
#pragma endregion Operators

#pragma region ProtectedMethods

#pragma endregion ProtectedMethods

template <Comparable Type>
void Iterator<Type>::indexCheck(const size_t line, const size_t offset) const
{
    if (offset >= size)
    {
        time_t cur_time = time(NULL);
        throw OutOfRangeException(ctime(&cur_time), __FILE__, line,
                                  typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
void Iterator<Type>::expiredCheckPtr(const size_t line) const
{
    if (ptr_.expired())
    {
        time_t cur_time = time(NULL);
        throw ExpiredException(ctime(&cur_time), __FILE__, line,
                               typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
void Iterator<Type>::expiredCheckRoot(const size_t line) const
{
    if (root_.expired())
    {
        time_t cur_time = time(NULL);
        throw ExpiredException(ctime(&cur_time), __FILE__, line,
                               typeid(*this).name(), __FUNCTION__);
    }
}

#endif // _ITERATOR_HPP_
