//
//  property.cpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//

#include <propertysdk/property.hpp>

namespace
{
  constexpr size_t c_bitrwise_shift = 16U;
  constexpr size_t c_bitrwise_check = 0xFF;
} // namespace

namespace property
{

  auto property_version() PROPERTY_NOEXCEPT -> size_t { return ((PROPERTYSDK_MAJOR << c_bitrwise_shift) | PROPERTYSDK_MINOR); }

  auto property_major() PROPERTY_NOEXCEPT -> size_t { return (property_version() >> c_bitrwise_shift); }

  auto property_minor() PROPERTY_NOEXCEPT -> size_t { return property_version() & c_bitrwise_check; }
} // namespace property
