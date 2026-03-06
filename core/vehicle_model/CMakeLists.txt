cmake_minimum_required(VERSION 3.5)
project(vehicle_model)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_BUILD_TYPE Release)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(common REQUIRED)

add_library(hkust_pl_vehicle_model
  src/vehicle_model/vehicle_model.cc
  src/vehicle_model/ideal_steer_model.cc
  src/vehicle_model/idm_model.cc
  src/vehicle_model/ctx_idm_model.cc
  src/vehicle_model/controllers/pure_pursuit_controller.cc
  src/vehicle_model/controllers/pid_controller.cc
  src/vehicle_model/controllers/idm_velocity_controller.cc
  src/vehicle_model/controllers/ctx_idm_velocity_controller.cc
)

target_include_directories(hkust_pl_vehicle_model PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<INSTALL_INTERFACE:include>
  ${EIGEN3_INCLUDE_DIRS}
)

ament_target_dependencies(hkust_pl_vehicle_model
  rclcpp
  visualization_msgs
  sensor_msgs
  geometry_msgs
  nav_msgs
  common
)

install(TARGETS hkust_pl_vehicle_model
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY inc/
  DESTINATION include
)

ament_export_include_directories(include)
ament_export_libraries(hkust_pl_vehicle_model)
ament_export_dependencies(
  rclcpp
  geometry_msgs
  nav_msgs
  common
)

ament_package()
