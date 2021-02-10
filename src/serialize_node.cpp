#include <propertysdk/serialize_node.hpp>

#include <yaml-cpp/yaml.h>

#include <stack>

namespace property
{

  SerializeNode::SerializeNode()  = default;
  SerializeNode::~SerializeNode() = default;

  SerializeNode::SerializeNode(const char *name, const char *desc, const char *type_name, const ObjectType object_type, SerializeNode *parent)
  : name{name}, type_name{type_name}, desc{desc}, object_type{object_type}, parent{SerializeNodePtr(parent, StackDeleter<SerializeNode>{})}
  {
    node[this->name]          = "";
    node[this->name + "_doc"] = desc;

    node[this->name].SetTag(type_name);
    node[this->name + "_doc"].SetTag("doc");
  }

  void serialize_as_sequence(YAML::Node node, std::stack<YAML::Node> &stack_serialize, SerializeNodePtr parent)
  {
    if (node.IsScalar())
    {

      return;
    }
    if (node.IsSequence())
    {
      stack_serialize.push(node);
    }
    if (node.IsMap())
    {
      stack_serialize.push(node);
    }
  }

  void serialize_as_map(YAML::Node node, std::stack<YAML::Node> &stack_serialize, SerializeNodePtr parent)
  {
    if (node.IsScalar())
    {
      return;
    }
    if (node.IsSequence())
    {
      stack_serialize.push(node);
    }
    if (node.IsMap())
    {
      stack_serialize.push(node);
    }
  }

  void serialize(std::string_view serdata, std::function<void(SerializeNodePtr root)> processed)
  {
    if (serdata.empty())
      throw empty_serialize_error();
    YAML::Node             root = YAML::Load(serdata.data());
    std::stack<YAML::Node> stack_serialize;

    stack_serialize.push(root);
    auto parent = std::make_shared<SerializeNode>();

    auto stack_parent = parent;

    while (!stack_serialize.empty())
    {
      auto stack_object = stack_serialize.top();
      stack_serialize.pop();
      for (YAML::const_iterator node = stack_object.begin(); node != stack_object.end(); ++node)
      {
        if (stack_object.IsMap())
        {
          serialize_as_map(node->second, stack_serialize, stack_parent);
        }
        if (stack_object.IsSequence())
        {
          serialize_as_sequence(node->as<YAML::Node>(), stack_serialize, stack_parent);
        }
      }
    }
    processed(parent);
  }

} // namespace property
