//
//  property.hpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//

#ifndef PROPERY_PROPERY_HPP
#define PROPERY_PROPERY_HPP

#include "property_config.hpp"

namespace property {
    PROPERTY_NODISCARD int property_version();
    PROPERTY_NODISCARD int property_major();
    PROPERTY_NODISCARD int property_minor();
}

#include "serializable_serialize_holder.hpp"
#include "serializable_scalar_holder.hpp"
#include "serializable_sequence_holder.hpp"
#include "serializable_map_holder.hpp"

#define SCALAR(param_name, param_type, param_desc) serializable_s(param_name, param_type, param_name, param_desc)
#define SEQUENCE(param_name, param_type, param_desc) serializable_v(param_name, param_type, param_name, param_desc)
#define MAP(param_name, param_type1, param_type2, param_desc) serializable_m(param_name, param_type1, param_type2, param_name, param_desc)

#define SERIALIZETHIS(type_name) \
 final: public property::Serialize < type_name >

/// class_name
#define CONSTRUCTORS(class_name) \
public: \
    class_name() : class_name { (std::string("class_name_") + std::string(#class_name)).data(), ""} { } \
    class_name(const std::string_view &name_, const std::string_view &desc_) : holder { name_.data(), desc_.data(), #class_name, nullptr } {} \
    class_name (const std::string_view &name_, const std::string_view &desc_, const std::string_view& type_name_, property::SerializeNode *parent) : holder { name_.data(), desc_.data(), type_name_.data(), parent } { } \

#endif // PROPERY_PROPERY_HPP
