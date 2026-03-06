cmake_minimum_required(VERSION 3.8)
project(common)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =========================
# C++ standard
# =========================
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -O3)

# =========================
# ament
# =========================
find_package(ament_cmake REQUIRED)

# =========================
# ROS2 dependencies
# =========================
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

# =========================
# Third-party dependencies
# =========================
find_package(Eigen3 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Glog REQUIRED)
find_package(OpenCV REQUIRED)
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

# =========================
# Paths
# =========================
set(COMMON_INC_DIR inc)
set(THIRD_PARTY_DIR thirdparty)
set(OOQP_INC_DIR thirdparty/ooqp/include/ooqp)

# =====================================================
# hkust_pl_ooqp_itf
# =====================================================
add_library(hkust_pl_ooqp_itf
  src/common/solver/ooqp_interface.cc
  src/common/solver/qp_solver.cc
)

target_link_libraries(hkust_pl_ooqp_itf
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpgendense.a
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpgensparse.a
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpgondzio.a
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpmehrotra.a
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpbound.a
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpdense.a
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpsparse.a
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ooqp/lib/libooqpbase.a
  ${BLAS_LIBRARIES}
  ${LAPACK_LIBRARIES}
  gfortran
)

target_include_directories(hkust_pl_ooqp_itf PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${COMMON_INC_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${THIRD_PARTY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${OOQP_INC_DIR}>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(hkust_pl_ooqp_itf
  Eigen3::Eigen
  ${GLOG_LIBRARIES}
  ${PROTOBUF_LIBRARY}
)

ament_target_dependencies(hkust_pl_ooqp_itf
  rclcpp
  visualization_msgs
  sensor_msgs
  tf2
  tf2_geometry_msgs
  nav_msgs
)

# =====================================================
# hkust_pl_common
# =====================================================
add_library(hkust_pl_common
  src/common/basics/colormap.cc
  src/common/basics/semantics.cc
  src/common/basics/shapes.cc
  src/common/basics/tool_func.cc
  src/common/math/calculations.cc
  src/common/rss/rss_checker.cc
  src/common/idm/intelligent_driver_model.cc
  src/common/mobil/mobil_model.cc
  src/common/mobil/mobil_behavior_prediction.cc
  src/common/spline/lookup_table.cc
  src/common/spline/spline_generator.cc
  src/common/lane/lane.cc
  src/common/lane/lane_generator.cc
  src/common/primitive/frenet_primitive.cc
  src/common/state/state_transformer.cc
  src/common/state/free_state.cc
  src/common/circle_arc/circle_arc.cc
  src/common/circle_arc/circle_arc_branch.cc
)

target_include_directories(hkust_pl_common PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${COMMON_INC_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${THIRD_PARTY_DIR}>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(hkust_pl_common
  hkust_pl_ooqp_itf
  Eigen3::Eigen
  tf2_geometry_msgs::tf2_geometry_msgs   # ✅ 新增（关键）
  ${OpenCV_LIBRARIES}
  ${GLOG_LIBRARIES}
)

ament_target_dependencies(hkust_pl_common
  rclcpp
  visualization_msgs
  sensor_msgs
  tf2
  tf2_geometry_msgs
  geometry_msgs        # ✅ 放在这里
)

# =========================
# Install
# =========================
install(
  TARGETS hkust_pl_ooqp_itf hkust_pl_common
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY inc/
  DESTINATION include
)

install(
  DIRECTORY thirdparty/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# =========================
# Export
# =========================
ament_export_targets(export_${PROJECT_NAME})
ament_export_include_directories(include)
ament_export_libraries(
  hkust_pl_common
  hkust_pl_ooqp_itf
)
ament_export_dependencies(
  rclcpp
  visualization_msgs
  sensor_msgs
  tf2
  tf2_geometry_msgs
  Eigen3
  Glog
  OpenCV
  Protobuf
)

ament_package()
