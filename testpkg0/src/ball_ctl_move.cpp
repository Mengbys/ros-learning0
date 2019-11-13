#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "ball_move_node"); 
	ros::NodeHandle n;  //句柄位置默认为当前命名空间;"n("~")"为进入节点的私有空间
	ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);  
	//"/cmd_vel"为在最开始的目录下的cmd_vel话题;"cmd_vel"为当前句柄位置下的话题
        
    ROS_INFO("draw_circle start...");
	ros::Rate loopRate(10);
	geometry_msgs::Twist vel_cmd;
	

	while(ros::ok())
	{
	    vel_cmd.linear.x = 2;
	    vel_cmd.linear.y = 0;
	    vel_cmd.linear.z = 0;
	    vel_cmd.angular.x = 0;
	    vel_cmd.angular.y = 0;
	    vel_cmd.angular.z = 1;
	    vel_pub.publish(vel_cmd);

		ros::spinOnce();  //调用此函数给其他回调函数得以执行（此例程未使用回调函数）
		loopRate.sleep();
	}

	return 0;
}
















