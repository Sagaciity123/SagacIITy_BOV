#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Float32.h>
#include<Servo.h>

const int SERVO = 9;
const float maxacc = 1;
const double G29max = 32767;
const double G29min = -32768;
const double G29braketh = 26000;
const int ServoStart = 180;
Servo myServo;
ros::NodeHandle nh;
std_msgs::Float32 acc_cmd;
std_msgs::Float32 brk_cmd;
std_msgs::Float32 servo_cmd;



void AccCmdCb(const std_msgs::Float32 &msg)
{
  acc_cmd.data = msg.data;
}

void BrakeCmdCb(const std_msgs::Float32 &msg)
{
  brk_cmd.data = msg.data;
}

ros::Subscriber<std_msgs::Float32> sub_acc_cmd("/keyboard/up_down", AccCmdCb );
ros::Subscriber<std_msgs::Float32> sub_brk_cmd("/brake_command_G29", BrakeCmdCb );

void setup() {
  // put your setup code here, to run once:
  nh.initNode();
  nh.subscribe(sub_acc_cmd);
  nh.subscribe(sub_brk_cmd);
  //nh.advertise(pub_servo_cmd);
  Serial.begin(57600);
  
  //nh.advertise(pub_servo_cmd);
  pinMode(SERVO,OUTPUT);
  myServo.attach(SERVO,600,2300);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();

  int angle = 150-acc_cmd.data;
  myServo.write(angle);

  

}
