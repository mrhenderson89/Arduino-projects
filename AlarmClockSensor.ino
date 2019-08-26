#include <Button.h>
#include <TimeAlarms.h>
#include <Time.h>
#include <DS3231.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


int currentScreen = 0;

Button button1 = Button(8,PULLUP);
Button button2 = Button(9, PULLUP);
Button button3 = Button(10, PULLUP);
Button button4 = Button(11, PULLUP);


//Variables for DS3231 RTC unit.
DS3231  rtc(SDA, SCL);
//Time t;

AlarmId alarm1;
int alarmHour;
int alarmMin;
boolean alarmSet = true;
boolean alarmTriggered = false;

// Photocell variable connected to Analog 3
byte photocellPin = A3;

//Variables for OLED screen
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {      

  Serial.begin(9600);
           
  // Initialize the rtc object
  rtc.begin();
  //rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(22, 34, 30);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(04, 04, 2016);   // Set the date to DD/MM/YYYY

  //setTime(rtc.getTime());
  alarmHour = 20;
  alarmMin = 00;

  alarm1 = Alarm.alarmRepeat(alarmHour,alarmMin,0, MorningAlarm);  // Set time every day

    // Send Current time
  Serial.print("Current Time.............................: ");
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  Serial.println(rtc.getTimeStr());

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
}
void loop() {
 
      // Read humidity (percent)
  //float h = dht.readHumidity();
  //String stringHumidity = String(h, 1);
  //String humidity = String("H: " + stringHumidity); 
  // Read temperature as Celsius
  float t = rtc.getTemp();
    // Read temperature as Fahrenheit
  //float f = dht.readTemperature(true);
  String stringTemp = String(t, 2);
  String temp = ("T: " + stringTemp);
  String stringTime = String(rtc.getTimeStr());

  // Read photocell
  int p = analogRead(photocellPin);
  // Testing revealed this value never goes below 50 or above 1000,
  //  so we're constraining it to that range and then mapping that range
  //  to 0-100 so it's like a percentage
  p = constrain(p, 50, 1000);
  p = map(p, 50, 1000, 0, 100);
  String light = ("L: " + p);

//printSerial(stringTemp, light);

  if(button1.isPressed()){
        if(currentScreen < 4)
        {
          currentScreen++;
        }
        else
        {
          currentScreen = 0;
        }
           delay(50);
}
  if(button2.isPressed()){
        if(currentScreen == 1)
        {
          alarmHour++;
        }
           delay(50);
}
  if(button3.isPressed()){
        if(currentScreen == 1)
        {
          alarmHour++;
        }
           delay(50);
}
  delay(1);
  Alarm.delay(1000);
  if(currentScreen == 0)
  {
    printScreenCurrent(stringTime, temp, light);
  }
  else if(currentScreen == 1)
  {
    printScreenAlarm();
  }
  else if(currentScreen == 2)
  {
    printScreenExtra();
  }
    else if(currentScreen == 3)
  {
    printScreenExtra2();
  }
}

void printSerial(String temp, String light)
{
  Serial.print("Sensor readings: \n");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("Light Reading: ");
  Serial.print(light);
  Serial.print(" % \n");
  delay(1000);
}

void printScreenCurrent(String stringTime, String temp, String light)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println(stringTime);
  display.setCursor(10, 25);
  display.println(temp);
  display.setCursor(10, 50);
  display.println(light);
  display.display();
}
void printScreenAlarm(void)
{
  String alarmHourString = String(alarmHour, 2);
  String alarmMinString = String(alarmMin,2);
  String alarmTimeString = (alarmHourString + ":" + alarmMinString);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("Alarm: ");
  //display.setCursor(10, 40);
  display.println(alarmTimeString);
  display.display();
}
void printScreenExtra(void)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("EXTRA SCREEN 1");
  display.display();
}
void printScreenExtra2(void)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("EXTRA SCREEN 2");
  display.display();
}
void testscrolltext(void) {


}
// functions to be called when an alarm triggers:
void MorningAlarm(){
alarmTriggered = true;
Serial.println("Alarm: - turn lights off");
   display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("ALARM RINGING");
  display.setCursor(10, 40);
  //display.println(alarmTime);
  display.display(); 
}
