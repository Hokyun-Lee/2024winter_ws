# 2024winter_ws


## Edited from packages below.(20240214)
https://github.com/saga0619/mujoco_ros_sim
https://github.com/saga0619/dyros_tocabi_v2
https://github.com/saga0619/tocabi_gui
https://github.com/saga0619/tocabi_cc

# mujoco RGBD refernce
https://github.com/gywhitel/mujoco_RGBD

# 수정사항
mujoco_ros_sim/mujoco_ros/CMakeLists.txt (수정)
mujoco_ros_sim/mujoco_ros/package.xml (수정)
mujoco_ros_sim/mujoco_ros/src/main.cpp (수정)

mujoco_ros_sim/mujoco_ros/src/mujoco_rgbd_camera.cpp (신규)
mujoco_ros_sim/mujoco_ros/include/mujoco_rgbd_camera.hpp (신규)
dyros_tocabi_v2/tocabi_description/mujoco_model/dyros_tocabi_with_camera.xml (신규)
dyros_tocabi_v2/tocabi_controller/launch/simulation_with_camera.launch (신규)

# 실행방법
catkin_make
roslaunch tocabi_controller simulation_with_camera.launch
(Simulation > Run)

Ctrl+Alt+T (New Terminal)
rostopic echo /mujoco_ros_interface/camera/flag

Ctrl+Alt+T (New Terminal)
rostopic pub -1 /mujoco_ros_interface/camera/flag std_msgs/Bool "data: true" 

# 작동방식
mujoco_ros_sim/mujoco_ros/src/main.cpp (수정)
의 343번째줄에서 visual_thread 실행(멀티쓰레딩)
의 183~201번째 줄에서 camera_pub_flag_ 가 true가되면 image publish하도록 만들어놓음(여기서 resize)
이렇게 해놓은 이유는, 무조코가 2000HZ(0.0005s)로 돌아가기 때문에 제어기(tocabi_cc)에서
flag를 publish 할때만 카메라 데이터를 받아오도록 하였음.


# TODO
tocabi_cc 에서 publisher/subscriber 만들어야함..^^

