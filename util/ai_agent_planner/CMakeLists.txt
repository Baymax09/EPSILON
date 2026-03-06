cmake_minimum_required(VERSION 3.5)
project(ai_agent_planner)

# 设置 C++ 标准
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_VERBOSE_MAKEFILE "true")
set(CMAKE_BUILD_TYPE "Release")

# 基础编译选项
add_compile_options("-O3" "-Wall" "-g")

# 1. 查找 ament 及其 ROS 2 核心依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# 2. 查找自定义/第三方依赖
find_package(common REQUIRED)
find_package(vehicle_model REQUIRED)
find_package(forward_simulator REQUIRED)
find_package(semantic_map_manager REQUIRED)
find_package(behavior_planner REQUIRED)
find_package(route_planner REQUIRED) # ✅ 修复：必须显式查找 route_planner

# 3. 创建可执行文件
add_executable(onlane_ai_agent
    src/onlane_ai_agent.cc
)

# 4. 链接依赖项
# ament_target_dependencies 会自动处理头文件路径和库链接
ament_target_dependencies(onlane_ai_agent
    rclcpp
    visualization_msgs
    sensor_msgs
    common
    vehicle_model
    forward_simulator
    semantic_map_manager
    behavior_planner
    route_planner
)

# 5. 安装规则
install(TARGETS onlane_ai_agent
    DESTINATION lib/${PROJECT_NAME}
)

# 6. 导出并声明 ament 包
ament_package()