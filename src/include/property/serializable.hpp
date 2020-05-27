#ifndef PROPERTY_SERIALIZABLE_HPP
#define PROPERTY_SERIALIZABLE_HPP

#include "serialize_node.hpp"

namespace property {

    template < typename T >
    struct Serializable : SerializeNode {
        T value;
        Serializable(const char *name_, const char *desc_, const char *type_name_, const ObjectType object_type) : SerializeNode { name_, desc_, type_name_, object_type} {
        }
    };
  
}

#endif // PROPERTY_SERIALIZABLE_HPP
