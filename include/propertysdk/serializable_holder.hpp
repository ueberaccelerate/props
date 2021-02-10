//
//  serializable_holder.hpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//
#ifndef PROPERTY_SERIALIZABLE_HOLDER_HPP
#define PROPERTY_SERIALIZABLE_HOLDER_HPP

#include "serializable.hpp"

#include <string_view>
#include <vector>

namespace property
{
  template <ObjectType objectType, typename O>
  struct holder;

  template <typename T>
  constexpr bool is_base_of_holder = std::is_base_of_v<holder<ObjectType::serialize, Serializable<T>>, T>;

  template <typename T>
  struct base_holder
  {
    using basename = T;
  };

  constexpr std::string_view concatenate_string_view(std::string_view str1, std::string_view str2, char *buffer)
  {
    int i = 0;

    for (std::string_view::iterator b = str1.begin(); b != str1.end(); ++b, ++i)
    {
      buffer[i] = *b;
    }

    for (std::string_view::iterator b = str2.begin(); b != str2.end(); ++b, ++i)
    {
      buffer[i] = *b;
    }
    return std::string_view(buffer);
  }

  template <typename T>
  struct is_vector_type : public std::false_type
  {
  };

  template <typename T>
  struct is_vector_type<std::vector<T>> : public std::true_type
  {
  };

  template <typename T>
  struct StringType : public std::false_type
  {
  };

  // SIMPLE LOOKUP: are we have c_str method then we have string type
  template <>
  struct StringType<std::string> : public std::true_type
  {
  };

  template <>
  struct StringType<const char *> : public std::true_type
  {
  };

  template <>
  struct StringType<std::string_view> : public std::true_type
  {
  };

  template <typename T>
  constexpr bool is_string_type_v = StringType<T>::value;

  template <typename T>
  constexpr bool is_vector_type_v = is_vector_type<T>::value;

  template <typename T>
  constexpr std::string_view deduce_prop_type_name(std::string_view stoke_type_name)
  {
    if constexpr (is_base_of_holder<T>)
    {
      return "serializable";
    }
    if constexpr (std::is_fundamental_v<T>)
    {
      return stoke_type_name;
    }
    if constexpr (is_string_type_v<T>)
    {
      return "string";
    }

    static_assert(is_vector_type_v<T> == false, "please use sequence define for vector type");
  }
} // namespace property

#endif // PROPERTY_SERIALIZABLE_HOLDER_HPP
