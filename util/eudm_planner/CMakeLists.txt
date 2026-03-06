cmake_minimum_required(VERSION 3.5)
project(eudm_planner)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
add_compile_options("-O3" "-Wall")

# 1. 查找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(common REQUIRED)
find_package(semantic_map_manager REQUIRED)
find_package(forward_simulator REQUIRED)
find_package(route_planner REQUIRED)
find_package(Protobuf REQUIRED)

# 2. Protobuf 生成
include_directories(${PROTOBUF_INCLUDE_DIRS})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS proto/eudm_config.proto)

# 3. 库定义 (使用 Project Name 确保唯一性)
add_library(eudm_planner SHARED
    src/eudm_planner/eudm_planner.cc
    src/eudm_planner/map_adapter.cc
    src/eudm_planner/eudm_manager.cc
    src/eudm_planner/dcp_tree.cc
    ${PROTO_SRCS}
)
target_include_directories(eudm_planner PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(eudm_planner ${Protobuf_LIBRARIES})
# target_link_libraries(eudm_planner hkust_pl_common)
ament_target_dependencies(eudm_planner rclcpp common semantic_map_manager forward_simulator route_planner)

add_library(eudm_server_ros SHARED src/eudm_planner/eudm_server_ros.cc)
target_include_directories(eudm_server_ros PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>)
target_link_libraries(eudm_server_ros eudm_planner)
ament_target_dependencies(eudm_server_ros rclcpp)

# 4. 安装与导出 (关键：必须安装到 lib 目录)
install(TARGETS eudm_planner eudm_server_ros
    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY inc/ DESTINATION include)
install(FILES ${PROTO_HDRS} DESTINATION include)

ament_export_include_directories(include)
ament_export_libraries(eudm_planner eudm_server_ros) # 👈 确保这行正确
ament_export_targets(export_${PROJECT_NAME})
ament_package()