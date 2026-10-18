#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Servo.h>

#define DHTPIN 7
#define DHTTYPE DHT11
#define RED_LED 4
#define MQ_PIN A0
#define SERVO_PIN 6
#define BUTTON_PIN 2  // The reset button pin

#define OLED_MOSI 11
#define OLED_CLK  13
#define OLED_DC   8
#define OLED_CS   10
#define OLED_RESET 9

Adafruit_SSD1306 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
DHT dht(DHTPIN, DHTTYPE);
Servo servo;

bool alarmLatched = false;

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  
  servo.attach(SERVO_PIN);
  servo.write(0);
  Serial.begin(9600);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Listen for reset button
  if (digitalRead(BUTTON_PIN) == LOW) {
    alarmLatched = false; 
    digitalWrite(RED_LED, LOW); 
    servo.write(0); // Snap back to safe position
  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasValue = analogRead(MQ_PIN);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (isnan(temperature) || isnan(humidity)) {
    digitalWrite(RED_LED, LOW);
    servo.write(0);
    display.println("SENSORS NOT WORKING");
    display.display();
    return;
  }

  if (gasValue > 550) {
    alarmLatched = true;
  }

  // 1. FIRE ALERT
  if (alarmLatched && temperature > 35 && humidity < 40) {
    digitalWrite(RED_LED, HIGH);
    
    servo.write(180); // FIXED: Only one clean command for 180 degrees
    
    display.println("FIRE ALERT!");
    display.setCursor(0, 25);
    display.println("SUPPRESSION");
    display.setCursor(0, 50);
    display.println("ACTIVATED");

  // 2. GAS LEAK ALERT
  } else if (alarmLatched) {
    digitalWrite(RED_LED, HIGH);
    delay(200);
    digitalWrite(RED_LED, LOW);
    delay(200);
    
    servo.write(90); // Command for 90 degrees
    
    display.println("GAS LEAK");
    display.setCursor(0, 25);
    display.println("DETECTED!");
    display.setCursor(0, 50);
    display.println("Ventilate Room");

  // 3. SAFE STATE
  } else {
    digitalWrite(RED_LED, LOW);
    
    servo.write(0); // Rest at 0 degrees
    
    display.println("STATUS: ALL SAFE");
    display.setCursor(0, 25);
    display.print("Temp: ");
    display.print(temperature);
    display.setCursor(0, 50);
    display.print("Humidity: ");
    display.print(humidity);
  }

  display.display();
  
  if (!alarmLatched) {
    delay(500);
  }
}