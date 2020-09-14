# Find mpfr with API version ?.?
#
# Usage:
#   find_package(mpfr [REQUIRED] [QUIET])
#
# Sets the following variables:
#   - MPFR_FOUND          .. true if library is found
#   - MPFR_LIBRARY      .. full path to library
#   - MPFR_INCLUDE_DIR    .. full path to include directory
#
# Honors the following optional variables:
#   - MPFR_INCLUDE_LOC    .. include directory path, to be searched before defaults
#   - MPFR_LIBRARY_LOC    .. the library's directory path, to be searched before defaults
#   - MPFR_STATIC_LIBRARY .. if true, find the static library version
#
# Copyright 2015 Joachim Coenen, Forschungszentrum Jülich.
# Redistribution permitted.

# find the mpfr include directory
find_path(MPFR_INCLUDE_DIR mpfr.h
    PATH_SUFFIXES include mpfr/include mpfr
    PATHS
    ${MPFR_INCLUDE_LOC}
    "C:/Program Files/mpfr/"
    ~/Library/Frameworks/
    /Library/Frameworks/
    /usr/local/
    /usr/
    /sw/ # Fink
    /opt/local/ # DarwinPorts
    /opt/csw/ # Blastwave
    /opt/
    )

set(CMAKE_REQUIRED_INCLUDES ${MPFR_INCLUDE_DIR})
set(CMAKE_REQUIRED_QUIET False)

# attempt to find static library first if this is set
if(MPFR_STATIC_LIBRARY)
    set(MPFR_STATIC mpfr.a)
    #set(MPFR_STATIC mpfr.lib)
endif()

# find the mpfr library
find_library(MPFR_LIBRARY_RELEASE
    NAMES
    #${MPFR_STATIC}
    mpfr
    PATH_SUFFIXES lib64 lib
    PATHS
    ${MPFR_LIBRARY_LOC}
    "C:/Program Files/mpfr/Release"
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    )
if (MPFR_USE_DEBUG_LIBRARY)
    # find the mpfr library
    find_library(MPFR_LIBRARY_DEBUG
        NAMES
        #${MPFR_STATIC}
        mpfr_d
        PATH_SUFFIXES lib64 lib
        PATHS
        ${MPFR_LIBRARY_LOC}
        "C:/Program Files/mpfr/Debug"
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        )
else()
    set(MPFR_LIBRARY_DEBUG ${MPFR_LIBRARY_RELEASE})
endif()

message(STATUS "Found FindMPFR (release): ${MPFR_LIBRARY_RELEASE}")
message(STATUS "Found FindMPFR (debug): ${MPFR_LIBRARY_DEBUG}")
mark_as_advanced(MPFR_INCLUDE_DIR MPFR_LIBRARY_RELEASE)
mark_as_advanced(MPFR_INCLUDE_DIR MPFR_LIBRARY_DEBUG)
