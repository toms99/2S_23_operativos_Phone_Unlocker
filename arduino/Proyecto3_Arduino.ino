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

int posX = 105;
int posY = 132; // 130 for medium
int posZ = 115;

int sepX = 20;
int sepYZ = 10;



void setup() {
  servoGarra.attach(garra);
  servoZ.attach(ejeZ);
  servoY.attach(ejeY);
  servoX.attach(ejeX);

  reset();
  Serial.begin(9600);
  moveServo(servoY, 90, 5);
  delay(100);
  moveServo(servoX, 80, 5);
  delay(100);
  cases(500);


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
      press(posX, posY, posZ);
      break;
    case 2:
      press(posX - sepX, posY, posZ);
      break;
    case 3:
      press(posX - 2 * sepX, posY, posZ);
      break;
    case 4:
      press(posX, posY - sepYZ, posZ - sepYZ);
      break;
    case 5:
      press(posX - sepX, posY - sepYZ, posZ - sepYZ);
      break;
    case 6:
      press(posX - 2 * sepX, posY - sepYZ, posZ - sepYZ);
      break;
    case 7:
      press(posX, posY - 2 * sepYZ, posZ - sepYZ );
      break;
    case 8:
      press(posX - sepX, posY - 2 * sepYZ, posZ - sepYZ);
      break;
    case 9:
      press(posX - 2 * sepX, posY - 2 * sepYZ, posZ - sepYZ);
      break;
    case 0:
      press(posX - sepX, posY - 3 * sepYZ - 5, posZ - 2*sepYZ);
      break;
    case 10: //Ok button
      press(65, 90, 100);
      delay(200);
      moveServo(servoY, 90, 5);
      delay(200);
      moveServo(servoX, 80, 5);
      delay(200);
      break;
    case 500://Large Screen

      posX = 105;
      posY = 130;
      posZ = 115;

      sepX = 20;
      sepYZ = 9;
      break;
    case 501: // Medium Screen

      posX = 105;
      posY = 127;
      posZ = 112;

      sepX = 18;
      sepYZ = 9;
      break;
    case 502: // Small Screen
      posY = 120;
      posZ = 107;
      sepYZ = 7;
      sepX = 17;
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
  /*moveServo(servoY, 90, 5);
    delay(200);
    moveServo(servoX, 80, 5);
    delay(200);*/
}

void press(int x, int y, int z) {
  moveServo(servoX, x, 10);
  delay(200);
  moveServo(servoY, y, 10);
  delay(200);
  moveServo(servoZ, z, 15);
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
