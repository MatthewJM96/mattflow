cmake_minimum_required(VERSION 3.22)

########################################################################################

# #######################
# #     Setup Conan     #
# #######################

message(STATUS "Downloading conan_provider.cmake from https://github.com/conan-io/cmake-conan")
file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/develop2/conan_provider.cmake"
            "${CMAKE_BINARY_DIR}/cmake/conan_provider.cmake"
            STATUS DOWNLOAD_STATUS)

list(GET DOWNLOAD_STATUS 0 DOWNLOAD_STATUS_CODE)
if(NOT ${DOWNLOAD_STATUS_CODE} EQUAL 0)
    message(FATAL_ERROR "Error downloading conan_provider.cmake: ${DOWNLOAD_STATUS}")
else()
    message(STATUS "Download result: ${DOWNLOAD_STATUS}")
endif()

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES
    "${CMAKE_PROJECT_TOP_LEVEL_INCLUDES};${CMAKE_BINARY_DIR}/cmake/conan_provider.cmake"
)

########################################################################################

############################
#     mattflow Options     #
############################

project(mattflow)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wold-style-cast -Wshadow -Wformat=2 -Werror")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wuseless-cast")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wused-but-marked-unused")
    endif()
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "/W3 /WX /EHsc /Zc:preprocessor")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Od /Zi")

    # Don't error on secure warnings.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
endif()

########################################################################################

#########################################
#     mattflow Dependency Discovery     #
#########################################

# Now all dependencies are built, we "discover them", i.e. set up some useful variables
# containing include directories and libraries we will add to our target.

find_package(concurrentqueue REQUIRED)
find_package(Boost REQUIRED)

########################################################################################

########################################
#     mattflow Compile Definitions     #
########################################

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG=1)
endif()

########################################################################################

################################
#     mattflow Source Tree     #
################################

add_library(mattflow
    "${PROJECT_SOURCE_DIR}/src/ast/parse.cpp"
    "${PROJECT_SOURCE_DIR}/src/lex/lexer.cpp"
    "${PROJECT_SOURCE_DIR}/src/type/number.cpp"
    "${PROJECT_SOURCE_DIR}/src/identifier.cpp"
    "${PROJECT_SOURCE_DIR}/src/number.cpp"
    "${PROJECT_SOURCE_DIR}/src/type.cpp"
)

target_precompile_headers(mattflow
    PUBLIC
        include/stdafx.h
)

########################################################################################

###################################################
#     mattflow Include and Library Directories     #
###################################################

# Globally used includes and libraries.

set(mattflow_Include_Dirs
    ${Boost_INCLUDE_DIRS}
    ${concurrentqueue_INCLUDE_DIRS}
)
# set(mattflow_Libraries
#     tbb
# )

########################################################################################

###################################################
#     mattflow Include and Library Directories     #
###################################################

target_include_directories(mattflow
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(mattflow
    SYSTEM
    PUBLIC
    ${mattflow_Include_Dirs}
)

# target_link_libraries(mattflow
#     ${mattflow_Libraries}
# )

########################################################################################

######################################
#     mattflow tests Source Tree     #
######################################

add_executable(mattflow_tests
    "${PROJECT_SOURCE_DIR}/test/main.cpp"
)

target_precompile_headers(mattflow_tests
    PUBLIC
        include/stdafx.h
)

########################################################################################

##########################################################
#     mattflow tests Include and Library Directories     #
##########################################################

# Globally used includes and libraries.

set(mattflow_tests_Include_Dirs
    ${Boost_INCLUDE_DIRS}
    ${concurrentqueue_INCLUDE_DIRS}
)
set(mattflow_tests_Libraries
    mattflow
)

########################################################################################

##########################################################
#     mattflow tests Include and Library Directories     #
##########################################################

target_include_directories(mattflow_tests
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(mattflow_tests
    SYSTEM
    PUBLIC
    ${mattflow_Include_Dirs}
)

target_link_libraries(mattflow_tests
    ${mattflow_tests_Libraries}
)
