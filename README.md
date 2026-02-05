# MazeSolvingTurtlebot4

In this project , i used turtlebot4 to solve a maze . turtlebot4 is able to align itself with the lane and go straight without bumping into wall.  
Also turtlebot is able backtracking with the help of DFS algorithm and start searching new path that may be out-way.

 ## Videos While Project Is Runnig 
[Click to that link ](https://www.youtube.com/watch?v=Z_2JqBKkc8k)



 ## Package Dependencies 
 - Ros Humble
 - Turtlebot4 humble version
 - Gazebo Ignition
 ## How to Build And Run The Project
 - first clone the project into local `git clone https://github.com/EnesBuyukyilmaz/MazeSolvingTurtlebot4/tree/master`
 - build with colcon build `colcon build`
 - add mazeModels into a world.sdf file that has been created by you , and add these world files also turtlebot4 ignition laucnh package worlds directory
 - run the command` ros2 launch turtlebot4_ignition_bringup turtlebot4_ignition.launch.py world:=maze6x6` for the 10x10 maze run the command with the argument of world:=maxe10x10
 - after gazebo initiliazed you can run the command `ros2 run mazePathFindingTurtleBot main` in order to start the node and you can see all the ourputs from the terminal while robot is navigating


