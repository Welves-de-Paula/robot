# ESP8266 Bridge Mode - Protocolo de Comunicação

## Visão Geral

O ESP8266 funciona como uma **ponte (bridge)** entre a rede WiFi/Internet e o Arduino UNO. Todas as requisições HTTP e mensagens WebSocket são redirecionadas para o Arduino via comunicação Serial.

## Arquitetura

```
[Cliente HTTP/WS] <--WiFi--> [ESP8266] <--Serial--> [Arduino UNO]
                              (Porta 80/81)         (115200 baud)
```

## Protocolo Serial

### Formato de Mensagem

Todas as mensagens são trocadas em formato **JSON**, terminadas com `\n` (newline).

### ESP8266 → Arduino (Comando)

```json
{
  "protocol": "http|ws",
  "method": "GET|POST|PUT|DELETE|message",
  "uri": "/api/path",
  "body": "conteúdo opcional"
}
```

**Campos:**

- `protocol`: Tipo de protocolo (`http` para HTTP, `ws` para WebSocket)
- `method`: Método HTTP (GET, POST, etc.) ou "message" para WebSocket
- `uri`: Caminho da requisição HTTP
- `body`: Corpo da requisição (opcional, pode ser JSON string)

**Exemplos:**

HTTP GET:

```json
{ "protocol": "http", "method": "GET", "uri": "/api/status", "body": "" }
```

HTTP POST:

```json
{
  "protocol": "http",
  "method": "POST",
  "uri": "/api/led",
  "body": "{\"state\":true}"
}
```

WebSocket:

```json
{
  "protocol": "ws",
  "method": "message",
  "uri": "",
  "body": "{\"action\":\"toggle\"}"
}
```

### Arduino → ESP8266 (Resposta)

```json
{
  "success": true|false,
  "data": "dados da resposta",
  "error": "mensagem de erro (se success=false)"
}
```

**Campos:**

- `success`: Indica se o comando foi executado com sucesso
- `data`: Dados de retorno (qualquer tipo JSON)
- `error`: Mensagem de erro (presente apenas se `success=false`)

**Exemplos:**

Resposta de sucesso:

```json
{ "success": true, "led": "ON", "sensor": 512, "uptime": 123456 }
```

Resposta de erro:

```json
{ "success": false, "error": "Route not found" }
```

### Arduino → ESP8266 (Broadcast)

Para enviar mensagens para **todos** os clientes WebSocket conectados:

```json
{
  "broadcast": true,
  "data": "dados para broadcast"
}
```

**Exemplo:**

```json
{ "broadcast": true, "sensor": 512, "temperature": 25.5 }
```

## Endpoints Disponíveis

### Rotas do ESP8266 (gerenciamento)

Estas rotas são tratadas diretamente pelo ESP8266:

- `GET /` - Página inicial
- `GET /config.html` - Configurações do ESP8266
- `GET /logs.html` - Logs HTTP
- `GET /api/config` - Obter configurações
- `POST /api/config` - Salvar configurações
- `GET /api/wifi/scan` - Escanear redes WiFi
- `GET /api/wifi/status` - Status da conexão WiFi
- `GET /api/logs` - Obter logs
- `DELETE /api/logs` - Limpar logs

### Rotas Redirecionadas ao Arduino

Todas as rotas com prefixo `/api/arduino` ou rotas não reconhecidas com `/api/` são redirecionadas ao Arduino:

- `ANY /api/arduino/*` - Redireciona para o Arduino
- `ANY /api/*` (não reconhecida) - Tenta redirecionar ao Arduino

**Exemplos:**

```bash
GET  /api/arduino/status
POST /api/arduino/led
GET  /api/arduino/sensor
POST /api/custom/endpoint
```

## WebSocket

### Conexão

```javascript
const ws = new WebSocket("ws://esp-gateway.local:81");

ws.onopen = () => {
  console.log("WebSocket conectado");
};

ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log("Recebido:", data);
};

// Enviar comando
ws.send(JSON.stringify({ action: "toggle" }));
```

### Eventos WebSocket

O ESP8266 notifica o Arduino sobre eventos de conexão:

**Conexão:**

```json
{ "ws": "connect", "client": 0, "ip": "192.168.4.2" }
```

**Desconexão:**

```json
{ "ws": "disconnect", "client": 0 }
```

## Timeout

O ESP8266 aguarda **5 segundos** (5000ms) pela resposta do Arduino. Se não receber resposta, retorna:

```json
{ "error": "Arduino timeout", "success": false }
```

## Exemplo de Implementação - Arduino

```cpp
void handleHttpCommand(const char* method, const char* uri, const char* body) {
  StaticJsonDocument<512> response;

  if (strcmp(uri, "/api/arduino/status") == 0) {
    response["success"] = true;
    response["uptime"] = millis();
    response["sensor"] = analogRead(A0);
  }

  serializeJson(response, Serial);
  Serial.println();
}
```

## Exemplo de Cliente HTTP

```bash
# Obter status do Arduino
curl http://esp-gateway.local/api/arduino/status

# Controlar LED
curl -X POST http://esp-gateway.local/api/arduino/led \
  -H "Content-Type: application/json" \
  -d '{"state":true}'
```

## Exemplo de Cliente JavaScript

```javascript
// HTTP
fetch("/api/arduino/status")
  .then((response) => response.json())
  .then((data) => console.log(data));

// WebSocket
const ws = new WebSocket("ws://esp-gateway.local:81");
ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log("Sensor:", data.sensor);
};
```

## Requisitos

### ESP8266

- Biblioteca: `ArduinoJson`
- Biblioteca: `WebSocketsServer` (ESP8266WebSockets)

### Arduino UNO

- Biblioteca: `ArduinoJson`
- Conexão Serial: TX → RX (ESP), RX → TX (ESP)
- Baudrate: 115200

## Pinout de Conexão

```
ESP8266          Arduino UNO
-------          -----------
TX (GPIO1)  <->  RX (D0)
RX (GPIO3)  <->  TX (D1)
GND         <->  GND
```

**ATENÇÃO:** Durante o upload de código para o Arduino, **desconecte** os fios TX/RX temporariamente.

## Vantagens do Modo Bridge

1. **Separação de responsabilidades**: ESP8266 cuida da rede, Arduino cuida da lógica
2. **Todos os protocolos suportados**: HTTP, WebSocket, etc.
3. **Flexibilidade**: Arduino pode processar qualquer tipo de comando
4. **Escalabilidade**: Fácil adicionar novos endpoints no Arduino
5. **Real-time**: WebSocket permite comunicação bidirecional em tempo real
