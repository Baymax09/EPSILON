cmake_minimum_required(VERSION 3.5) # ROS 2 建议至少 3.5 
project(forward_simulator)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置 C++ 标准 
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_VERBOSE_MAKEFILE "true")
set(CMAKE_BUILD_TYPE "Release")

# 基础编译选项优化 
add_compile_options("-O3" "-Wall")

# 1. 查找 ament 及其 ROS 2 核心依赖 
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)            # 代替 roscpp
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# 2. 查找第三方/自定义依赖 (确保这些包已完成 ROS 2 迁移) 
find_package(common REQUIRED)
find_package(vehicle_model REQUIRED)

# 3. 包含目录管理 (ROS 2 推荐使用 target_include_directories) 
# 如果此包定义了库或可执行文件，请使用下面的 target 方式。
# 这里先保留全局包含以便向后兼容，但推荐后续根据具体 target 细化。
include_directories(
    inc
)

# 4. 安装规则 (ROS 2 必需，否则其他包无法链接头文件或运行节点) 
install(DIRECTORY inc/
    DESTINATION include
)

# 5. 导出依赖项给下游包 (取代 catkin_package) 
ament_export_include_directories(include)
# 如果有导出的库，请在此处添加：
# ament_export_libraries(...)
ament_export_dependencies(rclcpp visualization_msgs sensor_msgs common vehicle_model)

# 6. 声明 ament 包 
ament_package()