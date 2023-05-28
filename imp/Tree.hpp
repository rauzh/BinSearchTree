#ifndef _TREE_HPP_
#define _TREE_HPP_

#include <memory>
#include <cmath>

#include "Tree.h"

#define EPS __DBL_EPSILON__ // 2.2204460492503131e-16

#pragma region Constructors
template <Comparable Type>
Tree<Type>::Tree(): BaseContainer() 
{
    allocateRoot();
}

template <Comparable Type>
Tree<Type>::Tree(const Tree<value_type> &tree): BaseContainer()
{
    allocateRoot();
    
    for (auto elem : tree)
        Insert(elem);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type>::Tree(const Tree<OtherType> &tree): BaseContainer()
{
    allocateRoot();
    
    for (auto elem : tree)
        Insert(elem);
}

template <Comparable Type>
Tree<Type>::Tree(const size_t sizeValue, const value_type *arr): BaseContainer()
{
    allocateRoot();

    for (size_t i = 0; i < size; ++i)
        Insert(arr[i]);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type>::Tree(const size_t sizeValue, const OtherType *arr): BaseContainer()
{
    allocateRoot();

    for (size_t i = 0; i < size; ++i)
        Insert(arr[i]);
}

template <Comparable Type>
Tree<Type>::Tree(const initializer_list<Type> elements): BaseContainer()
{
    allocateRoot();

    for (auto elem : elements)
        Insert(elem);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type>::Tree(const initializer_list<OtherType> elements): BaseContainer()
{ 
    allocateRoot();

    for (auto elem : elements)
        Insert(elem);
}

template <Comparable Type>
Tree<Type>::Tree(Tree<value_type> &&tree) noexcept: BaseContainer()
{
    root_ = tree.root_;
    tree.root_.reset();
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type>::Tree(Tree<OtherType> &&tree) noexcept: BaseContainer()
{
    root_ = tree.root_;
    tree.root_.reset();
}

template <Comparable Type>
Tree<Type>::Tree(iterator begin, iterator end): BaseContainer()
{
    allocateRoot();

    for (auto iter = begin; iter != end; ++iter)
        Insert(*iter);
}
 
template <Comparable Type>
template <InputIterator I>
requires requires(typename I::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type>::Tree(I begin, I end): BaseContainer()
{
    allocateRoot();

    for (auto iter = begin; iter != end; ++iter)
        Insert(*iter);
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type>::Tree(const Cont &container): BaseContainer() {
    allocateRoot();

    for (auto iter = container.begin(); iter != container.end(); ++iter)
        Insert(*iter);
}


#pragma endregion Constructors

#pragma region TreeOperations

template <Comparable Type>
bool Tree<Type>::Insert(const Type &data)
{
    shared_ptr<TreeNode<Type>> cur_ptr = root_;

    if (cur_ptr->isEmpty()) {
        cur_ptr->set(data);
        ++size;
        return true;
    }
    
    TreeNode<Type> tmpNode;
    shared_ptr<TreeNode<Type>> new_ptr = tmpNode.allocateNode();

    bool old_version = true;

    while (old_version) {
        if (data < cur_ptr->getData()) {
            if (cur_ptr->left() != nullptr) {
                cur_ptr = cur_ptr->left();
            } else {
                new_ptr->set(data);
                cur_ptr->set_left(new_ptr);
                ++size;
                old_version = false;
            }
        } else if (data > cur_ptr->getData()) {
            if (cur_ptr->right() != nullptr) {
                cur_ptr = cur_ptr->right();
            } else {
                new_ptr->set(data);
                cur_ptr->set_right(new_ptr);
                ++size;
                old_version = false;
            }
        } else if (data == cur_ptr->getData()) {
            return false;
        }
    }

    return true;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
bool Tree<Type>::Insert(const OtherType &data)
{
    Type insert_data = static_cast<Type>(data);
    bool res = Insert(insert_data);

    return res;
}

template <Comparable Type>
bool Tree<Type>::Delete(const Type &data) {

    shared_ptr<node> cur_ = root_;
    shared_ptr<node> prev_ = nullptr;

    // Ищем нужный элемент
    while (cur_ != nullptr && cur_->getData() != data) {
        prev_ = cur_;
        if (data < cur_->getData())
            cur_ = cur_->left();
        else
            cur_ = cur_->right();
    }

    // Если не нашли
    if (cur_ == nullptr) {
        return false;
    }

    // Если хотябы одно нул
    if (cur_->left() == nullptr || cur_->right() == nullptr) {

        shared_ptr<node> new_cur_;
 
        if (cur_->left() == nullptr)
            new_cur_ = cur_->right();
        else
            new_cur_ = cur_->left();
 
        if (prev_ == nullptr) {
            root_ = new_cur_;
            return true;
        }
 
        if (cur_ == prev_->left())
            prev_->set_left(new_cur_);
        else {
            prev_->set_right(new_cur_);
        }
    }
    else {
        shared_ptr<node> p_ = nullptr;
        shared_ptr<node> tmp_;
 
        tmp_ = cur_->right();

        while (tmp_->left() != nullptr) {
            p_ = tmp_;
            tmp_ = tmp_->left();
        }
 
        if (p_ != nullptr)
            p_->set_left(tmp_->right());
        else
            cur_->set_right(tmp_->right());
 
        cur_->getData() = tmp_->getData();
    }

    --size;
    return true;
}


template <Comparable Type>
ConstIterator<Type> Tree<Type>::Find(const Type &data) const 
{

    shared_ptr<TreeNode<Type>> cur = root_;

    while (cur != nullptr) {
        if (data > cur->getData())
            cur = cur->right();
        else if (data < cur->getData())
            cur = cur->left();
        else
            return const_iterator(root_, cur, size);;
    }

    return end();
}

template <Comparable Type>
void Tree<Type>::Intersection(const Tree<Type> &tree) 
{

    auto it = begin();
    auto end_ = end();
    auto tmp = it;

    while(it != end_) {
        if (tree.Find(*it) == tree.end()) {
            Delete(*it);
            it = tmp;
            end_ = end();
        }
        tmp = it;
        ++it;
    }
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator&(const Tree<Type> &tree) const
{
    Tree<Type> res(*this);
    res.Intersection(tree);
    return res;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator&=(const Tree<Type> &tree)
{
    Intersection(tree);
    return *this;
}

#pragma endregion TreeOperations

template <Comparable Type>
Tree<Type>::~Tree() = default;

BaseContainer::~BaseContainer() = default;

#pragma region Iterators
template <Comparable Type>
Iterator<Type> Tree<Type>::begin() noexcept
{
    return  iterator(root_, root_, 0, size);
}

template <Comparable Type>
Iterator<Type> Tree<Type>::end() noexcept
{        
    return iterator(root_, nullptr, size, size);
}


template <Comparable Type>
ConstIterator<Type> Tree<Type>::begin() const noexcept
{
    return  const_iterator(root_, root_, 0, size);
}

template <Comparable Type>
ConstIterator<Type> Tree<Type>::end() const noexcept
{        
    return const_iterator (root_, nullptr, size, size);
}


template <Comparable Type>
ConstIterator<Type> Tree<Type>::cbegin() const noexcept
{
    return  const_iterator(root_, root_, 0, size);
}

template <Comparable Type>
ConstIterator<Type> Tree<Type>::cend() const noexcept
{        
    return const_iterator(root_, nullptr, size, size);
}
#pragma endregion Iterators

#pragma region Operations

#pragma region Assignments
 template <Comparable Type>
Tree<Type> &Tree<Type>::operator=(const initializer_list<Type> elements)
{
     allocateRoot();
     size = 0;
     for (auto &elem : elements)
         Insert(elem);

     return *this;
 }

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::operator=(const initializer_list<OtherType> elements)
{
     allocateRoot();
    size = 0;
     for (auto &elem : elements)
         Insert(elem);

     return *this;
 }


template <Comparable Type>
Tree<Type> &Tree<Type>::operator=(const Tree<value_type> &tree)
{
    allocateRoot();
    size = 0;
    for (auto &elem : tree)
        Insert(elem);
 
    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::operator=(const Tree<OtherType>  &tree)
{
    allocateRoot();
    size = 0;
    for (auto &elem : tree)
        Insert(elem);
 
    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator=(Tree<value_type> &&tree) noexcept
{
    size = tree.size;
    root_ = tree.root_;

    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::operator=(Tree<OtherType> &&tree) noexcept
{
    size = tree.size;
    root_ = tree.root_;

    return *this;
}

template <Comparable Type>
template <ContainerComparable Cont>
Tree<Type> &Tree<Type>::operator=(const Cont &container)  
{

    allocateRoot();
    size = 0;
    for (auto iter = container.begin(); iter != container.end(); ++iter)
        Insert(*iter);
    
    return *this;
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> &Tree<Type>::operator=(const Cont &container)  
{

    allocateRoot();
    size = 0;
    for (auto iter = container.begin(); iter != container.end(); ++iter)
        Insert(*iter);
    
    return *this;
}

#pragma endregion Assignments

#pragma region Comparsions
template <Comparable Type>
bool Tree<Type>::isEqual(const Tree<value_type> &tree) const
{
    if (size != tree.Size())
        return false;

    const_iterator it = cbegin();

    for (auto &elem : tree) 
    {
        if (elem != *it)
            return false;

        ++it;
    }

    return true;
}

template <Comparable Type>
bool Tree<Type>::operator==(const Tree<value_type> &tree) const
{
    return isEqual(tree);
}

template <Comparable Type>
bool Tree<Type>::isNotEqual(const Tree<value_type> &tree) const
{
    return !isEqual(tree);
}

template <Comparable Type>
bool Tree<Type>::operator!=(const Tree<value_type> &tree) const
{
    return !isEqual(tree);
}
#pragma endregion Comparsions

#pragma region Indexations
template <Comparable Type>
Type &Tree<Type>::operator[](const size_t index)
{
   indexCheck(index, __LINE__);
   
   iterator it = begin();

   for (int i = 0; i < index; ++i, ++it) { }

   return *it;
}

template <Comparable Type>
const Type &Tree<Type>::operator[](const size_t index) const
{
   indexCheck(index, __LINE__);
   
   const_iterator it = begin();
   for (int i = 0; i < index; ++i, ++it) { }

   return *it;
}
#pragma endregion Indexations

#pragma region Add
template <Comparable Type>
Tree<Type> Tree<Type>::Add(const Tree<value_type> &tree) const
{
    Tree<value_type> res(*this);

    for (auto &elem : tree)
        res.Insert(elem);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator+(const Tree<value_type> &tree) const
{
    return Add(tree);
}

template <Comparable Type>
Tree<Type> Tree<Type>::Add(const Type &num) const
{
    Tree<value_type> res(*this);
    
    res.Insert(num);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator+(const Type &num) const
{
    return Add(num);
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqAdd(const Tree<value_type> &tree)
{
    for (auto &elem : tree)
        Insert(elem);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator+=(const Tree<value_type> &tree)
{
    return EqAdd(tree);
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqAdd(const Type &num)
{
    Insert(num);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator+=(const Type &num)
{
    return EqAdd(num);
}

template <Comparable Type>
Tree<Type> Tree<Type>::Add(const size_t sizeValue, const value_type *arr) const
{
    Tree<value_type> res(*this);

    for (size_t i = 0; i < size; ++i)
        res.Insert(arr[i]);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::Add(const initializer_list<value_type> elements) const 
{
    Tree<value_type> res(*this);

    for (auto elem : elements)
        res.Insert(elem);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::Add(Tree<value_type> &&tree) const 
{
    Tree<value_type> res(*this);

    for (auto elem : tree)
        res.Insert(elem);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::Add(iterator begin, iterator end) const
{
    Tree<value_type> res(*this);
    
    for (auto iter = begin; iter != end; ++iter)
        res.Insert(*iter);

    return res;
}

template <Comparable Type>
template <InputIterator I>
requires requires(typename I::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> Tree<Type>::Add(I begin, I end) const
{
    Tree<value_type> res(*this);

    for (auto iter = begin; iter != end; ++iter)
        res.Insert(*iter);

    return res;
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> Tree<Type>::Add(const Cont &container) const
{
    Tree<value_type> res(*this);

    for (auto iter = container.begin(); iter != container.end(); ++iter)
        res.Insert(*iter);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator+(const initializer_list<value_type> elements) const
{
    return Add(elements);
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator+(Tree<value_type> &&tree) const
{
    return Add(tree);
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> Tree<Type>::operator+(const Cont &container) const
{
    return Add(container);
}


template <Comparable Type>
Tree<Type> &Tree<Type>::EqAdd(const size_t sizeValue, const value_type *arr)
{
    for (size_t i = 0; i < size; ++i)
        Insert(arr[i]);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqAdd(const initializer_list<value_type> elements)
{
    for (auto elem : elements)
        Insert(elem);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqAdd(Tree<value_type> &&tree)
{
    for (auto elem : tree)
        Insert(elem);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqAdd(iterator begin, iterator end)
{
    for (auto iter = begin; iter != end; ++iter)
        Insert(*iter);

    return *this;
}

template <Comparable Type>
template <InputIterator I>
    requires requires(typename I::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> &Tree<Type>::EqAdd(I begin, I end)
{
    for (auto iter = begin; iter != end; ++iter)
        Insert(*iter);

    return *this;
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> &Tree<Type>::EqAdd(const Cont &container)
{
    for (auto iter = container.begin(); iter != container.end(); ++iter)
        Insert(*iter);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator+=(const initializer_list<value_type> elements)
{
    return EqAdd(elements);
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator+=(Tree<value_type> &&tree)
{
    return EqAdd(tree);
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> &Tree<Type>::operator+=(const Cont &container)
{
    return EqAdd(container);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::Add(const Tree<OtherType> &tree) const
{
    Tree<value_type> res(*this);

    for (auto &elem : tree)
        res.Insert(elem);

    return res;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::operator+(const Tree<OtherType> &tree) const
{
    return Add(tree);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::Add(const OtherType &num) const
{
    Tree<value_type> res(*this);
    
    res.Insert(num);

    return res;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::operator+(const OtherType &num) const
{
    return Add(num);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::EqAdd(const Tree<OtherType> &tree)
{
    for (auto &elem : tree)
        Insert(elem);

    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::operator+=(const Tree<OtherType> &tree)
{
    return EqAdd(tree);
}


template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::EqAdd(const OtherType &num)
{
    Insert(num);

    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::operator+=(const OtherType &num)
{
    return EqAdd(num);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::Add(const size_t sizeValue, const OtherType *arr) const
{
    Tree<value_type> res(*this);

    for (size_t i = 0; i < size; ++i)
        res.Insert(arr[i]);

    return res;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::Add(const initializer_list<OtherType> elements) const 
{
    Tree<value_type> res(*this);

    for (auto elem : elements)
        res.Insert(elem);

    return res;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::Add(Tree<OtherType> &&tree) const 
{
    Tree<value_type> res(*this);

    for (auto elem : tree)
        res.Insert(elem);

    return res;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::Add(Iterator<OtherType> begin, Iterator<OtherType> end) const
{
    Tree<value_type> res(*this);
    
    for (auto iter = begin; iter != end; ++iter)
        res.Insert(*iter);

    return res;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::operator+(const initializer_list<OtherType> elements) const
{
    return Add(elements);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> Tree<Type>::operator+(Tree<OtherType> &&tree) const
{
    return Add(tree);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::EqAdd(const size_t sizeValue, const OtherType *arr)
{
    for (size_t i = 0; i < size; ++i)
        Insert(arr[i]);

    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::EqAdd(const initializer_list<OtherType> elements)
{
    for (auto elem : elements)
        Insert(elem);

    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::EqAdd(Tree<OtherType> &&tree)
{
    for (auto elem : tree)
        Insert(elem);

    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::EqAdd(Iterator<OtherType> begin, Iterator<OtherType> end)
{
    for (auto iter = begin; iter != end; ++iter)
        Insert(*iter);

    return *this;
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::operator+=(const initializer_list<OtherType> elements)
{
    return EqAdd(elements);
}

template <Comparable Type>
template <Convertable_To<Type> OtherType>
Tree<Type> &Tree<Type>::operator+=(Tree<OtherType> &&tree)
{
    return EqAdd(tree);
}

#pragma endregion Add

#pragma region Remove

template <Comparable Type>
Tree<Type> Tree<Type>::Remove(const Tree<value_type> &tree) const
{
    Tree<value_type> res(*this);

    for (auto &elem : tree)
        res.Delete(elem);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator-(const Tree<value_type> &tree) const 
{
    return Remove(tree); 
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqRemove(const Tree<value_type> &tree)
{
    for (auto &elem : tree)
        Delete(elem);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator-=(const Tree<value_type> &tree) 
{ 
    return EqRemove(tree); 
}

template <Comparable Type>
Tree<Type> Tree<Type>::Remove(const Type &num) const
{
    Tree<value_type> res(*this);
    
    res.Delete(num);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator-(const Type &num) const 
{ 
    return Remove(num); 
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqRemove(const Type &num)
{
    Delete(num);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator-=(const Type &num) 
{ 
    return EqRemove(num); 
}

template <Comparable Type>
Tree<Type> Tree<Type>::Remove(const size_t sizeValue, const value_type *arr) const
{
    Tree<value_type> res(*this);

    for (size_t i = 0; i < size; ++i)
        res.Delete(arr[i]);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::Remove(const initializer_list<value_type> elements) const 
{
    Tree<value_type> res(*this);

    for (auto elem : elements)
        res.Delete(elem);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::Remove(Tree<value_type> &&tree) const 
{
    Tree<value_type> res(*this);

    for (auto elem : tree)
        res.Delete(elem);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::Remove(iterator begin, iterator end) const
{
    Tree<value_type> res(*this);
    
    for (auto iter = begin; iter != end; ++iter)
        res.Delete(*iter);

    return res;
}

template <Comparable Type>
template <InputIterator I>
    requires requires(typename I::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> Tree<Type>::Remove(I begin, I end) const
{
    Tree<value_type> res(*this);

    for (auto iter = begin; iter != end; ++iter)
        res.Delete(*iter);

    return res;
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> Tree<Type>::Remove(const Cont &container) const
{
    Tree<value_type> res(*this);

    for (auto iter = container.begin(); iter != container.end(); ++iter)
        res.Delete(*iter);

    return res;
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator-(const initializer_list<value_type> elements) const
{
    return Remove(elements);
}

template <Comparable Type>
Tree<Type> Tree<Type>::operator-(Tree<value_type> &&tree) const
{
    return Remove(tree);
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> Tree<Type>::operator-(const Cont &container) const
{
    return Remove(container);
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqRemove(const size_t sizeValue, const value_type *arr)
{
    for (size_t i = 0; i < size; ++i)
        Delete(arr[i]);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqRemove(const initializer_list<value_type> elements)
{
    for (auto elem : elements)
        Delete(elem);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqRemove(Tree<value_type> &&tree)
{
    for (auto elem : tree)
        Delete(elem);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::EqRemove(iterator begin, iterator end)
{
    for (auto iter = begin; iter != end; ++iter)
        Delete(*iter);

    return *this;
}

template <Comparable Type>
template <InputIterator I>
    requires requires(typename I::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> &Tree<Type>::EqRemove(I begin, I end)
{
    for (auto iter = begin; iter != end; ++iter)
        Delete(*iter);

    return *this;
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> &Tree<Type>::EqRemove(const Cont &container)
{
    for (auto iter = container.begin(); iter != container.end(); ++iter)
        Delete(*iter);

    return *this;
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator-=(const initializer_list<value_type> elements)
{
    return EqAdd(elements);
}

template <Comparable Type>
Tree<Type> &Tree<Type>::operator-=(Tree<value_type> &&tree)
{
    return EqRemove(tree);
}

template <Comparable Type>
template <ContainerComparable Cont>
requires requires(typename Cont::value_type t) {{ t } -> convertible_to<Type>; }
Tree<Type> &Tree<Type>::operator-=(const Cont &container)
{
    return EqRemove(container);
}

#pragma endregion Remove

#pragma endregion Operations

#pragma region ProtectedMethods
#pragma region Allocate
template <Comparable Type>
void Tree<Type>::allocateRoot() 
{
    try
    {
        node tmpNode;
        shared_ptr<node> tmp = tmpNode.allocateNode();
        root_ = tmp;
    }
    catch (bad_alloc &error)
    {
        time_t cur_time = time(NULL);
        throw MemoryException(ctime(&cur_time), __FILE__, __LINE__,
                              typeid(*this).name(), __FUNCTION__);
    }
}
#pragma endregion Allocate

#pragma region Checks
template <Comparable Type>
void Tree<Type>::zeroSizeCheck(const size_t line) const
{
    if (IsEmpty())
    {
        time_t cur_time = time(NULL);
        throw EmptyTreeException(ctime(&cur_time), __FILE__, line,
                                   typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
void Tree<Type>::indexCheck(const size_t index, const size_t line) const
{
    if (index >= Size())
    {
        time_t cur_time = time(NULL);
        throw OutOfRangeException(ctime(&cur_time), __FILE__, line,
                                  typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
template <typename OtherType>
void Tree<Type>::sizesCheck(const Tree<OtherType> &tree, const size_t line) const
{
    if (Size() != tree.Size())
    {
        time_t cur_time = time(NULL);
        throw NotEqualSizesException(ctime(&cur_time), __FILE__, line,
                                     typeid(*this).name(), __FUNCTION__);
    }
}

template <Comparable Type>
template <typename OtherType>
void Tree<Type>::divisionByZeroCheck(const OtherType &num, const size_t line) const
{
    if (abs(num) < EPS)
    {
        time_t cur_time = time(NULL);
        throw DivisionByZeroException(ctime(&cur_time), __FILE__, line,
                                      typeid(*this).name(), __FUNCTION__);
    }
}

#pragma endregion Checks
#pragma endregion ProtectedMethods
#endif // _TREE_HPP_
