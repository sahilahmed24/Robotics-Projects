/*
Rescue Robot MARK 2
*/
#define servo 2
#define motor_leftpwm 5
#define motor_rightpwm 6
#define motor_left_d 3
#define motor_left_a 4
#define motor_right_a 9
#define motor_right_d 8
#define buzzer A5
#define aux_gnd_1 A4
#define aux_gnd_2 A3
#define aux_vcc A2
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
RF24 radio(10, 7); // CE, CSN
Servo gripper;
const byte address[6] = "00001";
int gripperPos = 90;
void setup() {
Serial.begin(115200);
pinMode(servo, OUTPUT);
pinMode(motor_left_a, OUTPUT);
pinMode(motor_left_d, OUTPUT);
pinMode(motor_right_a, OUTPUT);
pinMode(motor_right_d, OUTPUT);
pinMode(motor_rightpwm, OUTPUT);
pinMode(motor_leftpwm, OUTPUT);
pinMode(buzzer, OUTPUT);
pinMode(aux_gnd_1, OUTPUT);
pinMode(aux_gnd_2, OUTPUT);
pinMode(aux_vcc, OUTPUT);
digitalWrite(motor_left_a, HIGH);
digitalWrite(motor_left_d, HIGH);
digitalWrite(motor_right_a, HIGH);
digitalWrite(motor_right_d, HIGH);
digitalWrite(aux_gnd_1, LOW);
digitalWrite(aux_gnd_2, LOW);
digitalWrite(aux_vcc, HIGH);
gripper.attach(servo);
gripper.write(90);
radio.begin();
radio.openReadingPipe(0, address);
radio.setPALevel(RF24_PA_MIN);
radio.startListening();
}
void loop() {
if (radio.available()) {
char message[32] = "";
radio.read(&message, sizeof(message));
if (message[0] == '1')closeGripper();
else if (message[1] == '1')openGripper();
if (message[2] == '1')digitalWrite(buzzer, HIGH);
else digitalWrite(buzzer, LOW);
driveRobot(message[3], message[4], message[5], message[6]);
}
}
// Gripper
void closeGripper() {
gripperPos++;
gripper.write(gripperPos);
delay(10);
}
void openGripper() {
gripperPos--;
gripper.write(gripperPos);
delay(10);
}
//Driverobot
void driveRobot(char X_dir, char Y_dir, char X, char Y) {
int X_val, Y_val;
if (X == '4')X_val = 240;
else if (X == '3')X_val = 180;
else if (X == '2')X_val = 120;
else if (X == '1')X_val = 60;
else if (X == '0')X_val = 0;
if (Y == '4')Y_val = 240;
else if (Y == '3')Y_val = 180;
else if (Y == '2')Y_val = 120;
else if (Y == '1')Y_val = 60;
else if (Y == '0')Y_val = 0;
if (X_dir == '1')X_val *= -1;
if (Y_dir == '1')Y_val *= -1;
float r = 0.75 * sqrt(pow(X_val, 2) + pow(Y_val, 2));
float theta = atan2(Y_val, X_val);
if (theta < 0)theta += 2 * PI;
float rightMotorVal, leftMotorVal;
float slope = 4 / PI;
if (theta <= PI / 2) {
leftMotorVal = 1;
rightMotorVal = -1 + slope * theta;
} else if (theta <= PI) {
leftMotorVal = 1 - slope * (theta - PI / 2);
rightMotorVal = 1;
} else if (theta <= 3 * PI / 2) {
leftMotorVal = -1;
rightMotorVal = 1 - slope * (theta - PI);
} else if (theta <= 2 * PI) {
leftMotorVal = -1 + slope * (theta - 3 * PI / 2);
rightMotorVal = -1;
}
leftMotorVal *= r;
rightMotorVal *= r;
//Motors
int rightMotorDir = rightMotorVal < 0 ? 1 : 0;
int leftMotorDir = leftMotorVal < 0 ? 1 : 0;
rightMotorVal = floor(abs(rightMotorVal));
leftMotorVal = floor(abs(leftMotorVal));
if (rightMotorDir) {
digitalWrite(motor_right_a, HIGH);
digitalWrite(motor_right_d, LOW);
analogWrite(motor_rightpwm, rightMotorVal);
} else {
digitalWrite(motor_right_a, LOW);
digitalWrite(motor_right_d, HIGH);
analogWrite(motor_rightpwm, rightMotorVal);
}
if (leftMotorDir) {
digitalWrite(motor_left_a, LOW);
digitalWrite(motor_left_d, HIGH);
analogWrite(motor_leftpwm, leftMotorVal);
} else {
digitalWrite(motor_left_a, HIGH);
digitalWrite(motor_left_d, LOW);
analogWrite(motor_leftpwm, leftMotorVal);
}
}
