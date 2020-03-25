#boost
find_package(Boost REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_libraries(${Boost_LIBRARIES})
	message(STATUS "Found boost v.${Boost_VERSION}: [${Boost_INCLUDE_DIRS}] [${Boost_LIBRARIES}]")
else()
	message(FATAL_ERROR "Could not find boost")
endif()

#include(ExternalProject)
#find_package(Git REQUIRED)

#ExternalProject_Add(install-cppcoro
#	PREFIX ${CMAKE_SOURCE_DIR}/3rdparty/cppcoro-tmp
#	SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/cppcoro
#	GIT_REPOSITORY https://github.com/lewissbaker/cppcoro.git
#	GIT_PROGRESS 1
#	GIT_SUBMODULES_RECURSE 1
#	GIT_TAG vs2019
#	CONFIGURE_COMMAND ""
#	BUILD_IN_SOURCE 1
#	BUILD_COMMAND cake.bat
#	INSTALL_COMMAND ""
#	LOG_DOWNLOAD 1
#)

#ExternalProject_Get_Property(install-cppcoro SOURCE_DIR)
#add_library(cppcoro INTERFACE)
#target_include_directories(cppcoro INTERFACE ${SOURCE_DIR})
#unset(SOURCE_DIR)
