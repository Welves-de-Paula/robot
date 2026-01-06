#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/* ================= METADATA ================= */
#define FW_VERSION "3.6.1-ESP-GATEWAY"

/* ================= PORTAS =================== */
#define HTTP_PORT 80
#define WS_PORT 81
#define UDP_PORT 4210
#define LED_PIN LED_BUILTIN

/* ================= SEGURANÇA ================ */
#define MAX_LOGIN_ATTEMPTS 5
#define LOGIN_TIMEOUT 300000
#define MIN_PASSWORD_LENGTH 8

/* ================= WI-FI ==================== */
#define WIFI_RECONNECT_INTERVAL 30000

/* ================= OBJETOS ================== */
ESP8266WebServer server(HTTP_PORT);
WebSocketsServer ws(WS_PORT);
WiFiUDP udp;

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
  bool http;
  bool ws;
  bool udp;
  bool led_status;
};

Config cfg;
const char *CFG_FILE = "/cfg.json";

/* ================= LOG ===================== */
void logEvent(const String &msg) {
  Serial.printf("[%lu] %s\n", millis(), msg.c_str());
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
    cfg.http = true;
    cfg.ws = true;
    cfg.udp = true;
    cfg.led_status = true;
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
  cfg.http = d["http"] | true;
  cfg.ws = d["ws"] | true;
  cfg.udp = d["udp"] | true;
  cfg.led_status = d["led_status"] | true;
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
  d["http"] = cfg.http;
  d["ws"] = cfg.ws;
  d["udp"] = cfg.udp;
  d["led_status"] = cfg.led_status;

  File f = LittleFS.open(CFG_FILE, "w");
  serializeJson(d, f);
  f.close();
}

/* ================= WIFI ===================== */
void startWiFi() {
  WiFi.persistent(false);

  if (strlen(cfg.sta_ssid) > 0) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
    logEvent("Tentando STA: " + String(cfg.sta_ssid));
  } else {
    WiFi.mode(WIFI_AP);
  }

  WiFi.softAP(cfg.ap_ssid, cfg.ap_pass);
  logEvent("AP ativo: " + WiFi.softAPIP().toString());
}

void checkWiFi() {
  if (millis() - state.lastWifiCheck < WIFI_RECONNECT_INTERVAL) return;
  state.lastWifiCheck = millis();

  if (strlen(cfg.sta_ssid) == 0) return;

  if (WiFi.status() != WL_CONNECTED && !state.wifiConnecting) {
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
    logEvent("Reconectando Wi-Fi...");
  }

  if (state.wifiConnecting && WiFi.status() == WL_CONNECTED) {
    state.wifiConnecting = false;
    logEvent("STA conectado: " + WiFi.localIP().toString());

    if (!state.mdnsStarted) {
      MDNS.begin(cfg.device);
      state.mdnsStarted = true;
      logEvent("mDNS ativo");
    }
  }
}

/* ================= LED ===================== */
void updateLED() {
  if (!cfg.led_status) {
    digitalWrite(LED_PIN, HIGH);
    return;
  }

  digitalWrite(
    LED_PIN,
    WiFi.status() == WL_CONNECTED ? LOW : (millis() / 500) % 2
  );
}

/* ================= STATUS =================== */
void handleStatus() {
  if (!auth()) return;

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
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>ESP8266 Admin</title>
<style>
body{
  background:#111;
  color:#eee;
  font-family:Arial;
  max-width:900px;
  margin:auto;
  padding:20px
}
.card{
  background:#1c1c1c;
  padding:15px;
  border-radius:8px;
  margin-bottom:15px
}
input{
  width:100%;
  padding:9px;
  margin-top:6px;
  border-radius:6px;
  border:none
}
button{
  padding:11px;
  width:100%;
  margin-top:10px;
  border:none;
  border-radius:6px;
  font-size:15px;
  cursor:pointer;
  transition:background .2s, transform .1s
}
button:hover{ transform:scale(1.01) }

.btn-save{ background:#2196f3;color:#fff }
.btn-save:hover{ background:#1e88e5 }

.btn-reboot{ background:#2e2e2e;color:#fff }
.btn-reboot:hover{ background:#3a3a3a }

.btn-reset{ background:#b71c1c;color:#fff }
.btn-reset:hover{ background:#c62828 }

.btn-logout{ background:#555;color:#fff }
.btn-logout:hover{ background:#666 }
</style>
</head>
<body>

<h2>Painel Administrativo – ESP8266</h2>

<div class="card" id="status">Carregando status...</div>

<form method="POST" action="/save">

<div class="card">
<b>Dispositivo</b>
<input name="device" value="%DEVICE%">
</div>

<div class="card">
<b>Wi-Fi STA</b>
<input name="sta_ssid" value="%STA_SSID%" placeholder="SSID">
<input type="password" name="sta_pass" value="%STA_PASS%" placeholder="Senha">
</div>

<div class="card">
<b>Access Point</b>
<input name="ap_ssid" value="%AP_SSID%" placeholder="Nome do AP">
<input type="password" name="ap_pass" value="%AP_PASS%" placeholder="Senha do AP">
</div>

<div class="card">
<b>Administrador</b>
<input name="admin_user" value="%ADMIN_USER%" placeholder="Usuário">
<input type="password" name="admin_pass" placeholder="Nova senha">
</div>

<div class="card">
<b>Protocolos</b><br>
<input type="checkbox" name="http" %HTTP%> HTTP<br>
<input type="checkbox" name="ws" %WS%> WebSocket<br>
<input type="checkbox" name="udp" %UDP%> UDP
</div>

<button class="btn-save" type="submit">Salvar configurações</button>
</form>

<div class="card">
<button class="btn-reboot" onclick="fetch('/reboot')">Reiniciar</button>
<button class="btn-reset" onclick="fetch('/factory')">Redefinir</button>
<button class="btn-logout" onclick="location.href='/logout'">Sair</button>
</div>

<script>
fetch('/status').then(r=>r.json()).then(s=>{
document.getElementById('status').innerHTML =
'<b>Status</b><br>' +
'Firmware: '+s.fw+'<br>' +
'Heap: '+s.heap+'<br>' +
'STA: '+(s.sta?'Conectado':'Desconectado')+'<br>' +
'IP STA: '+s.sta_ip+'<br>' +
'IP AP: '+s.ap_ip+'<br>' +
'RSSI: '+s.rssi;
});
</script>

</body>
</html>
)rawliteral";

/* ================= SETUP =================== */
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(115200);
  delay(100);

  LittleFS.begin();
  loadCfg();
  startWiFi();

  server.on("/", []() {
    if (!auth()) return;
    server.sendHeader("Location", "/admin");
    server.send(302, "text/plain", "");
  });

  server.on("/admin", []() {
    if (!auth()) return;
    String p = ADMIN_HTML;
    p.replace("%DEVICE%", cfg.device);
    p.replace("%STA_SSID%", cfg.sta_ssid);
    p.replace("%STA_PASS%", cfg.sta_pass);
    p.replace("%AP_SSID%", cfg.ap_ssid);
    p.replace("%AP_PASS%", cfg.ap_pass);
    p.replace("%ADMIN_USER%", cfg.admin_user);
    p.replace("%HTTP%", cfg.http ? "checked" : "");
    p.replace("%WS%", cfg.ws ? "checked" : "");
    p.replace("%UDP%", cfg.udp ? "checked" : "");
    server.send(200, "text/html", p);
  });

  server.on("/save", HTTP_POST, []() {
    if (!auth()) return;

    strlcpy(cfg.device, server.arg("device").c_str(), sizeof(cfg.device));
    strlcpy(cfg.sta_ssid, server.arg("sta_ssid").c_str(), sizeof(cfg.sta_ssid));
    strlcpy(cfg.sta_pass, server.arg("sta_pass").c_str(), sizeof(cfg.sta_pass));
    strlcpy(cfg.ap_ssid, server.arg("ap_ssid").c_str(), sizeof(cfg.ap_ssid));

    if (server.arg("ap_pass").length() >= MIN_PASSWORD_LENGTH)
      strlcpy(cfg.ap_pass, server.arg("ap_pass").c_str(), sizeof(cfg.ap_pass));

    strlcpy(cfg.admin_user, server.arg("admin_user").c_str(), sizeof(cfg.admin_user));

    if (server.arg("admin_pass").length() >= MIN_PASSWORD_LENGTH)
      strlcpy(cfg.admin_pass, server.arg("admin_pass").c_str(), sizeof(cfg.admin_pass));

    cfg.http = server.hasArg("http");
    cfg.ws   = server.hasArg("ws");
    cfg.udp  = server.hasArg("udp");

    saveCfg();
    server.send(200, "text/plain", "Salvo. Reiniciando...");
    delay(500);
    ESP.restart();
  });

  server.on("/status", handleStatus);

  server.on("/reboot", []() {
    if (!auth()) return;
    server.send(200, "text/plain", "Reiniciando");
    delay(300);
    ESP.restart();
  });

  server.on("/factory", []() {
    if (!auth()) return;
    LittleFS.remove(CFG_FILE);
    server.send(200, "text/plain", "Resetando");
    delay(300);
    ESP.restart();
  });

  server.on("/logout", []() {
    server.sendHeader("WWW-Authenticate", "Basic realm=\"ESP8266\"");
    server.send(401, "text/plain", "Logout");
  });

  if (cfg.http) server.begin();
  if (cfg.ws) ws.begin();
  if (cfg.udp) udp.begin(UDP_PORT);

  logEvent("Sistema pronto");
}

/* ================= LOOP ==================== */
void loop() {
  checkWiFi();
  updateLED();

  if (cfg.http) server.handleClient();
  if (cfg.ws) ws.loop();

  if (cfg.udp && udp.parsePacket()) {
    char buf[256];
    int len = udp.read(buf, sizeof(buf) - 1);
    buf[len] = 0;
    logEvent("UDP: " + String(buf));
  }

  MDNS.update();
  yield();
}
