#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "tf2/LinearMath/Quaternion.h"
/*  
    该节点用于设置机器人的初始化位姿
*/

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("机器人位姿初始化");

    //发布一个机器人的初始位姿消息，用于定位机器人位置
    ros::init(argc,argv,"car_init");
    ros::NodeHandle nh2;
    ros::NodeHandle nh("~");
    //每次调用发布一次消息即可
    ros::Publisher pub = nh2.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose",1,true);


    //解析参数
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    double roll = 0.0;
    double pitch = 0.0;
    double yaw = 0.0;

    std::string map = "/map";

    nh.getParam("x",x);
    nh.getParam("y",y);
    nh.getParam("z",z);
    
    nh.getParam("roll",z);
    nh.getParam("pitch",z);
    nh.getParam("yaw",z);

    nh.getParam("map",map);

    //组织消息
    geometry_msgs::PoseWithCovarianceStamped pose;
    pose.header.frame_id = map;
    pose.header.stamp = ros::Time::now();
    pose.pose.pose.position.x = x;
    pose.pose.pose.position.y = y;
    pose.pose.pose.position.z = z;

    //欧拉角转换成四元数
    tf2::Quaternion qtn;
    qtn.setRPY(roll,pitch,yaw);
    pose.pose.pose.orientation.x = qtn.x();
    pose.pose.pose.orientation.y = qtn.y();
    pose.pose.pose.orientation.z = qtn.y();
    pose.pose.pose.orientation.w = qtn.w();
    
    ros::Duration(1).sleep();
    pub.publish(pose);
    ROS_INFO("发送完毕....");
    
    ros::spin();
    return 0;
}

