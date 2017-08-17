cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
project(RadixEntity CXX)

add_library(RadixEntity STATIC
  src/iotypes/ValueType.cpp
  src/Entity.cpp
  src/Method.cpp
  src/Property.cpp
  src/Signal.cpp
)

target_include_directories(RadixEntity PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

target_compile_features(RadixEntity PUBLIC
  cxx_constexpr
  cxx_lambdas
  cxx_nullptr
  cxx_static_assert
  cxx_variadic_templates
)
