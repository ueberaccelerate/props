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

        public:
        holder(const char *name, const char *desc, const char *type_name) : holder_object { name, desc, type_name, ObjectType::serialize }, childs {} {
            DEBUG("created holder object: %s\n", name);
            holder_object.serialize = [&] (SerializeNode::SerializeQueue& out) {
              out.push("test Test The desc of test class");
                //out[holder_object.name.data()] = holder_object.desc.data();
            };
        }

        void serialize(PROPERTY_UNUSED ResultFunction completed)
        {
          SerializeNode::SerializeQueue out;
          holder_object.serialize(out);
          for (auto child : childs) {
              child->serialize(out);
          }
          holder_object.commit(out);
//
//            std::string data;
//            completed(data);
        }

        void deserialize(PROPERTY_UNUSED const std::string &serdata)
        {
        
        }
    };

    template < typename O >
    using Serialize = holder < ObjectType::serialize, Serializable < O >>;
}

#endif // SERIALIZABLE_SERIALIZE_HOLDER_HPP
