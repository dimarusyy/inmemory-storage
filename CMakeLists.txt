cmake_minimum_required (VERSION 3.8)
project ("inmemory-storage")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(compiler)
include(dependencies)

add_executable (${PROJECT_NAME} 
	"inmemory-storage.cpp" 
	"tests/orm_base_storage.tests.cpp" 
	"tests/orm_storage.child.tests.cpp"
	)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} cppcoro.lib)
add_dependencies(${PROJECT_NAME} dependencies)

#enable_testing()
#include(tests)
