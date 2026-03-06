cmake_minimum_required(VERSION 3.5)
project(semantic_map_manager)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =========================
# 编译标准
# =========================
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE Release)
add_compile_options(-O3 -Wall)

# =========================
# ROS 2 & ament
# =========================
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

# =========================
# 项目内依赖
# =========================
find_package(common REQUIRED)
find_package(motion_predictor REQUIRED)
find_package(vehicle_model REQUIRED)
find_package(vehicle_msgs REQUIRED)

# =========================
# 第三方库
# =========================
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

set(THIRD_PARTY_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/thirdparty
)

# =========================
# 核心算法库
# =========================
add_library(hkust_pl_smm
  src/semantic_map_manager.cc
  src/data_renderer.cc
  src/config_loader.cc
  src/traffic_signal_manager.cc
)

target_include_directories(hkust_pl_smm PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<BUILD_INTERFACE:${THIRD_PARTY_INCLUDE_DIRS}>
  $<INSTALL_INTERFACE:include>
  ${EIGEN3_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

ament_target_dependencies(hkust_pl_smm
  rclcpp
  common
  motion_predictor
  vehicle_model
  vehicle_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
)

target_link_libraries(hkust_pl_smm
  ${OpenCV_LIBRARIES}
)

# =========================
# ROS 适配层库
# =========================
add_library(hkust_smm_ros
  src/ros_adapter.cc
  src/visualizer.cc
)

target_include_directories(hkust_smm_ros PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(hkust_smm_ros
  hkust_pl_smm
)

ament_target_dependencies(hkust_smm_ros
  rclcpp
  visualization_msgs
  sensor_msgs
  geometry_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  vehicle_msgs
)

# =========================
# 安装
# =========================
install(
  TARGETS hkust_pl_smm hkust_smm_ros
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY inc/
  DESTINATION include
)

# =========================
# 导出（给下游包用）
# =========================
ament_export_include_directories(include)
ament_export_libraries(hkust_pl_smm hkust_smm_ros)
ament_export_targets(export_${PROJECT_NAME})

ament_export_dependencies(
  rclcpp
  visualization_msgs
  sensor_msgs
  geometry_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  common
  motion_predictor
  vehicle_model
  vehicle_msgs
)

ament_package()
