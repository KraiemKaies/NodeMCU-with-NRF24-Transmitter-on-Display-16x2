#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t ce_pin = D3;
uint8_t csn_pin = D4;
RF24 radio(ce_pin, csn_pin);     // CE, CSN
const byte address[6] = "00001"; // Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
uint8_t led_pin = D0;
uint8_t button_pin = D8;
boolean button_state = 0;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Transmitter.....");
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  radio.begin();                  // Starting the Wireless communication
  radio.openWritingPipe(address); // Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  // You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          // This sets the module as transmitter
  delay(2000);
}
void loop()
{
  button_state = digitalRead(button_pin);
  if (button_state == HIGH)
  {
    digitalWrite(led_pin, HIGH);
    Serial.println("Button On");
    lcd.println("Button On");
    const char text[] = "Your Button State is HIGH";
    radio.write(&text, sizeof(text)); // Sending the message to receiver
  }
  else
  {
    digitalWrite(led_pin, LOW);
    const char text[] = "Your Button State is LOW";
    lcd.println("Button Off");
    radio.write(&text, sizeof(text)); // Sending the message to receiver
  }
  radio.write(&button_state, sizeof(button_state)); // Sending the message to receiver
  delay(5);
  lcd.clear();
}