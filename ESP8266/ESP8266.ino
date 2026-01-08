#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "html_pages.h"

/* ================= METADATA ================= */
#define FW_VERSION "3.9.0-ESP-GATEWAY"

/* ================= PORTAS =================== */
#define HTTP_PORT 80

/* ================= SEGURANÇA ================ */
#define MAX_LOGIN_ATTEMPTS 5
#define LOGIN_TIMEOUT 300000

/* ================= WI-FI ==================== */
#define WIFI_RECONNECT_INTERVAL 30000

ESP8266WebServer server(HTTP_PORT);
String sessionToken = "";
unsigned long sessionExpiry = 0;
const unsigned long SESSION_TTL = 1800000; // 30 minutos

/* ================= ESTADO =================== */
struct State
{
  unsigned long lastWifiCheck = 0;
  unsigned long lastLoginFail = 0;
  int loginAttempts = 0;
  bool wifiConnecting = false;
  bool mdnsStarted = false;
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

  if (strcmp(path, "/login.html") == 0)
    html = HTML_LOGIN;
  else if (strcmp(path, "/index.html") == 0 || strcmp(path, "/") == 0)
    html = HTML_INDEX;
  else if (strcmp(path, "/dashboard.html") == 0)
    html = HTML_DASHBOARD;
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

bool auth()
{
  unsigned long now = millis();

  if (state.loginAttempts >= MAX_LOGIN_ATTEMPTS &&
      now - state.lastLoginFail < LOGIN_TIMEOUT)
  {
    server.send(429, "text/plain", "Bloqueado temporariamente");
    return false;
  }

  // Sessão via cookie
  if (server.hasHeader("Cookie"))
  {
    String cookie = server.header("Cookie");
    int pos = cookie.indexOf("session=");
    if (pos >= 0)
    {
      int end = cookie.indexOf(';', pos);
      String token = cookie.substring(pos + 8, end == -1 ? cookie.length() : end);
      if (token == sessionToken && now < sessionExpiry)
      {
        sessionExpiry = now + SESSION_TTL; // renova
        return true;
      }
    }
  }

  if (!server.authenticate(cfg.admin_user, cfg.admin_pass))
  {
    state.loginAttempts++;
    state.lastLoginFail = now;
    server.requestAuthentication();
    return false;
  }

  state.loginAttempts = 0;
  return true;
}

/* ================= LOGS (tempo real) ====== */
void handleLogsGet()
{
  StaticJsonDocument<768> d;
  JsonArray logs = d.createNestedArray("logs");

  unsigned long up = millis() / 1000;

  // Snapshot de sistema
  JsonObject sys = logs.createNestedObject();
  sys["timestamp"] = millis();
  sys["level"] = "info";
  sys["source"] = "system";
  sys["message"] = String("Uptime ") + up + "s, heap livre " + ESP.getFreeHeap() + " bytes";

  // Snapshot de Wi-Fi
  JsonObject wifi = logs.createNestedObject();
  wifi["timestamp"] = millis();
  wifi["level"] = (WiFi.status() == WL_CONNECTED) ? "success" : "warning";
  wifi["source"] = "wifi";
  if (WiFi.status() == WL_CONNECTED)
  {
    wifi["message"] = String("RSSI ") + WiFi.RSSI() + " dBm, IP " + WiFi.localIP().toString();
  }
  else
  {
    wifi["message"] = "Wi-Fi desconectado";
  }

  // Snapshot de filesystem
  FSInfo fs_info;
  LittleFS.info(fs_info);
  JsonObject fs = logs.createNestedObject();
  fs["timestamp"] = millis();
  fs["level"] = "debug";
  fs["source"] = "fs";
  fs["message"] = String("FS usado ") + fs_info.usedBytes + "/" + fs_info.totalBytes + " bytes";

  String out;
  serializeJson(d, out);
  server.send(200, "application/json", out);
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
  // Sempre usar a frequência real do chip; ignorar valor salvo para evitar instabilidade
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

  if (strlen(cfg.sta_ssid))
  {
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
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
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
  }

  if (state.wifiConnecting && WiFi.status() == WL_CONNECTED)
  {
    state.wifiConnecting = false;

    if (!state.mdnsStarted)
    {
      MDNS.begin(cfg.device);
      state.mdnsStarted = true;
    }
  }
}

/* ================= ROUTES =================== */
void setupRoutes()
{
  // Páginas HTML
  server.on("/login", HTTP_GET, []()
            { serveFile("/login.html"); });

  // Login via JSON para criar sessão baseada em cookie
  server.on("/login", HTTP_POST, []()
            {
    if (server.hasArg("plain")) {
      StaticJsonDocument<128> d;
      deserializeJson(d, server.arg("plain"));
      const char* user = d["username"] | "";
      const char* pass = d["password"] | "";

      if (strcmp(user, cfg.admin_user) == 0 && strcmp(pass, cfg.admin_pass) == 0) {
        sessionToken = String(random(0x7fffffff), HEX) + String(millis(), HEX);
        sessionExpiry = millis() + SESSION_TTL;
        server.sendHeader("Set-Cookie", "session=" + sessionToken + "; Path=/; HttpOnly");
        state.loginAttempts = 0;
        server.send(200, "application/json", "{\"success\":true}");
      } else {
        state.loginAttempts++;
        state.lastLoginFail = millis();
        server.send(401, "application/json", "{\"error\":\"invalid credentials\"}");
      }
    } else {
      server.send(400, "application/json", "{\"error\":\"Invalid request\"}");
    } });

  server.on("/", HTTP_GET, []()
            {
    if (!auth()) return;
    serveFile("/index.html"); });

  server.on("/dashboard.html", HTTP_GET, []()
            {
    if (!auth()) return;
    serveFile("/dashboard.html"); });

  server.on("/config.html", HTTP_GET, []()
            {
    if (!auth()) return;
    serveFile("/config.html"); });

  server.on("/logs.html", HTTP_GET, []()
            {
    if (!auth()) return;
    serveFile("/logs.html"); });

  // API Status simplificado (compatibilidade)
  server.on("/status", HTTP_GET, []()
            {
    StaticJsonDocument<256> d;
    d["fw"] = FW_VERSION;
    d["wifi"] = WiFi.status() == WL_CONNECTED;
    d["ip"] = WiFi.localIP().toString();

    String out;
    serializeJson(d, out);
    server.send(200, "application/json", out); });

  // API Status completo para Dashboard
  server.on("/api/status", HTTP_GET, []()
            {
    StaticJsonDocument<1024> d;
    
    // CPU
    d["cpuFreq"] = ESP.getCpuFreqMHz();
    d["cpuUsage"] = 0; // ESP8266 não expõe uso de CPU; retorna 0 para evitar valor falso
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
    
    // GPIO (exemplo)
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

  // API Config - GET
  server.on("/api/config", HTTP_GET, []()
            {
    if (!auth()) return;
    
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
    if (!auth()) return;
    
    if (server.hasArg("plain")) {
      StaticJsonDocument<768> d;
      deserializeJson(d, server.arg("plain"));
      
      // Salvar configurações (implementar salvamento real)
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
        // Ignorar frequência enviada; manter a real do chip para evitar travamentos
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
    if (!auth()) return;
    
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
    if (!auth()) return;
    server.send(200, "application/json", "{\"success\":true}"); });

  // API Logs Count
  server.on("/api/logs/count", HTTP_GET, []()
            {
    // Contagem fixa para os snapshots enviados em tempo real
    StaticJsonDocument<64> outDoc;
    outDoc["count"] = (WiFi.status() == WL_CONNECTED) ? 3 : 3; // número de itens enviados em /api/logs
    String out;
    serializeJson(outDoc, out);
    server.send(200, "application/json", out); });

  // API GPIO Control
  server.on("/api/gpio", HTTP_POST, []()
            {
    if (!auth()) return;
    
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
    if (!auth()) return;
    
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

  server.onNotFound([]()
                    { server.send(404, "text/plain", "Rota inválida"); });
}

/* ================= SETUP =================== */
void setup()
{
  Serial.begin(115200);

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
  Serial.println("Servidor HTTP iniciado");
}

/* ================= LOOP ==================== */
void loop()
{
  checkWiFi();
  server.handleClient();
  if (state.mdnsStarted)
    MDNS.update();
}
