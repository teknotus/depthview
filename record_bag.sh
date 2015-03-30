#! /bin/sh
rosbag record --output-name=beanbag.bag --duration=15 /camera/depth/camera_info /camera/depth/image_raw /camera/depth/points /camera/ir/image_raw /camera/ir/camera_info /tf /tf_static

