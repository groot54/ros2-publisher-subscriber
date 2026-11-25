#include "rclcpp/rclcpp.hpp"
#include "entri_ros2_pubsub_cpp/msg/robot_status.hpp"
#include <functional>
#include <memory>

class RobotStatusSubscriber : public rclcpp::Node
{
public:
    RobotStatusSubscriber() : Node("robot_status_subscriber")
    {
        subscription_ = this->create_subscription<
            entri_ros2_pubsub_cpp::msg::RobotStatus>(
            "robot_status",
            10,
            std::bind(
                &RobotStatusSubscriber::status_callback,
                this,
                std::placeholders::_1));
    }

private:
    void status_callback(
        const entri_ros2_pubsub_cpp::msg::RobotStatus::SharedPtr message)
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Robot %d is at %.1f%% battery. Status: %s",
            message->robot_id,
            message->battery_level,
            message->status_message.c_str());
    }

    rclcpp::Subscription<
        entri_ros2_pubsub_cpp::msg::RobotStatus>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RobotStatusSubscriber>());
    rclcpp::shutdown();
    return 0;
}
