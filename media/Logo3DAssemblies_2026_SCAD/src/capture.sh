#!/bin/sh

translat=0.75,-2.16,0.21
rotation=121.50,0.00,292.60
distance=82.67
openscad anchor.scad           -o anchor.png           --camera=$translat,$rotation,$distance --imgsize=820,570

translat=-5.47,8.07,8.57
rotation=20.00,0.00,340.20
distance=446.13
openscad assembly.scad           -o assembly.png           --camera=$translat,$rotation,$distance --imgsize=820,570

translat=1.73,0.73,0.11
rotation=324.00,0.00,179.60
distance=91.85
openscad bolt.scad             -o bolt.png             --camera=$translat,$rotation,$distance --imgsize=820,570

translat=10.87,-17.22,-18.05
rotation=49.40,0.00,186.60
distance=263.43
openscad broken_part.scad      -o broken_part.png      --camera=$translat,$rotation,$distance --imgsize=820,570

translat=1.70,-19.35,-16.82
rotation=48.70,0.00,185.90
distance=74.40
openscad nut.scad              -o nut.png              --camera=$translat,$rotation,$distance --imgsize=820,570

translat=-5.88,2.08,6.24
rotation=55.00,0.00,25.00
distance=263.43
openscad part_left.scad        -o part_left.png        --camera=$translat,$rotation,$distance --imgsize=820,570

translat=-6.55,-9.11,8.75
rotation=230.70,0.00,340.20
distance=361.36
openscad part_right_lower.scad       -o part_right_lower.png       --camera=$translat,$rotation,$distance --imgsize=820,570

translat=-3.22,-1.04,-12.97
rotation=45.20,0.00,212.80
distance=401.52
openscad part_right_upper.scad -o part_right_upper.png --camera=$translat,$rotation,$distance --imgsize=820,570

translat=0.60,-0.61,-1.23
rotation=50.10,0.00,18.70
distance=82.67
openscad text_left.scad -o text_left.png --camera=$translat,$rotation,$distance --imgsize=820,570

translat=-3.43,20.68,-2.62
rotation=66.70,0.00,25.00
distance=292.71
openscad text_right.scad -o text_right.png --camera=$translat,$rotation,$distance --imgsize=820,570

translat=6.23,15.80,-16.69
rotation=28.20,0.00,359.80
distance=446.13
openscad logo.scad                  -o logo.png                  --camera=$translat,$rotation,$distance --imgsize=820,570

