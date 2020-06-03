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

template <typename O>
struct holder<ObjectType::serialize, Serializable<O>> {
    using ResultFunction =
        std::function<void(const std::string &serialize_data)>;
    using value_type = Serializable<O *>;
   public:
    value_type holder_object;
   public:
    holder()
        : holder{"default_name", "default_desc", "default_type", nullptr} {}
    holder(const char *name, const char *desc, const char *type_name,
           SerializeNode *parent)
        : holder_object{name, desc, type_name, ObjectType::serialize, parent} {
        DEBUG("created holder object: %s\n", name);
        if (parent) {
            holder_object.parent->childs.insert(SerializeNodePtr(&holder_object, StackDeleter<SerializeNode>{}));

            parent->node[name] = holder_object.node;
            parent->node[name].SetTag(type_name);
        } else {
            holder_object.node[name] = "version: 1.0";
            holder_object.node[name].SetTag(type_name);
        }

        holder_object.deserialize = [&](YAML::Node newroot) {
            // serialize object is not main in the scope
            // so we search it in map by key
            if (holder_object.parent) {
                holder_object.node = newroot[holder_object.name];
            } else {
                holder_object.node = newroot;
                holder_object.name = newroot.begin()->first.as<std::string>();
            }
            for (auto child : holder_object.childs) {
                child->deserialize(holder_object.node);
            }
        };
    }

   public:
    void setParent(SerializeNode *parent) {
        parent->childs.insert(SerializeNodePtr(&holder_object, StackDeleter<SerializeNode>{}));
        //          parent->node[holder_object.name] = holder_object.node;
        //          parent->node[holder_object.name].SetTag(holder_object.type_name);
        //          holder_object.node[holder_object.name] = "";
    }

    void serialize(PROPERTY_UNUSED ResultFunction completed) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        for (YAML::const_iterator citrator = holder_object.node.begin();
             citrator != holder_object.node.end(); ++citrator) {
            out << YAML::Key << citrator->first << YAML::Value
                << citrator->second;
        }
        out << YAML::EndMap;
        completed(out.c_str());
    }

    void deserialize(PROPERTY_UNUSED const std::string &serdata) {
        deserialize(YAML::Load(serdata));
    }

    void deserialize(PROPERTY_UNUSED YAML::Node root) {
        DEBUG("Validation serdata...\n");
        YAML::Node &holder_node = holder_object.node;

        if (!root.size()) throw empty_serialize_error();
        if (root.size() != holder_node.size()) throw size_serialize_error();

        YAML::iterator header = root.begin();
        const auto &header_tag = header->second.Tag();
        if (header_tag != holder_object.type_name) {
            throw parser_serialize_error("hi->first != ri->first");
        }
        
        for (YAML::iterator ri = std::next(root.begin()),
                            hi = std::next(holder_node.begin());
             ri != root.end(); ++ri, ++hi) {
            if (hi->first.as<std::string>() != ri->first.as<std::string>()) {
                throw parser_serialize_error("hi->first != ri->first");
            }
        }
        DEBUG("Validation serdata...OK\n");

        holder_object.deserialize(root);
    }
};

template <typename O>
using Serialize = holder<ObjectType::serialize, Serializable<O>>;
}  // namespace property

#endif  // SERIALIZABLE_SERIALIZE_HOLDER_HPP
