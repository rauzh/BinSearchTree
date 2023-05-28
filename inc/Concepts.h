#ifndef _BASE_CONCEPT_H_
#define _BASE_CONCEPT_H_

#include <concepts>

using namespace std;

template <typename Type>
concept  Comparable = requires(Type a, Type b) {
    { a == b } -> convertible_to<bool>;
    { a != b } -> convertible_to<bool>;
    { b == a } -> convertible_to<bool>;
    { b != a } -> convertible_to<bool>;
    { a > b } -> convertible_to<bool>;
    { a >= b } -> convertible_to<bool>;
    { b > a } -> convertible_to<bool>;
    { b >= a } -> convertible_to<bool>;
    { a < b } -> convertible_to<bool>;
    { a <= b } -> convertible_to<bool>;
    { b < a } -> convertible_to<bool>;
    { b <= a } -> convertible_to<bool>;
};

template <typename OtherType, typename Type>
concept Convertable_To = convertible_to<OtherType, Type>;

template <typename Type>
concept ContainerComparable = requires(Type t)
{
    typename Type::value_type;
    typename Type::size_type;
    typename Type::iterator;
    typename Type::const_iterator;

    Comparable<typename Type::value_type>;

    { t.size() } noexcept -> same_as<typename Type::size_type>;
    { t.end() } noexcept -> same_as<typename Type::iterator>;
    { t.begin() } noexcept -> same_as<typename Type::iterator>;
};

template <typename I>
concept IteratorConcept = requires()
{
    typename I::value_type;
    typename I::difference_type;
    typename I::pointer;
    typename I::reference;
};

template <typename T, typename U>
concept DerivedFrom = is_base_of<U, T>::value;

# pragma region Input_Iterator
template<typename T>
concept EqualityComparable = requires(T a, T b)
{
    { a == b } -> same_as<bool>;
    { a != b } -> same_as<bool>;
};

template <typename I>
concept InputIterator = IteratorConcept<I> &&
requires { typename I::iterator_category; }&&
EqualityComparable<I>&&
DerivedFrom<typename I::iterator_category, input_iterator_tag>;

# pragma endregion

#endif // _BASE_CONCEPT_H_
