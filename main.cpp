#include <ros/ros.h>
#include <iostream>
#include <std_msgs/Float32.h>
#include "keyboard.h"
using namespace std;

int main(int argc, char** argv)
{  
  ros::init(argc, argv, "keyboard");
  ros::NodeHandle n("~");

  //ros::Publisher pub_down = n.advertise<keyboard::Key>("keydown", 10);
  //ros::Publisher pub_up = n.advertise<keyboard::Key>("keyup", 10);
  ros::Publisher pub_steering_command = n.advertise<std_msgs::Float32>("left_right", 10);
  ros::Publisher pub_acc_command = n.advertise<std_msgs::Float32>("up_down", 10);
  ros::Publisher pub_brk_command = n.advertise<std_msgs::Float32>("hold", 10);

  bool allow_repeat=false;
  int repeat_delay, repeat_interval;
  
  n.param<bool>( "allow_repeat", allow_repeat, false ); // disable by default
  n.param<int>( "repeat_delay", repeat_delay, SDL_DEFAULT_REPEAT_DELAY );
  n.param<int>( "repeat_interval", repeat_interval, SDL_DEFAULT_REPEAT_INTERVAL );
  
  if ( !allow_repeat ) repeat_delay=0; // disable 
  keyboard::Keyboard kbd( repeat_delay, repeat_interval );
  
  ros::Rate r(50);

  std_msgs::Float32 steering_command;
  std_msgs::Float32 acc_command; 
  std_msgs::Float32 brk_command;   
  float steer_ang_prev = 0, steer_ang, inc_ang = 2.0;
  float acc_prev = 0, acc,inc_acc = 2;
  float brk_prev = 0,brk, inc_brk = 2;

  keyboard::Key k;
  bool pressed, new_event;
  while (ros::ok() && kbd.get_key(new_event, pressed, k.code, k.modifiers)) {
    if (new_event || pressed) {
      k.header.stamp = ros::Time::now();
      if (pressed) { 
        //pub_down.publish(k);//119(W),115(D)
        if (new_event && k.code == 100) {
          if (inc_ang < 10){
            inc_ang = inc_ang + 0.5;
            
          }
        }
        if (new_event && k.code == 97) {
          if (inc_ang > 2){
            inc_ang = inc_ang - 0.5;
          }
        }
        if (k.code == 275) {
          if(steer_ang < 115.0){
            steer_ang = steer_ang_prev + inc_ang;
            //steer_ang = steer_ang_prev +1000;
            //steer_ang = 1;
	  }

          steering_command.data = steer_ang;
          steer_ang_prev = steer_ang;
        }
        else if (k.code == 276) {
          if(steer_ang > -115.0){
            steer_ang = steer_ang_prev - inc_ang;
            //steer_ang = steer_ang_prev - 1000;
            //steer_ang = 0;
          }
          steering_command.data = steer_ang;
          steer_ang_prev = steer_ang;
        }
        
        
        else if (k.code == 119){
      
      		if(acc <115)
      		{
      			acc = acc_prev + inc_acc;
      		}
      		acc_command.data = acc;
      		acc_prev = acc;
	}
	
	else if (k.code == 115){
      
      		if(acc > 0)
      		{
      			acc = acc_prev - inc_acc;
      		}
      		acc_command.data = acc;
      		acc_prev = acc;
	}
	
	else if (k.code == 32){
      
      		if(brk <100)
      		{
      			brk = brk_prev + inc_brk;
      		}
      		brk_command.data = brk;
      		brk_prev = brk;
      		
	}
	
	
	pub_steering_command.publish(steering_command);
	pub_acc_command.publish(acc_command);
	pub_brk_command.publish(brk_command);
        
      }
      
      
      else {
        //pub_up.publish(k);
        steering_command.data = steer_ang_prev;
        acc_command.data = acc_prev;
        brk_command.data = 0;
        brk_prev = 0;
        brk = 0;
        //for button sim
        steering_command.data = 0;
        steer_ang_prev = 0;
        
        pub_steering_command.publish(steering_command);
        pub_acc_command.publish(acc_command);
        pub_brk_command.publish(brk_command);
      }
    }
    ros::spinOnce();
    r.sleep();
  }
  
  ros::waitForShutdown();
}
