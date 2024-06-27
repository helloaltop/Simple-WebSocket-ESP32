#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "index.h"  // Mengimpor file index.h

// Ganti dengan SSID dan Password WiFi Anda
const char* ssid = "alitopan";
const char* password = "AliTopan2024";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup(){
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  // Menampilkan alamat IP
  Serial.print("Alamat IP ESP32-S3: ");
  Serial.println(WiFi.localIP());

  // Setup WebSocket
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // Rute untuk menyajikan file index.html dari string
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Memulai server
  server.begin();
  Serial.println("Server dimulai");
}

void loop(){
  // Mengirim data float setiap 5 detik
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 5000) {
    lastTime = currentTime;
    float data = random(0, 100) / 1.0; // Data float acak
    ws.textAll(String(data));
    Serial.println("Data terkirim: " + String(data));
  }
}

// Fungsi untuk menangani event WebSocket
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    Serial.printf("Klien %u tersambung\n", client->id());
  } else if(type == WS_EVT_DISCONNECT){
    Serial.printf("Klien %u terputus\n", client->id());
  } else if(type == WS_EVT_DATA){
    String msg = "";
    for(size_t i=0; i<len; i++){
      msg += (char) data[i];
    }
    Serial.printf("Pesan dari klien %u: %s\n", client->id(), msg.c_str());
    client->text("Pesan diterima");
  }
}
