#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/* ================= METADATA ================= */
#define FW_VERSION "3.7.0-ESP-GATEWAY"

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
    cfg.http = cfg.ws = cfg.udp = true;
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
  } else {
    WiFi.mode(WIFI_AP);
  }

  WiFi.softAP(cfg.ap_ssid, cfg.ap_pass);
}

void checkWiFi() {
  if (millis() - state.lastWifiCheck < WIFI_RECONNECT_INTERVAL) return;
  state.lastWifiCheck = millis();

  if (strlen(cfg.sta_ssid) == 0) return;

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
<!DOCTYPE html><html><head><meta charset="utf-8">
<title>ESP8266 Gateway</title>
<style>
body{background:#111;color:#eee;font-family:Arial;padding:80px 16px 16px;max-width:900px;margin:auto}
.navbar{position:fixed;top:0;left:0;right:0;height:56px;background:#1a1a1a;display:flex;align-items:center;justify-content:space-between;padding:0 16px;z-index:1000}
.dot{width:12px;height:12px;border-radius:50%}
.green{background:#2ecc71}.red{background:#e74c3c}.yellow{background:#f1c40f}
.pulse{animation:pulse 1.5s infinite}
@keyframes pulse{0%{box-shadow:0 0 0 0 rgba(241,196,15,.7)}70%{box-shadow:0 0 0 10px rgba(241,196,15,0)}100%{box-shadow:0 0 0 0 rgba(241,196,15,0)}}
.card{background:#1c1c1c;padding:15px;border-radius:10px;margin-bottom:15px}
input{width:100%;padding:10px;margin-top:6px;border-radius:8px;border:none}
button{padding:12px;width:100%;border:none;border-radius:8px;margin-top:10px;font-size:16px}
.btn-save{background:#2196f3;color:#fff}
.btn-reboot{background:#333;color:#fff}
.btn-reset{background:#b71c1c;color:#fff}
.btn-logout{background:#555;color:#fff}
.toast{position:fixed;bottom:20px;left:50%;transform:translateX(-50%);background:#333;color:#fff;padding:12px 18px;border-radius:8px;opacity:0}
.toast.show{opacity:1}
.lock{position:fixed;top:0;left:0;right:0;bottom:0;background:rgba(0,0,0,.8);display:none;align-items:center;justify-content:center;z-index:4000}
</style>
</head>
<body>

<div class="navbar">
<b>ESP8266 Gateway</b>
<span><span id="dot" class="dot red"></span> <span id="connText">Offline</span></span>
</div>

<div class="card" id="status">Carregando status...</div>

<form method="POST" action="/save">
<div class="card"><b>Dispositivo</b><input name="device" value="%DEVICE%"></div>
<div class="card"><b>Wi-Fi STA</b><input name="sta_ssid" value="%STA_SSID%"><input type="password" name="sta_pass" value="%STA_PASS%"></div>
<div class="card"><b>AP</b><input name="ap_ssid" value="%AP_SSID%"><input type="password" name="ap_pass" value="%AP_PASS%"></div>
<div class="card"><b>Admin</b><input name="admin_user" value="%ADMIN_USER%"><input type="password" name="admin_pass"></div>
<div class="card"><b>Protocolos</b><br>
<input type="checkbox" name="http" %HTTP%> HTTP<br>
<input type="checkbox" name="ws" %WS%> WS<br>
<input type="checkbox" name="udp" %UDP%> UDP</div>
<button class="btn-save">Salvar</button>
</form>

<div class="card">
<button class="btn-reboot" onclick="doReboot()">Reiniciar</button>
<button class="btn-reset" onclick="doReset()">Redefinir</button>
<button class="btn-logout" onclick="location.href='/logout'">Sair</button>
</div>

<div id="toast" class="toast"></div>
<div id="lock" class="lock"><div class="card">Processando...</div></div>

<script>
function toast(m){let t=document.getElementById('toast');t.innerText=m;t.classList.add('show');setTimeout(()=>t.classList.remove('show'),3000)}
function lock(m){document.getElementById('lock').style.display='flex'}
function update(){
fetch('/status').then(r=>r.json()).then(s=>{
document.getElementById('status').innerHTML='FW:'+s.fw+'<br>Heap:'+s.heap+'<br>RSSI:'+s.rssi;
let d=document.getElementById('dot'),t=document.getElementById('connText');
if(s.sta){d.className='dot green';t.innerText='Online'}else{d.className='dot yellow pulse';t.innerText='Reconectando'}
});
}
function doReboot(){toast('Reiniciando');lock();fetch('/reboot')}
function doReset(){toast('Resetando');lock();fetch('/factory')}
update();setInterval(update,5000);
</script>

</body></html>
)rawliteral";

/* ================= SETUP =================== */
void setup() {
  Serial.begin(115200);
  LittleFS.begin();
  loadCfg();
  startWiFi();

  server.on("/", [](){ if(!auth())return; server.sendHeader("Location","/admin"); server.send(302); });
  server.on("/admin", [](){
    if(!auth())return;
    String p=ADMIN_HTML;
    p.replace("%DEVICE%",cfg.device);
    p.replace("%STA_SSID%",cfg.sta_ssid);
    p.replace("%STA_PASS%",cfg.sta_pass);
    p.replace("%AP_SSID%",cfg.ap_ssid);
    p.replace("%AP_PASS%",cfg.ap_pass);
    p.replace("%ADMIN_USER%",cfg.admin_user);
    p.replace("%HTTP%",cfg.http?"checked":"");
    p.replace("%WS%",cfg.ws?"checked":"");
    p.replace("%UDP%",cfg.udp?"checked":"");
    server.send(200,"text/html",p);
  });

  server.on("/save",HTTP_POST,[](){
    if(!auth())return;
    strlcpy(cfg.device,server.arg("device").c_str(),32);
    strlcpy(cfg.sta_ssid,server.arg("sta_ssid").c_str(),32);
    strlcpy(cfg.sta_pass,server.arg("sta_pass").c_str(),32);
    strlcpy(cfg.ap_ssid,server.arg("ap_ssid").c_str(),32);
    if(server.arg("ap_pass").length()>=8)strlcpy(cfg.ap_pass,server.arg("ap_pass").c_str(),32);
    strlcpy(cfg.admin_user,server.arg("admin_user").c_str(),16);
    if(server.arg("admin_pass").length()>=8)strlcpy(cfg.admin_pass,server.arg("admin_pass").c_str(),16);
    cfg.http=server.hasArg("http");cfg.ws=server.hasArg("ws");cfg.udp=server.hasArg("udp");
    saveCfg();server.send(200,"text/plain","OK");delay(300);ESP.restart();
  });

  server.on("/status",handleStatus);
  server.on("/reboot",[](){ if(!auth())return; ESP.restart(); });
  server.on("/factory",[](){ if(!auth())return; LittleFS.remove(CFG_FILE); ESP.restart(); });
  server.on("/logout",[](){ server.sendHeader("WWW-Authenticate","Basic"); server.send(401); });

  if(cfg.http)server.begin();
}

/* ================= LOOP ==================== */
void loop() {
  checkWiFi();
  server.handleClient();
  MDNS.update();
}
