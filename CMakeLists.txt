cmake_minimum_required(VERSION 3.25)

# Application version.

file(READ "${CMAKE_SOURCE_DIR}/VERSION" MF_VERSION)
message(STATUS "mattflow version : ${MF_VERSION}")

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
#     mattflow Project     #
############################

project(mattflow VERSION "${MF_VERSION}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(MATTFLOW_ENABLE_GPERF_PROFILER "Whether to compile with GPerf profiler." OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# NOTE(Matthew): We don't set -Werror globally despite wanting to as GCC is moaning
#                about a maybe-uninitialized variable in boost graphviz. As best I can
#                tell this is spurious, and Clang is fine with the library.
if (UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wold-style-cast -Wshadow -Wformat=2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wuseless-cast")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -Werror")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wused-but-marked-unused -Werror")
    endif()
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "/W3 /WX /EHsc /Zc:preprocessor")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Od /Zi")

    # Don't error on secure warnings.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
endif()

########################################################################################
#
# mattflow Compiler Library
#
########################################################################################

#########################################
#     mattflow Dependency Discovery     #
#########################################

find_package(concurrentqueue REQUIRED)
find_package(Boost REQUIRED)
find_package(absl REQUIRED)
find_package(re2 REQUIRED)

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
    "${PROJECT_SOURCE_DIR}/src/ast/link.cpp"
    "${PROJECT_SOURCE_DIR}/src/ast/parse.cpp"
    "${PROJECT_SOURCE_DIR}/src/ast/state.cpp"
    "${PROJECT_SOURCE_DIR}/src/lex/lexer.cpp"
    "${PROJECT_SOURCE_DIR}/src/literal/identifier.cpp"
    "${PROJECT_SOURCE_DIR}/src/literal/number.cpp"
    "${PROJECT_SOURCE_DIR}/src/literal/string.cpp"
    "${PROJECT_SOURCE_DIR}/src/type/primitive.cpp"
    "${PROJECT_SOURCE_DIR}/src/source.cpp"
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
    ${concurrentqueue_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${absl_INCLUDE_DIRS}
    ${re2_INCLUDE_DIRS}
)
set(mattflow_Libraries
    re2::re2
)

if(MATTFLOW_ENABLE_GPERF_PROFILER)
set(mattflow_Libraries
    ${mattflow_Libraries}
    profiler
)
endif()

########################################################################################

####################################################
#     mattflow Include and Library Directories     #
####################################################

target_include_directories(mattflow
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(mattflow
    SYSTEM
    PUBLIC
    ${mattflow_Include_Dirs}
)

target_link_libraries(mattflow
    ${mattflow_Libraries}
)

target_compile_definitions(mattflow
    PUBLIC
        MATTFLOW_VERSION="${MF_VERSION}"
)

########################################################################################
#
# mattflow CLI Library
#
########################################################################################

#########################################
#     mattflow Dependency Discovery     #
#########################################

find_package(argparse REQUIRED)

####################################
#     mattflow cli Source Tree     #
####################################

add_executable(mattflow_cli
    "${PROJECT_SOURCE_DIR}/cli/main.cpp"
)

target_precompile_headers(mattflow_cli
    PUBLIC
        include/stdafx.h
)

target_compile_definitions(mattflow_cli
    PUBLIC
        MATTFLOW_VERSION="${MF_VERSION}"
)

if (RUNNER)
    target_compile_definitions(mattflow_cli
        PUBLIC
            RUNNER=1
    )
endif()

########################################################################################

########################################################
#     mattflow cli Include and Library Directories     #
########################################################

# Globally used includes and libraries.

set(mattflow_cli_Include_Dirs
    ${argparse_INCLUDE_DIRS}
)
set(mattflow_cli_Libraries
    mattflow
)

if(MATTFLOW_ENABLE_GPERF_PROFILER)
set(mattflow_cli_Libraries
    ${mattflow_cli_Libraries}
    profiler
)
endif()

########################################################################################

########################################################
#     mattflow cli Include and Library Directories     #
########################################################

target_include_directories(mattflow_cli
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(mattflow_cli
    SYSTEM
    PUBLIC
    ${mattflow_cli_Include_Dirs}
)

target_link_libraries(mattflow_cli
    ${mattflow_cli_Libraries}
)

########################################################################################
#
# mattflow tests Library
#
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

if (RUNNER)
    target_compile_definitions(mattflow_tests
        PUBLIC
            RUNNER=1
    )
endif()

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

if(MATTFLOW_ENABLE_GPERF_PROFILER)
set(mattflow_tests_Libraries
    ${mattflow_tests_Libraries}
    profiler
)
endif()

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
    ${mattflow_tests_Include_Dirs}
)

target_link_libraries(mattflow_tests
    ${mattflow_tests_Libraries}
)

target_compile_definitions(mattflow_tests
    PUBLIC
        MATTFLOW_VERSION="${MF_VERSION}"
)

########################################################################################

##################################
#     mattflow tests Testing     #
##################################

enable_testing()

add_test(
    NAME mattflow_tests
    COMMAND $<TARGET_FILE:mattflow_tests>
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/test"
)

########################################################################################
#
# mattflow CPack
#
########################################################################################

install(TARGETS mattflow_cli RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Matthew Marshall <matthew.marshall090396@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mattflow compiler.")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

include(CPack)
