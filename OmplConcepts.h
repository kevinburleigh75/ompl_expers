#ifndef __OMPL_CONCEPTS_H__
#define __OMPL_CONCEPTS_H__

#include <type_traits>

template <class T>
concept bool OmplEqualityComparable() {
  return requires(const T& a, const T& b) {
    {a == b} -> bool;
    {a != b} -> bool;
  };
}

template <class T>
concept bool OmplLessThanComparable() {
  return requires(const T& a, const T& b) {
    {a < b} -> bool;
  };
}

template <class Type>
concept bool OmplRegular () {
  return std::is_default_constructible<Type>::value &&
         std::is_copy_constructible<Type>::value &&
         std::is_copy_assignable<Type>::value &&
         std::is_destructible<Type>::value &&
         OmplEqualityComparable<Type>() &&
         OmplLessThanComparable<Type>();
}

template <class Type>
concept bool OmplHasStateTypeTypedef () {
  return requires(const Type& a) {
    typename Type::StateType;
  };
}

template <class Type>
concept bool OmplState () {
  // return  OmplRegular<Type>() &&
  return OmplHasStateTypeTypedef<Type>();
}

template <class Type>
concept bool OmplSpace () {
  return OmplHasStateTypeTypedef<Type>();
}

#endif // __OMPL_CONCEPTS_H__
