#ifndef _CONST_ITERATOR_HPP_
#define _CONST_ITERATOR_HPP_
#include <memory>
#include <stack>

#include "ConstIterator.h"

#pragma region Constructors
template <Comparable Type>
ConstIterator<Type>::ConstIterator(const tree &other) noexcept: BaseIterator() 
{
    root_ = other.root();
    ptr_ = other.ptr();
    size = other.size();
}

template <Comparable Type>
ConstIterator<Type>::ConstIterator(shared_ptr<node> root, shared_ptr<node> ptr, size_t idx, size_t size_): BaseIterator(), index_(idx) 
{
    root_ = root; 
    ptr_ = ptr; 
    size = size_;
}

template <Comparable Type>
ConstIterator<Type>::ConstIterator(const ConstIterator<value_type> &iterator) noexcept: BaseIterator(iterator),
               root_{iterator.root()}, index_{iterator.index()},
               ptr_{iterator.ptr()} { size = iterator.size; }
               
#pragma endregion Constructors

template <Comparable Type>
ConstIterator<Type>::~ConstIterator() = default;

#pragma region Operators
template <Comparable Type>
ConstIterator<Type>::operator bool() const 
{
    return size && index_ < size;
}

template <Comparable Type>
ConstIterator<Type> &ConstIterator<Type>::operator=(const ConstIterator<value_type> &iterator) noexcept = default;

template <Comparable Type>
template <Comparable OtherType>
ConstIterator<Type> ConstIterator<Type>::operator+(const OtherType diff) const 
{
    ConstIterator<value_type> res(*this);

    for (size_t i = 0; i < diff; ++i) 
        ++res;

    return res;
}

template <Comparable Type>
template <Comparable OtherType>
ConstIterator<Type> ConstIterator<Type>::operator-(const OtherType diff) const
{
    ConstIterator<value_type> res(*this);

    for (size_t i = 0; i < diff; ++i) 
        --res;

    return res;
}

template <Comparable Type>
size_t ConstIterator<Type>::operator-(const ConstIterator<Type> diff) const 
{
    if (index_ >= diff.index_)
        return index_ - diff.index_;
    else
        return diff.index_ - index_;
}

template <Comparable Type>
template <typename OtherType>
ConstIterator<Type> &ConstIterator<Type>::operator+=(const OtherType diff) 
{
    for (size_t i = 0; i < diff; ++i) ++(*this);
    return this;
}

template <Comparable Type>
template <typename OtherType>
ConstIterator<Type> &ConstIterator<Type>::operator-=(const OtherType diff)
{
    for (size_t i = 0; i < diff; ++i) ++(*this);
    return this;
}

template <Comparable Type>
ConstIterator<Type> &ConstIterator<Type>::operator++ () 
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
ConstIterator<Type> ConstIterator<Type>::operator++() const
{
    ConstIterator<value_type> tmp(*this);
    ++tmp;    

    return tmp;
}

template <Comparable Type>
ConstIterator<Type> &ConstIterator<Type>::operator--() 
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
ConstIterator<Type> ConstIterator<Type>::operator--() const
{
    ConstIterator<value_type> tmp(*this);
    --tmp;
    return tmp;
}

template <Comparable Type>
bool ConstIterator<Type>::operator==(const ConstIterator<value_type> &iterator) const 
{
    return index() == iterator.index();
}

template <Comparable Type>
bool ConstIterator<Type>::operator!=(const ConstIterator<value_type> &iterator) const 
{
    return index() != iterator.index();
}

template <Comparable Type>
bool ConstIterator<Type>::operator>(const ConstIterator<value_type> &iterator) const 
{
    return index() > iterator.index();
}

template <Comparable Type>
bool ConstIterator<Type>::operator>=(const ConstIterator<value_type> &iterator) const 
{
    return index() >= iterator.index();
}

template <Comparable Type>
bool ConstIterator<Type>::operator<(const ConstIterator<value_type> &iterator) const 
{
    return index() < iterator.index();
}

template <Comparable Type>
bool ConstIterator<Type>::operator<=(const ConstIterator<value_type> &iterator) const 
{
    return index() <= iterator.index();
}
#pragma endregion Operators

#pragma region ProtectedMethods

#pragma endregion ProtectedMethods

template <Comparable Type>
void ConstIterator<Type>::expiredCheckPtr(const size_t line) const
{
    if (ptr_.expired())
    {
        time_t cur_time = time(NULL);
        throw ExpiredException(ctime(&cur_time), __FILE__, line,
                               typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
void ConstIterator<Type>::expiredCheckRoot(const size_t line) const
{
    if (root_.expired())
    {
        time_t cur_time = time(NULL);
        throw ExpiredException(ctime(&cur_time), __FILE__, line,
                               typeid(*this).name(), __FUNCTION__);
    }
}

#endif // _CONST_ITERATOR_HPP_