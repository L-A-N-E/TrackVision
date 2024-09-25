// Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <RTClib.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PIN_RECEIVER 15  // Pin for the IR receiver
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

IRrecv receiver(PIN_RECEIVER); // Initialize the IR receiver
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);  // Initialize the LCD I2C
RTC_DS1307 rtc;  // Initialize the RTC

// Variables to store the time of presses
DateTime firstPressTime;
DateTime secondPressTime;
bool firstPress = false;
bool timeMeasured = false;
bool isLastLap = false;
int lapCount = 0;

unsigned long totalTime = 0;
unsigned long startMillis;
unsigned long currentMillis;
unsigned long elapsedMillis;

// Editable configuration variables
const char* default_SSID = "YOUR_WIFI_NETWORK"; // Name of the Wi-Fi network
const char* default_PASSWORD = "YOUR_WIFI_PASSWORD"; // Password of the Wi-Fi network
const char* default_BROKER_MQTT = "PUBLIC_IP"; // IP of the MQTT Broker
const int default_BROKER_PORT = 1883; // Port of the MQTT Broker
const char* default_TOPIC_SUBSCRIBE = "/TEF/TRV027/cmd"; // MQTT topic for subscription
const char* default_TOPIC_PUBLISH_1 = "/TEF/TRV027/attrs/t"; // MQTT topic for publishing
const char* default_TOPIC_PUBLISH_2 = "/TEF/TRV027/attrs/l"; // MQTT topic for publishing
const char* default_ID_MQTT = "fiware_027"; // MQTT ID
const int default_D4 = 2; // Pin for the onboard LED
// Declaration of the topic prefix variable
const char* topicPrefix = "lamp027";

// Variables for editable configurations
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPIC_SUBSCRIBE = const_cast<char*>(default_TOPIC_SUBSCRIBE);
char* TOPIC_PUBLISH_1 = const_cast<char*>(default_TOPIC_PUBLISH_1);
char* TOPIC_PUBLISH_2 = const_cast<char*>(default_TOPIC_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

WiFiClient espClient; // Initialize Wi-Fi
PubSubClient MQTT(espClient); // Initialize MQTT

// Initialize Wi-Fi
void initWiFi() {
    delay(10);
    Serial.println("------WI-FI CONNECTION------");
    Serial.print("Connecting to network: ");
    Serial.println(SSID);
    Serial.println("Please wait");
    reconectWiFi();
}

// Initialize MQTT connection
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

// Check if Wi-Fi is connected, if not try again
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to network ");
    Serial.print(SSID);
    Serial.println("Obtained IP: ");
    Serial.println(WiFi.localIP());

    // Ensure the onboard LED is off
    digitalWrite(D4, LOW);
}

// Check if MQTT and Wi-Fi are connected
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

// Check if MQTT is connected, if not try again
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Trying to connect to MQTT Broker: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Connected to MQTT broker!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        } else {
            Serial.println("Failed to reconnect to broker.");
            Serial.println("Will try again in 2s");
            delay(2000);
        }
    }
}

// Send the exact time and lap time to MQTT, formatted
void handleLapTime(unsigned long lapTimeMillis, DateTime exactLapTime) {
    if (timeMeasured) {
        // Format the exact lap time as "hh:mm:ss"
        String exactTimeMessage = String(exactLapTime.hour()) + ":" + String(exactLapTime.minute()) + ":" + String(exactLapTime.second());

        Serial.print("Sending exact lap time: ");
        Serial.println(exactTimeMessage.c_str());

        Serial.print("Lap time in ms: ");
        Serial.println(lapTimeMillis);

        // Send the exact lap time (hh:mm:ss) to MQTT
        MQTT.publish(TOPIC_PUBLISH_1, exactTimeMessage.c_str());

        // Convert lapTimeMillis to string before sending via MQTT
        String lapTimeArray = "[" + String(lapCount) + "," + (lapTimeMillis) + "]";
        MQTT.publish(TOPIC_PUBLISH_2, lapTimeArray.c_str());

        // Reset the time measurement state
        timeMeasured = false;
    }
}

void setup() {
    Serial.begin(115200); // Initialize serial communication
    clearSerialMonitor();
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the LCD backlight

    displayMenu();

    receiver.enableIRIn(); // Enable the IR receiver

    initWiFi();
    initMQTT();
    delay(5000);

    if (!rtc.begin()) {
        Serial.println("RTC not found");
        while (1);
    }
}

void loop() {
    if (receiver.decode()) { // Check if there is a decoded IR signal
        VerificaConexoesWiFIEMQTT();
        translateIR(); // Process the IR command
        receiver.resume(); // Prepare the IR receiver for the next reading
        MQTT.loop(); // Keep the MQTT connection active
    }
}

// Translate the received IR command
void translateIR() { 
    switch (receiver.decodedIRData.command) { // Check the received IR command
        case 162: // If the command is 162
            handleTimeMeasurement(); // Call the time measurement function
            break;
        case 226: // If the command is 226
            resetRace(); // Call the function to reset the race
            break;
        case 104: // If the command is 104
            isLastLap = true;
            break;
        default:
        lcd.clear();
        lcd.print("Press POWER!");
        displayMenu();
    }
}

// Measure the time
void handleTimeMeasurement() {
if (!firstPress) {
    // Initialize the first time measurement
    firstPressTime = rtc.now();
    firstPress = true;
    lapCount = 0;
    totalTime = 0;

    // Countdown
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("3");
    delay(1000);

    lcd.setCursor(7, 0);
    lcd.print("2");
    delay(1000);

    lcd.setCursor(9, 0);
    lcd.print("1");
    delay(1000);

    lcd.setCursor(5, 1);
    lcd.print("START");
    delay(50);

    // Capture the time in milliseconds
    startMillis = millis();  
    lcd.clear();
    lcd.setCursor(3, 0);
    // Display the start time on the LCD
    lcd.print("START:");
    lcd.setCursor(0, 3);
    lcd.print(firstPressTime.hour());
    lcd.print(":");
    lcd.print(firstPressTime.minute());
    lcd.print(":");
    lcd.println(firstPressTime.second());

    // Display the start time on the serial monitor
    Serial.print("Start time: ");
    Serial.print(firstPressTime.hour());
    Serial.print(":");
    Serial.print(firstPressTime.minute());
    Serial.print(":");
    Serial.println(firstPressTime.second());

    delay(2000);
    timeMeasured = false; // Time has not been measured

} else {
    // Capture the exact lap time
    secondPressTime = rtc.now(); 
    // Capture the current time in milliseconds
    currentMillis = millis();     
    lapCount++;

    // Calculate the lap time in milliseconds
    unsigned long lapTimeMillis = currentMillis - startMillis;  

    // Display the lap time in ms on the serial monitor
    Serial.print("Lap ");
    Serial.print(lapCount);
    Serial.print(": ");
    Serial.print(lapTimeMillis);  
    Serial.println("ms");

    // Display the lap time in ms on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lap ");
    lcd.print(lapCount);

    lcd.setCursor(0, 1);
    lcd.print(lapTimeMillis);
    lcd.print("ms");

    // Accumulate the total race time
    totalTime += lapTimeMillis;

    // Time has been measured
    timeMeasured = true;

    // If it's the last lap
    if (isLastLap) {
        handleLapTime(lapTimeMillis, secondPressTime); // Send the time to MQTT
        displayEndMessage(); // Display the total race time, formatted
    } else {
        firstPressTime = secondPressTime;
        startMillis = currentMillis;  // Update for the next lap
    }

    // Call the handleLapTime function, passing the time in milliseconds and the exact time (RTC)
    handleLapTime(lapTimeMillis, secondPressTime);
}
}

// Display the menu on the LCD
void displayMenu() {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("TrackVision");
    delay(300);
}

// Reset the race, resetting all variables
void resetRace() {
    firstPress = false;
    timeMeasured = false;
    lapCount = 0;
    totalTime = 0;
    isLastLap = false;

    // Return to the menu on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Returning to Menu");
    delay(2000);

    displayMenu();
}

// Display the total race time on the LCD, formatted, and on the serial monitor
void displayEndMessage() {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("END");

    int hours, totalMinutes, totalSeconds, totalMilliseconds;
    convertToHMS(totalTime, &hours, &totalMinutes, &totalSeconds, &totalMilliseconds);

    lcd.setCursor(3, 1);
    lcd.print(hours);
    lcd.print("h ");
    lcd.print(totalMinutes);
    lcd.print("m ");
    lcd.print(totalSeconds);
    lcd.print("s ");
    lcd.print(totalMilliseconds);
    lcd.print("ms");

    Serial.print("END OF RACE:");
    Serial.print(hours);
    Serial.print("h ");
    Serial.print(totalMinutes);
    Serial.print("m ");
    Serial.print(totalSeconds);
    Serial.print("s ");
    Serial.print(totalMilliseconds);
    Serial.print("ms");

    delay(5000);
    lcd.clear();
    resetRace();
}

// Convert the total time in milliseconds to hours, minutes, seconds, and milliseconds
void convertToHMS(unsigned long totalMillis, int *hours, int *minutes, int *seconds, int *milliseconds) {
    *hours = totalMillis / 3600000;       // Convert to hours
    totalMillis %= 3600000;
    *minutes = totalMillis / 60000;       // Convert to minutes
    totalMillis %= 60000;
    *seconds = totalMillis / 1000;        // Convert to seconds
    *milliseconds = totalMillis % 1000;   // Get the remainder as milliseconds
}

// Clear the serial monitor, printing several blank lines
void clearSerialMonitor() {
    // Number of blank lines to print to clear the serial monitor
    int linesToPrint = 20;  // Adjust as needed
    for (int i = 0; i < linesToPrint; i++) {
        Serial.println();
    }
}