#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#define RST_PIN         9           // Reset pin for RFID
#define SS_PIN          10          // Slave Select pin for RFID

#define SERVO_1_PIN     2           // Pin for Servo 1
#define SERVO_2_PIN     3           // Pin for Servo 2
#define SERVO_3_PIN     4           // Pin for Servo 3
#define SERVO_4_PIN     5           // Pin for Servo 4
#define SERVO_5_PIN     6           // Pin for Servo 5

Servo servo1, servo2, servo3, servo4, servo5;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

ThreeWire myWire(4,10,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

bool rfidLoop = true;
bool rtcLoop = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();            // Init SPI bus
  mfrc522.PCD_Init();     // Init MFRC522

  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);
  servo3.attach(SERVO_3_PIN);
  servo4.attach(SERVO_4_PIN);
  servo5.attach(SERVO_5_PIN);

  rtc.Begin();

  if (!rtc.IsDateTimeValid()) {
    rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
  }
}

void loop() {
  if (rfidLoop) {
    rfidLoopFunction();
  }
  else if (rtcLoop) {
    rtcLoopFunction();
  }
}

void rfidLoopFunction() {
  // Look for new RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Read UID of the card
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    // Print UID to serial monitor
    Serial.print("Card UID: ");
    Serial.println(uid);

    // Check UID
    if (uid == "74 22 1B 5B") {
      // Open all servos to 180 degrees
      servo1.write(180);
      servo2.write(180);
      servo3.write(180);
      servo4.write(180);
      servo5.write(180);
      
      delay(1000); // Delay to allow servo movement
    }
  }
}

void rtcLoopFunction() {
  RtcDateTime now = rtc.GetDateTime();
  int hour = now.Hour();
  int minute = now.Minute();

  // Check for specific times to open servos
  if (hour == 12 && minute == 0) {
    // Open servo 1
    servo1.write(90);
  }
  else if (hour == 13 && minute == 0) {
    // Open servo 2
    servo2.write(90);
  }
  // Add more conditions as needed for other times and servos
  
  // Delay to prevent checking every second
  delay(60000); // Check every minute
}
