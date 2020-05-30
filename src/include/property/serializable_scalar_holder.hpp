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

#include <sstream>

namespace property {

template<typename O>
struct holder<ObjectType::scalar, O> : base_holder<O> {
  using type = Serializable<O>;
  type holder_object;
private:
  void setup_scalar_holder() {
//    holder_object.serialize = [&] () {
        if(holder_object.parent) {
            holder_object.parent->node[holder_object.name] = holder_object.value;
            holder_object.parent->node[holder_object.name].SetTag(holder_object.type_name);
        }
//        (*parent)[this->name] = value;
//        (*parent)[this->name].SetTag(type_name.data());
//    };
  }
public:
  holder() : holder_object{"int_value", "int_value_desc", "scalar", ObjectType::scalar, nullptr} {
    setup_scalar_holder();
  }
  
//  holder(const char* name, const char* desc, PROPERTY_UNUSED const char* type_name) : holder_object{name, desc, type_name, ObjectType::scalar, nullptr}{
//      setup_scalar_holder();
//  }

  holder(const char* name_, const char* desc_, PROPERTY_UNUSED const char* type_name_, SerializeNode* parent) : holder_object{name_, desc_, type_name_, ObjectType::scalar, parent} {
      setup_scalar_holder();
  }
  void set(const O &value) {
    DEBUG( "set new value \n" );
    holder_object.value = value;
  }
  
  O& get() const {
    return holder_object.value;
  }
  
  template <typename Function>
  void serialize( Function completed) {

    // when serialize is completed
    // then return yaml node
//    holder_object.serialize(out);
//    completed(holder_object.commit(out));
  }
  
  template <typename Function>
  void deserialize( Function completed) {
    
  }
};

template<typename O>
using Scalar = std::conditional_t< is_base_of_holder<O>, O, holder<ObjectType::scalar, O>>;


template < typename T >
constexpr std::string_view deduce_prop_type_name(T && type, std::string_view stoke_type_name)
{
    if (is_base_of_holder<T>) {
        return stoke_type_name;
    }
    return "int";
}

}

#define serializable_s(param_name, param_type, param_name_text, param_desc) \
  public: \
property::Scalar<param_type>  param_name{ # param_name, param_desc, property::deduce_prop_type_name(param_type{}, #param_type).data(), &this->holder_object};

#endif // PROPERTY_SERIALIZABLE_SCALAR_HOLDER_HPP
