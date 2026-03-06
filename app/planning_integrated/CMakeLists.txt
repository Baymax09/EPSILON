cmake_minimum_required(VERSION 3.5)
project(planning_integrated)

add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
set(CMAKE_CXX_STANDARD 14)
add_compile_options("-O3")

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(common REQUIRED)
find_package(semantic_map_manager REQUIRED)
find_package(eudm_planner REQUIRED)
find_package(ssc_planner REQUIRED)
find_package(route_planner REQUIRED)
find_package(behavior_planner REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glog REQUIRED)

# =============================================================
# 可执行文件: test_ssc_with_eudm
# =============================================================
add_executable(test_ssc_with_eudm src/test_ssc_with_eudm.cc)

# 修复方案：直接使用 find_package 找到的变量
target_link_libraries(test_ssc_with_eudm
    ${eudm_planner_LIBRARIES}     # 👈 自动包含 eudm_planner 和 eudm_server_ros
    ${ssc_planner_LIBRARIES}      # 👈 自动包含 hkust_pl_ssc 和 ssc_server_ros
    ${route_planner_LIBRARIES}    # 👈 自动包含 hkust_pl_rp
    glog::glog
    OpenMP::OpenMP_CXX
)

ament_target_dependencies(test_ssc_with_eudm
    rclcpp common semantic_map_manager eudm_planner ssc_planner behavior_planner route_planner
)

# =============================================================
# 可执行文件: test_ssc_with_mpdm
# =============================================================
add_executable(test_ssc_with_mpdm src/test_ssc_with_mpdm.cc)
target_link_libraries(test_ssc_with_mpdm
    ${ssc_planner_LIBRARIES}
    ${behavior_planner_LIBRARIES}
    OpenMP::OpenMP_CXX
    ooqpbase
    ooqpgendense
    blas
    lapack
    gfortran
)
ament_target_dependencies(test_ssc_with_mpdm
    rclcpp common semantic_map_manager ssc_planner behavior_planner
)

install(TARGETS test_ssc_with_eudm test_ssc_with_mpdm DESTINATION lib/${PROJECT_NAME})
ament_package()