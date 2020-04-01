set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/await)
    #add_compile_options(/P)
else()
endif()