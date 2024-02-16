#include "ros/ros.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Bool.h>
#include <sys/stat.h>
#include <chrono>

std::string folderPath, fileName, filePath;

bool saveImage(const sensor_msgs::ImageConstPtr& image_msg) {
    cv::Mat image;
    try
    {
      image = cv_bridge::toCvShare(image_msg, image_msg->encoding.c_str())->image;
    }
    catch(cv_bridge::Exception)
    {
      ROS_ERROR("Unable to convert %s image to %s", image_msg->encoding.c_str(), image_msg->encoding.c_str());
      return false;
    }

    if (!image.empty()) {
        std::stringstream fileNameSS;
        fileNameSS << "image-" << ros::Time::now().sec << "-" << ros::Time::now().nsec << + ".jpg";
        fileName = fileNameSS.str();

        std::stringstream filePathSS;
        filePathSS << folderPath << "/" << fileName;
        filePath = filePathSS.str();

        cv::imwrite(filePath, image);
        ROS_INFO("Saved image %s", fileName.c_str());
    }
    else
    {
        ROS_ERROR("Couldn't save image, no data!");
        return false;
    }

    return true;
}

void camera_img_callback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        ROS_INFO("Camera Callback.");
        // save the image
        if (!saveImage(msg)) return;
        // ROS_INFO("Image Saved.");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;
    ros::Rate loop_rate(2000);
    // ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    
    ros::Publisher camera_flag_pub = nh.advertise<std_msgs::Bool>("/mujoco_ros_interface/camera/flag", 1);
    std_msgs::Bool camera_flag_msg;

    image_transport::ImageTransport it(nh);
    image_transport::Subscriber camera_image_sub = it.subscribe("/mujoco_ros_interface/camera/image", 1, camera_img_callback);

    int camera_tick_ = 0;
    bool data_collect_start_ = true;
    bool make_dir = true;
    while(ros::ok())
    {
        if(data_collect_start_){
            if(make_dir){
                std::stringstream folderPathSS;
                // [Check] ros::Time::now().sec is int32 type.
                auto now = std::chrono::system_clock::now();
                std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                // folderPathSS << "/home/hokyun20/2024winter_ws/src/camera_pubsub/" << ros::Time::now().sec;
                folderPathSS << "/home/hokyun20/2024winter_ws/src/camera_pubsub/" << std::put_time(std::localtime(&currentTime), "%Y%m%d_%H%M%S");
                
                folderPath = folderPathSS.str();
                int result = mkdir(folderPath.c_str(), 0777);
                if(result != 0){
                    ROS_ERROR("Couldn't make folder(dir), Check folderPath");
                    
                    return false;
                }
                make_dir = false;
            }

            if(camera_tick_%200 == 0)
            {
                ROS_INFO("I heard: [%d]", camera_tick_);
                camera_flag_msg.data = true;
                camera_flag_pub.publish(camera_flag_msg);
            }

            // data_collect_start_ == false;
        }
        camera_tick_++;
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
