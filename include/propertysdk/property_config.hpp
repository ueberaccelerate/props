//
//  property_config.hpp
//  propertysdk
//
//  Created by admin on 27.05.2020.
//

#ifndef PROPERTY_CONFIG_HPP
#define PROPERTY_CONFIG_HPP

#define PROPERTY_MAJOR 1
#define PROPERTY_MINOR 0

#define PROPERTY_VERSION ((PROPERTY_MAJOR << 16) | PROPERTY_MINOR)

#define PROPERTY_NOEXCEPT noexcept
#define PROPERTY_NODISCARD [[nodiscard]]
#define PROPERTY_UNUSED [[maybe_unused]]

#endif // PROPERTY_CONFIG_HPP
