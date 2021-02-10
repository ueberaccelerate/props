set build_dir=build
set vcpkg_toolchain_file=..\..\..\Common\vcpkg\scripts\buildsystems\vcpkg.cmake 

if not exist %build_dir%\ mkdir %build_dir%\ 

cd %build_dir%

cmake .. -DCMAKE_TOOLCHAIN_FILE=%vcpkg_toolchain_file% -DVCPKG_APPLOCAL_DEPS=ON



