#boost
find_package(Boost REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_libraries(${Boost_LIBRARIES})
	message(STATUS "Found boost v.${Boost_VERSION}: [${Boost_INCLUDE_DIRS}] [${Boost_LIBRARIES}]")
else()
	message(FATAL_ERROR "Could not find boost")
endif()


include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(install-magic_get
	PREFIX ${CMAKE_BINARY_DIR}/3rdparty/magic_get
	GIT_REPOSITORY https://github.com/apolukhin/magic_get.git
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(install-magic_get SOURCE_DIR)
add_library(magic_get INTERFACE)
target_include_directories(magic_get INTERFACE ${SOURCE_DIR})
unset(SOURCE_DIR)
