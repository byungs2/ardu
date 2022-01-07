#!/bin/bash

./bin/arduino-cli compile -b arduino:avr:uno load_cell/ -u -p /dev/ttyACM0
