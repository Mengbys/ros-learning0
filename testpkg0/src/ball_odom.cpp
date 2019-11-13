#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "tf/tf.h"
#include "tf/transform_listener.h"
#include <cmath>
#include <iostream>
using namespace std;

double pos_x = 0;
double pos_y = 0;
double pos_z = 0;
double or_x = 0;
double or_y = 0;
double or_z = 0;
double or_w = 0;

void odomCallback(const nav_msgs::Odometry& msg)
{
    pos_x = msg.pose.pose.position.x;
    pos_y = msg.pose.pose.position.y;
    pos_z = msg.pose.pose.position.z;
    or_x = msg.pose.pose.orientation.x;
    or_y = msg.pose.pose.orientation.y;
    or_z = msg.pose.pose.orientation.z;
    or_w = msg.pose.pose.orientation.w;
}
    

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "ball_odom_node"); 
	ros::NodeHandle n("~");
    ros::Subscriber odom_sub = n.subscribe("/odom", 100, odomCallback);    
	ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 100);
    tf::TransformListener listener0;
    
    double param_x0 = 0;
    double param_y0 = 0;
    double param_z0 = 0;
    double param_ball_vel = 0;
    
    n.param<double>("x0", param_x0, 3); 
    n.param<double>("y0", param_y0, 0); 
    n.param<double>("z0", param_z0, 0); 
    n.param<double>("ball_vel", param_ball_vel, 1); 
    
    ros::Rate loopRate(100); //100Hz
	geometry_msgs::Twist vel;
	vel.linear.x = 0;
    vel.linear.y = 0;
    vel.linear.z = 0;
    vel.angular.x = 0;
    vel.angular.y = 0;
    vel.angular.z = 0;
    vel_pub.publish(vel);
	
    double PI = 3.1415926;
    
    double t_pos_x_4[4] = {param_x0, param_x0, param_x0-3, param_x0-3};  //4个目标点的声明
    double t_pos_y_4[4] = {param_y0, param_y0+3, param_y0+3, param_y0};  
    
    int i = 0;

    double t_pos_x = 0;
    double t_pos_y = 0;
    double t_pos_z = 0;  //目标位置: (x,y,z)

    while(ros::ok())
    {
        t_pos_x = t_pos_x_4[i];
        t_pos_y = t_pos_y_4[i];


        geometry_msgs::PointStamped map0;
        map0.header.stamp = ros::Time();
        map0.header.frame_id = "map";
        map0.point.x = t_pos_x;
        map0.point.y = t_pos_y;
        map0.point.z = 0;

        geometry_msgs::PointStamped map0_t;
        try
        {
            listener0.transformPoint("base_footprint", map0, map0_t);
            double refX = map0_t.point.x;
            double refY = map0_t.point.y;
            double reftheta = atan2(refY, refX);  //这样就没用到odom信息了????!!!
            
            if(fabs(pos_y - t_pos_y) <= 0.1 && fabs(pos_x - t_pos_x) <= 0.1)
            {
                i++;
                if(i == 4)  i = 0;
            } 
            else if(fabs(reftheta) <= 0.1)
            {
				vel.linear.x = param_ball_vel;
                vel.angular.z = 0;
                vel_pub.publish(vel);
            }
            else
            {
            	vel.linear.x = 0;
                vel.angular.z = 1;
                vel_pub.publish(vel);
            }
        }
        catch(...)
        {
            continue;
        }

		loopRate.sleep();
        ros::spinOnce();
    }


	return 0;
}


