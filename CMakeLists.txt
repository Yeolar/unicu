# Copyright (C) 2018, Yeolar

# Define project
project(unicu)

cmake_minimum_required(VERSION 3.2)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-scripts)

# Set project version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

# Required packages
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex)
find_package(GFlags REQUIRED)
find_package(ICU REQUIRED)
find_package(Accelerator REQUIRED)

# Optional packages
find_package(GTest)

# Setup environment
set(CMAKE_BUILD_TYPE Release)   # Debug: -g; Release: -O3 -DNDEBUG
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mpclmul")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")    # memcheck
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Link libraries
link_libraries(
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${Boost_LIBRARIES}
    ${GFLAGS_LIBRARIES}
    ${ICU_LIBRARIES}
    ${ICU_I18N_LIBRARIES}
    ${ACCELERATOR_LIBRARY}
)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${ACCELERATOR_INCLUDE_DIR}
)

# Check and setup configurations
include(CheckCXXSourceCompiles REQUIRED)

check_cxx_source_compiles("
    #include <string.h>
    #include <errno.h>
    int main() {
        char buf[1024];
        buf[0] = 0;
        int ret = strerror_r(ENOMEM, buf, sizeof(buf));
        return ret;
    }"
    UNI_HAVE_XSI_STRERROR_R)

if(GFLAGS_FOUND)
    set(CMAKE_REQUIRED_LIBRARIES "gflags")

    check_cxx_source_compiles("
        #include <gflags/gflags.h>
        void foo() { google::GetArgv(); }
        int main() { return 0; }"
        UNI_UNUSUAL_GFLAGS_NAMESPACE)

    if(UNI_UNUSUAL_GFLAGS_NAMESPACE)
        set(UNI_GFLAGS_NAMESPACE "google")
    else()
        set(UNI_GFLAGS_NAMESPACE "gflags")
    endif()
endif()

configure_file(
    ${PROJECT_SOURCE_DIR}/unicu/unicu-config.h.in
    ${PROJECT_SOURCE_DIR}/unicu/unicu-config.h
)

# Work on all sub directories
add_subdirectory(unicu)

add_library(unicu_static STATIC
    $<TARGET_OBJECTS:unicu>
)
set_target_properties(unicu_static PROPERTIES OUTPUT_NAME unicu)

add_library(unicu_shared SHARED
    $<TARGET_OBJECTS:unicu>
)
set_target_properties(unicu_shared PROPERTIES OUTPUT_NAME unicu)

install(TARGETS unicu_static
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE
                GROUP_READ
                WORLD_READ
)
install(TARGETS unicu_shared
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)

# Binary

# Test
if(GTEST_FOUND)
    enable_testing()
    add_subdirectory(unicu/test)
endif()

# Pack
set(CPACK_SYSTEM_NAME "amd64")
find_program(DPKG_CMD dpkg)
if(DPKG_CMD)
    execute_process(
        COMMAND "${DPKG_CMD}" --print-architecture
        OUTPUT_VARIABLE CPACK_SYSTEM_NAME
        OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB;TXZ")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "rdd@rddoc.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "unicu: unicode utilities of rddoc.")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-all-dev,libgflags-dev,libicu-dev,accelerator")
include(CPack)

