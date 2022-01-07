#!/bin/bash
sudo docker run --gpus all -it --name arduino_test -v /home/byunghun/workspace/arduino:/home/workspace --privileged -v /dev:/dev -e GST_DEBUG=2 nn_rebuild:1.0 /bin/bash
