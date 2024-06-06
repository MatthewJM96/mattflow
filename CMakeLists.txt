cmake_minimum_required(VERSION 3.22)

########################################################################################

# #######################
# #     Setup Conan     #
# #######################

# message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
# file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
#             "${CMAKE_BINARY_DIR}/cmake/conan.cmake"
#             EXPECTED_HASH SHA256=5cdb3042632da3efff558924eecefd580a0e786863a857ca097c3d1d43df5dcd
#             TLS_VERIFY ON
#             STATUS DOWNLOAD_STATUS)

# list(GET DOWNLOAD_STATUS 0 DOWNLOAD_STATUS_CODE)
# if(NOT ${DOWNLOAD_STATUS_CODE} EQUAL 0)
#     message(FATAL_ERROR "Error downloading conan.cmake: ${DOWNLOAD_STATUS}")
# else()
#     message(STATUS "Download result: ${DOWNLOAD_STATUS}")
# endif()

# include(${CMAKE_BINARY_DIR}/cmake/conan.cmake)

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

#################################
#     mattflow Dependencies     #
#################################

# Here we specify libraries we require that Conan will be responsible for
# obtaining.

# set(mattflow
#     glm/0.9.9.8
# )

########################################################################################

#########################################
#     mattflow Depency Installation     #
#########################################

# Here we get Conan to install all those dependencies we have specified, using the same
# release mode, compiler, std library and so on that we will use to build mattflow.

# conan_cmake_configure(REQUIRES ${mattflow}
#                       GENERATORS cmake_find_package)

# conan_cmake_autodetect(settings)

# conan_cmake_install(PATH_OR_REFERENCE .
#                     BUILD missing
#                     REMOTE conancenter
#                     SETTINGS ${settings})

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

########################################################################################

#########################################
#     mattflow Dependency Discovery     #
#########################################

# Now all dependencies are built, we "discover them", i.e. set up some useful variables
# containing include directories and libraries we will add to our target.

# find_package(TBB REQUIRED COMPONENTS tbb)

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

add_executable(mattflow
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
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

# set(mattflow_Include_Dirs
#     ${glm_INCLUDE_DIRS}
# )
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

# target_include_directories(mattflow
#     SYSTEM
#     PUBLIC
#     ${mattflow_Include_Dirs}
# )

# target_link_libraries(mattflow
#     ${mattflow_Libraries}
# )
