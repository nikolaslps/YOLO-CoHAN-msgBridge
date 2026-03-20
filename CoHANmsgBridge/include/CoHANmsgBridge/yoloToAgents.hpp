#pragma once

#include <rclcpp/rclcpp.hpp>
#include <vision_msgs/msg/detection3_d_array.hpp>
#include <cohan_msgs/msg/tracked_agents.hpp>
#include <cohan_msgs/msg/tracked_segment.hpp>
#include <cohan_msgs/msg/tracked_segment_type.hpp>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

#include <string>

namespace CoHANmsgBridge
{

class CoHANmsgBridge : public rclcpp::Node 
{
public:
    CoHANmsgBridge();

private:
    void topic_callback(const vision_msgs::msg::Detection3DArray::SharedPtr msg);

    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

    rclcpp::Subscription<vision_msgs::msg::Detection3DArray>::SharedPtr subscription_;
    rclcpp::Publisher<cohan_msgs::msg::TrackedAgents>::SharedPtr publisher_;

    static constexpr double SCORE_THRESHOLD = 0.8;
    const std::string TARGET_FRAME = "kiro_base_link";
};

}  // namespace CoHANmsgBridge