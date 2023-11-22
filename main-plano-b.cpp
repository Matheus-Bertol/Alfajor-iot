#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

const char* qrcodeScript = "https://cdnjs.cloudflare.com/ajax/libs/jquery.qrcode/1.0/jquery.qrcode.min.js";
const char* ssid = "Helen";
const char* senha = "1112131415";
ESP8266WebServer server(80);
const int saída5 = 5; // Endereço IP D1
IPAddress local_IP(192, 168, 1, 115); // Troque o IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

bool qrcodeRead = false;
uint32_t tempoInicio = 0;

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"styles.css\">";
  html += "<script type=\"text/javascript\" src=\"" + String(qrcodeScript) + "\"></script>";
  html += "<style>";
  html += "body { font-family: Helvetica, sans-serif; margin: 0; padding: 0; background-color: #f4f4f4; color: #333; }";
  html += ".container { max-width: 600px; margin: 0 auto; padding: 20px; }";
  html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #77878A; }";
  html += "@media screen and (max-width: 600px) {";
  html += "  .button { padding: 12px 24px; font-size: 20px; }";
  html += "}";
  html += "</style></head>";
  html += "<body>";
  html += "<div class=\"container\">";
  html += "<h1>ESP8266 Web Server</h1>";
  html += "<h2>Alfajor Iot</h2>";

  if (qrcodeRead) {
    if (millis() - tempoInicio < 3600000) {
      html += "<button onclick=\"toggleLED('on')\" class=\"button\">O</button>";
    } else {
      html += "<p>Tempo decorrido. Faça a leitura do QRCode novamente para ativar as tomadas.</p>";
    }
  } else {
    html += "<p>Por favor, faça a leitura do QRCode para ativar as tomadas.</p>";
  }

  html += "<div id=\"qrcode\"></div>";
  html += "<script>";
  html += "function toggleLED(state) {";
  html += " if (" + String(qrcodeRead) + ") {";
  html += "   var xhr = new XMLHttpRequest();";
  html += "   xhr.open('GET', '/5/' + state, true);";
  html += "   xhr.send();";
  html += " } else {";
  html += "   alert('Faça a leitura do QRCode primeiro.');";
  html += " }";
  html += "}";
  html += "jQuery('#qrcode').qrcode({text: window.location.href, width: 200, height: 200});"; // Gera o QRCode com a URL atual
  html += "</script>";
  html += "</div>"; // Fechar a div .container
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  if (qrcodeRead) {
    digitalWrite(saída5, LOW);
    tempoInicio = millis();
    server.send(200, "text/plain", "LED Ligado!");
  } else {
    server.send(200, "text/plain", "Faça a leitura do QRCode primeiro.");
  }
}

void handleOff() {
  if (qrcodeRead) {
    digitalWrite(saída5, HIGH);
    qrcodeRead = false;
    tempoInicio = 0;
    server.send(200, "text/plain", "LED Desligado!");
  } else {
    server.send(200, "text/plain", "Faça a leitura do QRCode primeiro.");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(saída5, OUTPUT);
  digitalWrite(saída5, HIGH);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/5/on", HTTP_GET, handleOn);
  server.on("/5/off", HTTP_GET, handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
