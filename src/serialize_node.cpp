#include <property/serialize_node.hpp>

#include <yaml-cpp/yaml.h>

#include <ueberlog/ueberlog.hpp>
#include <strstream>
namespace property {


SerializeNode::SerializeNode() = default;
SerializeNode::~SerializeNode() = default;

SerializeNode::SerializeNode(const char *name, const char *desc, const char *type_name, const ObjectType object_type, SerializeNode *parent) : name{name}, type_name{type_name}, desc{desc}, object_type{object_type}, parent{parent}
{
    DEBUG("created node: %s %s %s \n", name, desc, type_name);
}
}
