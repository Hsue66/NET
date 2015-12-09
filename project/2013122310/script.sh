#!/bin/sh
gnome-terminal -e './broker'
gnome-terminal -e './pub p1'
gnome-terminal -e './sub s1'
gnome-terminal -e './pub p2'
gnome-terminal -e './sub s2'
sleep 3
gnome-terminal -e './pub p3'
sleep 2
gnome-terminal -e './sub s3'

