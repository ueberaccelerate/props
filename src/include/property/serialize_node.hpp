#ifndef PROPERTY_SERIALIZE_NODE_HPP
#define PROPERTY_SERIALIZE_NODE_HPP

#include <string>
#include <string_view>
#include <functional>
#include <queue>

#include <yaml-cpp/yaml.h>

//#include <vector>
//#include <map>
//#include <iostream>
//#include <string_view>
//#include <fstream>
//#include <cmath>
//#include <functional>
//#include <type_traits>

namespace property {




/*
*
*    ObjectType:
*      case serialize: // ...
*      case scalar: // ...
*      case sequence: // ...
*      case map: // ...
* */
    enum class ObjectType {
        serialize,
        scalar,
        sequence,
        map
    };

    struct SerializeNode {
        
        std::string name;
        std::string type_name;
        std::string desc;
        ObjectType object_type;
        
        YAML::Node node;
        SerializeNode *parent;
        
        using VoidFunction = std::function < void() >;

        VoidFunction serialize;
        VoidFunction deserialize;
        
        SerializeNode();
        SerializeNode(const char *name, const char *desc, const char *type_name, const ObjectType object_type, SerializeNode *parent);
        ~SerializeNode();
        
      
    };
}

#endif // PROPERTY_SERIALIZE_NODE_HPP
