# 2024winter_ws


## Edited from packages below.(20240214)
```
https://github.com/saga0619/mujoco_ros_sim
https://github.com/saga0619/dyros_tocabi_v2
https://github.com/saga0619/tocabi_gui
https://github.com/saga0619/tocabi_cc
```

# mujoco RGBD refernce
```
https://github.com/gywhitel/mujoco_RGBD
```

# 수정사항
```
mujoco_ros_sim/mujoco_ros/CMakeLists.txt (수정)
mujoco_ros_sim/mujoco_ros/package.xml (수정)
mujoco_ros_sim/mujoco_ros/src/main.cpp (수정)

mujoco_ros_sim/mujoco_ros/src/mujoco_rgbd_camera.cpp (신규)
mujoco_ros_sim/mujoco_ros/include/mujoco_rgbd_camera.hpp (신규)
dyros_tocabi_v2/tocabi_description/mujoco_model/dyros_tocabi_with_camera.xml (신규)
dyros_tocabi_v2/tocabi_controller/launch/simulation_with_camera.launch (신규)

camera_pubsub/* (신규, 20240216)
```

# 실행방법
dependency(OpenCV)
```
sudo apt update
sudo apt install libopencv-dev
```

Build & Simulation Start
```
catkin_make
roslaunch tocabi_controller simulation_with_camera.launch
(Simulation > Run)
```
Ctrl+Alt+T (New Terminal)
camera_pubsub Start(!!! You have to change folderPath at "camera_pubsub_node.cpp", line 85, in main.
```
rosrun camera_pubsub camera_pubsub_node
```


Ctrl+Alt+T (New Terminal)(Optional)
```
rostopic echo /mujoco_ros_interface/camera/image
```
Ctrl+Alt+T (New Terminal)(Optional)
```
rostopic pub -1 /mujoco_ros_interface/camera/flag std_msgs/Bool "data: true" 
```
# 작동방식
```
mujoco_ros_sim/mujoco_ros/src/main.cpp (수정)
의 343번째줄에서 visual_thread 실행(멀티쓰레딩)
의 183~201번째 줄에서 camera_pub_flag_ 가 true가되면 image publish하도록 만들어놓음(여기서 resize)

이렇게 해놓은 이유는, 무조코가 2000HZ(0.0005s)로 돌아가기 때문에 제어기(tocabi_cc)에서
flag를 publish 할때만 카메라 데이터를 받아오도록 하였음.
```

# TODO LIST
~~1. camera_pub_flag 보내고(publish) camera image 받아와서(subscribe) 폴더에 저장하는 패키지 만듦.(240216)~~
2. "camera_pubsub/src/camera_pubsub_node.cpp"를 참고하여 tocabi_cc 에서 의도대로 동작하도록 코드 옮겨야함.
3. "dyros_tocabi_v2/tocabi_description/mujoco_model/dyros_tocabi_with_camera.xml" 에서 카메라 위치 어디로 할지 정하고 옮겨야함.



