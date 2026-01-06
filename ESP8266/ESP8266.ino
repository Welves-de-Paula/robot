#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/* ================= METADATA ================= */
#define FW_VERSION "3.7.1-ESP-GATEWAY"

/* ================= PORTAS =================== */
#define HTTP_PORT 80

/* ================= SEGURANÇA ================ */
#define MAX_LOGIN_ATTEMPTS 5
#define LOGIN_TIMEOUT 300000
#define MIN_PASSWORD_LENGTH 8

/* ================= WI-FI ==================== */
#define WIFI_RECONNECT_INTERVAL 30000

ESP8266WebServer server(HTTP_PORT);

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
}

void checkWiFi() {
  if (millis() - state.lastWifiCheck < WIFI_RECONNECT_INTERVAL) return;
  state.lastWifiCheck = millis();

  if (!strlen(cfg.sta_ssid)) return;

  if (WiFi.status() != WL_CONNECTED && !state.wifiConnecting) {
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;
  }

  if (state.wifiConnecting && WiFi.status() == WL_CONNECTED) {
    state.wifiConnecting = false;
    if (!state.mdnsStarted) {
      MDNS.begin(cfg.device);
      state.mdnsStarted = true;
    }
  }
}

/* ================= STATUS (SEM AUTH) =================== */
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
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>ESP8266 Admin</title>
<style>
body{font-family:Arial;background:#111;color:#eee;padding:70px 16px;max-width:900px;margin:auto}
.navbar{position:fixed;top:0;left:0;right:0;height:56px;background:#1a1a1a;
display:flex;align-items:center;justify-content:space-between;padding:0 16px}
.card{background:#1c1c1c;padding:15px;border-radius:10px;margin-bottom:15px}
input,button{width:100%;padding:10px;margin-top:8px;border-radius:6px;border:none}
.btn-save{background:#2196f3;color:#fff}
.btn-reboot{background:#333;color:#fff}
.btn-reset{background:#b71c1c;color:#fff}
.btn-logout{background:#555;color:#fff}
.modal{position:fixed;top:0;left:0;right:0;bottom:0;background:rgba(0,0,0,.7);
display:none;align-items:center;justify-content:center}
.modal-box{background:#1c1c1c;padding:20px;border-radius:10px;width:90%;max-width:320px;text-align:center}
</style>
</head>
<body>

<div class="navbar">
<b>ESP8266 Gateway</b>
<button class="btn-logout" onclick="logout()">Sair</button>
</div>

<div class="card" id="status">Carregando status...</div>

<form method="POST" action="/save">
<div class="card">
<b>Dispositivo</b>
<input name="device" value="%DEVICE%">
</div>

<div class="card">
<b>Wi-Fi</b>
<input name="sta_ssid" value="%STA_SSID%" placeholder="SSID">
<input type="password" name="sta_pass" value="%STA_PASS%" placeholder="Senha">
</div>

<div class="card">
<b>Access Point</b>
<input name="ap_ssid" value="%AP_SSID%">
<input type="password" name="ap_pass" value="%AP_PASS%">
</div>

<div class="card">
<b>Administrador</b>
<input name="admin_user" value="%ADMIN_USER%">
<input type="password" name="admin_pass" placeholder="Nova senha">
</div>

<button class="btn-save">Salvar</button>
</form>

<div class="card">
<button class="btn-reboot" onclick="openModal('reboot')">Reiniciar</button>
<button class="btn-reset" onclick="openModal('factory')">Redefinir</button>
</div>

<div id="modal" class="modal">
<div class="modal-box">
<p id="modalText"></p>
<button id="confirmBtn">Confirmar</button>
<button onclick="closeModal()">Cancelar</button>
</div>
</div>

<script>
function logout(){
  fetch('/logout').then(()=>location.href='/');
}

function openModal(type){
  document.getElementById('modal').style.display='flex';
  document.getElementById('modalText').innerText =
    type==='reboot' ? 'Reiniciar o dispositivo?' : 'Redefinir para padrão?';

  document.getElementById('confirmBtn').onclick = ()=>{
    fetch('/'+type).then(()=>{
      if(type==='factory') location.href='/';
    });
  };
}

function closeModal(){
  document.getElementById('modal').style.display='none';
}

fetch('/status').then(r=>r.json()).then(s=>{
  document.getElementById('status').innerHTML =
    'Firmware: '+s.fw+'<br>Heap: '+s.heap+'<br>RSSI: '+s.rssi;
});
</script>

</body>
</html>
)rawliteral";

/* ================= SETUP =================== */
void setup() {
  Serial.begin(115200);
  LittleFS.begin();
  loadCfg();
  startWiFi();

  server.on("/", [](){
    server.send(200, "text/plain", "ESP8266 Gateway");
  });

  server.on("/admin", [](){
    if (!auth()) return;
    String p = ADMIN_HTML;
    p.replace("%DEVICE%", cfg.device);
    p.replace("%STA_SSID%", cfg.sta_ssid);
    p.replace("%STA_PASS%", cfg.sta_pass);
    p.replace("%AP_SSID%", cfg.ap_ssid);
    p.replace("%AP_PASS%", cfg.ap_pass);
    p.replace("%ADMIN_USER%", cfg.admin_user);
    server.send(200, "text/html", p);
  });

  server.on("/save", HTTP_POST, [](){
    if (!auth()) return;

    strlcpy(cfg.device, server.arg("device").c_str(), 32);
    strlcpy(cfg.sta_ssid, server.arg("sta_ssid").c_str(), 32);
    strlcpy(cfg.sta_pass, server.arg("sta_pass").c_str(), 32);
    strlcpy(cfg.ap_ssid, server.arg("ap_ssid").c_str(), 32);
    if (server.arg("ap_pass").length() >= 8)
      strlcpy(cfg.ap_pass, server.arg("ap_pass").c_str(), 32);
    strlcpy(cfg.admin_user, server.arg("admin_user").c_str(), 16);
    if (server.arg("admin_pass").length() >= 8)
      strlcpy(cfg.admin_pass, server.arg("admin_pass").c_str(), 16);

    saveCfg();
    server.send(200, "text/plain", "OK");
    delay(300);
    ESP.restart();
  });

  server.on("/status", handleStatus);

  server.on("/reboot", [](){
    if (!auth()) return;
    server.send(200);
    delay(300);
    ESP.restart();
  });

  server.on("/factory", [](){
    if (!auth()) return;
    LittleFS.remove(CFG_FILE);
    server.send(200);
    delay(300);
    ESP.restart();
  });

  server.on("/logout", [](){
    server.sendHeader("WWW-Authenticate", "Basic realm=\"ESP8266\"");
    server.send(401);
  });

  server.begin();
}

/* ================= LOOP ==================== */
void loop() {
  checkWiFi();
  server.handleClient();
  MDNS.update();
}
