#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "A";   // <-- Check this matches exactly
const char* password = "12345678";      // <-- Check carefully
//const char* ssid = "CAREN 6004";
//const char* password = "654321caren";
WebServer server(80);

void handleReceive() {
  Serial.println("📩 Signal to Arduino");
  Serial2.write('1');   // Send a simple command over TX to Arduino
  server.send(200, "text/plain", "Signal sent to Arduino");
}

void setup() {
  Serial.begin(115200);    // Debugging

   Serial2.begin(9600, SERIAL_8N1, 17, 5); // Serial2 on GPIO17 (RX), GPIO5 (TX)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  unsigned long startAttemptTime = millis();

  // Try for 15 seconds before fallback
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    delay(500);
    Serial.print(".");
    Serial.print(" Status: ");
    Serial.println(WiFi.status());
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected");
    Serial.print("ESP IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/receive", handleReceive);
    server.begin();
  } else {
    Serial.println("\n❌ Failed to connect to WiFi. Running fallback scan...");

    // Fallback: scan nearby networks
    WiFi.disconnect();
    delay(1000);
    int n = WiFi.scanNetworks();
    if (n == 0) {
      Serial.println("⚠️ No networks found!");
    } else {
      Serial.println("📡 Networks found:");
      for (int i = 0; i < n; ++i) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm)");
      }
    }
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }
}
