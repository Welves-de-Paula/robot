#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/* ================= METADATA ================= */
#define FW_VERSION "3.8.1-ESP-GATEWAY"

/* ================= PORTAS =================== */
#define HTTP_PORT 80
#define LED_PIN LED_BUILTIN

/* ================= SEGURANÇA ================ */
#define MAX_LOGIN_ATTEMPTS 5
#define LOGIN_TIMEOUT 300000
#define MIN_PASSWORD_LENGTH 8

/* ================= WI-FI ==================== */
#define WIFI_RECONNECT_INTERVAL 30000

ESP8266WebServer server(HTTP_PORT);

/* ================= LED ===================== */
enum LedState {
  LED_DISCONNECTED,
  LED_CONNECTED,
  LED_ADMIN_ACTIVE,
  LED_REBOOTING
};

LedState ledState = LED_DISCONNECTED;
unsigned long lastLedToggle = 0;
bool ledOn = false;

/* ================= ESTADO =================== */
struct State {
  unsigned long lastWifiCheck = 0;
  unsigned long lastLoginFail = 0;
  int loginAttempts = 0;
  bool wifiConnecting = false;
  bool mdnsStarted = false;
} state;

/* ================= CONFIG =================== */
struct Config {
  char device[32];
  char ap_ssid[32];
  char ap_pass[32];
  char sta_ssid[32];
  char sta_pass[32];
  char admin_user[16];
  char admin_pass[16];
};

Config cfg;
const char *CFG_FILE = "/cfg.json";

/* ================= LED CORE ================= */
void setLedState(LedState s) {
  ledState = s;
  lastLedToggle = 0;
}

void updateLed() {
  unsigned long now = millis();
  unsigned long interval = 0;

  switch (ledState) {
    case LED_DISCONNECTED: interval = 1000; break;
    case LED_ADMIN_ACTIVE: interval = 200;  break;
    case LED_REBOOTING:    interval = 80;   break;
    case LED_CONNECTED:
      digitalWrite(LED_PIN, LOW);
      return;
  }

  if (now - lastLedToggle >= interval) {
    lastLedToggle = now;
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? LOW : HIGH);
  }
}

/* ================= AUTH ===================== */
bool auth() {
  unsigned long now = millis();

  if (state.loginAttempts >= MAX_LOGIN_ATTEMPTS &&
      now - state.lastLoginFail < LOGIN_TIMEOUT) {
    server.send(429, "text/plain", "Bloqueado temporariamente");
    return false;
  }

  if (!server.authenticate(cfg.admin_user, cfg.admin_pass)) {
    state.loginAttempts++;
    state.lastLoginFail = now;
    server.requestAuthentication();
    return false;
  }

  state.loginAttempts = 0;
  return true;
}

/* ================= CONFIG =================== */
void loadCfg() {
  if (!LittleFS.exists(CFG_FILE)) {
    strcpy(cfg.device, "ESP8266");
    strcpy(cfg.ap_ssid, "ESP8266-SETUP");
    strcpy(cfg.ap_pass, "12345678");
    cfg.sta_ssid[0] = 0;
    cfg.sta_pass[0] = 0;
    strcpy(cfg.admin_user, "admin");
    strcpy(cfg.admin_pass, "admin123");
    return;
  }

  File f = LittleFS.open(CFG_FILE, "r");
  StaticJsonDocument<512> d;
  deserializeJson(d, f);
  f.close();

  strlcpy(cfg.device, d["device"] | "ESP8266", sizeof(cfg.device));
  strlcpy(cfg.ap_ssid, d["ap_ssid"] | "ESP8266-SETUP", sizeof(cfg.ap_ssid));
  strlcpy(cfg.ap_pass, d["ap_pass"] | "12345678", sizeof(cfg.ap_pass));
  strlcpy(cfg.sta_ssid, d["sta_ssid"] | "", sizeof(cfg.sta_ssid));
  strlcpy(cfg.sta_pass, d["sta_pass"] | "", sizeof(cfg.sta_pass));
  strlcpy(cfg.admin_user, d["admin_user"] | "admin", sizeof(cfg.admin_user));
  strlcpy(cfg.admin_pass, d["admin_pass"] | "admin123", sizeof(cfg.admin_pass));
}

void saveCfg() {
  StaticJsonDocument<512> d;
  d["device"] = cfg.device;
  d["ap_ssid"] = cfg.ap_ssid;
  d["ap_pass"] = cfg.ap_pass;
  d["sta_ssid"] = cfg.sta_ssid;
  d["sta_pass"] = cfg.sta_pass;
  d["admin_user"] = cfg.admin_user;
  d["admin_pass"] = cfg.admin_pass;

  File f = LittleFS.open(CFG_FILE, "w");
  serializeJson(d, f);
  f.close();
}

/* ================= WIFI ===================== */
void startWiFi() {
  WiFi.mode(strlen(cfg.sta_ssid) ? WIFI_AP_STA : WIFI_AP);
  WiFi.softAP(cfg.ap_ssid, cfg.ap_pass);

  if (strlen(cfg.sta_ssid)) {
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
  }

  setLedState(LED_DISCONNECTED);
}

void checkWiFi() {
  if (millis() - state.lastWifiCheck < WIFI_RECONNECT_INTERVAL) return;
  state.lastWifiCheck = millis();

  if (!strlen(cfg.sta_ssid)) return;

  if (WiFi.status() != WL_CONNECTED && !state.wifiConnecting) {
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
    setLedState(LED_DISCONNECTED);
  }

  if (state.wifiConnecting && WiFi.status() == WL_CONNECTED) {
    state.wifiConnecting = false;
    setLedState(LED_CONNECTED);

    if (!state.mdnsStarted) {
      MDNS.begin(cfg.device);
      state.mdnsStarted = true;
    }
  }
}

/* ================= STATUS (PÚBLICO) =================== */
void handleStatus() {
  StaticJsonDocument<256> d;
  d["fw"] = FW_VERSION;
  d["heap"] = ESP.getFreeHeap();
  d["sta"] = WiFi.status() == WL_CONNECTED;
  d["sta_ip"] = WiFi.localIP().toString();
  d["ap_ip"] = WiFi.softAPIP().toString();
  d["rssi"] = WiFi.RSSI();

  String out;
  serializeJson(d, out);
  server.send(200, "application/json", out);
}

/* ================= HTML =================== */
const char ADMIN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="utf-8">
<title>ESP8266 Admin</title></head>
<body>
<h2>Admin ESP8266</h2>
<button onclick="doLogout()">Sair</button>
<script>
function doLogout(){
  fetch('/logout').finally(()=>location.href='/');
}
</script>
</body></html>
)rawliteral";

/* ================= SETUP =================== */
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(115200);
  LittleFS.begin();
  loadCfg();
  startWiFi();

  server.on("/", [](){
    server.send(200, "text/plain", "ESP8266 Gateway");
  });

  server.on("/admin", [](){
    if (!auth()) return;
    setLedState(LED_ADMIN_ACTIVE);
    server.send(200, "text/html", ADMIN_HTML);
  });

  server.on("/save", HTTP_POST, [](){
    if (!auth()) return;
    saveCfg();
    setLedState(LED_REBOOTING);
    delay(300);
    ESP.restart();
  });

  server.on("/reboot", [](){
    if (!auth()) return;
    setLedState(LED_REBOOTING);
    delay(300);
    ESP.restart();
  });

  server.on("/factory", [](){
    if (!auth()) return;
    LittleFS.remove(CFG_FILE);
    setLedState(LED_REBOOTING);
    delay(300);
    ESP.restart();
  });

  /* ===== LOGOUT CORRETO ===== */
  server.on("/logout", [](){
    server.sendHeader(
      "WWW-Authenticate",
      "Basic realm=\"ESP8266-LOGOUT\""
    );
    server.send(401, "text/plain", "Logout");
    setLedState(WiFi.status()==WL_CONNECTED ? LED_CONNECTED : LED_DISCONNECTED);
  });

  server.on("/status", handleStatus);

  server.begin();
}

/* ================= LOOP ==================== */
void loop() {
  checkWiFi();
  updateLed();
  server.handleClient();
  MDNS.update();
}
