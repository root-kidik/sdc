cmake_minimum_required(VERSION 3.12)
project(uservice-dynconf CXX)

# Disable userver libraries that are not needed in this project
set(USERVER_FEATURE_MONGODB OFF CACHE BOOL "" FORCE)
set(USERVER_FEATURE_REDIS OFF CACHE BOOL "" FORCE)
set(USERVER_FEATURE_CLICKHOUSE OFF CACHE BOOL "" FORCE)
set(USERVER_FEATURE_RABBITMQ OFF CACHE BOOL "" FORCE)
set(DUSERVER_FEATURE_GRPC OFF CACHE BOOL "" FORCE)

# Compatibility mode: some systems don't support these features
set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)

add_executable(
  ${PROJECT_NAME}
  src/models/config.hpp
  src/utils/make_error.hpp
  src/utils/make_error.cpp
  src/cache/configs_cache.hpp
  src/cache/configs_cache.cpp
  src/handlers/admin_v1_configs.hpp
  src/handlers/admin_v1_configs.cpp
  src/handlers/configs_values.hpp
  src/handlers/configs_values.cpp
  src/handlers/admin_v1_configs_delete.hpp
  src/handlers/admin_v1_configs_delete.cpp
  src/sql/sql_query.hpp
  src/main.cpp)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} userver-core userver-postgresql)

include(UserverTestsuite)

userver_testsuite_add_simple()

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml
     ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT ${PROJECT_NAME})
install(
  FILES ${CONFIGS_FILES}
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
  COMPONENT ${PROJECT_NAME})
