#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* WIFI_SSID = "Error";  
const char* WIFI_PASSWORD = "wifinotfound_";

const char* MQTT_SERVER = "34.60.49.123";
const int MQTT_PORT = 1883;  
const char* MQTT_TOPIC_TELEMETRY = "sensor_data";

WiFiClient espClient;
PubSubClient client(espClient);

// Define constants and pins for Soil Moisture Sensor
int MinMoistureValue = 4095;
int MaxMoistureValue = 1500;
int MinMoisture = 0;
int MaxMoisture = 100;
int Moisture = 0;

const int moisturePin = 34;
const int SOIL_MOISTURE_THRESHOLD = 30; // Threshold for low soil moisture

// Define constants and pins for Water Level Sensor (Digital)
const int depthPin = 39; // Pin connected to the digital output of the water level sensor

// Define constants and pins for Rain Sensor (Analog)
int MinRainValue = 4095; 
int MaxRainValue = 0;
int MinRain = 0;
int MaxRain = 100;
int rainLevel = 0;

const int rainPin = 35;
const int RAIN_THRESHOLD = 40; // Threshold for rain detection

// Define constants and pins for Relay
const int relayPin = 32;

// Define constants and pins for Servo
const int servoPin = 16;
Servo servo;

const int minPulse = 500; 
const int maxPulse = 2500; 
bool pumpOn = false;

// Last message time
unsigned long lastMsgTime = 0;
const unsigned long LOOP_INTERVAL = 10000; // Adjust the interval as needed

// Function to control the relay (water pump)
void controlPump(bool turnOn) {
    pumpOn = turnOn;
    if (turnOn) {
        Serial.println("Relay ON: Activating Water Pump");
        digitalWrite(relayPin, HIGH);
    } else {
        Serial.println("Relay OFF: Deactivating Water Pump");
        digitalWrite(relayPin, LOW);
    }
}

// Filtered analog reading function
int readFilteredAnalog(int pin, int samples = 10) {
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(pin);
        delay(10);
    }
    return sum / samples;
}

// Publish combined telemetry data
void publishCombinedTelemetry(const char* topic, int moisture, int waterLevel, int rain, bool relayStatus) {
    char payload[256]; // Adjust buffer size if necessary
    snprintf(payload, sizeof(payload),
             "{\"soil_moisture\": %d, \"water_level\": %d, \"rain_level\": %d, \"relay_status\": %s}",
             moisture, waterLevel, rain, relayStatus ? "true" : "false");
    client.publish(topic, payload);
    Serial.print("Published: ");
    Serial.println(payload);
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client")) {
            Serial.println("Connected to MQTT server");
            
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Setup function
void setup() {
    Serial.begin(9600);
    setup_wifi();
    
    client.setServer(MQTT_SERVER, MQTT_PORT);

    pinMode(depthPin, INPUT);
    pinMode(rainPin, INPUT);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); // Ensure pump is off at startup
    
    servo.attach(servoPin, minPulse, maxPulse);
    servo.write(90); // Set initial position
}

// Loop function
void loop() {
    if(!client.connected()) {
      reconnect();
    }
    client.loop();

    unsigned long cur = millis();
    if (cur - lastMsgTime > LOOP_INTERVAL) {
        lastMsgTime = cur;

        // Soil Moisture Sensor
        int soilValue = readFilteredAnalog(moisturePin);
        Moisture = map(soilValue, MinMoistureValue, MaxMoistureValue, MinMoisture, MaxMoisture);

        // Water Level Sensor (Digital)
        int waterDetected = digitalRead(depthPin);

        // Rain Sensor (Analog)
        int rainValue = readFilteredAnalog(rainPin);
        rainLevel = map(rainValue, MinRainValue, MaxRainValue, MinRain, MaxRain);

        // Control Logic for Water Pump
        if (Moisture < SOIL_MOISTURE_THRESHOLD && rainLevel < RAIN_THRESHOLD && waterDetected == HIGH) {
            controlPump(true); // Activate water pump
        } else {
            controlPump(false); // Deactivate water pump
        }

        // Publish all sensor data and relay status together
        publishCombinedTelemetry(MQTT_TOPIC_TELEMETRY, Moisture, waterDetected, rainLevel, pumpOn);
    }

    // Servo Oscillation Logic moves left-right continuously when pump is on
    if (pumpOn) {
        static unsigned long lastServoMove = 0;
        static bool servoToggle = false;
        
        if (millis() - lastServoMove > 1000) { // Change direction every 1s
            servoToggle = !servoToggle;
            servo.write(servoToggle ? 45 : 135); // Moves between 45° and 135°
            lastServoMove = millis();
        }
    }
}