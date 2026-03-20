# CoHANmsgBridge

The **`CoHANmsgBridge`** is a custom ROS 2 node designed to facilitate communication between ultralytics_ros node and CoHAN social navigation planner. It acts as a translation layer, converting perception data into a format compatible with the human-aware navigation framework.

---

## Overview

The primary function of this node is to bridge the gap between real-time object detection and social path planning. The bridge was developed to convert the **`YoloResult`** message from the `ultralytics_ros` package into the **`TrackedAgents`** message required by the **Social Planner (CoHAN)**.

---

## Key Features

* **Message Translation**: Maps YOLO bounding boxes and detection confidence into structured agent tracking data.
* **Social Navigation Support**: Enables CoHAN to track humans detected by the Ultralytics YOLO inference engine.
* **Optimized for Vulcanexus**: Designed to run within the KIRO human detection Docker environment.

---

## Data Flow

| Component | Message Type | Topic (Default) |
| :--- | :--- | :--- |
| **Input Source** (`ultralytics_ros`) | `ultralytics_ros/msg/YoloResult` | `/yolo_3d_result` |
| **Output Destination** (`CoHAN-Nav2`) | `cohan_msgs/msg/TrackedAgents` | `/tracked_agents` |

---

## Installation & Build

This package is intended to be built as part of the `human_detect_ws` workspace. 

### Prerequisites
Ensure you have the following repositories cloned in your `src` folder:
* `ultralytics_ros` (branch: `feature-compressed-image`)
* `CoHAN-Nav2` (branch: `v2`)

### Build Command
To build the bridge and its necessary message dependencies, run:

```bash
colcon build --symlink-install --packages-select CoHANmsgBridge cohan_msgs ultralytics_ros
```