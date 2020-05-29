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

        value_type holder_object;
        std::vector < SerializeNode * > childs;
        private:
        std::shared_ptr<SerializeNode> null_holder_object;
        bool is_null_holder_added{false};
        public:
        holder() = default;
        holder(const char *name, const char *desc, const char *type_name) : holder_object { name, desc, type_name, ObjectType::serialize }, childs {} {
            DEBUG("created holder object: %s\n", name);
            push_null_holder();
            holder_object.serialize = [&] (SerializeNode::SerializeQueue& out) {
                out.push(holder_object);
                if( !is_null_holder_added ) {
                    childs.push_back(null_holder_object.get()); is_null_holder_added = !is_null_holder_added;
                }
                
                for (auto child : childs) {
                    child->serialize(out);
                }
                DEBUG("created holder object: %d\n", childs.size());
            };
        }
        protected:
        void push_null_holder() {
            // const char *name, const char *desc, const char *type_name, const ObjectType object_type
            null_holder_object = std::make_shared<SerializeNode> ("end_of_", holder_object.name.data(), holder_object.type_name.data(), ObjectType::serialize );
            null_holder_object->serialize = [&] (SerializeNode::SerializeQueue& out) {
                out.push(*null_holder_object);
            };
        }
        public:
        void serialize(PROPERTY_UNUSED ResultFunction completed)
        {
            SerializeNode::SerializeQueue out;
            holder_object.serialize(out);
            completed(holder_object.commit(out));
        }

        void deserialize(PROPERTY_UNUSED const std::string &serdata)
        {
        }
    };

    template < typename O >
    using Serialize = holder < ObjectType::serialize, Serializable < O >>;
}

#endif // SERIALIZABLE_SERIALIZE_HOLDER_HPP
