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

#define SCALAR(param_name, param_type, param_desc) serializable_s(param_name, param_type, param_name, param_desc)

#define SERIALIZETHIS(type_name) \
 final: public property::Serialize < type_name >

/// class_name
#define CONSTRUCTORS(class_name) \
public: \
    class_name(const std::string_view &name_, const std::string_view &desc_) : holder { name_.data(), desc_.data(), #class_name } {} \
    class_name(const std::string_view &name_, const std::string_view &desc_, const std::string_view& type_name_, std::vector < property::SerializeNode * > &childs_) : holder { name_.data(), desc_.data(), type_name_.data() } { \
        childs_.push_back(&holder_object); \
    }

#endif // PROPERY_PROPERY_HPP
