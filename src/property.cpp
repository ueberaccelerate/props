//
//  property.cpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//

#include <propertysdk/property.hpp>

namespace property
{

  int property_version() { return ((PROPERTY_MAJOR << 16) | PROPERTY_MINOR); }

  int property_major() { return (property_version() >> 16); }

  int property_minor() { return property_version() & 0xFF; }
} // namespace property
