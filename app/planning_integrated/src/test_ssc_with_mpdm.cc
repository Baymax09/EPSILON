/**
 * @file test_ssc_with_mpdm.cc
 * @author HKUST Aerial Robotics Group (lzhangbz@ust.hk)
 * @brief
 * @version 0.1
 * @date 2020-09-21
 * @copyright Copyright (c) 2020
 */
#include "rclcpp/rclcpp.hpp"
#include <stdlib.h>

#include <chrono>
#include <iostream>

#include "behavior_planner/behavior_server_ros.h"
#include "semantic_map_manager/data_renderer.h"
#include "semantic_map_manager/ros_adapter.h"
#include "semantic_map_manager/semantic_map_manager.h"
#include "semantic_map_manager/visualizer.h"
#include "ssc_planner/ssc_server_ros.h"

// DECLARE_BACKWARD;
double ssc_planner_work_rate = 20.0;
double bp_work_rate = 20.0;

planning::SscPlannerServer* p_ssc_server_{nullptr};
planning::BehaviorPlannerServer* p_bp_server_{nullptr};

int BehaviorUpdateCallback(
    const semantic_map_manager::SemanticMapManager& smm) {
  if (p_ssc_server_) p_ssc_server_->PushSemanticMap(smm);
  return 0;
}

int SemanticMapUpdateCallback(
    const semantic_map_manager::SemanticMapManager& smm) {
  if (p_bp_server_) p_bp_server_->PushSemanticMap(smm);
  return 0;
}

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto nh = std::make_shared<rclcpp::Node>("test_ssc_with_mpdm_node");
  nh->declare_parameter<std::string>("ego_id", "");
  nh->declare_parameter<std::string>("agent_config_path", "");
  nh->declare_parameter<std::string>("ssc_config_path", "");

  int ego_id;
  if (!nh->get_parameter("ego_id", ego_id)) {
    RCLCPP_ERROR(rclcpp::get_logger("test_ssc_with_mpdm_node"),
                 "Failed to get param %d", ego_id);
    assert(false);
  }
  std::string agent_config_path;
  if (!nh->get_parameter("agent_config_path", agent_config_path)) {
    RCLCPP_ERROR(rclcpp::get_logger("test_ssc_with_mpdm_node"),
                 "Failed to get param %s", agent_config_path.c_str());
    assert(false);
  }

  std::string ssc_config_path;
  if (!nh->get_parameter("ssc_config_path", ssc_config_path)) {
    RCLCPP_ERROR(rclcpp::get_logger("test_ssc_with_mpdm_node"),
                 "Failed to get param ssc_config_path %s",
                 ssc_config_path.c_str());
    assert(false);
  }

  semantic_map_manager::SemanticMapManager semantic_map_manager(
      ego_id, agent_config_path);
  semantic_map_manager::RosAdapter smm_ros_adapter(nh, &semantic_map_manager);
  smm_ros_adapter.BindMapUpdateCallback(SemanticMapUpdateCallback);

  double desired_vel;
  nh->declare_parameter<std::string>("desired_vel", "6.0");
  // Declare bp
  p_bp_server_ = new planning::BehaviorPlannerServer(nh, bp_work_rate, ego_id);
  p_bp_server_->set_user_desired_velocity(desired_vel);
  p_bp_server_->BindBehaviorUpdateCallback(BehaviorUpdateCallback);
  p_bp_server_->set_autonomous_level(3);
  p_bp_server_->enable_hmi_interface();

  p_ssc_server_ =
      new planning::SscPlannerServer(nh, ssc_planner_work_rate, ego_id);

  p_ssc_server_->Init(ssc_config_path);
  p_bp_server_->Init();
  smm_ros_adapter.Init();

  p_bp_server_->Start();
  p_ssc_server_->Start();

  // TicToc timer;
  rclcpp::Rate rate(100);
  while (rclcpp::ok()) {
    rclcpp::spin_some(nh);
    rate.sleep();
  }

  return 0;
}
