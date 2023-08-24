#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Float32.h>
#include<Servo.h>

const int SERVO = 10;
const float maxacc = 0.1;
const double G29max = 32767;
const double G29min = -32768;
Servo myServo;
ros::NodeHandle nh;
std_msgs::Float32 brk_cmd;
std_msgs::Float32 brkservo_cmd;



void BrakeCmdCb(const std_msgs::Float32 &msg)
{
  brk_cmd.data = msg.data;
}

ros::Subscriber<std_msgs::Float32> sub_brk_cmd("/keyboard/hold", BrakeCmdCb );


void setup() {
  // put your setup code here, to run once:
  nh.initNode();
  nh.subscribe(sub_brk_cmd);
  Serial.begin(57600);
  
  //nh.advertise(pub_servo_cmd);
  pinMode(SERVO,OUTPUT);
  myServo.attach(SERVO,600,2300);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  

  int anglepre = 0+20;
 
  if(brk_cmd.data > 50 && brk_cmd.data < 100)
    anglepre = 20;
  else if(brk_cmd.data == 100)
    anglepre = 20+50;
  myServo.write(anglepre);
 

}
