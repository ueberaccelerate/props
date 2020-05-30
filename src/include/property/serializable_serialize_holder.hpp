//
//  serializable_serialize_holder.hpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//
#ifndef SERIALIZABLE_SERIALIZE_HOLDER_HPP
#define SERIALIZABLE_SERIALIZE_HOLDER_HPP

#include "property_config.hpp"

#include "serializable_holder.hpp"

#include <ueberlog/ueberlog.hpp>

namespace property {
    template < typename O >
    struct holder < ObjectType::serialize, Serializable < O >> {
        using ResultFunction = std::function < void (const std::string &serialize_data) >;
        using value_type = Serializable < O * >;
        protected:
        value_type holder_object;
        
        public:
        holder() : holder{"default_name", "default_desc", "default_type", nullptr} {}
        holder(const char *name, const char *desc, const char *type_name, SerializeNode *parent) : holder_object { name, desc, type_name, ObjectType::serialize, parent } {
            DEBUG("created holder object: %s\n", name);
            
//            holder_object.serialize = [&] () {
                holder_object.node[name] = "version: 1.0";
                holder_object.node[name].SetTag(name);
                
                if(parent) {
                    holder_object.node[holder_object.name + "_doc"] = type_name;
                    holder_object.node.remove(name);
                    parent->node[name] = holder_object.node;
                }

//            };
        }
        public:
        void serialize(PROPERTY_UNUSED ResultFunction completed)
        {
            YAML::Emitter out;
            out << YAML::BeginMap;
            for(YAML::const_iterator citrator = holder_object.node.begin(); citrator != holder_object.node.end(); ++citrator) {
                out << YAML::Key << citrator->first << YAML::Value << citrator->second;
            }
            out << YAML::EndMap;
//            holder_object.serialize(out);
            completed(out.c_str());
        }

        void deserialize(PROPERTY_UNUSED const std::string &serdata)
        {
        }
    };

    template < typename O >
    using Serialize = holder < ObjectType::serialize, Serializable < O >>;
}

#endif // SERIALIZABLE_SERIALIZE_HOLDER_HPP
