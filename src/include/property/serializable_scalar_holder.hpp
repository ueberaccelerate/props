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

template<typename O>
struct holder<ObjectType::scalar, O> : base_holder<O> {
  using type = Serializable<O>;
  type holder_object;
  
  holder() : holder_object{"int_value", "int_value_desc", "scalar", ObjectType::scalar} {
    holder_object.serialize = [&] () {
      DEBUG("%s %s\n", holder_object.name.data(), holder_object.type_name.data());
    };
  }
  
  holder(const char* name, const char* desc, PROPERTY_UNUSED const char* type_name) : holder_object{name, desc, "scalar", ObjectType::scalar}{
      holder_object.serialize = [&] () {
        DEBUG("%s %s\n", holder_object.name.data(), holder_object.type_name.data());
      };
  }

  holder(const char* name_, const char* desc_, PROPERTY_UNUSED const char* type_name_, std::vector<SerializeNode*>& childs) : holder_object{name_, desc_, "scalar", ObjectType::scalar}{
    holder_object.serialize = [&] (SerializeNode::SerializeQueue& out) {
      out.push(holder_object.name.data());
//        out[holder_object.name.data()] = holder_object.type_name.data();
      };
      childs.push_back(&holder_object);
      holder_object.value = 42;
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

    SerializeNode::SerializeQueue out;
    // when serialize is completed
    // then return yaml node
    holder_object.serialize(out);
    holder_object.commit(out);
  }
  
  template <typename Function>
  void deserialize( Function completed) {
    
  }
};

template<typename O>
using Scalar = std::conditional_t< is_base_of_holder<O>, O, holder<ObjectType::scalar, O>>;

}

#define serializable_s(param_name, param_type, param_name_text, param_desc) \
  public: \
    property::Scalar<param_type>  param_name{ # param_name, param_desc, #param_type, this->childs};

#endif // PROPERTY_SERIALIZABLE_SCALAR_HOLDER_HPP
