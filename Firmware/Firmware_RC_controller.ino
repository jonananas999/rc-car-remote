
 // RC Controller 


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define JOY_H     34  // Horizontal (links/rechts)
#define JOY_V     35  // Vertikal (vorne/rückwärts)  
#define JOY_SEL   33  // Joystick Button

#define SW1       16  // Button 1
#define SW2        4  // Button 2
#define SW3        5  // Button 3
#define SW5       17  // Button 4
#define SW8       23  // Button 5
#define SW9       25  // Button 6

#define OLED_SDA  21
#define OLED_SCL  22

// OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RC-Datenstruktur 
struct RC_Data {
  uint8_t throttle;   // 0-100 (JOY_V)
  uint8_t steering;   // 0-100 (JOY_H, 50=neutral)
  uint8_t buttons;    // Bitfield: SW1=0,SW2=1,SW3=2,SW5=3,SW8=4,SW9=5,SEL=6
  uint8_t checksum;
};

RC_Data rc_data = {50, 50, 0, 0};

// Debounce Timers
unsigned long lastDebounce[7] = {0};
bool lastButtonState[7] = {1,1,1,1,1,1,1};

void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Pin Setup (alle Buttons mit INPUT_PULLUP)
  pinMode(JOY_SEL, INPUT_PULLUP);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(SW8, INPUT_PULLUP);
  pinMode(SW9, INPUT_PULLUP);
  
  // OLED 
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Fehler!");
    for(;;);
  }
  
  showBootScreen();
  Serial.println("=== RC MACRO CONTROLLER READY ===");
  Serial.println("T:S:B (throttle:steering:buttons)");
}

void loop() {
  readControls();
  updateDisplay();
  printDebug();
  
  delay(50);  // 20Hz Update Rate
}

void readControls() {
  // Joystick analog (0-4095 -> 0-100)
  int h_raw = analogRead(JOY_H);
  int v_raw = analogRead(JOY_V);
  
  rc_data.steering = map(h_raw, 0, 4095, 0, 100);
  rc_data.throttle = map(v_raw, 0, 4095, 0, 100);
  
  // Buttons mit Debounce lesen
  bool buttons[7];
  buttons[0] = !readDebounced(SW1);
  buttons[1] = !readDebounced(SW2);
  buttons[2] = !readDebounced(SW3);
  buttons[3] = !readDebounced(SW5);
  buttons[4] = !readDebounced(SW8);
  buttons[5] = !readDebounced(SW9);
  buttons[6] = !readDebounced(JOY_SEL);
  
  // Buttons in Bitfield(0=SW1, 6=JOY_SEL)
  rc_data.buttons = 0;
  for(int i=0; i<7; i++) {
    if(buttons[i]) rc_data.buttons |= (1 << i);
  }
  
  // Checksum
  rc_data.checksum = rc_data.throttle + rc_data.steering + rc_data.buttons;
}

bool readDebounced(int pin) {
  bool reading = !digitalRead(pin);
  static bool lastReading[40] = {1};  // Array für alle GPIOs
  static unsigned long lastDebounceTime[40] = {0};
  int index = pin % 40;
  
  if (reading != lastReading[index]) {
    lastDebounceTime[index] = millis();
  }
  
  if ((millis() - lastDebounceTime[index]) > 25) {
    return reading;
  }
  
  lastReading[index] = reading;
  return lastReading[index];
}

void showBootScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
   display.println("RC MACRO v1.0");
  display.println("Hack Club Blueprint");
  display.printf("Pins: H%d V%d\n", JOY_H, JOY_V);
  display.println("Joystick + 6 Buttons");
  display.display();
  delay(2000);
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.printf("T:%3d S:%3d\n", rc_data.throttle, rc_data.steering);
  display.printf("B:0x%02X\n", rc_data.buttons);
   display.printf("CS:%3d\n", rc_data.checksum);
  
  // Button Status anzeigen
  display.setCursor(0, 32);
   if(rc_data.buttons & 0x01) display.print("1 "); else display.print("_ ");
  if(rc_data.buttons & 0x02) display.print("2 "); else display.print("_ ");
  if(rc_data.buttons & 0x04) display.print("3 "); else display.print("_ ");
  if(rc_data.buttons & 0x08) display.print("5 "); else display.print("_ ");
  if(rc_data.buttons & 0x10) display.print("8 "); else display.print("_ ");
  if(rc_data.buttons & 0x20) display.print("9 "); else display.print("_ ");
  if(rc_data.buttons & 0x40) display.print("SEL"); else display.print("___");
  
   display.display();
}

void printDebug() {
  static unsigned long lastPrint = 0;
  if(millis() - lastPrint > 200) {  // 5Hz Debug
    Serial.printf("T:%3d S:%3d B:0x%02X CS:%3d\n", 
       rc_data.throttle, rc_data.steering, rc_data.buttons, rc_data.checksum);
     lastPrint = millis();
   }
} 
