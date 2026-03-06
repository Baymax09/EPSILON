cmake_minimum_required(VERSION 3.5)
project(phy_simulator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 解决 Boost 警告并添加编译选项
add_compile_options(-O3 -Wall -DBOOST_BIND_GLOBAL_PLACEHOLDERS)

# =========================
# 1. 查找依赖
# =========================
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(common REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(vehicle_model REQUIRED)
find_package(vehicle_msgs REQUIRED)

# =========================
# 2. 核心库 phy_simulator_lib
# =========================
add_library(phy_simulator_lib
  src/phy_simulator/arena_loader.cc
  src/phy_simulator/phy_simulator.cc
)

target_include_directories(phy_simulator_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  ${Eigen3_INCLUDE_DIRS}
)

# 为库添加依赖
ament_target_dependencies(phy_simulator_lib
  common
  vehicle_model
  vehicle_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  geometry_msgs
)

# =========================
# 3. 可执行节点
# =========================
add_executable(phy_simulator_planning_node
  src/phy_simulator_planning_node.cc
  src/phy_simulator/visualizer.cc
  src/phy_simulator/ros_adapter.cc
)

target_include_directories(phy_simulator_planning_node PRIVATE
  ${Eigen3_INCLUDE_DIRS}
)

# 链接内部库
target_link_libraries(phy_simulator_planning_node
  phy_simulator_lib
)

# 确保 ament 处理 tf2 的链接顺序
ament_target_dependencies(phy_simulator_planning_node
  rclcpp
  visualization_msgs
  sensor_msgs
  geometry_msgs
  nav_msgs
  vehicle_msgs
  vehicle_model
  tf2
  tf2_ros
  tf2_geometry_msgs
)

# =========================
# 4. 安装
# =========================
install(
  TARGETS phy_simulator_lib phy_simulator_planning_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()