# MazeSolvingTurtlebot4

In this project , i used turtlebot4 to solve a maze . turtlebot4 is able to align itself with the lane and go straight without bumping into wall.  
Also turtlebot is able backtracking with the help of DFS algorithm and start searching new path that may be out-way.

 ## Videos While Project Is Runnig 




 ## Package Dependencies 
 - Ros Humble is needed for that project
 ## How to Build And Run The Project
 - first clone the project into local git clone 
 - build with colcon build `colcon build`
 - add models  directory into turtlebot4 ignition laucnh package worlds directory
 - run the command` ros2 launch turtlebot4_ignition_bringup turtlebot4_ignition.launch.py world:=maze6x6` for the 10x10 maze run the command with the argument of world:=maxe10x10
