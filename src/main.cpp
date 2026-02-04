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
enum class Directions{
    FRONT,
    RIGHT,
    BACK,
    LEFT
};




enum class RobotDirections{
            NORTH,
            EAST,
            SOUTH,
            WEST,
        };
enum class RotationDirection{
    LEFT,
    RIGHT,
    TURNBACK,
    NODIRECTION
};
struct AvailableDirectionsWhileReturningBack{
    
   bool directions[4]  = {false,false,false,false};//North,East,South,West
        
};
class CellNode{
    public:
        char getName(){
            return name;
        }
        std::shared_ptr<CellNode> getParent(){
            std::cout<<"name of the parent"<<parent->getName()<<std::endl;
            return parent;
        }
        CellNode(char name){
            std::cout<<"first Node is created " << name <<std::endl;
            this->name = name;
        }
        CellNode(char name, std::shared_ptr<CellNode >parent,Directions dirFromParent,RobotDirections directionOfRobotAfterFinding){
            this->name = name;
            // parent->addChildren(this,dirFromParent,directionOfRobotAfterFinding,directionOfRobotAfterFinding);
            std::cout<<name<<" Node is created "  <<std::endl;

            this->parent = parent;
            this->whichDirectionFromParent = dirFromParent;//direction for parent to follow to reach this CellNode
            this->robotsFirstDirectionAfterFinding = directionOfRobotAfterFinding;
        }
        CellNode(char name, std::shared_ptr<CellNode >parent,RotationDirection rotationDirection,RobotDirections directionOfRobotAfterFinding){
            this->name = name;
            //constructor for after finding obstacle
            // parent->addChildren(this,dirFromParent,directionOfRobotAfterFinding,directionOfRobotAfterFinding);
            std::string destination;
             if(directionOfRobotAfterFinding == RobotDirections::EAST){
                destination = "EAST";
             }
             if(directionOfRobotAfterFinding == RobotDirections::NORTH){
                                destination = "NORTH";

             }if(directionOfRobotAfterFinding == RobotDirections::SOUTH){
                                destination = "SOUTH";

             }if(directionOfRobotAfterFinding == RobotDirections::WEST){
                    std::cout<<name<<" Node is created   rotation direction  LEFT and current destinatnion WEST " <<std::endl;

                                destination = "WEST";

             }
            if(rotationDirection == RotationDirection::LEFT){

                std::cout<<name<<" Node is created   rotation direction  LEFT and current destinatnion "<<destination <<std::endl;
            } if(rotationDirection == RotationDirection::RIGHT){

                std::cout<<name<<" Node is created   rotation direction  RIGHT and current destinatnion "<<destination <<std::endl;
            }if(rotationDirection == RotationDirection::TURNBACK){

                std::cout<<name<<" Node is created   rotation direction  TURNBACK and current destinatnion "<<destination <<std::endl;
            }
            createdAfterObstacleFinding = true;
            this->parent = parent;
            this->rotationDirectionAfterGenerationOfNode = rotationDirection;//direction for parent to follow to reach this CellNode
            this->robotsFirstDirectionAfterFinding = directionOfRobotAfterFinding;
        }
        std::shared_ptr<CellNode> addChildren(std::shared_ptr<CellNode> child,Directions dirToChild,RobotDirections robotsFirstDirectionAfterFinding){
                     //dirToChild:= child.whichDirectionFromParent
            // for(int i=0;i<3;i++){
            //     if(children[i]==nullptr){
            //         children[i] = child;
            //         directionsFromThisCellNodeToChildren[i] = dirToChild;
            //         this->robotsFirstDirectionAfterFinding = robotsFirstDirectionAfterFinding;
            //         return child;
            //     }
            // }

            if(dirToChild==Directions::RIGHT){
                children[0] = child;
            }else if(dirToChild==Directions::LEFT){
                children[1] = child;
                std::cout<<"left child added left parent node is "<< name <<std::endl;

            }else if (dirToChild==Directions::FRONT){
                children[2] = child;

            }

            return nullptr;

        }
        bool createdAfterFindingObstacle(){
            return createdAfterObstacleFinding;
        }
        int isThereAvailableChild(){
            std::cout<<"checking available child  right "<< addableChilds[0] <<"left"<<addableChilds[1]  <<std::endl;
            if(addableChilds[0]){//right children
                if(createdAfterObstacleFinding){    
                    if(children[0]!=nullptr && children[0]==nullptr){
                        std::cout<<"visited status  right ,after obstacledetected false"<<std::endl;

                        return 0;   
                    }

                }else{
                    if(children[0]==nullptr){
                                                std::cout<<"visited status  right  false "<<std::endl;

                        return 0;   
                    }
                }
                
            }
           if(addableChilds[1]){//left children
               
                    if(createdAfterObstacleFinding){
                        if(addableChilds[0]&&children[1]==nullptr){
                            
                            std::cout<<"left is visible "<<std::endl;
    
                            return 1;   
                        }
                    }else{
                        if(children[1]==nullptr){
                                                    std::cout<<"visited status  left "<<std::endl;

                            return 1;   
                        }
                    }

                
            }
            return -1;//previous node
        }
        std::shared_ptr<CellNode>   getAvailableChild(int index){
            if(index == 0){
                return children[0];
            } if(index == 1){
                return children[1];
            } if(index == 2){
                return children[2];
            }
        }
        AvailableDirectionsWhileReturningBack findAvailableDirectionsWhileReturningBack(){//en son fonksiton cagrildigi yerden degeri alip kopyalanan degerin 
            struct AvailableDirectionsWhileReturningBack  availableDirections;
            std::cout<<"current node "<< name << " robot first direction after finding "<< static_cast<int>(robotsFirstDirectionAfterFinding)<<std::endl;
            int directionOfRightChild = -1;
            int directionOfLeftChild = -1;
            if(addableChilds[0]){//right children
                directionOfRightChild = (static_cast<int>(robotsFirstDirectionAfterFinding) + 1)%4;
                std::cout<<"there is a addable right child direction of right child "<< directionOfRightChild<<std::endl;
            }
            if(addableChilds[1]){//left children
                directionOfLeftChild = (static_cast<int>(robotsFirstDirectionAfterFinding) - 1 + 4)%4;
                std::cout<<"there is a addable Left child direction of Left child "<< directionOfLeftChild<<std::endl;

            }
            if(directionOfRightChild!=-1){
                availableDirections.directions[directionOfRightChild] = true;
            }
            if(directionOfLeftChild!=-1){
                availableDirections.directions[directionOfLeftChild] = true;
            }
            int front =static_cast<int>(robotsFirstDirectionAfterFinding);
            int back  = (front+2)%4;
            std::string directions [4] = {"NORTH","EAST","SOUTH","WEST"};
            
            availableDirections.directions[back] = true;
            if(!createdAfterObstacleFinding){
                availableDirections.directions[front] = true;
            }
            return availableDirections;
        }
       
        bool getVisited(){
            return visited;
        }
        void setVisited(){
            visited = true;
        }
        bool getAddableChild(int indexNo){
            return addableChilds[indexNo];
        }
        void setAddableChild( int indexNo){
            addableChilds[indexNo] = true;
        }
        RobotDirections getRobotsFirstDirectionAfterFinding(){
            return robotsFirstDirectionAfterFinding;
        }
        void setCreatedAfterObstacleFinding(bool status){
            createdAfterObstacleFinding = status;
        }
    private:
        bool addableChilds[2] = {false,false};//right  - left
        bool createdAfterObstacleFinding = false;
        RotationDirection rotationDirectionAfterGenerationOfNode;
        bool visited =false;
        std::shared_ptr <CellNode> parent;
        char name ;
        std::shared_ptr<CellNode> children[3] = {nullptr,nullptr,nullptr};// right left front
        Directions whichDirectionFromParent ;//parenttan bu celle gelmek icin gereken yon, diger hesaplamalar robotun mevcut yonune ile celli ilk buldugu yone gore yapilacak
        Directions directionsFromThisCellNodeToChildren[3];
        RobotDirections robotsFirstDirectionAfterFinding ;//robotun bu celli buldutansonraki ilk yonu
        

    };
    class NodesManager{
    public:
        inline static int CellNodeCounter =0;

        NodesManager(){
            currentCellNode = std::make_shared<CellNode>(65+NodesManager::CellNodeCounter);
            rootCellNode = currentCellNode;
            CellNodeCounter++;
        }
        bool findAddableNodes(std::vector<float> &lidarRays,RobotDirections currentRobotDirection){//10 hz ile cagirilir
            if(goingBackToParent){
                returnBackToParentWithChild(lidarRays,currentRobotDirection);
                return false;
            }
            float frontDistance = lidarRays[160];
            float leftDistance = lidarRays[319];
            float rightDistance = lidarRays[0];
            // std::cout<<"Front Distance: "<<frontDistance<< " Left Distance: "<<leftDistance<<" Right Distance: "<<rightDistance<<std::endl;
            // if(frontDistance>0.7){
            //     std::cout<<"There is an addable node in FRONT"<<std::endl;
            //     addableNodes[0]= true;
            // }
            if(frontDistance>1){
                
                if((leftDistance >0.7 || rightDistance>0.7)&&CellNodesWaitingTimeAfterBeingAdded[0]==0){
                    std::cout<<"There is an addable node in FRONT"<<std::endl;
    
                    std::shared_ptr<CellNode> newCurrent  = std::make_shared<CellNode>(65+CellNodeCounter,currentCellNode,Directions::FRONT,currentRobotDirection); 
                    int robotDirectionInt = static_cast<int>(currentRobotDirection);
                    int getRobotsFirstDirectionAfterFinding  = static_cast<int>(currentCellNode->getRobotsFirstDirectionAfterFinding());
                    if(!currentCellNode->createdAfterFindingObstacle()){
                        if(getRobotsFirstDirectionAfterFinding==robotDirectionInt){
                            std::cout<<"There is an addable node in FRONT"<<std::endl;  
                            currentCellNode->addChildren(newCurrent,Directions::FRONT,currentRobotDirection);
                        }else if(getRobotsFirstDirectionAfterFinding==(robotDirectionInt +1)%4){
                            std::cout<<"There is an addable node in RIGHT"<<std::endl;
                            currentCellNode->addChildren(newCurrent,Directions::RIGHT,currentRobotDirection);

                        }else if(getRobotsFirstDirectionAfterFinding==(robotDirectionInt +3)%4){
                            std::cout<<"There is an addable node in LEFT"<<std::endl;
                            currentCellNode->addChildren(newCurrent,Directions::LEFT,currentRobotDirection);

                        }
                    }else{
                        if(getRobotsFirstDirectionAfterFinding==(robotDirectionInt +1)%4){
                            std::cout<<"There is an addable node in RIGHT"<<std::endl;
                            
                            currentCellNode->addChildren(newCurrent,Directions::RIGHT,currentRobotDirection);

                        }else if(getRobotsFirstDirectionAfterFinding==(robotDirectionInt +3)%4){
                            std::cout<<"There is an addable node in LEFT"<<std::endl;

                            currentCellNode->addChildren(newCurrent,Directions::LEFT,currentRobotDirection);

                        }
                    }
                    visitedNodesStack.push_back(currentCellNode);
                    newCurrent->setVisited();
                    currentCellNode = newCurrent;
                    CellNodesWaitingTimeAfterBeingAdded[0]=lock;
                    CellNodeCounter++;
                    
                }else if(CellNodesWaitingTimeAfterBeingAdded[0] >0){
                    CellNodesWaitingTimeAfterBeingAdded[0]--;
                }
                if(leftDistance>0.7&&CellNodesWaitingTimeAfterBeingAdded[1] ==0){
                    std::cout<<"There is an addable node in LEFT"<<std::endl;
                    //
                    // std::shared_ptr<CellNode> childCellNode = std::make_shared<CellNode>(65+CellNodeCounter,currentCellNode,Directions::LEFT,currentRobotDirection);
                    // currentCellNode->addChildren(childCellNode,Directions::LEFT,currentRobotDirection);
                    CellNodesWaitingTimeAfterBeingAdded[1]=lock;
                    // CellNodeCounter++;
                    currentCellNode ->setAddableChild(1);//left
                    
    
                }else if(CellNodesWaitingTimeAfterBeingAdded[1]>0){
                    CellNodesWaitingTimeAfterBeingAdded[1]--;
                }
                if(rightDistance>0.7&&CellNodesWaitingTimeAfterBeingAdded[2]==0){
                    std::cout<<"There is an addable node in RIGHT"<<std::endl;
    
                    // std::shared_ptr<CellNode> childCellNode = std::make_shared<CellNode>(65+CellNodeCounter,currentCellNode,Directions::RIGHT,currentRobotDirection);
                    // currentCellNode->addChildren(childCellNode,Directions::RIGHT,currentRobotDirection);
                    CellNodesWaitingTimeAfterBeingAdded[2]=lock;
                    // CellNodeCounter++;
                    currentCellNode ->setAddableChild(0);//right

                }else if(CellNodesWaitingTimeAfterBeingAdded[2]>0){
                    CellNodesWaitingTimeAfterBeingAdded[2]--;
                }
            }
            return true;
        }
        RotationDirection getRotationDirection(){
            return rotationDirection;
        }
        void findobstacle(RobotDirections currentRobotDirection){
            // std::shared_ptr<CellNode> temp = currentCellNode->getParent();
            // std::cout<<"parent" << temp->getName();
            // temp->getAvailableChild();
            // bool control  = true;
            // while (temp != nullptr)
            // {
            //    currentCellNode = currentCellNode->getParent();
            //    temp = currentCellNode->getAvailableChild();
            // std::cout<<"parent" << temp->getName();
                

            // }
            // std::cout<<"node robot will go back to" << temp->getName();
            // std::shared_ptr<CellNode> tempParent = currentCellNode->getParent();
            // std::shared_ptr<CellNode> tempChild;
            // while (tempParent) {
            //     tempChild = tempParent->getAvailableChild();

            //     if (tempChild) {
            //         // ARADIĞIN CHILD BULUNDU
            //         break;
            //     }

            //     tempParent = tempParent->getParent();
            // }
            for(int i = 0;i<3;i++){
                obstacle[i] = false;
            }
            std::shared_ptr<CellNode> previousNode=visitedNodesStack.back();
            availableDirections =previousNode->findAvailableDirectionsWhileReturningBack();
            std::string directions [4] = {"NORTH","EAST","SOUTH","WEST"};
            for(int i =0;i<4;i++){
                std::cout<<"name of the node"<<previousNode->getName() <<" available direction while returning back "<< directions[i]<<" is "<< availableDirections.directions[i]<<std::endl;
            }
            convertAvailableDirecitonsForRobotToUnderstand(currentRobotDirection);
            
        }   
        void returnBackToParentWithChild(std::vector<float> &lidarRays,RobotDirections currentRobotDirection){
            
        }
        bool convertAvailableDirecitonsForRobotToUnderstand(RobotDirections currentRobotDirection){//mevcut yonune gore nasil bir node aramasi gerektigini bul
            int currentDirectionInt = static_cast<int> (currentRobotDirection);
            int backDirection = (currentDirectionInt + 2)%4;
            int leftDirection =  (currentDirectionInt - 1 + 4)%4;
            int  rightDirection= (currentDirectionInt + 1)%4;
            std::cout<<"currentDirectionInt "<< currentDirectionInt <<std::endl;
            if( !availableDirections.directions[rightDirection]){//right taraf kapaliysa
                obstacle[0] = true;
            } if( !availableDirections.directions[leftDirection]){//left taraf kapaliysa
                obstacle[1] = true;
            } if( !availableDirections.directions[currentDirectionInt]){//front taraf kapaliysa
                obstacle[2] = true;
            }
            std::string directionStatus[3] = {"right","left","front"};
            for(int i = 0 ;i<3;i++){
                std::cout<<"direction "<< directionStatus[i]<<" to spot is "<<obstacle[i]<<std::endl;
            }
            


        }
        // std::shared_ptr<CellNode> parentNodeHasAvailableChild(){
            
            
        //     std::shared_ptr<CellNode> tempNode ;
        //     tempNode=visitedNodesStack.back()->getAvailableChild();
        //     if(tempNode!=nullptr){
        //         return tempNode;
        //     }

        //     return tempNode;
        // }
        bool afterObstacleDetectedNodes(std::vector<float> &lidarRays,RobotDirections currentRobotDirection,RotationDirection rotationDirection){//robotun dondugu aci

            float leftDistance = lidarRays[319];
            float rightDistance = lidarRays[0];

            std::shared_ptr<CellNode> newCurrent  = std::make_shared<CellNode>(65+CellNodeCounter,currentCellNode,rotationDirection,currentRobotDirection); 
            // currentCellNode->setAddableChild(0);//right
            if(!currentCellNode->createdAfterFindingObstacle()){
                 int robotDirectionInt = static_cast<int>(currentRobotDirection);
                 std::cout<<"robotDirectionInt before rotation "<< robotDirectionInt <<std::endl;
                int getRobotsFirstDirectionAfterFinding  = static_cast<int>(currentCellNode->getRobotsFirstDirectionAfterFinding());
                std::cout<<"getRobotsFirstDirectionAfterFinding  "<< getRobotsFirstDirectionAfterFinding <<std::endl;
                // if(rotationDirection == RotationDirection::LEFT){
                //     robotDirectionInt = (robotDirectionInt +3)%4;
                // }else if (rotationDirection == RotationDirection::RIGHT){
                //     robotDirectionInt = (robotDirectionInt +1)%4;
                // }
                // std::cout<<"robotDirectionInt after rotation "<< robotDirectionInt <<std::endl;
                if(getRobotsFirstDirectionAfterFinding==robotDirectionInt){
                    std::cout<<"There is an addable node in FRONT"<<std::endl;  
                    currentCellNode->addChildren(newCurrent,Directions::FRONT,currentRobotDirection);
                }else if((getRobotsFirstDirectionAfterFinding+1)%4==robotDirectionInt ){
                    std::cout<<"There is an addable node in RIGHT"<<std::endl;
                    currentCellNode->addChildren(newCurrent,Directions::RIGHT,currentRobotDirection);

                }else if((getRobotsFirstDirectionAfterFinding+3)%4==robotDirectionInt ){
                    std::cout<<"There is an addable node in LEFT"<<std::endl;
                    currentCellNode->addChildren(newCurrent,Directions::LEFT,currentRobotDirection);

                }

            }else{
                if(currentCellNode->getAddableChild(0)&&currentCellNode->getAvailableChild(0)==nullptr)//right
                {
                    std::cout<<"There is an addable node in Right"<<std::endl;

                    currentCellNode->addChildren(newCurrent,Directions::RIGHT,currentRobotDirection);
                }
                else if(currentCellNode->getAddableChild(1)&&currentCellNode->getAvailableChild(1)==nullptr)//left
                    {   
                        std::cout<<"There is an addable node in Left"<<std::endl;
                        currentCellNode->addChildren(newCurrent,Directions::LEFT,currentRobotDirection);
                    }
            }
            visitedNodesStack.push_back(currentCellNode);
            newCurrent->setVisited();
            
            currentCellNode = newCurrent;
            CellNodeCounter++;
            if(RotationDirection::RIGHT == rotationDirection){
                currentCellNode ->setAddableChild(0);//right
                std::cout<<"added right child after obstacle detection"<<std::endl;
                if(leftDistance>0.7){
                    std::cout<<"There is an addable node in LEFT"<<std::endl;

                    //
                    // std::shared_ptr<CellNode> childCellNode = std::make_shared<CellNode>(65+CellNodeCounter,currentCellNode,Directions::LEFT,currentRobotDirection);
                    // currentCellNode->addChildren(childCellNode,Directions::LEFT,currentRobotDirection);
                    CellNodesWaitingTimeAfterBeingAdded[1]=lock;
                    // CellNodeCounter++;
                    currentCellNode ->setAddableChild(1);//left
                }
            }
            if(RotationDirection::LEFT == rotationDirection){
                currentCellNode ->setAddableChild(1);//left
                std::cout<<"added left child after obstacle detection"<<std::endl;

            }
            CellNodesWaitingTimeAfterBeingAdded[1]=lock;
            CellNodesWaitingTimeAfterBeingAdded[0]=lock;
            CellNodesWaitingTimeAfterBeingAdded[2]=lock;
            return true;

        }
        int foundPrevNode = 0;
        void returnBack(std::vector<float> &lidarRays,RobotDirections &currentRobotDirection,bool & rotating,float & angleToRotate,RotationDirection & rotationDirection,std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::Twist>> velocityPublisher){//runs when goingBackToParent is true;
            
            //find Directions to search for
            float frontDistance = lidarRays[159];
            float leftDistance = lidarRays[319];
            float rightDistance = lidarRays[0];
            std::cout<<"frontDistance "<<frontDistance<<" leftDistance "<<leftDistance<<" rightDistance "<<rightDistance<<std::endl;
            bool consistentDirections[3] = {false,false,false};//right ,left , front
            //robotun mevcut konumuna gore saginin solunun ve onunun acik mi kapali olmasi gerektigini bulmam gerekli 
            //ardindan tutarli oldugu ilk noktadan itibaren 30 cm ilerlemesine izin verip
            //hangi child node gidecegi bulunmali

            //eger o path aciksa direction to spot true olmali
            if(obstacle[2]){
                if(frontDistance<0.7) 
                    consistentDirections[2] = true;
            }else{
                if(frontDistance>0.7) 
                    consistentDirections[2] = true;
            }
            if(obstacle[1]){
                if(leftDistance<0.7) 
                    consistentDirections[1] = true;
            }else{
                if(leftDistance>0.7) 
                    consistentDirections[1] = true;
            }
            if(obstacle[0]){
                if(rightDistance<0.7) 
                    consistentDirections[0] = true;
            }else{
                if(rightDistance>0.7) 
                    consistentDirections[0] = true;
            }
            std::string directionStatus[3] = {"right","left","front"};
            bool nodeIsDetected = true;
            if(CellNodesWaitingTimeAfterBeingFound>0){
                CellNodesWaitingTimeAfterBeingFound--;
            }
            for(int i = 0 ;i<3;i++){
                std::cout<<"direction "<< directionStatus[i]<<" is consistent "<<consistentDirections[i]<<std::endl;
                if(!consistentDirections[i]||CellNodesWaitingTimeAfterBeingFound>0){
                    nodeIsDetected = false;
                    
                }
                
            }

            
            std::cout<<"node is detected "<<nodeIsDetected<<std::endl;
            if(nodeIsDetected){//node tespiti gerceklestiyse gerceklesmediyse aramaya devam
                CellNodesWaitingTimeAfterBeingFound = 27;
                //node tespit edildi bu nodeun visited olmayan childi  var mi varsa bu taraftan devam et   
                //nodun visited olmayan childi yok bir ust parentina cik bunun icin stack islemleri ve yeniden aranacak nodelari bulma 
                std::cout<<"child index And current node "<< currentCellNode->getName()<<std::endl;
                currentCellNode = visitedNodesStack.back();
                std::cout<<"child index And current node "<< currentCellNode->getName()<<std::endl;

                int childIndex = currentCellNode->isThereAvailableChild();
                
                
                int previousCellNodeIndex= static_cast<int> (currentCellNode->getRobotsFirstDirectionAfterFinding());
                int locationDirectionIndex;
                std::cout<<"child index And current node "<< childIndex<<std::endl;
                if(childIndex!=-1){
                    // if(childIndex==0){ //if right child is available
                    //     currentCellNode->getAvailableChild(0);
                    // }else{
                        
                    // }
                    goingBackToParent = false;
                    std::shared_ptr<CellNode> node ;
                    if(childIndex==0){//sag taraf ise
                        locationDirectionIndex = (previousCellNodeIndex + 1)%4;
                        // node  = std::make_shared<CellNode>(65+CellNodeCounter,currentCellNode,Directions::RIGHT,currentCellNode->getRobotsFirstDirectionAfterFinding()); 
                        // CellNodeCounter++;
                        // currentCellNode->addChildren( node,Directions::RIGHT,currentCellNode->getRobotsFirstDirectionAfterFinding());
                        // std::cout<<"added right child while returning back "<<std::endl;
                        

                    }else{//sol taraf ise
                        locationDirectionIndex = (previousCellNodeIndex + 3)%4;
                        // node = std::make_shared<CellNode>(65+CellNodeCounter,currentCellNode,Directions::LEFT,currentCellNode->getRobotsFirstDirectionAfterFinding()); 
                        // CellNodeCounter++;
                        // currentCellNode->addChildren( node,Directions::LEFT,currentCellNode->getRobotsFirstDirectionAfterFinding());
                        // std::cout<<"added left child while returning back "<<std::endl;
                    }
                    // currentCellNode = node;


                }else{//not avaialbe child
                    locationDirectionIndex = (previousCellNodeIndex + 2)%4;//back
                }
                currentCellNode = visitedNodesStack.back();
                visitedNodesStack.pop_back();
                // Determine rotation direction based on movement direction
                // if(childIndex == 0){  // right child
                //     rotationDirection = RotationDirection::RIGHT;
                // }else if(childIndex == 1){  // left child
                //     rotationDirection = RotationDirection::LEFT;
                // }else{  // back direction
                //     rotationDirection = RotationDirection::TURNBACK;
                // }
                if(locationDirectionIndex!=static_cast<int>(currentRobotDirection) ){
                    angleToRotate = 90*(M_PI/180);
                    rotating =true;
                    int currentRobotIndex = static_cast<int>(currentRobotDirection);
                    if(currentRobotIndex==(locationDirectionIndex+3)%4){//eger ki sag tarafsa current robota esitlenir
                        rotationDirection =RotationDirection::RIGHT; //sag taraf
                        currentRobotIndex = (currentRobotIndex + 1)%4;
                    }else{
                        rotationDirection =RotationDirection::LEFT; //sag Sola
                        currentRobotIndex = (currentRobotIndex +3)%4;
                    }
                    currentRobotDirection = static_cast<RobotDirections>(currentRobotIndex);
                    foundPrevNode = 1;
                    auto geometricMsg = geometry_msgs::msg::Twist();
                    geometricMsg.angular.x= 0.30;
                    velocityPublisher->publish(geometricMsg);
                }else{
                    angleToRotate= 0;
                    rotating = false;
                }
                if(goingBackToParent){
                    findobstacle(currentRobotDirection);
                }
                std::cout<<"new current node  "<< currentCellNode->getName()<<std::endl;
            }
        }
        void setCurrentCellNode(std::shared_ptr<CellNode> cellNode){
            currentCellNode = cellNode;
        }
        std::shared_ptr<CellNode> getCurrentCellNode(){
            return currentCellNode;
        }
        void setGoindBackToParent(){
            goingBackToParent = !goingBackToParent ;
        }
        bool getGoingBackToParent(){
            return goingBackToParent;
        }
    private:
        bool obstacle[3]  = {false,false,false};// Right,Left,Front         
           //eger o path aciksa direction to spot true olmali
        RotationDirection rotationDirection;
        struct AvailableDirectionsWhileReturningBack availableDirections;
        bool goingBackToParent = false;
        int lock = 27;// lock that side for 2.7 second  80 cm 
        std::vector<std::shared_ptr<CellNode> > visitedNodesStack ;
        int CellNodesWaitingTimeAfterBeingAdded[3] ={15,15,15} ;//after adding each CellNode wait for some time to add again on that side
        int CellNodesWaitingTimeAfterBeingFound =0 ;
        // 0 - front, 1-left ,2-right
        std::shared_ptr<CellNode> currentCellNode= nullptr;
        std::shared_ptr<CellNode> rootCellNode = nullptr;

};

class RobotsFirsAdjustmentAccodingToMaze {

    public:
        RobotsFirsAdjustmentAccodingToMaze(float rotationAngle,RotationDirection rotationDirection){
            this->rotationAngle = rotationAngle;
            this->rotationDirection = rotationDirection;

        }
        RotationDirection getRotationDirection(){
            return rotationDirection;

        }
        float getRotationAngle(){
            return rotationAngle;

        }
    private:
        float rotationAngle;
        RotationDirection rotationDirection;


};
class MazeSolverNode : public rclcpp::Node{


    public:
        MazeSolverNode(const rclcpp::NodeOptions &options) :rclcpp::Node("mazeSolverNode",options),remainingAngle(0.0),rotating(false){
            nodesManager = std::make_shared<NodesManager>();
            velocityPublisher = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel",10);

            timer = rclcpp::create_timer(
              this->get_node_base_interface(),
              this->get_node_timers_interface(),
              this->get_clock(), // Bu saat artık simülasyon saatidir (use_sim_time=true sayesinde)
              500ms,
              std::bind(&MazeSolverNode::pubhlisherCallBack, this)
            );
            timer2 = rclcpp::create_timer(
              this->get_node_base_interface(),
              this->get_node_timers_interface(),
              this->get_clock(), // Bu saat artık simülasyon saatidir (use_sim_time=true sayesinde)
              100ms,
              std::bind(&MazeSolverNode::addNodes, this)
            );
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
            
            if(!enteredIntoMaze){
                lidarRays = msg.ranges;
                if(didEnteredIntoMaze()){
                    std::cout<<"enteredintomaze"<<std::endl;
                    auto geometricMsg = geometry_msgs::msg::Twist();
                    geometricMsg.linear.x = 0;
                    velocityPublisher->publish(geometricMsg);
                    

                }

                
            }else if(!rotating){

                if(!allowanceForNodeSearching){
                    allowanceForNodeSearching = true;
                }
                float angleIncrement = msg.angle_increment;
                lidarRays = msg.ranges;
                laserScanOld = msg;
                float angleMin = findMinAngleRadian(angleIncrement,lidarRays,80,240);//msg.angle_min
                int index = angleMin/angleIncrement;
                // std::cout<<"distance : "<<lidarRays[index]<<std::endl;

                if(index>=80&&index<=240&&msg.ranges[index]<0.28){//onunde engel var mi 
                    rotating = true;
                    wentBack = false;
                    wentBackCounter = 2;
                    std::cout<<"index"<< index<<std::endl;
                    auto geometricMsg = geometry_msgs::msg::Twist();
                    geometricMsg.linear.x = 0;
                    velocityPublisher->publish(geometricMsg);
                    count =0;
                    findWhichDirectionToRotate(lidarRays,angleIncrement,index);
                    minAngleRadian = findMinAngleRadian(laserScanOld.angle_increment,laserScanOld.ranges);
                    indexRotation = minAngleRadian/laserScanOld.angle_increment;
                    remainingAngle = (indexRotation)*laserScanOld.angle_increment;
                    RotationDirection robotsRotationDirection;
                    if(rotationDirection==RotationDirection::RIGHT){
                        //rotate right
                        remainingAngle = (320-index)*angleIncrement;
                        // std::shared_ptr<CellNode> currentNode = nodesManager->getCurrentCellNode();
                        // std::shared_ptr<CellNode> childCellNode = std::make_shared<CellNode>(65+NodesManager::CellNodeCounter,currentNode,Directions::FRONT,robotDirection);

                        std::cout<<"Rotate Right Remaining Angle: subscriber"<<remainingAngle*180/M_PI<<std::endl;
                        nodesManager->afterObstacleDetectedNodes(lidarRays,robotDirection,RotationDirection:: RIGHT);
                        robotDirection  = nextState(robotDirection,Directions::RIGHT); 
                        //burada bir getgoinf back to parent degerini kontrol et
                        if(nodesManager->getGoingBackToParent()){
                            nodesManager->findobstacle(robotDirection);
                        }    
                    }else if(rotationDirection==RotationDirection:: LEFT){
                        remainingAngle = (index)*angleIncrement;
                        std::cout<<"Rotate Left Remaining Angle: subscriber"<<remainingAngle*180/M_PI<<std::endl;
                        nodesManager->afterObstacleDetectedNodes(lidarRays,robotDirection,RotationDirection:: LEFT);
                        robotDirection  = nextState(robotDirection,Directions::LEFT); 
                        //parenta gidiyorsa robotun bulmasi gereken nodun etrafindaki enngelleri bul
                        if(nodesManager->getGoingBackToParent()){
                                nodesManager->findobstacle(robotDirection);
                        }
                    
                    }else{//eger donus yoksa ve engele gelmissse child ekler ve turn back yapar
                        remainingAngle = M_PI;
                        // std::shared_ptr<CellNode> childCellNode = std::make_shared<CellNode>(65+NodesManager::CellNodeCounter,currentNode,Directions::FRONT,robotDirection);
                        // currentNode->addChildren(childCellNode,Directions::FRONT,robotDirection);
                        
                        std::cout<<"Rotate Left Remaining Angle: subscriber"<<remainingAngle*180/M_PI<<std::endl;
                        nodesManager->afterObstacleDetectedNodes(lidarRays,robotDirection,RotationDirection::TURNBACK);
                        nodesManager->setGoindBackToParent();
                        robotDirection  = nextState(robotDirection,Directions::RIGHT); 
                        robotDirection  = nextState(robotDirection,Directions::RIGHT); 
                        std::shared_ptr<CellNode> currentNode = nodesManager->getCurrentCellNode();
                        currentNode->setVisited();
                        nodesManager->findobstacle(robotDirection);
                    }
                    
                }
            }
                
            

        }
        RobotDirections nextState(RobotDirections current,Directions direction) //if right true left false
        {
            constexpr int stateCount = 4;

            int index = static_cast<int>(current);
            if(direction == Directions::RIGHT){
                
                index = (index + 1) % stateCount;
            }else{
                if(index == 0 ){
                    index = 3;
                }else{

                    index = (index - 1) % stateCount;
                }
            }
            std::cout<<index<<" which direction going in "<<std::endl;
            return static_cast<RobotDirections>(index);
        }
        void addNodes(){
            
            if((!rotating&&enteredIntoMaze)&&allowanceForNodeSearching){
                // std::cout<<"addNodes "<<std::endl;
                if(!nodesManager->getGoingBackToParent()){
                   nodesManager->findAddableNodes(lidarRays,robotDirection);
                }else{
                    nodesManager->returnBack(lidarRays,robotDirection,rotating,remainingAngle,rotationDirection,velocityPublisher);//angle will be type of radian
                }
            }
        }
        void pubhlisherCallBack(){
            auto msg = geometry_msgs::msg::Twist();

            if(!rotating||nodesManager->foundPrevNode>0){
                        std::cout<<"Published Velocity "<<std::endl;
                
                msg.linear.x = 1;
                if(rotating){

                    nodesManager->foundPrevNode--;
                    msg.linear.x = 0.30;
                }
                
            }else{
                if(wentBack==false){
                    wentBackCounter--;
                    std::cout<<"Going Back"<< wentBackCounter<<std::endl;
                    if(wentBackCounter<0){
                        wentBack = true;
                        wentBackCounter = 2;
                    }
                    msg.linear.x = -1;
                }else{
                    indexRotation = minAngleRadian/(320.0/640.0);
                    std::cout<<"min angle Radian"<<minAngleRadian<<"min angle indexRotation"<<
                    indexRotation<<" min angle distance"<<std::endl;
                    // if(!nodesManager->getGoingBackToParent()){

                        rotateRobot(msg,rotationDirection);
                    // }else{
                    //     rotateRobot(msg,nodesManager->getRotationDirection());
                    // }
                    
                }
            }
            velocityPublisher->publish(msg);
            std::cout<<"Published Velocity "<<std::endl;
        }
    
        void findWhichDirectionToRotate(const std::vector<float> &lidarRays,float angleIncrement    ,int indexMinAngle){
            if(lidarRays[0]>0.70){    
                rotationDirection = RotationDirection::RIGHT;
            }else if(lidarRays[319]>0.70){
                rotationDirection = RotationDirection:: LEFT ;

            }else{
                rotationDirection = RotationDirection::TURNBACK;
            }

        }
        void rotateRobot(geometry_msgs::msg::Twist &msg,RotationDirection rotationDirection){
            int hz =2;
            float angleTorotate =0;
            if(remainingAngle<0.25){
                rotating = false;
                angleTorotate= remainingAngle*hz;
                std::cout<<"Finished Rotation"<<std::endl;
            }else{
                angleTorotate = 0.5;
            }
            
            if(rotationDirection==RotationDirection:: RIGHT ){
                msg.angular.z = -angleTorotate;
                remainingAngle -= angleTorotate/hz;
                std::cout<<"Rotate Right Remaining Angle: rotateRobot"<<remainingAngle*180/M_PI<<std::endl;
            }else if(rotationDirection==RotationDirection:: LEFT){
                msg.angular.z = angleTorotate;
                remainingAngle -= angleTorotate/hz;
                std::cout<<"Rotate Left Remaining Angle: rotateRobot"<<remainingAngle*180/M_PI<<std::endl;

            }else{
                msg.angular.z = angleTorotate;
                remainingAngle -= angleTorotate/hz;
                std::cout<<"Rotate Turn Back Remaining Angle: rotateRobot"<<remainingAngle*180/M_PI<<std::endl;
            }
        }
        bool didEnteredIntoMaze(){
            float minLeft  = INFINITY;
            int minIndexLeft = -1;
            for(int i = 160;i<480;i++){
                if(lidarRays[i]!=-INFINITY&&lidarRays[i]<minLeft){
                    minIndexLeft = i;
                    minLeft = lidarRays[i];

                }
            }
            float minRight = INFINITY;
            int minIndexRight = -1;

            for(int i = 0;i<160;i++){
                if(lidarRays[i]!=-INFINITY&&lidarRays[i]<minRight){
                    minIndexRight = i;
                    minRight = lidarRays[i];

                }
            }
            for(int i = 480;i<640;i++){
                if(lidarRays[i]!=-INFINITY&&lidarRays[i]<minRight){
                    minIndexRight = i;
                    minRight = lidarRays[i];
                    
                }
            }
            if((minIndexLeft!=-1 || minIndexRight!=-1)){//her iki yanindaki en minlerin aralarindaki farki egerki 180 ise labirente giris yapilmistir
                std::cout<<"minRight"<<minRight<<std::endl;
                std::cout<<"min Left"<<minLeft<<std::endl;

                std::cout<<"MinIndexRight"<<minIndexRight<<std::endl;
                std::cout<<"MInIndexLeft"<<minIndexLeft<<std::endl;

                int sumIndex = minIndexLeft - minIndexRight;
                if(minIndexRight>160){
                    sumIndex  = minIndexRight - minIndexLeft;
                }
                std::cout<<"sumIndex"<<sumIndex<<std::endl;
                
                if((sumIndex<=322&& sumIndex>=318)){    
                    rotating =true;
                    enteredIntoMaze = true;
                    float angle = 0;
                    if(minIndexLeft>320){

                    }
                    if(minIndexRight<160){
                        rotationDirection = RotationDirection::LEFT;
                        angle = minIndexRight*(360.0/640.0);
                    }else{
                        rotationDirection = RotationDirection::RIGHT;
                        
                        std::cout<<angle<<" 180- aci "<< minIndexLeft*(360.0/640.0)<<std::endl;

                        angle = 180-(minIndexLeft*(360.0/640.0));

                    }
                    std::cout<<angle<<" angle"<<std::endl;

                    angle  *= (M_PI/180); 
                    remainingAngle = angle;
                    std::cout<<remainingAngle*(180/M_PI)<<"remaining angle"<<std::endl;
                    robotsFirsAdjustmentAccodingToMaze = std::make_unique< RobotsFirsAdjustmentAccodingToMaze>(angle,rotationDirection);


                }
                

            }


            return enteredIntoMaze;

        }
        
    private:  
        bool allowanceForNodeSearching= false;
        std::unique_ptr<RobotsFirsAdjustmentAccodingToMaze>  robotsFirsAdjustmentAccodingToMaze ;
        RotationDirection rotationDirection;
        bool adjustedRobotsOrientation = false;
        bool enteredIntoMaze = false;
        bool addableNodes[3] = {false,false,false};//front,left,right 
        int indexRotation =0;
        float minAngleRadian =0;
        int count = 0;
        int wentBackCounter = 2;
        bool wentBack = false;
        sensor_msgs::msg::LaserScan laserScanOld;
        sensor_msgs::msg::LaserScan laserScanNew;
        float remainingAngle ;
        bool rotating ;
        bool direction;
        std::vector<float> lidarRays; 
        rclcpp::TimerBase::SharedPtr timer;
        rclcpp::TimerBase::SharedPtr timer2;
        std::shared_ptr<NodesManager> nodesManager ;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocityPublisher;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laserSubscription;
        RobotDirections robotDirection =RobotDirections:: NORTH;
    };


int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    options.parameter_overrides({
        {"use_sim_time", true}
    });

    auto node = std::make_shared<MazeSolverNode>(options);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;


    return 0;
}
