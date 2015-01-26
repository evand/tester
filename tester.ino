#include <Arduino.h>
#include <string.h>

char* build_str = "build 0001";
int command_len = 5;
char command_str[6];
int c = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino pin tester");
  Serial.println(build_str);
  for (int i = 0; i < command_len + 1; i++) command_str[i] = 0;
  
  Serial.print("A0: ");
  Serial.println(A0);
  Serial.print("A7: ");
  Serial.println(A7);
  Serial.print("A15: ");
  Serial.println(A15);
}


/* format:
 * [PIN] [COMMAND]
 *
 * PIN:
 * Either a number (digital pin) or An (analog pin)
 * 
 * COMMAND:
 * i set input mode
 * p input pullup mode
 * o set output mode
 *
 * d digital read
 * a analog read
 *
 * l output low
 * h output high
 */

void loop() {
  if (Serial.available()) {
    int r = Serial.read();
    if (r < 0 || r > 255) {
      Serial.println("input error!");
      c = 0;
      command_str[c] = 0;
      return;
    }
    command_str[c] = (char)(r);
    command_str[c+1] = 0;
    if (c < command_len) c++;
    if (r == '\r' || r == '\n') {
      handle_cmd();
      c = 0;
      command_str[c] = 0;
    }
  }
}

void handle_cmd() {
  if (0) {
    Serial.print("Command received: \"");
    Serial.print(command_str);
    Serial.println("\"");
  }
  
  //tokenize on space
  //handle first part
  //handle second
  
  char cmd = 0;
  char* pin_str = command_str;
  boolean error = true;
  boolean analog = false;
  int pin = -1;
  
  int i = 0;
  while (i < command_len) {
    if (command_str[i] == 0) {
      break;
    }
    if (command_str[i] == ' ') {
      error = false;
      command_str[i] = 0;
      cmd = command_str[i+1];
      break;
    }
    i++;
  }  
  //i = length of first part
  if (error) {
    Serial.println("Command parse error. (Probably no separator.)");
    return;
  }
  
  Serial.print("Pin string: ");
  Serial.println(pin_str);
  Serial.print("Command character: ");
  Serial.println(cmd);
  
  if (i > 3) {
    Serial.print("Pin string too long. Length: ");
    Serial.println(i);
    return;
  }
  
  if (pin_str[0] == 'a') {
    pin_str = &pin_str[1];
    analog = true;
    Serial.print("Analog pin number: ");
    Serial.println(pin_str);
  }
  pin = atoi(pin_str);
  Serial.print("Parsed pin number: ");
  Serial.println(pin);
  if (analog) {
    pin += A0;
  }
  
  switch (cmd) {
    case 'i':
      pinMode(pin, INPUT);
      break;
    case 'p':
      pinMode(pin, INPUT_PULLUP);
      break;
    case 'o':
      pinMode(pin, OUTPUT);
      break;
    case 'd':
      Serial.print("Digital read: ");
      Serial.println(digitalRead(pin));
      break;
    case 'a':
      Serial.print("Analog read: ");
      Serial.println(analogRead(pin));
      break;
    case 'l':
      digitalWrite(pin, LOW);
      break;
    case 'h':
      digitalWrite(pin, HIGH);
      break;
    default:
      Serial.println("Bad command character.");
      break;
  }
}
