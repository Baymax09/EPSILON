cmake_minimum_required(VERSION 3.5)
project(ssc_planner)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("-O3" "-Wall")
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

# 1. 查找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Protobuf REQUIRED)
find_package(common REQUIRED)
find_package(semantic_map_manager REQUIRED)
find_package(OpenCV REQUIRED) # 👈 修复：必须查找 OpenCV
find_package(glog REQUIRED)

# 2. Protobuf 生成
include_directories(${PROTOBUF_INCLUDE_DIRS})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS proto/ssc_config.proto)

# =============================================================
# 3. 库: hkust_pl_ssc
# =============================================================
add_library(hkust_pl_ssc
    src/ssc_planner/ssc_planner.cc
    src/ssc_planner/ssc_map.cc
    src/ssc_planner/map_adapter.cc
    ${PROTO_SRCS}
)

target_include_directories(hkust_pl_ssc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    ${common_INCLUDE_DIRS}               # 👈 修复：显式引入 common 头文件
    ${semantic_map_manager_INCLUDE_DIRS} # 👈 修复：显式引入语义地图头文件
    ${OpenCV_INCLUDE_DIRS}               # 👈 修复：显式引入 OpenCV 头文件
)

target_link_libraries(hkust_pl_ssc
    ${Protobuf_LIBRARIES}
    ${OpenCV_LIBS}                       # 👈 修复：链接 OpenCV 库
    OpenMP::OpenMP_CXX
    glog::glog
)

ament_target_dependencies(hkust_pl_ssc
    rclcpp
    visualization_msgs
    sensor_msgs
    common
    semantic_map_manager
)

# =============================================================
# 4. 库: ssc_server_ros
# =============================================================
add_library(ssc_server_ros
    src/ssc_planner/ssc_server_ros.cc
    src/ssc_planner/ssc_visualizer.cc
)

# 修复：ssc_server_ros 也需要继承这些包含路径
target_include_directories(ssc_server_ros PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(ssc_server_ros
    hkust_pl_ssc
)

ament_target_dependencies(ssc_server_ros
    rclcpp
    visualization_msgs
    sensor_msgs
    common
    semantic_map_manager
)

# =============================================================
# 5. 安装与导出
# =============================================================
install(TARGETS hkust_pl_ssc ssc_server_ros
    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY inc/ DESTINATION include)
install(FILES ${PROTO_HDRS} DESTINATION include)

ament_export_include_directories(include)
ament_export_libraries(hkust_pl_ssc ssc_server_ros)
# 修复：将 OpenCV 和 OpenMP 导出给下游包（如 planning_integrated）
ament_export_dependencies(rclcpp visualization_msgs sensor_msgs common semantic_map_manager OpenMP OpenCV glog)
ament_export_targets(export_${PROJECT_NAME})

ament_package()