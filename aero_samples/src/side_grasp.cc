#include <aero_std/AeroMoveitInterface.hh>
#include <aero_std/SideGrasp-inl.hh>

int main(int argc, char **argv)
{
  // init ros
  ros::init(argc, argv, "side_grasp_sample_node");
  ros::NodeHandle nh;
  
  // init robot interface
  aero::interface::AeroMoveitInterfacePtr interface(new aero::interface::AeroMoveitInterface(nh));
  interface->sendResetManipPose();
  sleep(1);


  // grasp object from side
  Eigen::Vector3d obj;// target object's position
  obj << 0.5, 0.0, 1.1;

  aero::SideGrasp side;// grasping information
  side.arm = aero::arm::either;
  side.object_position = obj;
  side.height = 0.2;

  auto req = aero::Grasp<aero::SideGrasp>(side);
  req.mid_ik_range = aero::ikrange::torso;
  req.end_ik_range = aero::ikrange::torso;


  interface->openHand(req.arm);

  if (interface->sendPickIK(req)) {
    ROS_INFO("success");
    interface->sendGrasp(req.arm);
    sleep(3);
    interface->openHand(req.arm);
    sleep(1);
    ROS_INFO("reseting robot pose");
    interface->sendResetManipPose();
  }
  else ROS_INFO("failed");

  ROS_INFO("demo node finished");
  ros::shutdown();
  return 0;
}
