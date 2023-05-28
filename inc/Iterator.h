#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include <iterator>
#include <memory>
#include <stack>

#include "BaseIterator.h"
#include "TreeNode.h"
#include "Concepts.h"
#include "Exceptions.h"

using namespace std;

template <Comparable Type>
class Tree;

template <Comparable Type>
class Iterator : public BaseIterator {

    friend class Tree<Type>;

    using node = TreeNode<Type>;
    using tree = Tree<Type>;

public:

    using iterator_category = random_access_iterator_tag;
    using value_type = Type;
    using difference_type = ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    
    Iterator() noexcept = default;
    Iterator(Iterator<Type> &&other) noexcept = default;
    Iterator(const shared_ptr<node> root = nullptr, shared_ptr<node> ptr = nullptr, size_t idx = 0, size_t size_ = 0);
  
    Iterator(const tree &other) noexcept;
    Iterator(const Iterator<Type> &iterator) noexcept;

    ~Iterator();

    operator bool() const;
    Iterator<Type> &operator=(const Iterator<Type> &iterator) noexcept;
    Iterator<Type> &operator=(Iterator<Type> &&iterator) noexcept = default;

    reference operator*() { return ptr().lock()->getData(); };
    pointer operator->() { return &ptr().lock()->getData(); } ;

    size_t operator-(const Iterator<Type> diff) const;

    template <Comparable OtherType>
    Iterator<Type> operator+(const OtherType diff) const;
    template <Comparable OtherType>
    Iterator<Type> operator-(const OtherType diff) const;

    template <typename OtherType>
    Iterator<Type> &operator+=(const OtherType diff);
    template <typename OtherType>
    Iterator<Type> &operator-=(const OtherType diff);
    
    Iterator<Type> &operator++();
    Iterator<Type> operator++() const ;

    Iterator<Type> &operator--();
    Iterator<Type> operator--() const ;

    Type &operator[](const size_t offset);
    const Type &operator[](const size_t offset) const;
    

    bool operator==(const Iterator<Type> &iterator) const;
    bool operator!=(const Iterator<Type> &iterator) const;

    bool operator>(const Iterator<Type> &iterator) const;
    bool operator>=(const Iterator<Type> &iterator) const;

    bool operator<(const Iterator<Type> &iterator) const;
    bool operator<=(const Iterator<Type> &iterator) const;

protected:

    shared_ptr<node> left() { expiredCheckPtr(__LINE__); return ptr_.lock()->left(); };
    shared_ptr<node> right() { expiredCheckPtr(__LINE__); return ptr_.lock()->right(); };

    weak_ptr<node> root() const { return root_; };
    weak_ptr<node> ptr() const { return ptr_; };
    size_t index() const { return index_; };

    void expiredCheckPtr(const size_t line) const;
    void expiredCheckRoot(const size_t line) const;
    void indexCheck(const size_t line, const size_t offset) const;

private:
    weak_ptr<node> ptr_;
    weak_ptr<node> root_;
    size_t index_;
    stack<weak_ptr<node>> sub_stack_;
    stack<weak_ptr<node>> stack_;
};

#endif // _ITERATOR_H_
