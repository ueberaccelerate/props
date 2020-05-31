#ifndef PROPERTY_SERIALIZABLE_SEQUENCE_HOLDER_HPP
#define PROPERTY_SERIALIZABLE_SEQUENCE_HOLDER_HPP

#include "property_config.hpp"
#include "serializable_holder.hpp"

#include <ueberlog/ueberlog.hpp>

#include <vector>

namespace property {

  
  template<typename O>
  struct holder<ObjectType::sequence, std::vector<O>> {
    using type = Serializable<std::vector<O>>;
  protected:
    type holder_object;
    void setup_sequence_holder() {
      if (holder_object.parent) {
        if constexpr (is_base_of_holder<O>) {
          //for (auto serialize_holder : holder_object.value) {
          //  serialize_holder.holder_object.node;
          //}
          holder_object.parent->node[holder_object.name] = YAML::Load("[]");;
          holder_object.parent->node[holder_object.name].SetTag(holder_object.type_name);
        }
        else {
          holder_object.parent->node[holder_object.name] = holder_object.value;
          holder_object.parent->node[holder_object.name].SetTag(holder_object.type_name);
        }
      }
    }
    void updateIfNeeded() {
      auto &node   = holder_object.parent->node[holder_object.name];
      auto &values = holder_object.value;
      for (auto n : node) {

        std::cout << n;
      }
    }
  public:

    holder() : holder_object{ "int_value", "int_value_desc", "scalar", ObjectType::sequence, nullptr } {
      setup_sequence_holder();
    }

    holder(const char* name_, const char* desc_, PROPERTY_UNUSED const char* type_name_, SerializeNode* parent) : holder_object{ name_, desc_, type_name_, ObjectType::sequence, parent } {
      setup_sequence_holder();
    }

    void push_back(const O& value) {
      if constexpr (is_base_of_holder<O>) {
        //for (auto serialize_holder : holder_object.value) {
        //  serialize_holder.holder_object.node;
        //}
        //holder_object.parent->node[holder_object.name] = YAML::Load("[]");;
        //holder_object.parent->node[holder_object.name].SetTag(holder_object.type_name);
      }
      else {
        DEBUG("push new value \n");
        updateIfNeeded();
        holder_object.value.push_back(value);
        holder_object.parent->node[holder_object.name].push_back(value);
        holder_object.parent->node[holder_object.name].SetTag(holder_object.type_name);
      }
    }
  };

  template<typename O>
  using Sequence = holder<ObjectType::sequence, std::vector<O>>;


  //template<typename K, typename O>
  //struct holder<ObjectType::map, Serializable<std::map<K, O>>> {
  //  using type = Serializable<std::map<K,O>>;
  //  type holder_object;
  //  holder(const char* name_, const char* desc_, std::vector<SerializeNode*> &childs) : holder_object{name_, desc_, "map"} {
  //    //childs.push_back(&holder_object.node);
  //    }
  //};

}

#define serializable_v(param_name, param_type, param_name_text, param_desc) \
  public: \
    property::Sequence<param_type>  param_name{ # param_name, param_desc, property::deduce_prop_type_name(param_type{}, #param_type).data(), &this->holder_object};

#endif // PROPERTY_SERIALIZABLE_SEQUENCE_HOLDER_HPP