#include <WiFi.h>
#include "time.h"
#include <PxMatrix.h>
// ESP32 Pins for LED MATRIX
#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15  // NOT USED for 1/16 scan
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

uint8_t display_draw_time=0;

PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);


const char* ssid       = "my ssid";
const char* password   = "my password";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * -6;
const int   daylightOffset_sec = 3600;


int months[]= {31, 28, 31, 30, 31, 30, 31,
      31, 30, 31, 30, 31};

int loopCounter= 0;


void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void display_update_enable(bool is_enable)
{
if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  }
else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}
void setup() 
{
   WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  display.begin(16); // 1/16 scan
  display.setFastUpdate(true);
  display_update_enable(true);
  display.clearDisplay();
  display.setTextColor(myWHITE);
  display.setCursor(5,7);
  display.print("HackerBox");
}

void loop()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    display.clearDisplay();
    display.setTextColor(myWHITE);
    display.setCursor(5,7);
    display.print("fail");
    return;
  }  
  display.clearDisplay();
  int daysOfMonthsPast = 0;
  for (int i = 0; i < timeinfo.tm_mon; i++)
  {
    daysOfMonthsPast = daysOfMonthsPast + months[i];
  }
  if (timeinfo.tm_year % 4 == 0 && timeinfo.tm_mon > 1) daysOfMonthsPast++;
  int daysTill = timeinfo.tm_year % 4 == 0 ? 360 -(timeinfo.tm_mday + daysOfMonthsPast)  : 359 - (timeinfo.tm_mday + daysOfMonthsPast) ;
  if (daysTill < 0) daysTill =  (timeinfo.tm_year + 1)% 4 == 0 ? 366 + daysTill : 365 + daysTill;
  String timeStr = String(daysTill) + " Days";
  if (daysTill == 0)
  {
      display.setCursor(5,7); 
      display.setTextColor(myGREEN);
      display.print("Merry");
      display.setCursor(5,18);
      display.setTextColor(myRED);
      display.print("Christmas");
  }
  else
  {
    display.setTextColor(myWHITE);
    display.setCursor(5,7); 
    display.print(timeStr);
    display.setCursor(5,18); 
    display.print("Till Xmas");
  }
 
  delay(1000 * 60 * 60);
}
