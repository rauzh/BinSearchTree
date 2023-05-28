#ifndef _CONST_ITERATOR_H_
#define _CONST_ITERATOR_H_

#include <iterator>
#include <memory>
#include <stack>

#include "BaseIterator.h"
#include "Concepts.h"
#include "Exceptions.h"

using namespace std;

template <Comparable Type>
class Tree;

template <Comparable Type>
class ConstIterator : public BaseIterator {

    friend class Tree<Type>;
    using node = TreeNode<Type>;
    using tree = Tree<Type>;

public:

    using iterator_category = bidirectional_iterator_tag;
    using value_type = Type;
    using difference_type = ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    
    ConstIterator() noexcept = default;
    ConstIterator(ConstIterator<Type> &&other) noexcept = default;
    ConstIterator(shared_ptr<node> root = nullptr, shared_ptr<node> ptr = nullptr, size_t idx = 0, size_t size_ = 0);
  
    ConstIterator(const tree &other) noexcept;
    ConstIterator(const ConstIterator<Type> &iterator) noexcept;

    ~ConstIterator();

    operator bool() const;
    ConstIterator<Type> &operator=(const ConstIterator<Type> &iterator) noexcept;
    ConstIterator<Type> &operator=(ConstIterator<Type> &&iterator) noexcept = default;

    reference operator*() { return ptr().lock()->getData(); };
    const pointer operator->() { return &ptr().lock()->getData(); } ;

    size_t operator-(const ConstIterator<Type> diff) const;

    template <Comparable OtherType>
    ConstIterator<Type> operator+(const OtherType diff) const;
    template <Comparable OtherType>
    ConstIterator<Type> operator-(const OtherType diff) const;

    template <typename OtherType>
    ConstIterator<Type> &operator+=(const OtherType diff);
    template <typename OtherType>
    ConstIterator<Type> &operator-=(const OtherType diff);
    
    ConstIterator<Type> &operator++();
    ConstIterator<Type> operator++() const ;

    ConstIterator<Type> &operator--();
    ConstIterator<Type> operator--() const ;

    bool operator==(const ConstIterator<Type> &iterator) const;
    bool operator!=(const ConstIterator<Type> &iterator) const;

    bool operator>(const ConstIterator<Type> &iterator) const;
    bool operator>=(const ConstIterator<Type> &iterator) const;

    bool operator<(const ConstIterator<Type> &iterator) const;
    bool operator<=(const ConstIterator<Type> &iterator) const;

protected:

    shared_ptr<node> left() { expiredCheckPtr(__LINE__); return ptr_.lock()->left(); };
    shared_ptr<node> right() { expiredCheckPtr(__LINE__); return ptr_.lock()->right(); };

    weak_ptr<node> root() const { return root_; };
    weak_ptr<node> ptr() const { return ptr_; };
    size_t index() const { return index_; };

    void expiredCheckPtr(const size_t line) const;
    void expiredCheckRoot(const size_t line) const;

private:
    weak_ptr<node> ptr_;
    weak_ptr<node> root_;
    size_t index_;
    stack<weak_ptr<node>> sub_stack_;
    stack<weak_ptr<node>> stack_;
};

#endif // _CONST_ITERATOR_H_