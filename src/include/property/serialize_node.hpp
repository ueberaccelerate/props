#ifndef PROPERTY_SERIALIZE_NODE_HPP
#define PROPERTY_SERIALIZE_NODE_HPP

#include <yaml-cpp/yaml.h>

#include <functional>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_set>
#include <iostream>

namespace property {

class serialize_error : public std::exception {};
class empty_serialize_error : public serialize_error {
   public:
    virtual const char *what() const throw() override {
        return "serialize: empty serialize data error";
    }
};
class size_serialize_error : public serialize_error {
   public:
    virtual const char *what() const throw() override {
        return "serialize: dismatch serialize data and class member";
    }
};
class parser_serialize_error : public serialize_error {
    std::string_view message;

   public:
    parser_serialize_error(std::string_view message) : message{message} {}
    virtual const char *what() const throw() override { return message.data(); }
};

/*
 *
 *    ObjectType:
 *      case serialize: // ...
 *      case scalar: // ...
 *      case sequence: // ...
 *      case map: // ...
 * */
enum class ObjectType { serialize, scalar, sequence, map };
struct SerializeNode;

using SerializeNodePtr = std::shared_ptr<SerializeNode>;

template <typename T>
class StackDeleter {
public:
    StackDeleter &operator()(T *) {
        return *this;
    }
};
struct SerializeNode {
    std::string name;
    std::string type_name;
    std::string desc;
    ObjectType object_type;

    YAML::Node node;
    SerializeNodePtr parent;

    std::unordered_set<SerializeNodePtr> childs;

    using VoidFunction = std::function<void(YAML::Node)>;

    VoidFunction serialize;
    VoidFunction deserialize;

    SerializeNode();
    SerializeNode(const char *name, const char *desc, const char *type_name,
                  const ObjectType object_type, SerializeNode *parent);
    ~SerializeNode();
    
    template<typename T>
    inline auto as() {
        if( type_name == "string") {
        return node[name].as<std::string>();
        }
        return 0;
    }
};
void serialize(std::string_view serdata,
               std::function<void(SerializeNodePtr )>);

}  // namespace property

#endif  // PROPERTY_SERIALIZE_NODE_HPP
