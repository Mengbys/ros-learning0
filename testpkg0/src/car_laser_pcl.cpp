#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud2.h>
#include <laser_geometry/laser_geometry.h>

class PointCloudPublisher{
private:
	ros::NodeHandle node;
	laser_geometry::LaserProjection projector_;
	tf::TransformListener listener_;
	ros::Publisher pointcloud_publisher;
	ros::Subscriber sub;
public:
	PointCloudPublisher();
	void scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan_in);
};

PointCloudPublisher::PointCloudPublisher()
{
	pointcloud_publisher = node.advertise<sensor_msgs::PointCloud2> ("/laserscan_to_pointcloud", 100, false);
	sub = node.subscribe("scan", 1000, &PointCloudPublisher::scanCallback,this);
}

void PointCloudPublisher::scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan_in)
{
	if(!listener_.waitForTransform(
	scan_in->header.frame_id,
	"/map",
	scan_in->header.stamp + ros::Duration().fromSec(scan_in->ranges.size()*scan_in->time_increment),
	ros::Duration(1.0)))
	{
		ROS_INFO("wait form Transform failed");
		return;
	}
	sensor_msgs::PointCloud2 cloud;
	projector_.transformLaserScanToPointCloud("/map",*scan_in,cloud,listener_);
	pointcloud_publisher.publish(cloud);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "laserscan_to_pointcloud_node");
	PointCloudPublisher pb;
	ros::spin();
	return 0;
}
