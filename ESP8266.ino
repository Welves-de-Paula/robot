#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/* ================= METADATA ================= */
#define FW_VERSION "3.5.0-ESP-GATEWAY"

/* ================= PORTAS =================== */
#define WS_PORT 81
#define UDP_PORT 4210
#define LED_PIN LED_BUILTIN

/* ================= SEGURANÇA ================ */
#define MAX_LOGIN_ATTEMPTS 5
#define LOGIN_TIMEOUT 300000 // 5 minutos
#define MIN_PASSWORD_LENGTH 8

/* ================= WI-FI ==================== */
#define WIFI_RECONNECT_INTERVAL 30000 // 30 segundos
#define WIFI_CONNECT_TIMEOUT 20000    // 20 segundos

/* ================= OBJETOS ================== */
ESP8266WebServer server(80);
WebSocketsServer ws(WS_PORT);
WiFiUDP udp;

/* ================= ESTADO =================== */
struct State
{
  unsigned long lastWifiCheck;
  unsigned long lastLoginFail;
  int loginAttempts;
  bool wifiConnecting;
  String sessionToken;
} state;

/* ================= CONFIG =================== */
struct Config
{
  char device[32];

  char ap_ssid[32];
  char ap_pass[32];

  char sta_ssid[32];
  char sta_pass[32];

  char admin_user[16];
  char admin_pass[16];

  bool dhcp;
  bool http;
  bool ws;
  bool udp;
  bool led_status;
};

Config cfg;
const char *CFG_FILE = "/cfg.json";

/* ================= HTML ===================== */
const char ADMIN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta charset="utf-8">
<title>ESP8266</title>
<style>
body{background:#0f0f0f;color:#eee;font-family:Arial;max-width:600px;margin:auto;padding:20px}
h2{margin-bottom:5px}
small{opacity:.7}
input{width:100%;padding:10px;margin:6px 0 14px;border-radius:6px;border:0}
label{font-size:14px}
button{width:100%;padding:12px;border:0;border-radius:6px;font-size:16px;cursor:pointer}
.primary{background:#2196f3;color:#fff}
.warn{background:#2a2a2a;color:#eee;margin-top:10px}
.danger{background:#b71c1c;color:#fff;margin-top:10px}
.status{background:#1b1b1b;padding:12px;border-radius:6px;font-size:14px;margin:15px 0}
</style>
</head><body>

<h2>ESP8266</h2>
<small>Painel de configuração e status</small>

<div class="status" id="status">Carregando status...</div>

<form method="POST" action="/save">
<label>Nome do dispositivo</label>
<input name="device" value="%DEVICE%">

<label>SSID do AP</label>
<input name="ap_ssid" value="%AP_SSID%">

<label>Senha do AP</label>
<input type="password" name="ap_pass" value="%AP_PASS%">

<label>SSID Wi-Fi</label>
<input name="sta_ssid" value="%STA_SSID%">

<label>Senha Wi-Fi</label>
<input type="password" name="sta_pass" value="%STA_PASS%">

<label>Usuário admin</label>
<input name="admin_user" value="%ADMIN_USER%">

<label>Senha admin</label>
<input type="password" name="admin_pass" value="%ADMIN_PASS%">

<label><input type="checkbox" name="dhcp" %DHCP%> DHCP</label><br>
<label><input type="checkbox" name="http" %HTTP%> HTTP</label><br>
<label><input type="checkbox" name="ws" %WS%> WebSocket</label><br>
<label><input type="checkbox" name="udp" %UDP%> UDP</label><br>
<label><input type="checkbox" name="led_status" %LED_STATUS%> LED de Status</label><br><br>

<button class="primary">Salvar configurações</button>
<button class="warn" type="button" onclick="fetch('/reboot')">Reiniciar</button>
<button class="danger" type="button" onclick="fetch('/factory_reset')">Reset de fábrica</button>
</form>

<script>
fetch('/status',{credentials:'include'}).then(r=>r.json()).then(s=>{
document.getElementById('status').innerHTML =
'Firmware: '+s.fw+'<br>'+
'Heap livre: '+s.heap+'<br>'+
'Wi-Fi conectado: '+(s.sta?'Sim':'Não')+'<br>'+
'IP STA: '+s.sta_ip+'<br>'+
'IP AP: '+s.ap_ip+'<br>'+
'Sinal: '+s.rssi+' dBm';
});
</script>
</body></html>
)rawliteral";

/* ================= LOGS ===================== */
void logEvent(String event)
{
  String timestamp = String(millis());
  Serial.println("[" + timestamp + "] " + event);
}

/* ================= AUTH ===================== */
bool auth()
{
  unsigned long now = millis();

  // Verificar rate limiting
  if (state.loginAttempts >= MAX_LOGIN_ATTEMPTS)
  {
    if (now - state.lastLoginFail < LOGIN_TIMEOUT)
    {
      server.send(429, "text/plain", "Muitas tentativas. Aguarde 5 minutos.");
      logEvent("BLOQUEIO: Tentativas excessivas de login de " + server.client().remoteIP().toString());
      return false;
    }
    else
    {
      state.loginAttempts = 0;
    }
  }

  if (!server.authenticate(cfg.admin_user, cfg.admin_pass))
  {
    state.loginAttempts++;
    state.lastLoginFail = now;
    server.requestAuthentication();
    logEvent("FALHA: Tentativa de login (" + String(state.loginAttempts) + "/" + String(MAX_LOGIN_ATTEMPTS) + ")");
    return false;
  }

  state.loginAttempts = 0;
  logEvent("SUCESSO: Login autenticado");
  return true;
}

/* ================= CONFIG =================== */
void loadCfg()
{
  if (!LittleFS.exists(CFG_FILE))
  {
    strcpy(cfg.device, "ESP8266");
    strcpy(cfg.ap_ssid, "ESP8266-SETUP");
    strcpy(cfg.ap_pass, "12345678");
    cfg.sta_ssid[0] = 0;
    cfg.sta_pass[0] = 0;
    strcpy(cfg.admin_user, "admin");
    strcpy(cfg.admin_pass, "admin123"); // Senha padrão mais forte
    cfg.dhcp = true;
    cfg.http = cfg.ws = cfg.udp = true;
    cfg.led_status = true;
    return;
  }

  File f = LittleFS.open(CFG_FILE, "r");
  StaticJsonDocument<512> d;
  deserializeJson(d, f);
  f.close();

  strlcpy(cfg.device, d["device"], 32);
  strlcpy(cfg.ap_ssid, d["ap_ssid"], 32);
  strlcpy(cfg.ap_pass, d["ap_pass"], 32);
  strlcpy(cfg.sta_ssid, d["sta_ssid"], 32);
  strlcpy(cfg.sta_pass, d["sta_pass"], 32);
  strlcpy(cfg.admin_user, d["admin_user"], 16);
  strlcpy(cfg.admin_pass, d["admin_pass"], 16);
  cfg.dhcp = d["dhcp"];
  cfg.http = d["http"];
  cfg.ws = d["ws"];
  cfg.udp = d["udp"];
  cfg.led_status = d["led_status"] | true;
}

void saveCfg()
{
  StaticJsonDocument<512> d;
  d["device"] = cfg.device;
  d["ap_ssid"] = cfg.ap_ssid;
  d["ap_pass"] = cfg.ap_pass;
  d["sta_ssid"] = cfg.sta_ssid;
  d["sta_pass"] = cfg.sta_pass;
  d["admin_user"] = cfg.admin_user;
  d["admin_pass"] = cfg.admin_pass;
  d["dhcp"] = cfg.dhcp;
  d["http"] = cfg.http;
  d["ws"] = cfg.ws;
  d["udp"] = cfg.udp;
  d["led_status"] = cfg.led_status;

  File f = LittleFS.open(CFG_FILE, "w");
  serializeJson(d, f);
  f.close();
}

/* ================= WIFI ===================== */
void checkWiFi()
{
  unsigned long now = millis();

  if (now - state.lastWifiCheck < WIFI_RECONNECT_INTERVAL)
    return;
  state.lastWifiCheck = now;

  if (WiFi.status() != WL_CONNECTED && strlen(cfg.sta_ssid) > 0 && !state.wifiConnecting)
  {
    logEvent("Wi-Fi desconectado. Tentando reconectar...");
    state.wifiConnecting = true;
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
  }

  if (state.wifiConnecting && WiFi.status() == WL_CONNECTED)
  {
    state.wifiConnecting = false;
    logEvent("Wi-Fi reconectado! IP: " + WiFi.localIP().toString());
  }
}

void updateLED()
{
  if (!cfg.led_status)
  {
    digitalWrite(LED_PIN, HIGH); // LED apagado
    return;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_PIN, LOW); // LED aceso fixo
  }
  else
  {
    // Piscar LED quando desconectado
    digitalWrite(LED_PIN, (millis() / 500) % 2);
  }
}

/* ================= STATUS =================== */
void status()
{
  if (!auth())
    return;
  StaticJsonDocument<256> d;
  d["fw"] = FW_VERSION;
  d["heap"] = ESP.getFreeHeap();
  d["rssi"] = WiFi.RSSI();
  d["ap_ip"] = WiFi.softAPIP().toString();
  d["sta_ip"] = WiFi.localIP().toString();
  d["sta"] = WiFi.status() == WL_CONNECTED;
  d["uptime"] = millis() / 1000;
  String out;
  serializeJson(d, out);
  server.send(200, "application/json", out);
}

/* ================= SETUP =================== */
void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  delay(100);
  logEvent("=== ESP8266 Gateway v" + String(FW_VERSION) + " ===");

  LittleFS.begin();
  loadCfg();

  // Inicializar estado
  state.lastWifiCheck = 0;
  state.lastLoginFail = 0;
  state.loginAttempts = 0;
  state.wifiConnecting = false;
  state.sessionToken = String(ESP.getChipId(), HEX) + String(millis(), HEX);

  // ===== CONFIGURAÇÃO DE WI-FI =====
  WiFi.persistent(false);
  WiFi.disconnect(true);
  yield();

  WiFi.mode(WIFI_AP_STA);
  WiFi.hostname(cfg.device);

  const char *apSSID = strlen(cfg.ap_ssid) ? cfg.ap_ssid : "ESP8266-SETUP";
  const char *apPASS = strlen(cfg.ap_pass) >= 8 ? cfg.ap_pass : "12345678";

  WiFi.softAP(apSSID, apPASS);
  logEvent("AP iniciado: " + String(apSSID) + " / IP: " + WiFi.softAPIP().toString());
  yield();

  if (strlen(cfg.sta_ssid) > 0)
  {
    logEvent("Conectando ao Wi-Fi: " + String(cfg.sta_ssid));
    WiFi.begin(cfg.sta_ssid, cfg.sta_pass);
    state.wifiConnecting = true;

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_CONNECT_TIMEOUT)
    {
      yield();
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      state.wifiConnecting = false;
      logEvent("Wi-Fi conectado! IP: " + WiFi.localIP().toString());
    }
    else
    {
      logEvent("Timeout na conexão Wi-Fi. Continuando...");
    }
  }
  // ====================================

  MDNS.begin(cfg.device);
  logEvent("mDNS iniciado: " + String(cfg.device) + ".local");
  server.on("/", []()
            {
    if (!auth()) return;
    String p = ADMIN_HTML;
    p.replace("%DEVICE%", cfg.device);
    p.replace("%AP_SSID%", cfg.ap_ssid);
    p.replace("%AP_PASS%", cfg.ap_pass);
    p.replace("%STA_SSID%", cfg.sta_ssid);
    p.replace("%STA_PASS%", cfg.sta_pass);
    p.replace("%ADMIN_USER%", cfg.admin_user);
    p.replace("%ADMIN_PASS%", cfg.admin_pass);
    p.replace("%DHCP%", cfg.dhcp ? "checked" : "");
    p.replace("%HTTP%", cfg.http ? "checked" : "");
    p.replace("%WS%", cfg.ws ? "checked" : "");
    p.replace("%UDP%", cfg.udp ? "checked" : "");
    p.replace("%LED_STATUS%", cfg.led_status ? "checked" : "");
    server.send(200,"text/html",p); });

  server.on("/save", HTTP_POST, []()
            {
    if (!auth()) return;
    
    // Validar senhas
    String apPass = server.arg("ap_pass");
    String adminPass = server.arg("admin_pass");
    
    if (apPass.length() > 0 && apPass.length() < MIN_PASSWORD_LENGTH) {
      server.send(400, "text/plain", "Senha do AP deve ter no mínimo 8 caracteres");
      return;
    }
    
    if (adminPass.length() > 0 && adminPass.length() < MIN_PASSWORD_LENGTH) {
      server.send(400, "text/plain", "Senha admin deve ter no mínimo 8 caracteres");
      return;
    }
    
    strlcpy(cfg.device, server.arg("device").c_str(), 32);
    strlcpy(cfg.ap_ssid, server.arg("ap_ssid").c_str(), 32);
    if (apPass.length() > 0) strlcpy(cfg.ap_pass, apPass.c_str(), 32);
    strlcpy(cfg.sta_ssid, server.arg("sta_ssid").c_str(), 32);
    strlcpy(cfg.sta_pass, server.arg("sta_pass").c_str(), 32);
    strlcpy(cfg.admin_user, server.arg("admin_user").c_str(), 16);
    if (adminPass.length() > 0) strlcpy(cfg.admin_pass, adminPass.c_str(), 16);
    cfg.dhcp = server.hasArg("dhcp");
    cfg.http = server.hasArg("http");
    cfg.ws = server.hasArg("ws");
    cfg.udp = server.hasArg("udp");
    cfg.led_status = server.hasArg("led_status");
    
    saveCfg();
    logEvent("Configurações salvas. Reiniciando...");
    server.send(200,"text/plain","Salvo. Reiniciando...");
    yield();
    ESP.restart(); });

  server.on("/factory_reset", []()
            {
    if (!auth()) return;
    logEvent("AVISO: Factory reset executado!");
    server.send(200, "text/plain", "Resetando para configurações de fábrica...");
    LittleFS.remove(CFG_FILE);
    yield();
    ESP.restart(); });

  server.on("/reboot", []()
            {
    if (!auth()) return;
    logEvent("Sistema reiniciando...");
    server.send(200, "text/plain", "Reiniciando...");
    yield();
    ESP.restart(); });

  server.on("/status", status);

  if (cfg.http)
  {
    server.begin();
    logEvent("Servidor HTTP iniciado na porta 80");
  }

  if (cfg.ws)
  {
    ws.begin();
    ws.onEvent([](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
               {
      if (type == WStype_CONNECTED) {
        logEvent("WebSocket conectado: cliente #" + String(num));
      } else if (type == WStype_DISCONNECTED) {
        logEvent("WebSocket desconectado: cliente #" + String(num));
      } else if (type == WStype_TEXT) {
        String msg = String((char*)payload).substring(0, length);
        logEvent("WS recebido: " + msg);
        Serial.println(msg);
      } });
    logEvent("WebSocket iniciado na porta " + String(WS_PORT));
  }

  if (cfg.udp)
  {
    udp.begin(UDP_PORT);
    logEvent("UDP iniciado na porta " + String(UDP_PORT));
  }

  logEvent("=== Sistema pronto ===");
}

/* ================= LOOP ==================== */
void loop()
{
  checkWiFi();
  updateLED();

  if (cfg.http)
    server.handleClient();
  if (cfg.ws)
    ws.loop();

  if (cfg.udp)
  {
    int p = udp.parsePacket();
    if (p)
    {
      char buf[512]; // Buffer aumentado
      int len = udp.read(buf, 511);
      buf[len] = 0;
      logEvent("UDP de " + udp.remoteIP().toString() + ":" + String(udp.remotePort()) + " -> " + String(buf));
      Serial.println(String("UDP:") + buf);
    }
  }

  if (Serial.available())
  {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0)
    {
      logEvent("Serial->WS: " + msg);
      if (cfg.ws)
        ws.broadcastTXT(msg);
    }
  }

  MDNS.update();
  yield();
}
