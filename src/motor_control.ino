
const int motorRightOnOff = 9;
const int motorLeftOnOff = 10;
const int motorRightIn1 = 3;
const int motorRightIn2 = 2;
const int motorLeftIn1 = 5;
const int motorLeftIn2 = 4;

String buffer = "";
boolean stringComplete = false;

int recvSerial(char buffer[], int maxLength) {
  // Receive data from Serial into buffer, filling it up to maxLength
  int index = 0;
  while (Serial.available() > 0) {
    // Only get data if Serial data is there and we won't overflow the buffer
    if (index < maxLength) {
      buffer[index++] = Serial.read();
    }
    else {
      // Discard data that overflows the buffer
      Serial.read();
    }
  }
  buffer[index] = 0;
  return index;
}

void motorDigitalWrite(char motor, int pinValue1, int pinValue2)
{
  if (motor == 'r') {
    digitalWrite(motorRightIn1, pinValue1);
    digitalWrite(motorRightIn2, pinValue2);
  }
  else if (motor == 'l') {
    digitalWrite(motorLeftIn1, pinValue1);
    digitalWrite(motorLeftIn2, pinValue2);
  }
}

void setMotor(char motor, char dir)
{
  if (dir == 'f') {
      motorDigitalWrite(motor, HIGH, LOW);
  }
  else if (dir == 'b') {
    motorDigitalWrite(motor, LOW, HIGH);
  }
  else if (dir == 's') {
    motorDigitalWrite(motor, LOW, LOW);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(motorRightOnOff, OUTPUT);
  pinMode(motorLeftOnOff, OUTPUT);
  pinMode(motorRightIn1, OUTPUT);
  pinMode(motorRightIn2, OUTPUT);
  pinMode(motorLeftIn1, OUTPUT);
  pinMode(motorLeftIn2, OUTPUT);
  
  digitalWrite(motorRightOnOff, HIGH);
  digitalWrite(motorLeftOnOff, HIGH);
  
  setMotor('l', 's');
  setMotor('r', 's');
  
  Serial.println("ready");
}

void loop()
{
  if (stringComplete) {
    if (buffer[0] == 'p') {
      int time = buffer.substring(1).toInt();
      delay(time);
    }
    else if (buffer.length() == 4) {
      setMotor(buffer[0], buffer[1]);
      setMotor(buffer[2], buffer[3]);
    }
    else if (buffer.length() == 2) {
      setMotor(buffer[0], buffer[1]);
    }
    stringComplete = false;
    buffer = "";
  }
}

void serialEvent()
{
  while (Serial.available()) {
    char readChar = (char) Serial.read();
    Serial.println(buffer);
    if (readChar == ';') {
      stringComplete = true;
      return;
    }
    else {
      buffer += readChar;
    }
  }
}
