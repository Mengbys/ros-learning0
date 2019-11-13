#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "tf/transform_listener.h"
#include "cmath"

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "body_move_node"); 
	ros::NodeHandle n;  //句柄位置默认为当前命名空间;"n("~")"为进入节点的私有空间
	ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);  
	//"/cmd_vel"为在最开始的目录下的cmd_vel话题;"cmd_vel"为当前句柄位置下的话题
    tf::TransformListener listener0;           

    geometry_msgs::Twist vel_cmd;
	vel_cmd .linear.x = 0;
	vel_cmd.linear.y = 0;
    vel_cmd.linear.z = 0;
	vel_cmd.angular.x = 0;
	vel_cmd.angular.y = 0;
	vel_cmd.angular.z = 0;

    ros::Rate loopRate(100);
    vel_pub.publish(vel_cmd);
	
	while(ros::ok())
	{
        tf::StampedTransform transform0;
        try
        {
            listener0.lookupTransform("foot_base", "base_footprint", ros::Time(0), transform0);
            double orginX = transform0.getOrigin().x();
            double orginY = transform0.getOrigin().y();
            double theta = atan2(orginY, orginX);

            if(fabs(orginX - 0) <= 0.1 && fabs(orginY - 0) <= 0.1)
            {
            	vel_cmd.linear.x = 0;
                vel_cmd.angular.z = 0;
            }
            else if(fabs(theta - 0) <= 0.01)
            {
                vel_cmd.linear.x = 2;
                vel_cmd.angular.z = 0;
            }
            else
            {
                if(theta > 0)
                {
                    vel_cmd.linear.x = 2;
                    vel_cmd.angular.z = 2;
                }
                else
                {

                    vel_cmd.linear.x = 2;
                    vel_cmd.angular.z = -2;
                }
            }

            vel_pub.publish(vel_cmd);

        }
        catch(...)
        {
            continue;
        }


		ros::spinOnce();  //调用此函数给其他回调函数得以执行（此例程未使用回调函数）
		loopRate.sleep();
	}

	return 0;
}










