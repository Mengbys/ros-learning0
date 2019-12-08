#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/simple_goal_state.h>
#include <actionlib/client/simple_client_goal_state.h>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class MoveControl{
private:
    MoveBaseClient ac;
    double x_t_pos[10]; // 目标点的x坐标
    double y_t_pos[10]; // 目标点的y坐标
    int t_pos; // 下一个目标点
public:
	MoveControl();
    void move();
    void SimpleDoneCallback(const actionlib::SimpleClientGoalState& state,
    const move_base_msgs::MoveBaseResultConstPtr& result);
    void SimpleFeedbackCallback(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback);
    void SimpleActiveCallBack();
};

MoveControl::MoveControl():ac("/move_base", true)
{
    x_t_pos[0]=4;           y_t_pos[0]=2;
    x_t_pos[1]=-5.78;       y_t_pos[1]=7.65;
    x_t_pos[2]=-6.14;       y_t_pos[2]=3.54;
    x_t_pos[3]=-10.9;       y_t_pos[3]=2.38;
    x_t_pos[4]=-13.4;       y_t_pos[4]=6.18;
    x_t_pos[5]=-15.4;       y_t_pos[5]=7.76;
    x_t_pos[6]=-15.8;       y_t_pos[6]=3.85;
    x_t_pos[7]=-20.6;       y_t_pos[7]=1.81;
    x_t_pos[8]=-26,6;       y_t_pos[8]=1.76;
    x_t_pos[9]=1;           y_t_pos[9]=-2;
    t_pos=0;

    ROS_INFO("Wating the Action server to start"); 
    ac.waitForServer();
    ROS_INFO("Done");
}

void MoveControl::move()
{
    move_base_msgs::MoveBaseGoal goal; 
    goal.target_pose.header.frame_id = "map"; 
    goal.target_pose.header.stamp = ros::Time::now(); 

    goal.target_pose.pose.position.x = x_t_pos[t_pos];
    goal.target_pose.pose.position.y = y_t_pos[t_pos];
    goal.target_pose.pose.position.z = 0; 
    goal.target_pose.pose.orientation.x = 0;
    goal.target_pose.pose.orientation.y = 0;
    goal.target_pose.pose.orientation.z = 0;
    goal.target_pose.pose.orientation.w = 1; 
    ac.sendGoal(goal,
    boost::bind(&MoveControl::SimpleDoneCallback,this,_1,_2),
    boost::bind(&MoveControl::SimpleActiveCallBack,this),
    boost::bind(&MoveControl::SimpleFeedbackCallback,this,_1)
    );
}

void MoveControl::SimpleDoneCallback(const actionlib::SimpleClientGoalState& state,
const move_base_msgs::MoveBaseResultConstPtr& result)
{
    t_pos++;
    if(t_pos>9) t_pos=0;
    move();
    ROS_INFO("Finished in state [%s] ", state.toString().c_str());
}

void MoveControl::SimpleFeedbackCallback(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback)
{

}

void MoveControl::SimpleActiveCallBack()
{

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "move_control");
    ros::NodeHandle node; 
    MoveControl moveControl;
    moveControl.move();
    ros::spin();
    return 0;
}
