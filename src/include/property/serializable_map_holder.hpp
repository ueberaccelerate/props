#ifndef PROPERTY_SERIALIZABLE_MAP_HOLDER_HPP
#define PROPERTY_SERIALIZABLE_MAP_HOLDER_HPP

#include "property_config.hpp"
#include "serializable_holder.hpp"

#include <ueberlog/ueberlog.hpp>

#include <unordered_map>


namespace property {


template<typename K, typename O>
struct holder<ObjectType::map, Serializable<std::map<K, O>>> {
  using type = Serializable<std::map<K,O>>;
  type holder_object;
  holder(const char* name_, const char* desc_, std::vector<SerializeNode*>
      &childs) : holder_object{name_, desc_, "map"} {
   //childs.push_back(&holder_object.node);
   }

};

}

#endif //PROPERTY_SERIALIZABLE_MAP_HOLDER_HPP