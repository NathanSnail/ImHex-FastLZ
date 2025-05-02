# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-src"
  "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-build"
  "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-subbuild/fastlz-populate-prefix"
  "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-subbuild/fastlz-populate-prefix/tmp"
  "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-subbuild/fastlz-populate-prefix/src/fastlz-populate-stamp"
  "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-subbuild/fastlz-populate-prefix/src"
  "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-subbuild/fastlz-populate-prefix/src/fastlz-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-subbuild/fastlz-populate-prefix/src/fastlz-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/nathan/Documents/code/ImHex-FastLZ/_deps/fastlz-subbuild/fastlz-populate-prefix/src/fastlz-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
