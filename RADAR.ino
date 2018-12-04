#include <Servo.h> // Sambærilegt og import í python

Servo motor; // bý til tilvik af Servo klasanum
int motor_pinni = 9; // pinninn sem ég nota til að stjórna mótornum
int distance;
const int alarmLight = 4;

void setup() {
  setupServo();
  setupSensor();
  setupJoystick();
  pinMode(alarmLight, OUTPUT);
  Serial.begin(9600); 
}
int joystickValue;
void loop() {
  joystickValue = loopJoystick();
  distance = loopSensor();
  //Serial.print("JoystickValue="+joystickValue);
  String str = " distance=";
  str += distance;
  str += " joystickValue=";
  str += joystickValue;
  //Serial.println(str);

  if (distance >= 10) {digitalWrite(alarmLight, LOW);}
  else {digitalWrite(alarmLight, HIGH);}
  
  if (joystickValue == 9999 and distance >= 10){
    loopServo();
  }
  else if (joystickValue != 9999) {motor.write(motor.read() + joystickValue);}
}

/* Servo mótor.
 * Brúnn tengist í GND, rauður tengist í 5V og 
 * appelsínugulur í pinnann sem stjórnar snúningnum
 */

int motor_direction = -1;

void setupServo() {
  motor.attach(motor_pinni); // segi servo tilvikinu hvaða pinna á að nota
}

void loopServo() {
  // Servo mótorinn getur snúist 90° til vinstri og 90° til hægri, samtals 180°.
  // write fallið tekur á móti gráðunum
  if (motor.read() <= 5) {
    motor_direction = 1;
  }
  else if (motor.read() >= 175) {
    motor_direction = -1;
  }
  motor.write(motor.read() + motor_direction);
}

/*
  SENSOR
  VCC tengist í 5V
  Trig tengist í digital pinna.
  Echo tegist í digital pinna.
  Gnd tengist í GND.
*/

const int TrigPin = 2; 
const int EchoPin = 3; 
float fjarlaegd; // fjarlægð þess sem mælt er

void setupSensor() {
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
}

int loopSensor () {

  // sendir út púlsar
  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin,LOW);

  // mælt hvað púlsarnir voru lengi að berast til baka
  fjarlaegd = pulseIn(EchoPin,HIGH)/58.0; // deilt með 58 til að breyta í cm
  
  if(fjarlaegd < 0) { // fjarlægð getur ekki verið mínustala
    fjarlaegd = 0;
  } 
  return fjarlaegd;
}


/*
 JOYSTICK
  GND tengist í GND
  +5V tengist í 5V
  VRx tengist í analog pinna
  VRy tengist í analog pinna
  SW tengist í digital pinna
*/

const int x = A0;
const int y = A1;
const int takki = 8;

int x_hreyfing = 0;
int y_hreyfing = 0;
int takki_stada = 0;
bool manualMode;

void setupJoystick() {
  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(takki, INPUT_PULLUP); 
}

int loopJoystick() {
  x_hreyfing = analogRead(x);
  y_hreyfing = analogRead(y);
  if (digitalRead(takki) == LOW) {manualMode = !manualMode;}
  
  if (!manualMode) {return 9999;}
  if (y_hreyfing <= 300) {return -1;}
  else if (y_hreyfing >= 700) {return 1;}
  else {return 0;}
}
