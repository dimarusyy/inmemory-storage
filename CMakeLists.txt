cmake_minimum_required (VERSION 3.8)
project ("inmemory-storage")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(compiler)
include(dependencies)

add_executable (inmemory-storage 
	"inmemory-storage.cpp" 
	"tests/orm_base_storage.tests.cpp" 
	"tests/orm_storage.child.tests.cpp")

target_include_directories(inmemory-storage  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(inmemory-storage  PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/cppcoro/include)
#add_dependencies(${PROJECT_NAME} cppcoro)

#enable_testing()
#include(tests)

if(CMAKE_CL_64)
	target_link_directories(inmemory-storage PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/cppcoro/build/windows_x64_msvc15_optimised/lib)
else()
	target_link_directories(inmemory-storage PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/cppcoro/build/windows_x86_msvc15_optimised/lib)
endif()
target_link_libraries(inmemory-storage cppcoro.lib)