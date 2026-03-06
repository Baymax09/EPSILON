# ROS 2 建议至少使用 3.5 版本 
cmake_minimum_required(VERSION 3.5)
project(route_planner)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ROS 2 默认推荐 C++14 或 C++17 
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_VERBOSE_MAKEFILE "true")
set(CMAKE_BUILD_TYPE "Release")

# 基础编译选项优化
add_compile_options("-O3" "-Wall")

# 1. 查找 ament_cmake 和 ROS 2 核心依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)            # 取代 roscpp 
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# 2. 查找算法/自定义依赖 (需确保这些包已完成 ROS 2 迁移)
find_package(common REQUIRED)
find_package(semantic_map_manager REQUIRED)

# 3. 创建库: hkust_pl_rp
add_library(hkust_pl_rp
    src/route_planner/route_planner.cc
)

# 使用 target_include_directories 代替全局 include_directories 
target_include_directories(hkust_pl_rp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

# ament_target_dependencies 会自动处理头文件路径和库链接 
ament_target_dependencies(hkust_pl_rp
    rclcpp
    visualization_msgs
    sensor_msgs
    common
    semantic_map_manager
)

# 4. 安装规则 (ROS 2 必须显式安装) 
install(TARGETS hkust_pl_rp
    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY inc/
    DESTINATION include
)

# 5. 导出依赖项给下游包 (取代 catkin_package) 
ament_export_include_directories(include)
ament_export_libraries(hkust_pl_rp)
ament_export_targets(export_${PROJECT_NAME})
ament_export_dependencies(rclcpp common semantic_map_manager)

ament_package() # 必须以该宏结尾