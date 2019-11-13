#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
int main(int argc, char *argv[])
{
	ros::init(argc, argv, "lxh_drawcircle_node");  //对该节点进行初始化
	ros::NodeHandle n;  //声明一个NodeHandle对象n，并用n生成一个广播对象vel_pub
	ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
	//geometry_msg::Twist为消息msg类型，/turtle1/cmd_vel为话题名称
	//参数10是什么NY
	ROS_INFO("draw_circle start...");  //输出显示信息
	ros::Rate loopRate(10); //xHz，每秒x次//每秒执行x次循环//每秒发x次msg
	geometry_msgs::Twist vel_cmd;
	
	int count = 0;
	int step = 0;
	float pi = 3.1415926;
	
	while(ros::ok())
	{
		/*
		ros::ok()返回false，代表可能发生了以下事件：
		1.SIGINT被触发（CTRL-C）调用了ros::shutdown()
		2.被另一个同名节点踢出ROS网络
		3.ros::shutdown()被调用
		4.节点中的所有ros::NodeHandle都已经被销毁
		*/
		if(step == 0)
		{
			vel_cmd.linear.x = 3;
		
			vel_cmd.linear.y = 0;
			vel_cmd.linear.z = 0;
			vel_cmd.angular.x = 0;
			vel_cmd.angular.y = 0;
			vel_cmd.angular.z = 0;
			vel_pub.publish(vel_cmd);
		}
		else
		{
			vel_cmd.linear.x = 0;
			vel_cmd.linear.y = 0;
			vel_cmd.linear.z = 0;
			vel_cmd.angular.x = 0;
			vel_cmd.angular.y = 0;
			vel_cmd.angular.z = pi;
			vel_pub.publish(vel_cmd);
		}
		
		count++;
		if(count == 10)
		{
			step = step == 0? 1:0;
			count = 0;
		}
		
		
		ros::spinOnce();  //调用此函数给其他回调函数得以执行（此例程未使用回调函数）
		loopRate.sleep();
	}

	

	return 0;
}


/*
注意的点：
1. 消息中的vel_cmd.linear.x = 3的3是指速度
2. 发送一次vel_cmd.linear.x = 3，龟的速度就会到达3，没有加速过程。同样，也没有减速过程
3. 发送一次vel_cmd.linear.x = 3消息，龟以3的速度运动一定时间，应该是1秒
3. 为了改进这一点，就引入了用于平缓速度的包
*/














