#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_transport/image_transport.hpp>

class ImageGrayScaleNode : public rclcpp::Node
{
public:
    ImageGrayScaleNode() : Node("image_grayscale_node")
    {
        // Initialize the subscription to /image_raw topic using image_transport
        image_subscriber_ = image_transport::create_subscription(
            this, "/image_raw", 
            std::bind(&ImageGrayScaleNode::image_callback, this, std::placeholders::_1), 
            "raw");

        // Initialize the publisher for the grayscale image
        image_publisher_ = image_transport::create_publisher(this, "/image_grayscale");
    }

private:
    void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg)
    {
        // Convert ROS image message to OpenCV image
        cv_bridge::CvImagePtr cv_ptr;
        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception &e)
        {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }

        // Convert the image to grayscale
        cv::Mat gray_image;
        cv::cvtColor(cv_ptr->image, gray_image, cv::COLOR_BGR2GRAY);

        // Convert the grayscale image back to a ROS message and publish it
        cv_bridge::CvImage gray_msg;
        gray_msg.header = msg->header;
        gray_msg.encoding = sensor_msgs::image_encodings::MONO8;
        gray_msg.image = gray_image;

        image_publisher_.publish(gray_msg.toImageMsg());
    }

    image_transport::Subscriber image_subscriber_;
    image_transport::Publisher image_publisher_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImageGrayScaleNode>());
    rclcpp::shutdown();
    return 0;
}
