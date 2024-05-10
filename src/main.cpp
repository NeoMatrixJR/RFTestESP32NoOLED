#include <Arduino.h>
#include <WiFi.h>
#include <ESPNowW.h>

// ESPNow Setup
#define PEER_ADDRESS "B4:E6:2D:8E:5D:3D" // Replace with the MAC address of the OLED board

uint32_t last_send_time = 0;

void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

void setup() {
  Serial.begin(115200);
  Serial.printf("Setting up Bouncer\n");
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  // Initialize ESPNow
  WiFi.mode(WIFI_STA); // MUST NOT BE WIFI_MODE_NULL
  WiFi.disconnect();
  ESPNow.init();
  ESPNow.add_peer((uint8_t *)PEER_ADDRESS);
  ESPNow.reg_recv_cb(onRecv);
  Serial.println("Setup Complete - Listening...");
}

void loop() {
  // Send packet every 5 seconds
  static uint32_t now = millis();
  if (now - last_send_time >= 5000) {
    last_send_time = now;
    uint8_t packet[] = "Hello!";
    Serial.println("Sending data...");
    int status = ESPNow.send_message((uint8_t *)PEER_ADDRESS, packet, sizeof(packet));
    Serial.print("Send status: ");
    Serial.println(status);
    Serial.println("Data sent!");
  }
}

void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.println("Received data!");
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
           mac_addr[5]);
  Serial.print("Last Packet Recv from: ");
  Serial.println(macStr);
  Serial.print("Last Packet Recv Data: ");
  // if it could be a string, print as one
  if (data[data_len - 1] == 0)
    Serial.printf("%s\n", data);
  // additionally print as hex
  for (int i = 0; i < data_len; i++) {
    Serial.printf("%x ", data[i]);
  }
  Serial.println("");
}