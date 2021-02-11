# Property sdk
[![Build Status](https://travis-ci.com/ueberaccelerate/props.svg?branch=master)](https://travis-ci.com/ueberaccelerate/props)

Propertysdk is a C++ project that allow you make serialize object.

## Build the library

```
mkdir build && cd build
cmake ..
make -j
```
## Examples
```
#include <property/property.hpp>

#include <string>

class Test SERIALIZETHIS(Test)
{
    SCALAR(age, int, "age of test");
    SEQUENCE(childs, std::string, "names vector");
    public:
    CONSTRUCTORS(Test)
    
    bool operator==(const Test &test) const{
        if (this->age.get() != test.age.get()) return false;
        if (this->childs.size() != test.childs.size()) return false;
        
        for (size_t i = 0; i < this->childs.size(); ++i) {
            if(this->childs[i] != test.childs[i]) return false;
        }
        return true;
    }
    bool operator!=(const Test &test) const  {
        return !Test::operator==(test);
    }
};
int main() {
  Test test{"test", "Simple Test class with scalar and basic sequnece"};
  test.age.set(77);
  test.childs.push_back("string one");
  test.childs.push_back("string two");
  std::string serdata;
  test3.serialize([&serdata](const std::string &sd) {
    serdata = sd;
  });
    
  Test ser_data;
  try {
    ser_data.deserialize(serdata);
  }
  catch (property::serialize_error &e) {
    return -1;
  }
}

```
## Maintenance

Maintainers: ueberacclerate <ueberacclerate@gmail.com>
