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

ExternalProject_Add(install-cppcoro
	PREFIX ${CMAKE_SOURCE_DIR}/3rdparty/cppcoro-tmp
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/cppcoro
	GIT_REPOSITORY https://github.com/lewissbaker/cppcoro.git
	GIT_PROGRESS 1
	GIT_SUBMODULES_RECURSE 1
	GIT_TAG vs2019
	CONFIGURE_COMMAND ""
	BUILD_IN_SOURCE 1
	BUILD_COMMAND cake.bat lib/build.cake
	#BUILD_ALWAYS 1
	INSTALL_COMMAND ""
	LOG_DOWNLOAD 1
	LOG_BUILD 1
	LOG_OUTPUT_ON_FAILURE 1
)

ExternalProject_Add(install-magic_get
	PREFIX ${CMAKE_SOURCE_DIR}/3rdparty/magic_get-tmp
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/magic_get
	GIT_REPOSITORY https://github.com/apolukhin/magic_get.git
	GIT_PROGRESS 1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	LOG_DOWNLOAD 1
	LOG_OUTPUT_ON_FAILURE 1
)

add_custom_target(dependencies ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} DEPENDS install-cppcoro install-magic_get)

ExternalProject_Get_Property(install-cppcoro SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)
if(MSVC)
	if(CMAKE_CL_64)
		link_directories(${SOURCE_DIR}/build/windows_x64_msvc15_optimised/lib)
	else()
		link_directories(${SOURCE_DIR}/build/windows_x86_msvc15_optimised/lib)
	endif()
else()
endif()
unset(SOURCE_DIR)

ExternalProject_Get_Property(install-magic_get SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)
unset(SOURCE_DIR)