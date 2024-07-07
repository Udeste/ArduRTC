#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDRESS            0x8
#define I2C_SET_TIMESTAMP_CODE 0x0A
#define I2C_SET_TRIGGER_CODE   0x0B
#define I2C_GET_TIMESTAMP_CODE 0x0C
#define I2C_GET_TRIGGER_CODE   0x0D
#define TRIGGER_PIN            3
#define TIMESTAMP_LENGTH_BYTES 4
// #define SERIAL_DEBUG

uint32_t timestamp = 0;
uint32_t triggerTime = 0;
uint8_t txCmdCode = 0x0;

uint32_t readTimestamp () {
  uint32_t t;

  if (Wire.available() == TIMESTAMP_LENGTH_BYTES) {
    t =  (uint32_t) Wire.read() << (0 * 8);
    t |= (uint32_t) Wire.read() << (1 * 8);
    t |= (uint32_t) Wire.read() << (2 * 8);
    t |= (uint32_t) Wire.read() << (3 * 8);
  }
  return t;
}

void writeTimestamp (uint32_t timestamp) {
  byte timestamp_bytes[TIMESTAMP_LENGTH_BYTES] = {};

  timestamp_bytes[0] = (timestamp & 0x000000ff) >> (0 * 8);
  timestamp_bytes[1] = (timestamp & 0x0000ff00) >> (1 * 8);
  timestamp_bytes[2] = (timestamp & 0x00ff0000) >> (2 * 8);
  timestamp_bytes[3] = (timestamp & 0xff000000) >> (3 * 8);

  Wire.write((byte *)&timestamp_bytes, TIMESTAMP_LENGTH_BYTES);
}

void receiveEvent(int byteCount) {
  //  No response necessary. I2C master is only checking for presence
  if (byteCount == 0) return;

  byte command = Wire.read();

  switch (command) {
    case I2C_SET_TIMESTAMP_CODE:
      timestamp = readTimestamp();
      break;
    case I2C_SET_TRIGGER_CODE:
      triggerTime = readTimestamp();
      break;
    case I2C_GET_TIMESTAMP_CODE:
      txCmdCode = I2C_GET_TIMESTAMP_CODE;
      break;
    case I2C_GET_TRIGGER_CODE:
      txCmdCode = I2C_GET_TRIGGER_CODE;
      break;
    
  }
}

void sendEvent() {
  switch (txCmdCode) {
    case I2C_GET_TIMESTAMP_CODE:
      writeTimestamp(timestamp);
      break;
    case I2C_GET_TRIGGER_CODE:
      writeTimestamp(triggerTime);
      break;
  }
  txCmdCode = 0x0;
}

void setup() {
  #ifdef SERIAL_DEBUG
    Serial.begin(9600);
    Serial.println("RTC init");
  #endif

  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendEvent);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
}

void loop() {
  if (timestamp == triggerTime) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(TRIGGER_PIN, HIGH);
    delay(500);                    
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(TRIGGER_PIN, LOW);
  }
  
  timestamp++;

  #ifdef SERIAL_DEBUG
    Serial.println("loop:\n");
    Serial.println(timestamp);
    Serial.println(triggerTime);
  #endif

  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);                    
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
