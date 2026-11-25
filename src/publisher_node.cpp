#include "rclcpp/rclcpp.hpp"
#include "entri_ros2_pubsub_cpp/msg/robot_status.hpp"
#include <functional>
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class RobotStatusPublisher : public rclcpp::Node
{
public:
    RobotStatusPublisher() : Node("robot_status_publisher"), battery_level_(100.0)
    {
        publisher_ = this->create_publisher<
            entri_ros2_pubsub_cpp::msg::RobotStatus>("robot_status", 10);

        timer_ = this->create_wall_timer(
            1000ms,
            std::bind(&RobotStatusPublisher::publish_status, this));
    }

private:
    void publish_status()
    {
        auto message = entri_ros2_pubsub_cpp::msg::RobotStatus();

        message.robot_id = 12;
        message.battery_level = battery_level_;
        message.status_message = "Operating";

        publisher_->publish(message);

        RCLCPP_INFO(
            this->get_logger(),
            "Published robot status: ID=%d, Battery=%.1f%%, Status=%s",
            message.robot_id,
            message.battery_level,
            message.status_message.c_str());

        battery_level_ -= 0.5;

        if (battery_level_ < 0.0) {
            battery_level_ = 100.0;
        }
    }

    rclcpp::Publisher<entri_ros2_pubsub_cpp::msg::RobotStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    float battery_level_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RobotStatusPublisher>());
    rclcpp::shutdown();
    return 0;
}
