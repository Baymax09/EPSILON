cmake_minimum_required(VERSION 3.5)
project(aux_tools)

# ROS 2 建议使用 C++14 或 C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-O3")
endif()

# 1. 查找 ament 及其依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)            # 代替 roscpp
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_ros REQUIRED)           # 代替 tf
find_package(vehicle_msgs REQUIRED)      # 假设此包已完成 ROS 2 迁移
find_package(OpenCV REQUIRED)

# 2. 包含目录
# ROS 2 推荐使用 target_include_directories，但在定义 target 之前
# 我们先保留全局包含路径，或者在 target 处单独指定
include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

# --- 假设你有一个可执行文件 aux_node (根据 ROS 1 惯例补充) ---
# add_executable(aux_node src/aux_node.cpp)
# target_include_directories(aux_node PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>
# )
# ament_target_dependencies(aux_node
#   rclcpp
#   std_msgs
#   sensor_msgs
#   tf2_ros
#   vehicle_msgs
# )
# target_link_libraries(aux_node ${OpenCV_LIBRARIES})

# 3. 安装规则 (ROS 2 必须显式安装，否则 ros2 run 找不到)
# install(TARGETS aux_node
#   DESTINATION lib/${PROJECT_NAME}
# )

# 4. 导出依赖项给 downstream
ament_export_dependencies(
  rclcpp
  std_msgs
  sensor_msgs
  tf2_ros
  vehicle_msgs
)

# 5. 声明 ament 包
ament_package()