#include <Servo.h>

#define garra 6
#define ejeZ 11
#define ejeY 10
#define ejeX 9

Servo servoGarra; //180 Closed
Servo servoZ; // 90 down 180 up
Servo servoY; // 90 Behind
Servo servoX; //90 Center

String inputString = "";   // Variable para almacenar la cadena de entrada
boolean stringComplete = false;  // Bandera que indica si se ha completado la cadena de entrada



void setup() {
  servoGarra.attach(garra);
  servoZ.attach(ejeZ);
  servoY.attach(ejeY);
  servoX.attach(ejeX);

  reset();
  Serial.begin(9600);


}

void loop() {
  if (stringComplete)
  {
    processInputString();  // Procesar la cadena de entrada
    inputString = "";     // Reiniciar la cadena de entrada
    stringComplete = false;  // Restablecer la bandera
  }

}

void processInputString() {
  String tempString = inputString;
  int commaIndex = tempString.indexOf(',');

  while (commaIndex != -1) {
    String numServoString = tempString.substring(0, commaIndex);
    int numServo = numServoString.toInt();

    // Mover el servo según el número especificado
    Serial.println(numServo);
    cases(numServo);
    tempString = tempString.substring(commaIndex + 1);
    commaIndex = tempString.indexOf(',');
    delay(500);
  }
  cases(10);

}

void cases(int caso) {
  switch (caso) {
    case 1:
      press(105, 135, 115);
      break;
    case 2:
      press(85, 135, 115);
      break;
    case 3:
      press(65, 135, 115);
      break;
    case 4:
      press(105, 125, 105);
      break;
    case 5:
      press(85, 120, 105);
      break;
    case 6:
      press(65, 125, 105);
      break;
    case 7:
      press(105, 115, 100);
      break;
    case 8:
      press(85, 115, 105);
      break;
    case 9:
      press(65, 115, 105);
      break;
    case 0:
      press(85, 105, 95);
      break;
    case 10:
      press(65, 90, 100);
      break;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '%') {  // Caracter de terminación
      stringComplete = true;
    } else {
      inputString += inChar;
      Serial.println(inputString);
    }
  }
}

void reset() {
  servoGarra.write(180);
  delay(200);
  moveServo(servoZ, 180, 5);
  delay(200);
  moveServo(servoY, 90, 5);
  delay(200);
  moveServo(servoX, 80, 5);
  delay(200);
}

void press(int x, int y, int z) {
  moveServo(servoX, x, 10);
  delay(1000);
  moveServo(servoY, y, 10);
  delay(500);
  moveServo(servoZ, z, 10);

  delay(100);
  reset();
}

void moveServo(Servo servo, int target, int velocity) {
  int currentPos = servo.read();
  if (currentPos < target) {
    for (int pos = servo.read(); pos <= target; pos++) {
      servo.write(pos);
      delay(velocity);
    }
  }
  else {
    for (int pos = servo.read(); pos >= target; pos--) {
      servo.write(pos);
      delay(velocity);
    }
  }
}
