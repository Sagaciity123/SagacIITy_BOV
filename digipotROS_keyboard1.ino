#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Float32.h>
#include <SPI.h>

const uint8_t CSpin = 10;

ros::NodeHandle nh;
std_msgs::Float32 leftright_cmd;
std_msgs::Float32 digi_input_cmd;

void leftrightCmdCb(const std_msgs::Float32 &msg)
{
  leftright_cmd.data = msg.data;
}
ros::Subscriber<std_msgs::Float32> sub_leftright_cmd("/keyboard/left_right",leftrightCmdCb );
//ros::Subscriber<std_msgs::Float32> sub_leftright_cmd("/sineinput",leftrightCmdCb );
ros::Publisher pub_digi_cmd("/digiInput",&digi_input_cmd);
void setup() {
  // put your setup code here, to run once:
  nh.initNode();
  nh.subscribe(sub_leftright_cmd);

  SPI.begin();
  pinMode(CSpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  update_digipot(0x01,512+int(leftright_cmd.data));
  nh.spinOnce();
  delay(15);
  update_digipot(0x02,512-int(leftright_cmd.data));
  nh.spinOnce();
  delay(15);
  digi_input_cmd.data = leftright_cmd.data;
  //pub_digi_cmd.publish(&digi_input_cmd);
  //nh.spinOnce();

}

void update_digipot(uint8_t reg, uint16_t value) {
    union {
    uint16_t val;
    uint8_t bytes[2];
    } in;

    in.val = value << 6; //as per datasheet

    digitalWrite(CSpin, LOW);

    //Write Wiper Register.  0x01: wiper1, 0x02: wiper2
    SPI.transfer(reg);

    //send upper value byte
    SPI.transfer(in.bytes[1]);

    //send lower value byte
    SPI.transfer(in.bytes[0]);

    digitalWrite(CSpin, HIGH);
    }
