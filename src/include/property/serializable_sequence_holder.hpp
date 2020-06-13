#ifndef PROPERTY_SERIALIZABLE_SEQUENCE_HOLDER_HPP
#define PROPERTY_SERIALIZABLE_SEQUENCE_HOLDER_HPP

#include "property_config.hpp"
#include "serializable_holder.hpp"

#include <ueberlog/ueberlog.hpp>

#include <vector>

namespace property {

template <typename O> struct holder<ObjectType::sequence, std::vector<O>> {
  using type = Serializable<std::vector<O>>;

private:
  std::vector<std::shared_ptr<O>> holder_objects;

protected:
  type holder_object;
  void setup_sequence_holder() {
    if (holder_object.parent) {
      holder_object.parent->childs.insert(
          SerializeNodePtr(&holder_object, StackDeleter<SerializeNode>{}));

      for (YAML::const_iterator element = holder_object.node.begin();
           element != holder_object.node.end(); ++element) {
        holder_object.parent->node[element->first] = element->second;
      }

      if constexpr (is_base_of_holder<O>) {
        holder_object.parent->node[holder_object.name] = YAML::Load("[]");
        holder_object.parent->node[holder_object.name].SetTag(
            holder_object.type_name);

        holder_object.deserialize = [&](YAML::Node newroot) {
          O holderValue{};
          YAML::Node vectorofholder = newroot[holder_object.name];
          for (YAML::iterator holder = vectorofholder.begin();
               holder != vectorofholder.end(); ++holder) {
            holderValue.deserialize(*holder);
            holder_object.value.push_back(holderValue);
          }
        };
      } else {
        holder_object.parent->node[holder_object.name] = holder_object.value;
        holder_object.parent->node[holder_object.name].SetTag(
            holder_object.type_name);

        holder_object.deserialize = [&](YAML::Node newroot) {
          YAML::Node newvalue = newroot[holder_object.name];
          newvalue.SetTag(holder_object.type_name);
          holder_object.value = newvalue.as<std::vector<O>>();
        };
      }
    }
  }
  void updateIfNeeded() {}

public:
  holder()
      : holder_object{"int_value", "int_value_desc", "scalar",
                      ObjectType::sequence, nullptr} {
    setup_sequence_holder();
  }

  holder(const char *name_, const char *desc_,
         PROPERTY_UNUSED const char *type_name_, SerializeNode *parent)
      : holder_object{name_, desc_, type_name_, ObjectType::sequence, parent} {
    setup_sequence_holder();
  }

  void push_back(const O &value) {
    if constexpr (is_base_of_holder<O>) {
      holder_object.value.push_back(value);

      holder_object.parent->node[holder_object.name].push_back(
          value.holder_object.node);
      holder_object.parent->node[holder_object.name].SetTag(
          holder_object.type_name);

    } else {
      DEBUG("push new value \n");
      updateIfNeeded();
      holder_object.value.push_back(value);

      holder_object.parent->node[holder_object.name].push_back(value);
      holder_object.parent->node[holder_object.name].SetTag(
          holder_object.type_name);
    }
  }

  O &operator[](const size_t index) {
    if (index > holder_object.value.size())
      throw std::out_of_range("index > size() !!!");
    return holder_object.value[index];
  }
  O operator[](const size_t index) const {
    if (index > holder_object.value.size())
      throw std::out_of_range("index > size() !!!");
    return holder_object.value[index];
  }
  auto size() const { return holder_object.value.size(); }
};

template <typename O>
using Sequence = holder<ObjectType::sequence, std::vector<O>>;
} // namespace property

#define serializable_v(param_name, param_type, param_name_text, param_desc)    \
public:                                                                        \
  property::Sequence<param_type> param_name{                                   \
      #param_name, param_desc,                                                 \
      (std::string("vector:") +                                                \
       std::string(property::deduce_prop_type_name<param_type>(#param_type)))  \
          .data(),                                                             \
      &this->holder_object};

#endif // PROPERTY_SERIALIZABLE_SEQUENCE_HOLDER_HPP
