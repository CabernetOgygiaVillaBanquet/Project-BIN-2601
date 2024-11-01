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

ThreeWire myWire(12,11,13); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

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

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  // Check UID and time
  if (content.substring(1) == "74 22 1B 5B") {
  RtcDateTime now = rtc.GetDateTime();
  int hour = now.Hour();
  int minute = now.Minute();

  // Calculate angles for servo motors based on current time
  int angle1 = map(hour, 0, 23, 0, 180);
  int angle2 = map(minute, 0, 59, 0, 180);
  int angle3 = 90; // Example angle
  int angle4 = 90; // Example angle
  int angle5 = 90; // Example angle

  // Set servo angles
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(angle4);
  servo5.write(angle5);
      
  delay(1000); // Delay to allow servo movement
  }
}
