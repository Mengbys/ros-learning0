#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
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
	
    double PI = 3.1415926;
    
    double t_pos_x_4[4] = {param_x0, param_x0, param_x0-3, param_x0-3};  //4个目标点的声明
    double t_pos_y_4[4] = {param_y0, param_y0+3, param_y0+3, param_y0};  
    
    int i = 0;

    double t_pos_x = 0;
    double t_pos_y = 0;
    double t_pos_z = 0;  //目标位置: (x,y,z)

    double alpha_now;  //当前方向的逆时针偏转角alpha
    double alpha_target;  //目标方向的逆时针偏转角 

    while(ros::ok())
    {
        t_pos_x = t_pos_x_4[i];
        t_pos_y = t_pos_y_4[i];

        // 得到目前方向和目标方向，相对与当前点
        if(or_z >= 0)   alpha_now = 2*acos(or_w);
        else    alpha_now = 2*PI - 2*acos(or_w);

        if(fabs(pos_x - t_pos_x) <= 1e-7)
        {
            if(pos_y - t_pos_y > 0)
                alpha_target = PI*3/2;
            else if(pos_y - t_pos_y < 0)
                alpha_target = PI*1/2;
        }
        else if(fabs(pos_y - t_pos_y) <= 1e-7 )
        {
            if(pos_x - t_pos_x > 0)
                alpha_target = PI;
            else
                alpha_target = 0;
        }
        else if(t_pos_x - pos_x > 0 && t_pos_y - pos_y > 0)  //第一象限
        {
            alpha_target = atan((pos_y - t_pos_y)/(pos_x - t_pos_x));
        }
        else if(t_pos_x - pos_x < 0 && t_pos_y - pos_y > 0) //第二象限
        {
            alpha_target = atan((pos_y - t_pos_y)/(pos_x - t_pos_x)) + PI;
        }
        else if(t_pos_x - pos_x < 0 && t_pos_y - pos_y < 0)  //第三象限
        {
            alpha_target = atan((pos_y - t_pos_y)/(pos_x - t_pos_x)) + PI;
        }
        else if(t_pos_x - pos_x > 0 && t_pos_y - pos_y < 0)  //第四象限
        {
            alpha_target = atan((pos_y - t_pos_y)/(pos_x - t_pos_x)) + 2*PI;
        }

        cout << "alpha_now: " << alpha_now << endl;
        cout << "alpha_target: " << alpha_target << endl;



        if(fabs(pos_y - t_pos_y) <= 0.1 && fabs(pos_x - t_pos_x) <= 0.1)   //方向正确且位置正确了，则停下
        {
           i++;
           if(i == 4)   i = 0;
        }
        else  if(fabs(alpha_now - alpha_target) > 0.1 && 2*PI - fabs(alpha_now - alpha_target) > 0.1)  //调整方向
        {
            //得到当前方向转到目标方向
            double alpha_target_u;
            if(alpha_target - alpha_now > 0)
                alpha_target_u = alpha_target - alpha_now;
            else
                alpha_target_u = alpha_target - alpha_now + 2*PI;

            int leftOrRight = 0;  //turn left - 1  turn-right - -1
            if(alpha_target_u > 0 && alpha_target_u <= PI)
                leftOrRight = 1;
            else if(alpha_target_u > PI && alpha_target_u < 2*PI)
                leftOrRight = -1;
            
            vel.linear.x = 0;
            vel.linear.y = 0;
            vel.linear.z = 0;
            vel.angular.x = 0;
            vel.angular.y = 0;
            vel.angular.z = 1*leftOrRight;
            vel_pub.publish(vel);
        }
        else  //如果距离还很远，则前进
        {
            vel.linear.x = param_ball_vel;
            vel.linear.y = 0;
            vel.linear.z = 0;
            vel.angular.x = 0;
            vel.angular.y = 0;
            vel.angular.z = 0;
            vel_pub.publish(vel);
        }

		loopRate.sleep();
        ros::spinOnce();
    }


	return 0;
}


