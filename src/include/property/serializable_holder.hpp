//
//  serializable_holder.hpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//
#ifndef PROPERTY_SERIALIZABLE_HOLDER_HPP
#define PROPERTY_SERIALIZABLE_HOLDER_HPP

#include "serializable.hpp"

#include <ueberlog/ueberlog.hpp>

#include <vector>

namespace property {
    template < ObjectType objectType, typename O >
    struct holder;

    template < typename T >
    constexpr bool is_base_of_holder = std::is_base_of_v < holder < ObjectType::serialize, Serializable < T >>, T >;

    template < typename T >
    struct base_holder {
        using basename = T;
    };

}

#endif // PROPERTY_SERIALIZABLE_HOLDER_HPP
