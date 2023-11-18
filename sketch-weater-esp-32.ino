// Author: Fernando Leon
// GitHub: https://github.com/Fernando-Leon

#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 4     
#define DHTTYPE DHT11   

// Reemplaza con los datos de tu red WiFi
const char* ssid = "your ssid";
const char* password = "your password";

DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(100);

  dht.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conecting to WiFi...");
  }
    Serial.println("Ip address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Sensor de temperatura</title><style>body {margin: 0;display: flex;flex-direction: column;align-items: center;justify-content: center;height: 100vh;background-color: #f0f0f0;font-family: Arial, sans-serif;}.main {display: flex;justify-content: space-around;width: 300px;background-color: white;padding: 20px;border-radius: 10px;box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);}.item {text-align: center;}</style></head><body><h1>Estación meteorológica con ESP-32 y el sensor DHT11</h1><div class='main'><div class='item'>";
    html += "<p>Temperatura: <span id='temp'> " + String(t) + "</span> °C <span id='ic-temp'></span></p> </div><div class='item'>";
    html += "<p>Humedad: <span id='hume'> " + String(h)+ "</span>% <span id='ic-hume'></span></p>";
    html += "</div></div><script> const temperatura = parseFloat(document.getElementById('temp').textContent); const humedad = parseFloat(document.getElementById('hume').textContent); const iconTemp = document.getElementById('ic-temp'); const iconHume = document.getElementById('ic-hume'); if (temperatura > 25) { iconTemp.innerHTML = '☀️'; } else { iconTemp.innerHTML = '❄️'; } if (humedad > 70) { iconHume.innerHTML = '💧'; } else { iconHume.innerHTML = '😊'; } </script></body></html>";


    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println(html);
  }
}
