cmake_minimum_required(VERSION 3.5)
project(vehicle_msgs)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(common REQUIRED)

set(msg_files
  "msg/State.msg"
  "msg/ControlSignal.msg"
  "msg/Lane.msg"
  "msg/LaneNet.msg"
  "msg/VehicleParam.msg"
  "msg/Vehicle.msg"
  "msg/VehicleSet.msg"
  "msg/OccupancyGridFloat.msg"
  "msg/OccupancyGridUInt8.msg"
  "msg/Circle.msg"
  "msg/CircleObstacle.msg"
  "msg/PolygonObstacle.msg"
  "msg/ObstacleSet.msg"
  "msg/FreeState.msg"
  "msg/ArenaInfo.msg"
  "msg/ArenaInfoDynamic.msg"
  "msg/ArenaInfoStatic.msg"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  DEPENDENCIES std_msgs geometry_msgs nav_msgs
)

# ===== 1. 定义接口库 =====
add_library(vehicle_msgs_interfaces INTERFACE)

target_include_directories(vehicle_msgs_interfaces
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

# ===== 2. 修正安装逻辑 =====
# 注意：这里安装的是 inc/ 目录，这样 install 文件夹下才会出现 include/vehicle_msgs/xxx.h
install(
  DIRECTORY inc/
  DESTINATION include
)

# 也要安装 target 本身，否则下游找不到这个 target
install(TARGETS vehicle_msgs_interfaces
  EXPORT export_vehicle_msgs_interfaces
)

# ===== 3. 正确导出 =====
# 导出包含目录
ament_export_include_directories(include)

# 导出 Target，这样下游包只需要 ament_target_dependencies(xxx vehicle_msgs) 就能自动包含路径
ament_export_targets(export_vehicle_msgs_interfaces HAS_LIBRARY_TARGET)

# 导出依赖
ament_export_dependencies(rosidl_default_runtime)

ament_package()
