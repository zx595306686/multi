#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "nav_msgs/Odometry.h"
#include "math.h"

/*  

    需求: car2 跟随 car1 运动，car2 保持在 car1 的正后方 0.8m 左右的距离。
    流程:
        1.订阅 car1 的位姿信息;
        2.然后根据指定偏移量计算 car2 的目标点;
        3.发布目标点到 car2。

    创建一个类，实现订阅与发布

*/

class ZhuiSui {
    private:
        ros::NodeHandle nh;
        ros::NodeHandle nh_private;
        ros::Publisher goalPub;
        ros::Subscriber poseSub;
        double length;//间距
        std::string master_name;
    public:
        ZhuiSui(){

            
            nh_private = ros::NodeHandle("~");
            length = 0.5;
            master_name = "car1";
            nh_private.getParam("length",length);
            nh_private.getParam("master",master_name);
            
            goalPub = nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal",1);
            poseSub = nh.subscribe<nav_msgs::Odometry>("/" + master_name + "/base_pose_ground_truth",100,&ZhuiSui::cb,this);
            ros::spin();
        
        }
        void cb(const nav_msgs::Odometry::ConstPtr& p){
            //1.先获取目标车辆坐标与偏航的弧度值
            //坐标只取 x y 即可
            double carX = p->pose.pose.position.x;
            double carY = p->pose.pose.position.y;
            //弧度需要先将四元数转换成欧拉角，然后取偏航值
            tf2::Quaternion qtn(p->pose.pose.orientation.x,
                                p->pose.pose.orientation.y,
                                p->pose.pose.orientation.z,
                                p->pose.pose.orientation.w);
            double r_value = 0.0;
            double p_value = 0.0;
            double y_value = 0.0;

            tf2::Matrix3x3 m(qtn);
            m.getRPY(r_value,p_value,y_value);  

            //2.计算目标点坐标
            //先获取安全距离

            double goalX = carX - cos(y_value) * length;
            double goalY = carY - sin(y_value) * length;

            //-------------------------------------
            
            //3.发布目标点
            geometry_msgs::PoseStamped test;
            test.header.stamp = ros::Time::now();
            test.header.frame_id = "map";
            test.pose.position.x = goalX;
            test.pose.position.y = goalY;
            test.pose.position.z = 0.0;

            // tf2::Quaternion qtn;
            // qtn.setRPY(0,0,0);
            // test.pose.orientation = tf2::toMsg(qtn);
            test.pose.orientation = p->pose.pose.orientation;
            goalPub.publish(test);
            // ROS_INFO("send msg ....");
            ros::Duration(0.5).sleep();
        }
};

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"zhuiSui");
    ZhuiSui zhuiSui;
    return 0;
}
