#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 5
#define LED_PIN 2  // LED connected to GPIO2

RF24 radio(CE_PIN, CSN_PIN);
const byte esp32Address[6] = "ESP32";  // Address for ESP32 transmitter
bool scanningForUnknown = false;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
    digitalWrite(LED_PIN, LOW); // Keep LED OFF initially

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);  // Use low power mode for stability
    radio.openReadingPipe(0, esp32Address);
    radio.startListening();
    Serial.println("ESP32 Receiver Ready...");
}

void loop() {
    char receivedText[32] = "";
    digitalWrite(LED_PIN, LOW);  // Turn off LED by default

    if (!scanningForUnknown) {
        Serial.println("📡 Listening for ESP32 Transmitter...");
        delay(1000);

        if (radio.available()) {
            radio.read(&receivedText, sizeof(receivedText));

            if (strncmp(receivedText, "ESP32_ID_1234:", 14) == 0) {
                Serial.print("✅ ESP32 Message Received: ");
                Serial.println(receivedText + 14);
            } else {
                Serial.print("❌ Unknown Signal on Fixed Address: ");
                Serial.println(receivedText);
            }
        }
    } else {
        Serial.println("🔍 Scanning for Unknown RF Signals...");
        int detectedChannel = -1;

        for (int channel = 0; channel <= 125; channel++) {
            radio.setChannel(channel);
            delay(10);

            if (radio.testCarrier()) {  // Detect if a signal is active
                detectedChannel = channel;
                Serial.print("⚠️ Unknown RF Signal Detected on Channel: ");
                Serial.println(detectedChannel);

                // Blink LED 3 times when unknown RF signal is detected
                for (int i = 0; i < 3; i++) {
                    digitalWrite(LED_PIN, HIGH);
                    delay(300);
                    digitalWrite(LED_PIN, LOW);
                    delay(300);
                }
                break;
            }
        }

        if (detectedChannel == -1) {
            Serial.println("❌ No Unknown RF Signals Detected.");
        }
    }

    scanningForUnknown = !scanningForUnknown;
    delay(1000);
}
