#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <time.h>

// Cấu hình WiFi và MQTT
const char* ssid = "LAPTOP-LC";
const char* password = "10062003";
const char* mqtt_server = "172.20.10.14";

// Cấu hình cảm biến DHT
#define DPIN 33        
#define DTYPE DHT11   
DHT dht(DPIN, DTYPE);   

// Cấu hình cảm biến
#define LIGHT_SENSOR_PIN 32
#define WIND_SENSOR_PIN 32   // Cảm biến gió (giả lập)
#define SOIL_MOISTURE_PIN 34 // Cảm biến độ ẩm đất
#define RELAY_PIN 13
// Cấu hình chân LED cho ESP32
#define LED_PIN_1 25  
#define LED_PIN_2 26  
#define LED_PIN_3 27  

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (150)
char msg[MSG_BUFFER_SIZE];

// Cấu hình NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

String getFormattedTime() {
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);  
  char buffer[30];
  strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

void setup_wifi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) message += (char)payload[i];
  
  if (message == "LED1_ON") digitalWrite(LED_PIN_1, HIGH);
  else if (message == "LED1_OFF") digitalWrite(LED_PIN_1, LOW);
  else if (message == "LED2_ON") digitalWrite(LED_PIN_2, HIGH);
  else if (message == "LED2_OFF") digitalWrite(LED_PIN_2, LOW);
  else if (message == "LED3_ON") digitalWrite(LED_PIN_3, HIGH);
  else if (message == "LED3_OFF") digitalWrite(LED_PIN_3, LOW);
  else if (message == "ON_ALL") {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, HIGH);
    digitalWrite(LED_PIN_3, HIGH);
  } else if (message == "OFF_ALL") {
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), "huy", "123")) {
      Serial.println("connected");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  
  setup_wifi();
  client.setServer(mqtt_server, 1006);
  client.setCallback(callback);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 10000) { 
    lastMsg = now;

    // Đọc cảm biến
    float tc = dht.readTemperature(false);
    float hu = dht.readHumidity();
    int lightValue = analogRead(LIGHT_SENSOR_PIN);
    int windSpeed = random(0, 100);
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN); // Đọc độ ẩm đất

    // Chuyển đổi độ ẩm đất thành % (tương đối)
    int soilMoisturePercent = map(soilMoisture, 4095, 0, 0, 100);

    if (isnan(tc) || isnan(hu)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Lấy thời gian thực từ NTP
    String currentTime = getFormattedTime();

    // Gửi dữ liệu lên MQTT
    snprintf(msg, MSG_BUFFER_SIZE, 
             "Time: %s, Temp: %.2f C, Hum: %.0f%%, Light: %d Lux, Wind: %d km/h, Soil Moisture: %d%%", 
             currentTime.c_str(), tc, hu, lightValue, windSpeed, soilMoisturePercent);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);

    // Kiểm tra nếu đất khô, bật cảnh báo (LED 1)
    if (soilMoisturePercent < 35) {
      digitalWrite(LED_PIN_1, HIGH); 
    } else {
      digitalWrite(LED_PIN_1, LOW);
    }
  }
}