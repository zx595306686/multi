#include "ros/ros.h"
#include "math.h"
/*  
    正弦 Math.sin() 直角三角形中 对边和斜边的比值
	余弦 Math.cos() 直角三角形中 临边和斜边的比值 
	正切 Math.tan() 直角三角形中 对边和临边的比值 
*/

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("三角函数演示:");
    // 已知A点坐标，B点与A点距离，以及B点与A点的夹角(弧度)，求 B 点坐标。
    double a_x = 100;
    double a_y = -100;
    double jiao = 1.0;
    double length = 50; //斜边

    double b_x = a_x - cos(jiao) * length;
    double b_y = a_y - sin(jiao) * length;
    ROS_INFO("(%.6f,%.6f)",b_x,b_y);


    return 0;
}
