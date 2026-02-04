#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <functional>
#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist.hpp"
using namespace std::chrono_literals;
class MazeSolverNode : public rclcpp::Node{


    public:
        MazeSolverNode() :rclcpp::Node("mazeSolverNode"),remainingAngle(0.0),rotating(false){
            velocityPublisher = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel",10);

            timer = this->create_wall_timer(0.2s,
            std::bind(&MazeSolverNode::pubhlisherCallBack,this));

            laserSubscription = this->create_subscription<sensor_msgs::msg::LaserScan>
            ("scan",10,std::bind(&MazeSolverNode::subscribtionCallBack
            ,this,std::placeholders::_1));
        
        
        }
        float findMinAngleRadian(float angleIncrement , const std::vector<float> &lidarRays,int indexStart=0,int indexEnd=319){
            float minDistance = std::numeric_limits<float>::infinity();
            int minIndex = -1;
            for(size_t i=indexStart;i<indexEnd;i++){
                if(lidarRays[i]!=-INFINITY && lidarRays[i]<minDistance){
                    minDistance = lidarRays[i];
                    minIndex = i;
                }
            }
            return angleIncrement*minIndex;
        }
        void subscribtionCallBack(const sensor_msgs::msg::LaserScan &msg){
            
                laserScanNew = msg;
                
                
                if(!rotating){
                    float angleIncrement = msg.angle_increment;
                    lidarRays = msg.ranges;
                    laserScanOld = msg;
                    float angleMin = findMinAngleRadian(angleIncrement,lidarRays,80,240);//msg.angle_min
                    int index = angleMin/angleIncrement;
                    std::cout<<"distance : "<<lidarRays[index]<<std::endl;

                    if(index>=80&&index<=240&&msg.ranges[index]<0.25){//onunde engel var mi 
                        rotating = true;
                        wentBack = false;
                        wentBackCounter = 30;
                        std::cout<<"index"<< index<<std::endl;
                        auto geometricMsg = geometry_msgs::msg::Twist();
                        geometricMsg.linear.x = 0;
                        velocityPublisher->publish(geometricMsg);
                        count =0;
                        if(index>=160 ){
                            //rotate right
                            direction = true;
                            remainingAngle = (320-index)*angleIncrement;
    
                            std::cout<<"Rotate Right Remaining Angle: subscriber"<<remainingAngle*180/M_PI<<std::endl;
    
                        
                        }else{
                            direction = false;
                            remainingAngle = (index)*angleIncrement;
                            std::cout<<"Rotate Left Remaining Angle: subscriber"<<remainingAngle*180/M_PI<<std::endl;
    
                        
                        }
                    }
                }
                
            

        }
        
        void pubhlisherCallBack(){
            auto msg = geometry_msgs::msg::Twist();
            if(!rotating){
                
                msg.linear.x = 1;
                
                
            }else{
                if(wentBack==false&&rotating ==false){
                    wentBackCounter--;
                    std::cout<<"Going Back"<< wentBackCounter<<std::endl;
                    if(wentBackCounter<0){
                        wentBack = true;
                        wentBackCounter = 30;
                    }
                    msg.linear.x = -1;
                }else{
                    float minAngleRadian = findMinAngleRadian(laserScanNew.angle_increment,laserScanNew.ranges);
                    int index = minAngleRadian/laserScanNew.angle_increment;
                    std::cout<<"min angle Radian"<<minAngleRadian<<"min angle index"<<index<<" min angle distance"<< laserScanNew.ranges[index]<<std::endl;
                    if(index>=160 ){
                        //rotate right
                        direction = true;
                        remainingAngle = (320-index)*laserScanNew.angle_increment;
                        std::cout<<"Rotate Right Remaining Angle: Publisher"<<remainingAngle*180/M_PI<<std::endl;

                    
                    }else{
                        direction = false;
                        remainingAngle = (index)*laserScanNew.angle_increment;
                        std::cout<<"Rotate Left Remaining Angle: Publisher"<<remainingAngle*180/M_PI<<std::endl;

                    
                    }

                    if(remainingAngle>=0.1){
                        if(!direction){
                            msg.angular.z = 0.5;
    
                        }else{
                            msg.angular.z = -0.5;
                        }
                        remainingAngle -= 0.1;
                        std::cout<<"Remaining Angle: "<<remainingAngle*180/M_PI<<"toplm index"<< count<<std::endl;
                        count++;
                    }else{
                        if(!direction){
                            msg.angular.z = remainingAngle*5;
    
                        }else{
                            msg.angular.z = -remainingAngle*5;
                        }
                        remainingAngle = 0.0;
                        std::cout<<"Remaining Angle: "<<remainingAngle*180/M_PI<<std::endl;
                       
                        rotating = false;
                    }
                }
            }
            velocityPublisher->publish(msg);
            std::cout<<"Published Velocity "<<std::endl;
        }
        
    private:   
     int count = 0;
        int wentBackCounter = 30;
        bool wentBack = false;
        sensor_msgs::msg::LaserScan laserScanOld;
        sensor_msgs::msg::LaserScan laserScanNew;
        float remainingAngle ;
        bool rotating ;
        bool direction;
        std::vector<float> lidarRays; 
        rclcpp::TimerBase::SharedPtr timer;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocityPublisher;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laserSubscription;
};


int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MazeSolverNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;


    return 0;
}
