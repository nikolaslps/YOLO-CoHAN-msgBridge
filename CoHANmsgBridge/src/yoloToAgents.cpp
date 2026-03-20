#include "CoHANmsgBridge/yoloToAgents.hpp"

namespace CoHANmsgBridge
{

CoHANmsgBridge::CoHANmsgBridge() : Node("cohan_msg_bridge") {
    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_, this);

    // subscribe for YOLO 3D results
    subscription_ = this->create_subscription<vision_msgs::msg::Detection3DArray>(
        "yolo_3d_result", 10, 
        std::bind(&CoHANmsgBridge::topic_callback, this, std::placeholders::_1)
    );

    // publish for CoHAN
    publisher_ = this->create_publisher<cohan_msgs::msg::TrackedAgents>("tracked_agents", 10);
    
    RCLCPP_INFO(this->get_logger(), "Bridge Node started. Mapping YOLO to CoHAN Agents...");
}


void CoHANmsgBridge::topic_callback(const vision_msgs::msg::Detection3DArray::SharedPtr msg) {
    auto output_msg = cohan_msgs::msg::TrackedAgents();
    
    output_msg.header.frame_id = TARGET_FRAME;
    output_msg.header.stamp = msg->header.stamp;

    geometry_msgs::msg::TransformStamped transform;
    try {
        transform = tf_buffer_->lookupTransform(
            TARGET_FRAME, msg->header.frame_id, 
            msg->header.stamp, rclcpp::Duration::from_seconds(0.1));
    } catch (const tf2::TransformException & ex) {
        RCLCPP_WARN(this->get_logger(), "TF Error: %s", ex.what());
        return;
    }
    
    for (const auto & detection : msg->detections) {
        for (const auto & result : detection.results) {
            if (result.hypothesis.class_id == "person" && result.hypothesis.score >= SCORE_THRESHOLD) {
                
                cohan_msgs::msg::TrackedAgent agent;
                // check the .track_id for consistency across detections
                if (!detection.id.empty() && std::all_of(detection.id.begin(), detection.id.end(), ::isdigit)) {
                    agent.track_id = std::stoull(detection.id);
                } else {
                    agent.track_id = output_msg.agents.size(); 
                }
                agent.name = "human_" + std::to_string(agent.track_id);
                agent.state = cohan_msgs::msg::TrackedAgent::STATIC;

                cohan_msgs::msg::TrackedSegment segment;
                segment.type = cohan_msgs::msg::TrackedSegmentType::TORSO;

                tf2::doTransform(detection.bbox.center, segment.pose.pose, transform); // segment.pose.pose

                agent.segments.push_back(segment);
                output_msg.agents.push_back(agent);

                break;
            }
        }
    }
    if (!output_msg.agents.empty()) {
        publisher_->publish(output_msg);
    }
}

}  // namespace CoHANmsgBridge

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CoHANmsgBridge::CoHANmsgBridge>());
    rclcpp::shutdown();
    return 0;
}