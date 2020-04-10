#include <pcl/io/pcd_io.h>
#include <pcl/common/impl/io.hpp>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <iostream>
#include <iostream>
#include <ostream>
#include <pcl/filters/passthrough.h>

typedef pcl::PointXYZ PointTypeIO;
using namespace pcl;
using namespace std;

int main(int argc, char** argv)
{
	pcl::PointCloud<PointTypeIO>::Ptr cloud_o(new pcl::PointCloud<PointTypeIO>);
	pcl::PointCloud<PointTypeIO>::Ptr cloud_d(new pcl::PointCloud<PointTypeIO>);
	//pcl::PointCloud<PointTypeIO>::Ptr newcloud_o(new pcl::PointCloud<PointTypeIO>);
	pcl::io::loadPCDFile<PointTypeIO>(argv[1], *cloud_o);
	pcl::io::loadPCDFile<PointTypeIO>(argv[2], *cloud_d);
	std::cout << "load file success" << endl;
	//pcl::copyPointCloud(*cloud_o, *newcloud_o);
	std::cout << "Cloud1 before filtering："<<cloud_o->size()<<endl;
	std::cout << "Cloud2 before filtering：" << cloud_d->size() << endl;
	for (int i = 0;i < cloud_o->size();i++)
	{
		for (int j = 0;j < cloud_d->size();j++)
		{
			if (cloud_o->points[i].x == cloud_d->points[j].x && cloud_o->points[i].y == cloud_d->points[j].y && cloud_o->points[i].z == cloud_d->points[j].z)
			{
				cloud_o->points[i].z = 10000;//赋予一个肯定无效的值
			}

			else
			{
				//std::cout << i << std::endl;
				continue;
			}
		}
		if (i % 1000 == 0)
			std::cout << "第" << i << "步" << std::endl;
	}

	//直通滤波
	std::cerr << "start to filter" << std::endl;
	pcl::PointCloud<PointTypeIO>::Ptr cloud_filtered(new pcl::PointCloud<PointTypeIO>);
	pcl::PassThrough<PointTypeIO> pass;
	pass.setInputCloud(cloud_o);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(9000, 100000);
	pass.setFilterLimitsNegative (true);
	pass.filter(*cloud_filtered);
	std::cerr << "Cloud after filtering: " <<cloud_filtered->size()<< std::endl;

	pcl::PCDWriter writer;
	writer.write<PointTypeIO>("cloud_del.pcd", *cloud_filtered, false);


	return 0;
}