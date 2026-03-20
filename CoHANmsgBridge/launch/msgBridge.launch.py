import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import AnyLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    ultralytics_launch_dir = get_package_share_directory('ultralytics_ros')

    yolo_tracker = IncludeLaunchDescription(
        AnyLaunchDescriptionSource(
            os.path.join(ultralytics_launch_dir, 'launch', 'tracker_with_cloud.launch.xml')
        ),
        launch_arguments={
            'use_sim_time': 'true',
            'debug': 'true',
            'input_topic': '/camera/camera/color/image_raw/compressed',
            'camera_info_topic': '/camera/camera/color/camera_info',
            'lidar_topic': '/bpearl_lidar/points'
        }.items()
    )

    bridge_node = Node(
        package='CoHANmsgBridge',
        executable='cohan_msg_bridge_node',
        name='cohan_msg_bridge',
        output='screen'
    )

    # Start the msgBridge after YOLO starts
    delayed_bridge_node = TimerAction(
        period=10.0,
        actions=[bridge_node]
    )

    return LaunchDescription([
        yolo_tracker,
        delayed_bridge_node
    ])