# inmemory-storage
In-memory storage implementation based on boost::multi_index_container.
Current implementation uses simple **Todo** struct declared in *include/config.h* file.

# Dependencies
The project depends on boost and doctest.
The easies way to get dependencies is to use **vcpkg** and install **boost** and **doctest** ports

# Build
Currently, build is only available on Windows with VS2019.
Please, perform the following steps:
* install vcpkg and boost+doctest dependencies
* open folder as cmake-project in VS2019
* build project

If you want to build on source directory, perform the following steps:
* *mkdir build; cd build*
* run cmake, e.g. : *cmake .. -DCMAKE_TOOLCHAIN_FILE="D:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows*
* *cmake --build .*

# TODO
* add reflection (magic_get and/or boost.fusion)
* consider using boost.intrusive and it's hashmap
* consider using google b-tree
* extend API with ring producer-consumer paradigm for read/write operations
