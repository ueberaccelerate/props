//
//  serializable_scalar_holder.hpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//
#ifndef PROPERTY_SERIALIZABLE_SCALAR_HOLDER_HPP
#define PROPERTY_SERIALIZABLE_SCALAR_HOLDER_HPP

#include "property_config.hpp"
#include "serializable_holder.hpp"

#include <ueberlog/ueberlog.hpp>

namespace property {

template <typename O>
struct holder<ObjectType::scalar, O> : base_holder<O> {
    using type = Serializable<O>;

   protected:
    type holder_object;

   private:
    void setup_scalar_holder() {
        if (holder_object.parent) {
            holder_object.parent->childs.insert(&holder_object);

            holder_object.parent->node[holder_object.name] =
                holder_object.value;
            holder_object.parent->node[holder_object.name].SetTag(
                holder_object.type_name);

            if constexpr (is_base_of_holder<O>) {
                holder_object.deserialize = [&](YAML::Node newroot) {
                    // special case
                    DEBUG("set special case");
                };
            } else {
                holder_object.deserialize = [&](YAML::Node newroot) {
                    YAML::Node newvalue = newroot[holder_object.name];
                    holder_object.value = newvalue.as<O>();
                };
            }
        }
    }

   public:
    holder()
        : holder_object{"int_value", "int_value_desc", "scalar",
                        ObjectType::scalar, nullptr} {
        setup_scalar_holder();
    }

    holder(const char* name_, const char* desc_,
           PROPERTY_UNUSED const char* type_name_, SerializeNode* parent)
        : holder_object{name_, desc_, type_name_, ObjectType::scalar, parent} {
        setup_scalar_holder();
    }
    void set(const O& value) {
        DEBUG("set new value \n");
        holder_object.value = value;
        holder_object.parent->node[holder_object.name] = holder_object.value;
        holder_object.parent->node[holder_object.name].SetTag(
            holder_object.type_name);
    }

    O get() const { return holder_object.value; }
    O& get() { return holder_object.value; }
};

template <typename O>
using Scalar =
    std::conditional_t<is_base_of_holder<O>, O, holder<ObjectType::scalar, O>>;

template <typename FromT>
auto deduce_type() {
    if constexpr (is_base_of_holder<FromT>) {
        return std::declval<FromT>();
    } else {
        return std::declval<holder<ObjectType::scalar, FromT>>();
    }
}
}  // namespace property

#define serializable_s(param_name, param_type, param_name_text, param_desc) \
   public:                                                                  \
    property::Scalar<param_type> param_name{               \
        #param_name, param_desc,                                            \
        property::deduce_prop_type_name(param_type{}, #param_type).data(),  \
        &this->holder_object};

#endif  // PROPERTY_SERIALIZABLE_SCALAR_HOLDER_HPP
