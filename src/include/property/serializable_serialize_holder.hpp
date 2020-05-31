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
  class serialize_error : public std::exception {

  };
  class empty_serialize_error : public serialize_error {
  public:
    virtual const char* what() const throw() override {
      return "serialize: empty serialize data error";
    }
  };
  class size_serialize_error : public serialize_error {
  public:
    virtual const char* what() const throw() override {
      return "serialize: dismatch serialize data and class member";
    }
  };
  class parser_serialize_error : public serialize_error {
    std::string_view message;
  public:
    parser_serialize_error(std::string_view message) : message{message} {}
    virtual const char* what() const throw() override {
      return message.data();
    }
  };
  template < typename O >
  struct holder < ObjectType::serialize, Serializable < O >> {
      using ResultFunction = std::function < void (const std::string &serialize_data) >;
      using value_type = Serializable < O * >;
      public:
      value_type holder_object;
        
      public:
      holder() : holder{"default_name", "default_desc", "default_type", nullptr} {}
      holder(const char *name, const char *desc, const char *type_name, SerializeNode *parent) : holder_object { name, desc, type_name, ObjectType::serialize, parent } {
          DEBUG("created holder object: %s\n", name);
          if (!parent) {
            holder_object.node[name] = "version: 1.0";
            holder_object.node[name].SetTag(name);
          }

          if (parent) {
            parent->node[name] = holder_object.node;
            parent->node[name].SetTag(type_name);
          }
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
          completed(out.c_str());
      }

      void deserialize(PROPERTY_UNUSED const std::string &serdata)
      {
        YAML::Node root = YAML::Load(serdata);
        YAML::Node &holder_node = holder_object.node;

        if(!root.size()) throw empty_serialize_error();
        if(root.size() != holder_node.size()) throw size_serialize_error();

        for (YAML::iterator ri = root.begin(), hi = holder_node.begin(); ri != root.end(); ++ri, ++hi) {

          if (hi->first.as<std::string>() == ri->first.as<std::string>()) hi->second = ri->second;
          else {
            std::cout <<  hi->first << " " << ri->first << "\n";
            throw parser_serialize_error("hi->first != ri->first");
          }
        }

      }
  };

  template < typename O >
  using Serialize = holder < ObjectType::serialize, Serializable < O >>;
}

#endif // SERIALIZABLE_SERIALIZE_HOLDER_HPP
