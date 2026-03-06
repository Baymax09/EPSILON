cmake_minimum_required(VERSION 3.5) # ROS 2 建议至少 3.5
project(behavior_planner)

# 设置 C++ 标准
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14) # ROS 2 默认推荐 C++14 或 17
endif()

set(CMAKE_VERBOSE_MAKEFILE "true")
set(CMAKE_BUILD_TYPE "Release")

# 基础编译选项优化
add_compile_options("-O3" "-Wall")

# 1. 查找 ament 及其 ROS 2 依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)            # 代替 roscpp
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

# 2. 查找第三方/自定义依赖 (确保这些包已完成 ROS 2 迁移)
find_package(common REQUIRED)
find_package(vehicle_model REQUIRED)
find_package(semantic_map_manager REQUIRED)
find_package(forward_simulator REQUIRED)
find_package(route_planner REQUIRED)

# 3. 创建库: behavior_planner_lib
add_library(behavior_planner_lib
    src/behavior_planner/behavior_planner.cc
    src/behavior_planner/map_adapter.cc
)

# 使用 target_include_directories 而非全局 include_directories
target_include_directories(behavior_planner_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

# ament_target_dependencies 会自动处理头文件路径和库链接
ament_target_dependencies(behavior_planner_lib
    rclcpp
    visualization_msgs
    sensor_msgs
    common
    vehicle_model
    semantic_map_manager
    forward_simulator
    route_planner
)

# 4. 创建库: behavior_planner_ros
add_library(behavior_planner_ros
    src/behavior_planner/behavior_server_ros.cc
)

target_include_directories(behavior_planner_ros PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

# 链接内部库
target_link_libraries(behavior_planner_ros
    behavior_planner_lib
)

ament_target_dependencies(behavior_planner_ros
    rclcpp
    semantic_map_manager
)

# 5. 安装规则 (ROS 2 必须显式安装) 
install(TARGETS behavior_planner_lib behavior_planner_ros
    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY inc/
    DESTINATION include
)

# 6. 导出依赖项给 downstream 包
ament_export_include_directories(include)
ament_export_libraries(behavior_planner_lib behavior_planner_ros)
ament_export_targets(export_${PROJECT_NAME})
ament_package() # 取代 catkin_package()