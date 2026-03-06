cmake_minimum_required(VERSION 3.5)
project(motion_predictor)

# ROS 2 默认使用 C++14 或更高版本 
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_VERBOSE_MAKEFILE "true")
set(CMAKE_BUILD_TYPE "Release")

# 基础编译选项优化
add_compile_options("-O3" "-Wall" "-fPIC")

# 1. 查找 ament_cmake 及其 ROS 2 核心依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)            # 代替 roscpp
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# 2. 查找自定义/第三方依赖 (确保这些包已完成 ROS 2 迁移)
find_package(common REQUIRED)
find_package(vehicle_model REQUIRED)
find_package(forward_simulator REQUIRED)

# 3. 包含路径管理
# ROS 2 推荐使用针对目标 (Target) 的配置
# 如果本包定义了库（假设名为 hkust_pl_mp），应使用 target_include_directories
include_directories(
    inc
)

# 4. 安装规则 (ROS 2 必需) 
# 导出头文件以便其他包引用
install(DIRECTORY inc/
    DESTINATION include
)

# 5. 导出接口给下游包 (取代 catkin_package) 
ament_export_include_directories(include)
ament_export_dependencies(
    rclcpp 
    visualization_msgs 
    sensor_msgs 
    common 
    vehicle_model 
    forward_simulator
)

# 6. 声明 ament 包
ament_package()