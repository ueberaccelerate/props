#ifndef PROPERTY_SERIALIZABLE_MAP_HOLDER_HPP
#define PROPERTY_SERIALIZABLE_MAP_HOLDER_HPP

#include "property_config.hpp"
#include "serializable_holder.hpp"

#include <ueberlog/ueberlog.hpp>

#include <unordered_map>

namespace property {
    template < typename K, typename O>
    struct holder < ObjectType::map, Serializable < std::unordered_map < K, O >> > {
        using type = Serializable < std::unordered_map < K, O >>;
        type holder_object;
    private:
        void setup_map_holder() {
            if (holder_object.parent) {
                holder_object.parent->childs.insert(SerializeNodePtr(&holder_object, StackDeleter<SerializeNode>{}));
                static_assert(!is_base_of_holder<K>, "don't use serializable as key");
                static_assert(!is_vector_type_v<K>, "don't use vector as key");
                
                if constexpr (is_base_of_holder<O>) {
                    holder_object.parent->node[holder_object.name] = holder_object.node;
                    holder_object.parent->node[holder_object.name].SetTag(
                        holder_object.type_name);

                    holder_object.deserialize = [&](YAML::Node newroot) {
                        YAML::Node newvalue = newroot[holder_object.name];
                        newvalue.SetTag(holder_object.type_name);
                        
                        for (YAML::const_iterator item = newvalue.begin(); item != newvalue.end(); ++item) {
                            const auto key = item->first.as<K>();
                            const auto value = item->second.as<YAML::Node>();
                            auto holderValue = O();
                            holderValue.deserialize(value);
                            holder_object.value.insert_or_assign(key, holderValue);
                        }
                        holder_object.node = newvalue;
                    };
                } else {
                    holder_object.parent->node[holder_object.name] = holder_object.node;
                    holder_object.parent->node[holder_object.name].SetTag(
                        holder_object.type_name);
                    holder_object.deserialize = [&](YAML::Node newroot) {
                        YAML::Node newvalue = newroot[holder_object.name];
                        newvalue.SetTag(holder_object.type_name);
                        for (YAML::const_iterator item = newvalue.begin(); item != newvalue.end(); ++item) {
                            const auto key = item->first.as<K>();
                            const auto value = item->second.as<O>();
                            holder_object.value.insert_or_assign(key, value);
                        }
                        holder_object.node = newvalue;
                    };
                }
            }
        }
    public:
        holder()
            : holder_object { "int_value", "int_value_desc", "scalar",
                              ObjectType::sequence, nullptr } {
            setup_map_holder();
        }

        holder(const char *name_, const char *desc_,
               PROPERTY_UNUSED const char *type_name_, SerializeNode * parent)
            : holder_object { name_, desc_, type_name_, ObjectType::sequence,
                              parent } {
//            DEBUG("typename: %s\n", holder_object.type_name.data());
//            auto it = std::find_if(holder_object.type_name.begin(), holder_object.type_name.end(), [](const auto ch){return ch=='!';});
//            std::string type1(holder_object.type_name.begin(),it);
//            std::string type2(std::next(it,2), holder_object.type_name.end());
//            DEBUG("typename: %s    %s\n", type1.data(), type2.data());
            setup_map_holder();
        }
        void insert_or_assign(const K& key, const O& value) {
            if constexpr (is_base_of_holder<O>) {
                holder_object.value.insert_or_assign(key, value);
                holder_object.node[key] = value.holder_object.node;
            } else {
                holder_object.node[key] = value;
            }
        }
    };

template<typename K, typename O>
using Map = holder<ObjectType::map, Serializable<std::unordered_map<K, O>>>;

inline std::string concatenate_with_string_inmiddle(const std::string &f, const std::string &s, const std::string &m) {
    return f + m + s;
}

}
#define serializable_m(param_name, param_type1, param_type2, param_name_text, param_desc) \
  public: \
property::Map<param_type1, param_type2>  param_name{ #param_name, param_desc, property::concatenate_with_string_inmiddle(property::deduce_prop_type_name<param_type1>( #param_type1 ).data(), property::deduce_prop_type_name<param_type2>( #param_type2 ).data(), "!!").data(),  \
&this->holder_object};

#endif //PROPERTY_SERIALIZABLE_MAP_HOLDER_HPP
