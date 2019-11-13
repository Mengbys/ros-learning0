#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "param_get_node");
    ros::NodeHandle nh("~");  //nh(~)为进入节点下的空间：/node3/...    nh为节点的平行空间：/...
    ros::Publisher str_pub = nh.advertise<std_msgs::Int8>("/chatter", 100);
    
    int param_get = 0;
    nh.param<int>("param0", param_get, 2);  //2为找不到参数时，param_get的默认设置值

    ros::Rate loopRate(10);

	while(ros::ok())
	{
        std_msgs::Int8 msg;
        msg.data = param_get;
	    str_pub.publish(msg);
    }
	
	return 0;
}
