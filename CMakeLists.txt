cmake_minimum_required (VERSION 3.8)
project ("inmemory-storage")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(compiler)
include(dependencies)

add_executable (inmemory-storage 
	"inmemory-storage.cpp" 
	"tests/orm_storage.tests.cpp" 
	"tests/orm_storage.child.tests.cpp")

target_include_directories(inmemory-storage  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#enable_testing()
#include(tests)