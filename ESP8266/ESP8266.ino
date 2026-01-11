#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "html_pages.h"
#include "logs.h"

/* ================= METADATA ================= */
#define FW_VERSION "4.0.0-ESP-BRIDGE"

/* ================= PORTAS =================== */
#define HTTP_PORT 80
#define WS_PORT 81

/* ================= SERIAL BRIDGE ============ */
#define SERIAL_BAUDRATE 115200
#define SERIAL_TIMEOUT 5000
#define SERIAL_BUFFER_SIZE 2048

/* ================= SEGURANÇA ================ */
#define MAX_LOGIN_ATTEMPTS 5
#define LOGIN_TIMEOUT 300000

/* ================= WI-FI ==================== */
#define WIFI_RECONNECT_INTERVAL 30000

ESP8266WebServer server(HTTP_PORT);
WebSocketsServer webSocket(WS_PORT);
LogSystem logSystem;

/* ================= ESTADO =================== */
struct State
{
  unsigned long lastWifiCheck = 0;
  bool wifiConnecting = false;
  bool mdnsStarted = false;
  char serialBuffer[SERIAL_BUFFER_SIZE];
  uint16_t serialBufferPos = 0;
} state;

/* ================= CONFIG =================== */
struct Config
{
  char device[32];
  char ap_ssid[32];
  char ap_pass[32];
  uint8_t ap_max_conn;
  bool ap_hidden;
  char sta_ssid[32];
  char sta_pass[32];
  char admin_user[16];
  char admin_pass[16];
  uint16_t update_interval_ms;
  uint16_t cpu_freq_mhz;
  uint16_t pwm_freq;
  bool gpio0;
  bool gpio2;
};

Config cfg;
const char *CFG_FILE = "/cfg.json";

/* ================= AUTH ===================== */
void serveFile(const char *path)
{
  const char *html = nullptr;

  if (strcmp(path, "/index.html") == 0 || strcmp(path, "/") == 0)
    html = HTML_INDEX;
  else if (strcmp(path, "/config.html") == 0)
    html = HTML_CONFIG;
  else if (strcmp(path, "/logs.html") == 0)
    html = HTML_LOGS;

  if (html)
  {
    // Criar String da PROGMEM e substituir placeholders
    String content = String(FPSTR(html));
    content.replace("{{DEVICE_NAME}}", cfg.device);

    server.send(200, "text/html", content);
  }
  else
  {
    server.send(404, "text/plain", "Arquivo não encontrado");
  }
}

/* ================= SERIAL BRIDGE ============ */
String sendToArduino(const char *protocol, const char *method, const char *uri, const char *body)
{
  // Criar comando JSON para enviar ao Arduino
  StaticJsonDocument<1024> cmd;
  cmd["protocol"] = protocol;
  cmd["method"] = method;
  cmd["uri"] = uri;
  if (body && strlen(body) > 0)
  {
    cmd["body"] = body;
  }

  // Enviar para Arduino via Serial
  serializeJson(cmd, Serial);
  Serial.println(); // Terminador de linha
  Serial.flush();

  // Aguardar resposta do Arduino
  unsigned long startTime = millis();
  state.serialBufferPos = 0;

  while (millis() - startTime < SERIAL_TIMEOUT)
  {
    if (Serial.available())
    {
      char c = Serial.read();

      if (c == '\n' || c == '\r')
      {
        if (state.serialBufferPos > 0)
        {
          state.serialBuffer[state.serialBufferPos] = '\0';
          String response = String(state.serialBuffer);
          state.serialBufferPos = 0;
          return response;
        }
      }
      else if (state.serialBufferPos < SERIAL_BUFFER_SIZE - 1)
      {
        state.serialBuffer[state.serialBufferPos++] = c;
      }
    }
    yield();
  }

  // Timeout - retornar erro
  return "{\"error\":\"Arduino timeout\",\"success\":false}";
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.print("{\"ws\":\"disconnect\",\"client\":");
    Serial.print(num);
    Serial.println("}");
    break;

  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.print("{\"ws\":\"connect\",\"client\":");
    Serial.print(num);
    Serial.print(",\"ip\":\"");
    Serial.print(ip.toString());
    Serial.println("\"}");
  }
  break;

  case WStype_TEXT:
  {
    // Recebeu mensagem WebSocket - enviar para Arduino
    String msg = sendToArduino("ws", "message", "", (char *)payload);

    // Enviar resposta de volta via WebSocket
    webSocket.sendTXT(num, msg);
  }
  break;

  case WStype_BIN:
    // Dados binários - enviar para Arduino
    Serial.print("{\"ws\":\"binary\",\"length\":");
    Serial.print(length);
    Serial.println("}");
    break;
  }
}

void processSerialFromArduino()
{
  while (Serial.available())
  {
    char c = Serial.read();

    if (c == '\n' || c == '\r')
    {
      if (state.serialBufferPos > 0)
      {
        state.serialBuffer[state.serialBufferPos] = '\0';

        // Processar mensagem do Arduino
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, state.serialBuffer);

        if (!error)
        {
          // Se for uma mensagem broadcast, enviar para todos os clientes WebSocket
          if (doc.containsKey("broadcast"))
          {
            webSocket.broadcastTXT(state.serialBuffer);
          }
        }

        state.serialBufferPos = 0;
      }
    }
    else if (state.serialBufferPos < SERIAL_BUFFER_SIZE - 1)
    {
      state.serialBuffer[state.serialBufferPos++] = c;
    }
  }
}

/* ================= LOGS (tempo real) ====== */
void handleLogsGet()
{
  String json = logSystem.getLogsJSON();
  server.send(200, "application/json", json);
}

/* ================= CONFIG =================== */
void loadCfg()
{
  if (!LittleFS.exists(CFG_FILE))
  {
    strcpy(cfg.device, "esp-gateway");
    strcpy(cfg.ap_ssid, "ESP8266-SETUP");
    strcpy(cfg.ap_pass, "12345678");
    cfg.ap_max_conn = 4;
    cfg.ap_hidden = false;
    cfg.sta_ssid[0] = 0;
    cfg.sta_pass[0] = 0;
    strcpy(cfg.admin_user, "admin");
    strcpy(cfg.admin_pass, "admin123");
    cfg.update_interval_ms = 2000;
    cfg.cpu_freq_mhz = ESP.getCpuFreqMHz();
    cfg.pwm_freq = 1000;
    cfg.gpio0 = false;
    cfg.gpio2 = false;
    return;
  }

  File f = LittleFS.open(CFG_FILE, "r");
  StaticJsonDocument<512> d;
  if (deserializeJson(d, f) != DeserializationError::Ok)
  {
    f.close();
    return;
  }
  f.close();

  strlcpy(cfg.device, d["device"] | "esp-gateway", sizeof(cfg.device));
  strlcpy(cfg.ap_ssid, d["ap_ssid"] | "ESP8266-SETUP", sizeof(cfg.ap_ssid));
  strlcpy(cfg.ap_pass, d["ap_pass"] | "12345678", sizeof(cfg.ap_pass));
  cfg.ap_max_conn = d["ap_max_conn"] | 4;
  cfg.ap_hidden = d["ap_hidden"] | false;
  strlcpy(cfg.sta_ssid, d["sta_ssid"] | "", sizeof(cfg.sta_ssid));
  strlcpy(cfg.sta_pass, d["sta_pass"] | "", sizeof(cfg.sta_pass));
  strlcpy(cfg.admin_user, d["admin_user"] | "admin", sizeof(cfg.admin_user));
  strlcpy(cfg.admin_pass, d["admin_pass"] | "admin123", sizeof(cfg.admin_pass));
  cfg.update_interval_ms = d["update_interval_ms"] | 2000;
  cfg.cpu_freq_mhz = ESP.getCpuFreqMHz();
  cfg.pwm_freq = d["pwm_freq"] | 1000;
  cfg.gpio0 = d["gpio0"] | false;
  cfg.gpio2 = d["gpio2"] | false;
}

void saveCfg()
{
  StaticJsonDocument<512> d;

  d["device"] = cfg.device;
  d["ap_ssid"] = cfg.ap_ssid;
  d["ap_pass"] = cfg.ap_pass;
  d["ap_max_conn"] = cfg.ap_max_conn;
  d["ap_hidden"] = cfg.ap_hidden;
  d["sta_ssid"] = cfg.sta_ssid;
  d["sta_pass"] = cfg.sta_pass;
  d["admin_user"] = cfg.admin_user;
  d["admin_pass"] = cfg.admin_pass;
  d["update_interval_ms"] = cfg.update_interval_ms;
  d["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  d["pwm_freq"] = cfg.pwm_freq;
  d["gpio0"] = cfg.gpio0;
  d["gpio2"] = cfg.gpio2;

  File f = LittleFS.open(CFG_FILE, "w");
  if (f)
  {
    serializeJson(d, f);
    f.close();
  }
}

/* ================= WIFI ===================== */
void startWiFi()
{
  WiFi.mode(strlen(cfg.sta_ssid) ? WIFI_AP_STA : WIFI_AP);
  WiFi.hostname(cfg.device);
  WiFi.softAP(cfg.ap_ssid, cfg.ap_pass, 1, cfg.ap_hidden, cfg.ap_max_conn);

  Serial.println("Access Point iniciado:");
  Serial.print("  SSID: ");
  Serial.println(cfg.ap_ssid);
  Serial.print("  IP: ");
  Serial.println(WiFi.softAPIP());

  if (strlen(cfg.sta_ssid))
  {
    Serial.println("Conectando ao WiFi...");
    Serial.print("  SSID: ");
    Serial.println(cfg.sta_ssid);
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
  }
  else
  {
    Serial.println("Modo AP apenas - sem conexão com internet");
  }
}

void checkWiFi()
{
  if (millis() - state.lastWifiCheck < WIFI_RECONNECT_INTERVAL)
    return;

  state.lastWifiCheck = millis();

  if (!strlen(cfg.sta_ssid))
    return;

  if (WiFi.status() != WL_CONNECTED && !state.wifiConnecting)
  {
    Serial.println("WiFi desconectado. Reconectando...");
    WiFi.disconnect();
    delay(100);
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
  }

  if (state.wifiConnecting && WiFi.status() == WL_CONNECTED)
  {
    state.wifiConnecting = false;
    Serial.println("WiFi conectado!");
    Serial.print("  IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("  Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("  DNS: ");
    Serial.println(WiFi.dnsIP());
    Serial.print("  RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    if (!state.mdnsStarted)
    {
      if (MDNS.begin(cfg.device))
      {
        state.mdnsStarted = true;
        Serial.print("mDNS iniciado: http://");
        Serial.print(cfg.device);
        Serial.println(".local");
      }
    }
  }
}

/* ================= ROUTES =================== */
void setupRoutes()
{
  server.on("/", HTTP_GET, []()
            {
    logSystem.addLog(server);
    serveFile("/index.html"); });

  server.on("/config.html", HTTP_GET, []()
            {
    logSystem.addLog(server);
    serveFile("/config.html"); });

  server.on("/logs.html", HTTP_GET, []()
            {
    logSystem.addLog(server);
    serveFile("/logs.html"); });

  // API Status simplificado (compatibilidade)
  server.on("/status", HTTP_GET, []()
            {
    logSystem.addLog(server);
    StaticJsonDocument<256> d;
    d["fw"] = FW_VERSION;
    d["wifi"] = WiFi.status() == WL_CONNECTED;
    d["ip"] = WiFi.localIP().toString();

    String out;
    serializeJson(d, out);
    server.send(200, "application/json", out); });

  // API Status completo
  server.on("/api/status", HTTP_GET, []()
            {
    logSystem.addLog(server);
    StaticJsonDocument<1024> d;
    
    // CPU
    d["cpuFreq"] = ESP.getCpuFreqMHz();
    d["cpuUsage"] = 0;
    d["cpuCycles"] = ESP.getCycleCount();
    d["uptime"] = millis() / 1000;
    
    // Memória
    d["freeHeap"] = ESP.getFreeHeap();
    d["totalHeap"] = ESP.getFreeHeap();
    d["heapFrag"] = ESP.getHeapFragmentation();
    
    // Flash
    d["flashSize"] = ESP.getFlashChipSize();
    d["flashUsed"] = ESP.getSketchSize();
    d["flashSpeed"] = ESP.getFlashChipSpeed() / 1000000;
    d["chipId"] = String(ESP.getChipId(), HEX);
    
    // Wi-Fi
    d["ssid"] = WiFi.SSID();
    d["localIP"] = WiFi.localIP().toString();
    d["rssi"] = WiFi.RSSI();
    d["channel"] = WiFi.channel();
    d["mac"] = WiFi.macAddress();
    
    // Sistema de Arquivos
    FSInfo fs_info;
    LittleFS.info(fs_info);
    d["fsTotal"] = fs_info.totalBytes;
    d["fsUsed"] = fs_info.usedBytes;
    
    // Reset/Boot
    d["resetReason"] = ESP.getResetReason();
    d["bootMode"] = ESP.getBootMode();
    d["bootVersion"] = ESP.getBootVersion();
    
    JsonObject gpio = d.createNestedObject("gpio");
    gpio["0"] = digitalRead(0);
    gpio["2"] = digitalRead(2);
    gpio["4"] = digitalRead(4);
    gpio["5"] = digitalRead(5);
    gpio["12"] = digitalRead(12);
    gpio["13"] = digitalRead(13);
    gpio["14"] = digitalRead(14);
    gpio["15"] = digitalRead(15);

    String out;
    serializeJson(d, out);
    server.send(200, "application/json", out); });

  // API WiFi Status
  server.on("/api/wifi/status", HTTP_GET, []()
            {
    logSystem.addLog(server);
    StaticJsonDocument<512> d;
    
    d["connected"] = WiFi.status() == WL_CONNECTED;
    d["apIP"] = WiFi.softAPIP().toString();
    d["stationIP"] = WiFi.localIP().toString();
    d["gateway"] = WiFi.gatewayIP().toString();
    d["dns"] = WiFi.dnsIP().toString();
    d["rssi"] = WiFi.RSSI();
    d["ssid"] = WiFi.SSID();
    d["channel"] = WiFi.channel();
    d["hasInternet"] = (WiFi.status() == WL_CONNECTED && WiFi.localIP() != IPAddress(0,0,0,0));
    
    String out;
    serializeJson(d, out);
    server.send(200, "application/json", out); });

  // API WiFi Scan
  server.on("/api/wifi/scan", HTTP_GET, []()
            {
    logSystem.addLog(server);
    
    int n = WiFi.scanNetworks();
    StaticJsonDocument<2048> d;
    JsonArray networks = d.createNestedArray("networks");
    
    for (int i = 0; i < n; i++) {
      JsonObject net = networks.createNestedObject();
      net["ssid"] = WiFi.SSID(i);
      net["rssi"] = WiFi.RSSI(i);
      net["channel"] = WiFi.channel(i);
      net["encryption"] = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "open" : "secure";
    }
    
    d["count"] = n;
    
    String out;
    serializeJson(d, out);
    server.send(200, "application/json", out); });

  // API Config - GET
  server.on("/api/config", HTTP_GET, []()
            {
    logSystem.addLog(server);
    
    StaticJsonDocument<768> d;
    
    JsonObject wifi = d.createNestedObject("wifi");
    wifi["ssid"] = cfg.sta_ssid;
    
    JsonObject ap = d.createNestedObject("ap");
    ap["ssid"] = cfg.ap_ssid;
    ap["maxConnections"] = cfg.ap_max_conn;
    ap["hidden"] = cfg.ap_hidden;
    
    JsonObject sys = d.createNestedObject("system");
    sys["name"] = cfg.device;
    sys["cpuFreq"] = ESP.getCpuFreqMHz();
    sys["updateInterval"] = cfg.update_interval_ms;
    
    JsonObject gpioObj = d.createNestedObject("gpio");
    gpioObj["pwmFreq"] = cfg.pwm_freq;
    gpioObj["gpio0"] = cfg.gpio0;
    gpioObj["gpio2"] = cfg.gpio2;
    
    JsonObject security = d.createNestedObject("security");
    security["user"] = cfg.admin_user;

    String out;
    serializeJson(d, out);
    server.send(200, "application/json", out); });

  // API Config - POST
  server.on("/api/config", HTTP_POST, []()
            {
    logSystem.addLog(server);
    
    if (server.hasArg("plain")) {
      StaticJsonDocument<768> d;
      deserializeJson(d, server.arg("plain"));
      
      if (d.containsKey("wifi")) {
        strlcpy(cfg.sta_ssid, d["wifi"]["ssid"] | "", sizeof(cfg.sta_ssid));
        strlcpy(cfg.sta_pass, d["wifi"]["password"] | "", sizeof(cfg.sta_pass));
      }
      
      if (d.containsKey("ap")) {
        strlcpy(cfg.ap_ssid, d["ap"]["ssid"] | "ESP8266-SETUP", sizeof(cfg.ap_ssid));
        strlcpy(cfg.ap_pass, d["ap"]["password"] | "", sizeof(cfg.ap_pass));
        cfg.ap_max_conn = constrain((int)(d["ap"]["maxConnections"] | cfg.ap_max_conn), 1, 4);
        cfg.ap_hidden = d["ap"]["hidden"] | false;
      }
      
      if (d.containsKey("system")) {
        char newName[32];
        strlcpy(newName, d["system"]["name"] | "esp-gateway", sizeof(newName));
        bool nameChanged = strcmp(cfg.device, newName) != 0;
        strlcpy(cfg.device, newName, sizeof(cfg.device));
        cfg.cpu_freq_mhz = ESP.getCpuFreqMHz();
        cfg.update_interval_ms = d["system"]["updateInterval"] | cfg.update_interval_ms;

        if (nameChanged) {
          WiFi.hostname(cfg.device);
          if (state.mdnsStarted) {
            MDNS.end();
            state.mdnsStarted = false;
          }
          if (WiFi.status() == WL_CONNECTED) {
            if (MDNS.begin(cfg.device)) {
              state.mdnsStarted = true;
            }
          }
        }
      }
      
      if (d.containsKey("security")) {
        strlcpy(cfg.admin_user, d["security"]["user"] | "admin", sizeof(cfg.admin_user));
        if (d["security"]["newPassword"].as<String>().length() > 0) {
          strlcpy(cfg.admin_pass, d["security"]["newPassword"] | "admin123", sizeof(cfg.admin_pass));
        }
      }

      if (d.containsKey("gpio")) {
        cfg.gpio0 = d["gpio"]["gpio0"] | cfg.gpio0;
        cfg.gpio2 = d["gpio"]["gpio2"] | cfg.gpio2;
        cfg.pwm_freq = d["gpio"]["pwmFreq"] | cfg.pwm_freq;
      }

      analogWriteFreq(cfg.pwm_freq);
      pinMode(0, OUTPUT);
      digitalWrite(0, cfg.gpio0 ? HIGH : LOW);
      pinMode(2, OUTPUT);
      digitalWrite(2, cfg.gpio2 ? HIGH : LOW);

      saveCfg();
      
      server.send(200, "application/json", "{\"success\":true}");
    } else {
      server.send(400, "application/json", "{\"error\":\"Invalid request\"}");
    } });

  // API Config Reset
  server.on("/api/config/reset", HTTP_POST, []()
            {
    logSystem.addLog(server);
    
    strcpy(cfg.device, "esp-gateway");
    strcpy(cfg.ap_ssid, "ESP8266-SETUP");
    strcpy(cfg.ap_pass, "12345678");
    cfg.ap_max_conn = 4;
    cfg.ap_hidden = false;
    cfg.sta_ssid[0] = 0;
    cfg.sta_pass[0] = 0;
    strcpy(cfg.admin_user, "admin");
    strcpy(cfg.admin_pass, "admin123");
    cfg.update_interval_ms = 2000;
    cfg.cpu_freq_mhz = ESP.getCpuFreqMHz();
    cfg.pwm_freq = 1000;
    cfg.gpio0 = false;
    cfg.gpio2 = false;
    saveCfg();
    
    server.send(200, "application/json", "{\"success\":true}"); });

  // API Logs - GET
  server.on("/api/logs", HTTP_GET, []()
            { handleLogsGet(); });

  // API Logs - DELETE
  server.on("/api/logs", HTTP_DELETE, []()
            {
    logSystem.clear();
    server.send(200, "application/json", "{\"success\":true}"); });

  // API Logs Count
  server.on("/api/logs/count", HTTP_GET, []()
            {
    StaticJsonDocument<64> outDoc;
    outDoc["count"] = logSystem.getCount();
    String out;
    serializeJson(outDoc, out);
    server.send(200, "application/json", out); });

  // API GPIO Control
  server.on("/api/gpio", HTTP_POST, []()
            {
    logSystem.addLog(server);
    
    if (server.hasArg("plain")) {
      StaticJsonDocument<128> d;
      deserializeJson(d, server.arg("plain"));
      
      int pin = d["pin"];
      String action = d["action"].as<String>();
      
      if (action == "toggle") {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, !digitalRead(pin));
      }
      
      server.send(200, "application/json", "{\"success\":true}");
    } else {
      server.send(400, "application/json", "{\"error\":\"Invalid request\"}");
    } });

  // API Reset
  server.on("/api/reset", HTTP_POST, []()
            {
    logSystem.addLog(server);
    
    if (server.hasArg("plain")) {
      StaticJsonDocument<64> d;
      deserializeJson(d, server.arg("plain"));
      
      String type = d["type"].as<String>();
      
      server.send(200, "application/json", "{\"success\":true}");
      delay(500);
      ESP.restart();
    } else {
      server.send(400, "application/json", "{\"error\":\"Invalid request\"}");
    } });

  // API Arduino Bridge - redireciona para Arduino
  server.on("/api/arduino", HTTP_ANY, []()
            {
    logSystem.addLog(server);
    
    String method;
    switch(server.method()) {
      case HTTP_GET: method = "GET"; break;
      case HTTP_POST: method = "POST"; break;
      case HTTP_PUT: method = "PUT"; break;
      case HTTP_DELETE: method = "DELETE"; break;
      default: method = "UNKNOWN"; break;
    }
    
    String body = server.hasArg("plain") ? server.arg("plain") : "";
    String response = sendToArduino("http", method.c_str(), "/api/arduino", body.c_str());
    
    server.send(200, "application/json", response); });

  server.onNotFound([]()
                    { 
    // Tentar redirecionar rotas desconhecidas para o Arduino
    String uri = server.uri();
    if (uri.startsWith("/api/")) {
      logSystem.addLog(server);
      
      String method;
      switch(server.method()) {
        case HTTP_GET: method = "GET"; break;
        case HTTP_POST: method = "POST"; break;
        case HTTP_PUT: method = "PUT"; break;
        case HTTP_DELETE: method = "DELETE"; break;
        default: method = "UNKNOWN"; break;
      }
      
      String body = server.hasArg("plain") ? server.arg("plain") : "";
      String response = sendToArduino("http", method.c_str(), uri.c_str(), body.c_str());
      
      server.send(200, "application/json", response);
    } else {
      server.send(404, "text/plain", "Rota inválida");
    } });
}

/* ================= SETUP =================== */
void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  delay(100);

  Serial.println("\n\n=== ESP8266 Bridge Mode ===");
  Serial.print("Firmware: ");
  Serial.println(FW_VERSION);

  if (!LittleFS.begin())
  {
    Serial.println("LittleFS init falhou - usando config padrão");
  }

  loadCfg();
  analogWriteFreq(cfg.pwm_freq);
  pinMode(0, OUTPUT);
  digitalWrite(0, cfg.gpio0 ? HIGH : LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, cfg.gpio2 ? HIGH : LOW);
  startWiFi();
  setupRoutes();

  server.begin();
  Serial.println("Servidor HTTP iniciado na porta 80");

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Servidor WebSocket iniciado na porta 81");

  Serial.println("=== Bridge pronta ===");
}

/* ================= LOOP ==================== */
void loop()
{
  checkWiFi();
  server.handleClient();
  webSocket.loop();
  processSerialFromArduino();

  if (state.mdnsStarted)
    MDNS.update();
}
