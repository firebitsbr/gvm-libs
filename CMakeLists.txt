# Greenbone Vulnerability Management
# $Id$
# Description: Top-level cmake control for the Greenbone Vulnerability Management Libraries.
#
# Authors:
# Matthew Mundell <matthew.mundell@greenbone.net>
# Jan-Oliver Wagner <jan-oliver.wagner@greenbone.net>
#
# Copyright:
# Copyright (C) 2011-2016 Greenbone Networks GmbH
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.

cmake_minimum_required(VERSION 3.0)

include (gvm_cmake_macros)

message ("-- Configuring the Greenbone Vulnerability Management Libraries...")

project (gvm-libs
  VERSION 1.0.0
  LANGUAGES C)

if (POLICY CMP0005)
  cmake_policy (SET CMP0005 NEW)
endif (POLICY CMP0005)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_BUILD_TYPE)

OPTION(BUILD_STATIC "Build static versions of the libraries" OFF)

if (NOT BUILD_STATIC)
  set (BUILD_SHARED ON)
else (NOT BUILD_STATIC)
  set (BUILD_SHARED OFF)
endif (NOT BUILD_STATIC)

## Retrieve svn revision (at configure time)
#  Not using Subversion_WC_INFO, as it would have to connect to the repo
find_program (SVN_EXECUTABLE svn DOC "subversion command line client")

macro (Subversion_GET_REVISION dir variable)
  execute_process (COMMAND ${SVN_EXECUTABLE} info ${CMAKE_SOURCE_DIR}/${dir}
    OUTPUT_VARIABLE ${variable}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  string (REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
    "\\2" ${variable} "${${variable}}")
endmacro (Subversion_GET_REVISION)

if (NOT CMAKE_BUILD_TYPE MATCHES "Release")
  if (EXISTS "${CMAKE_SOURCE_DIR}/.svn/" OR EXISTS "${CMAKE_SOURCE_DIR}/../.svn/")
    if (SVN_EXECUTABLE)
      Subversion_GET_REVISION(. ProjectRevision)
      set (SVN_REVISION "~svn${ProjectRevision}")
    else (SVN_EXECUTABLE)
      set (SVN_REVISION "~svn")
    endif (SVN_EXECUTABLE)
  endif (EXISTS "${CMAKE_SOURCE_DIR}/.svn/" OR EXISTS "${CMAKE_SOURCE_DIR}/../.svn/")
endif (NOT CMAKE_BUILD_TYPE MATCHES "Release")

# Set beta version if this is a beta release series,
# unset if this is a stable release series.
set (PROJECT_BETA_RELEASE 1)

if (SVN_REVISION)
  set (PROJECT_VERSION_SVN "${SVN_REVISION}")
endif (SVN_REVISION)

# If PROJECT_BETA_RELEASE is set, the version string will be set to:
#   "major.minor+beta${PROJECT_BETA_RELEASE}"
# If PROJECT_BETA_RELEASE is NOT set, the version string will be set to:
#   "major.minor.patch"
if (PROJECT_BETA_RELEASE)
  set (PROJECT_VERSION_SUFFIX "+beta${PROJECT_BETA_RELEASE}")
else (PROJECT_BETA_RELEASE)
  set (PROJECT_VERSION_SUFFIX ".${PROJECT_VERSION_PATCH}")
endif (PROJECT_BETA_RELEASE)

set (PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}${PROJECT_VERSION_SUFFIX}")

## CPack configuration

set (CPACK_CMAKE_GENERATOR "Unix Makefiles")
set (CPACK_GENERATOR "TGZ")
set (CPACK_INSTALL_CMAKE_PROJECTS ".;gvm-libs;ALL;/")
set (CPACK_MODULE_PATH "")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set (CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set (CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.md")
set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_TOPLEVEL_TAG "")
set (CPACK_SYSTEM_NAME "")
set (CPACK_TOPLEVEL_TAG "")

set (CPACK_PACKAGE_VERSION "${PROJECT_VERSION_STRING}${PROJECT_VERSION_SVN}")

set (CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_PACKAGE_VENDOR "Greenbone Networks")
set (CPACK_SOURCE_IGNORE_FILES
"${CMAKE_BINARY_DIR}"
"/.svn/"
"swp$"
)

include (CPack)

## Variables

if (SYSCONF_INSTALL_DIR)
  set (SYSCONFDIR "${SYSCONF_INSTALL_DIR}")
endif (SYSCONF_INSTALL_DIR)

if (NOT SYSCONFDIR)
  set (SYSCONFDIR "${CMAKE_INSTALL_PREFIX}/etc")
endif (NOT SYSCONFDIR)

if (NOT EXEC_PREFIX)
  set (EXEC_PREFIX "${CMAKE_INSTALL_PREFIX}")
endif (NOT EXEC_PREFIX)

if (NOT BINDIR)
  set (BINDIR "${EXEC_PREFIX}/bin")
endif (NOT BINDIR)

if (NOT SBINDIR)
  set (SBINDIR "${EXEC_PREFIX}/sbin")
endif (NOT SBINDIR)

if (NOT LIBDIR)
  set (_DEFAULT_LIBRARY_INSTALL_DIR lib)
  if (EXISTS "${EXEC_PREFIX}/lib32/" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    set (_DEFAULT_LIBRARY_INSTALL_DIR lib32)
  endif (EXISTS "${EXEC_PREFIX}/lib32/" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
  if (EXISTS "${CMAKE_INSTALL_PREFIX}/lib64/" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (_DEFAULT_LIBRARY_INSTALL_DIR lib64)
  endif (EXISTS "${CMAKE_INSTALL_PREFIX}/lib64/" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  set( LIBRARY_INSTALL_DIR "${_DEFAULT_LIBRARY_INSTALL_DIR}")
  set (LIBDIR "${EXEC_PREFIX}/${LIBRARY_INSTALL_DIR}")
endif (NOT LIBDIR)

if (NOT LOCALSTATEDIR)
  set (LOCALSTATEDIR "${CMAKE_INSTALL_PREFIX}/var")
endif (NOT LOCALSTATEDIR)

if (NOT INCLUDEDIR)
  set (INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include")
endif (NOT INCLUDEDIR)

if (NOT DATADIR)
  set (DATADIR "${CMAKE_INSTALL_PREFIX}/share")
endif (NOT DATADIR)

if (NOT GVM_PID_DIR)
  set (GVM_PID_DIR "${LOCALSTATEDIR}/run")
endif (NOT GVM_PID_DIR)

if (NOT GVM_SYSCONF_DIR)
  set (GVM_SYSCONF_DIR "${SYSCONFDIR}/gvm")
endif (NOT GVM_SYSCONF_DIR)

message ("-- Install prefix: ${CMAKE_INSTALL_PREFIX}")

set (HARDENING_FLAGS "-Wformat -Wformat-security -D_FORTIFY_SOURCE=2 -fstack-protector")
set (LINKER_HARDENING_FLAGS "-Wl,-z,relro -Wl,-z,now")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Werror")
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${HARDENING_FLAGS}")


## Version

if (PROJECT_BETA_RELEASE)
  set (LIBGVMCONFIG_VERSION "${PROJECT_VERSION}")
else (PROJECT_BETA_RELEASE)
  set (LIBGVMCONFIG_VERSION "${PROJECT_VERSION_STRING}")
endif (PROJECT_BETA_RELEASE)

set (GVMLIB_VERSION "${LIBGVMCONFIG_VERSION}")

# Configure Doxyfile with version number
configure_file (doc/Doxyfile.in ${CMAKE_BINARY_DIR}/doc/Doxyfile @ONLY)
configure_file (doc/Doxyfile_full.in ${CMAKE_BINARY_DIR}/doc/Doxyfile_full @ONLY)
configure_file (VERSION.in ${CMAKE_BINARY_DIR}/VERSION @ONLY)

## Program

if (NOT SKIP_SRC)
  add_subdirectory (base)
  add_subdirectory (util)
  add_subdirectory (osp)
  add_subdirectory (gmp)
endif (NOT SKIP_SRC)

## Documentation

add_subdirectory (doc)

## Tests

add_subdirectory (tests)

enable_testing ()

## End
