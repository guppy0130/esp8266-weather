#include <ESP8266WiFi.h>
#include <GxEPD.h>
#include <WiFiClientSecure.h>
#include <time.h>

// The 1.54" b/w/r waveshare
#include <GxGDEW0154Z04/GxGDEW0154Z04.cpp>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include <ArduinoJson.h>

GxIO_Class io(SPI, 15, 0, 2);
GxEPD_Class display(io, -1, 4);
#define HAS_RED_COLOR

const GFXfont* FreeMonoBold9 = &FreeMonoBold9pt7b;

void setup() {
  Serial.begin(115200);
  display.init();
  WiFi.mode(WIFI_STA);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(OFFSET * 3600, 1, "pool.ntp.org", "time.nist.gov"); // replace OFFSET with your timezone offset from GMT
  while (!time(nullptr)) {
    delay(1000);
  }

  const GFXfont* FreeMonoBold9 = &FreeMonoBold9pt7b;
  
  clearScreen();
  display.println(WiFi.localIP());
  weather();  
  display.update();
//  ESP.deepSleep(36e8);
}

void clearScreen() {
  display.fillScreen(GxEPD_WHITE);
  display.setRotation(0); // 0 is text towards ribbon, 45 is rotated left
  display.setCursor(0, 0);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(FreeMonoBold9);
  display.println();
}

void weather() {
  WiFiClientSecure client;
  char* host = "api.darksky.net";
  char* url = "/forecast/DARKSKY-API-KEY/LAT,LON?exclude=minutely,hourly,daily,alerts,flags";
  const char* fingerprint = "c0cdf85cdbf3520daf7975ea612a958a4907dc33";
 
  if (client.connect(host, httpsPort)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") == 0) {
      char endOfHeaders[] = "\r\n\r\n";
      if (client.find(endOfHeaders)) {
        const size_t capacity = JSON_OBJECT_SIZE(5) + JSON_ARRAY_SIZE(19) + 470;
        DynamicJsonBuffer jsonBuffer(capacity);
        JsonObject& root = jsonBuffer.parseObject(client);
        if (root.success()) {
          JsonObject& currently = root["currently"];

          // Header
          display.println("<< WEATHER >>");
          
          // time
          int offset = root["offset"];
          
          time_t now = time(nullptr);
          char buff[35];
          strftime(buff, sizeof buff, "%D %R", localtime(&now));
          display.println(buff);

          // summary + temp
          const char* currently_summary = currently["summary"];
          display.print(currently_summary);
          display.print(", ");
          float temperature = currently["temperature"];
          if (temperature > 85.0) {
            display.setTextColor(GxEPD_RED);
            display.print(temperature);
            display.setTextColor(GxEPD_BLACK);
          } else {
            display.print(temperature);
          }
          display.println("F");

          // precipitation
          float precipProbability = currently["precipProbability"];
          display.print("Precip prob: ");
          if (precipProbability > 0.3) {
            display.setTextColor(GxEPD_RED);
            display.println(precipProbability);
            display.setTextColor(GxEPD_BLACK);
          } else {
            display.println(precipProbability);
          }

          // uv index
          int uv = currently["uvIndex"];
          display.print("UV: ");
          if (uv > 6) {
            display.setTextColor(GxEPD_RED);
            display.println(uv);
            display.setTextColor(GxEPD_BLACK);
          } else {
            display.println(uv);
          }

          // wind
          double windSpeed = currently["windSpeed"];
          display.print("Wind: ");
          if (windSpeed > 10.0) {
            display.setTextColor(GxEPD_RED);
            display.println(windSpeed);
            display.setTextColor(GxEPD_BLACK);
          } else {
            display.println(windSpeed);
          }
        }
      }
    }
    client.stop();
  }
}

int loopCounter = 0;
void loop() {
  loopCounter++;
  if (loopCounter == 300) { // every 300 seconds
    clearScreen();
    display.println(WiFi.localIP());
    weather();
    display.update();
    loopCounter = 0;
  }
  delay(1000);
}
