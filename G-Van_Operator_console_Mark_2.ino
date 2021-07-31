*/Operator console MARK 2*/
#define js_click A0
#define js_y A1
#define js_x A2
#define yellow_button 5
#define green_button 4
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
int jsY_offset, jsX_offset;
void setup() {
Serial.begin(115200);
pinMode(js_click, INPUT_PULLUP);
pinMode(js_y, INPUT_PULLUP);
pinMode(js_y, INPUT_PULLUP);
pinMode(yellow_button, INPUT_PULLUP);
pinMode(green_button, INPUT_PULLUP);
radio.begin();
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MIN);
radio.stopListening();
jsY_offset = analogRead(js_y);
jsX_offset = analogRead(js_x);
}
void loop() {
int jsX = jsY_offset - analogRead(js_y);
int jsY = jsX_offset - analogRead(js_x);
int jsC = digitalRead(js_click);
int yellowButton = !digitalRead(yellow_button);
int greenButton = !digitalRead(green_button);
char yellowButton_char = yellowButton == 0 ? '0' : '1';
char greenButton_char = greenButton == 0 ? '0' : '1';
char jsY_dir = jsY > 0 ? '0' : '1';
char jsX_dir = jsX > 0 ? '0' : '1';
char jsC_char = jsC == 0 ? '1' : '0';
char jsY_abs, jsX_abs;
if (abs(jsY) > 400)jsY_abs = '4';
else if (abs(jsY) > 300)jsY_abs = '3';
else if (abs(jsY) > 200)jsY_abs = '2';
else if (abs(jsY) > 100)jsY_abs = '1';
else if (abs(jsY) > 0)jsY_abs = '0';
if (abs(jsX) > 400)jsX_abs = '4';
else if (abs(jsX) > 300)jsX_abs = '3';
else if (abs(jsX) > 200)jsX_abs = '2';
else if (abs(jsX) > 100)jsX_abs = '1';
else if (abs(jsX) > 0)jsX_abs = '0';
const char message[] = {yellowButton_char, greenButton_char,
jsC_char, jsX_dir, jsY_dir, jsX_abs, jsY_abs
};
radio.write(&message, sizeof(message));
}
