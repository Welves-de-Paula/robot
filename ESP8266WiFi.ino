#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// WiFi credentials
const char *ssid = "ROBOT";
const char *password = "APP12345678";

// Server parameters
WiFiServer server(80);
String header;

// Motor control pins
const int enA = 5;   // PWM pin for motor A speed control
const int in1A = 14; // digital pin for motor A direction control 1
const int in2A = 12; // digital pin for motor A direction control 2
const int enB = 4;   // PWM pin for motor B speed control
const int in3B = 0;  // digital pin for motor B direction control 1
const int in4B = 2;  // digital pin for motor B direction control 2

void setup()
{
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  // Start the server
  server.begin();
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Wait for the client to send a request
  while (!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Parse the request to get the joystick data
  int x1 = req.substring(req.indexOf("x1=") + 3, req.indexOf("&y1=")).toInt();
  int y1 = req.substring(req.indexOf("y1=") + 3, req.indexOf("&x2=")).toInt();
  int x2 = req.substring(req.indexOf("x2=") + 3, req.indexOf("&y2=")).toInt();
  int y2 = req.substring(req.indexOf("y2=") + 3).toInt();

  // Map joystick values to motor speeds
  int speedA = map(y1, 0, 1023, -255, 255);
  int speedB = map(y2, 0, 1023, -255, 255);

  // Map joystick values to motor directions
  int dirA = (x1 > 500) ? HIGH : LOW;
  int dirB = (x2 > 500) ? HIGH : LOW;

  // Set the motor directions
  digitalWrite(in1A, dirA);
  digitalWrite(in2A, !dirA);
  digitalWrite(in3B, dirB);
  digitalWrite(in4B, !dirB);

  // Set the motor speeds
  analogWrite(enA, abs(speedA));
  analogWrite(enB, abs(speedB));

  // Send a response to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  // Create the web interface with two joysticks
  client.println("<head><meta name=" viewport " content=" width = device - width, initial - scale = 1.0 "><title>Robot Control Interface</title>");
client.println("<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script>");
client.println("<script>");
client.println("$(document).ready(function() {");
client.println("var touchSupported = ('ontouchstart' in window);");
client.println("var joystick1 = createJoystick('joystick1', {size: 200, threshold: 0.1, lockX: true, lockY: false});");
client.println("var joystick2 = createJoystick('joystick2', {size: 200, threshold: 0.1, lockX: true, lockY: false});");
client.println("if (touchSupported) {");
client.println("$('#joystick1').on('touchstart', function(e) {e.preventDefault(); joystick1.start(e.originalEvent.touches[0]);});");
client.println("$('#joystick1').on('touchmove', function(e) {e.preventDefault(); joystick1.move(e.originalEvent.touches[0]);});");
client.println("$('#joystick1').on('touchend', function(e) {e.preventDefault(); joystick1.end();});");
client.println("$('#joystick2').on('touchstart', function(e) {e.preventDefault(); joystick2.start(e.originalEvent.touches[0]);});");
client.println("$('#joystick2').on('touchmove', function(e) {e.preventDefault(); joystick2.move(e.originalEvent.touches[0]);});");
client.println("$('#joystick2').on('touchend', function(e) {e.preventDefault(); joystick2.end();});");
client.println("} else {");
client.println("$('#joystick1').on('mousedown', function(e) {e.preventDefault(); joystick1.start(e.originalEvent);});");
client.println("$('#joystick1').on('mousemove', function(e) {e.preventDefault(); joystick1.move(e.originalEvent);});");
client.println("$('#joystick1').on('mouseup', function(e) {e.preventDefault(); joystick1.end();});");
client.println("$('#joystick2').on('mousedown', function(e) {e.preventDefault(); joystick2.start(e.originalEvent);});");
client.println("$('#joystick2').on('mousemove', function(e) {e.preventDefault(); joystick2.move(e.originalEvent);});");
client.println("$('#joystick2').on('mouseup', function(e) {e.preventDefault(); joystick2.end();});");
client.println("}");
client.println("function createJoystick(id, options) {");
client.println("var element = $('#' + id);");
client.println("var joystick = {");
client.println("dx: 0,");
client.println("dy: 0,");
client.println("startX: 0,");
client.println("startY: 0,");
client.println("threshold: options.threshold || 0,");
client.println("lockX: options.lockX || false,");
client.println("lockY: options.lockY || false,");
client.println("size: options.size || 100,");
client.println("start: function(event) {");
client.println("event.preventDefault();");
client.println("var offset = element.offset();");
client.println("this.startX = event.pageX -offset.left - (this.size / 2);");
client.println("this.startY = event.pageY - offset.top - (this.size / 2);");
client.println("},");
client.println("move: function(event) {");
client.println("event.preventDefault();");
client.println("var offset = element.offset();");
client.println("var x = event.pageX - offset.left - (this.size / 2);");
client.println("var y = event.pageY - offset.top - (this.size / 2);");
client.println("var distance = Math.sqrt((x * x) + (y * y));");
client.println("if (distance > this.size / 2) {");
client.println("x = x * (this.size / 2) / distance;");
client.println("y = y * (this.size / 2) / distance;");
client.println("distance = this.size / 2;");
client.println("}");
client.println("this.dx = x;");
client.println("this.dy = y;");
client.println("if (this.lockX) {");
client.println("this.dx = 0;");
client.println("}");
client.println("if (this.lockY) {");
client.println("this.dy = 0;");
client.println("}");
client.println("if (distance < this.threshold * (this.size / 2)) {");
client.println("this.dx = 0;");
client.println("this.dy = 0;");
client.println("}");
client.println("sendJoystickData();");
client.println("},");
client.println("end: function() {");
client.println("this.dx = 0;");
client.println("this.dy = 0;");
client.println("sendJoystickData();");
client.println("}");
client.println("};");
client.println("return joystick;");
client.println("}");
client.println("function sendJoystickData() {");
client.println("var dx1 = joystick1.dx.toFixed(2);");
client.println("var dy1 = joystick1.dy.toFixed(2);");
client.println("var dx2 = joystick2.dx.toFixed(2);");
client.println("var dy2 = joystick2.dy.toFixed(2);");
client.println("var url = '/joystick?dx1=' + dx1 + '&dy1=' + dy1 + '&dx2=' + dx2 + '&dy2=' + dy2;");
client.println("$.get(url);");
client.println("}");
client.println("</script>");
client.println("</head>");
client.println("<body>");
client.println("<h1>Robot Control Interface</h1>");
client.println("<div id="joystick1" style="width: 200px; height: 200px; background-color: #DDD; border-radius: 100px; display: inline-block; margin-right: 50px;"></div>");
client.println("<div id="joystick2" style="width: 200px; height: 200px; background-color: #DDD; border-radius: 100px; display: inline-block;"></div>");
client.println("</body>");
client.println("</html>");
}

// Function to handle the joystick data
void handleJoystickData()
{
// Get the joystick data from the URL parameters
float dx1 = server.arg("dx1").toFloat();
float dy1 = server.arg("dy1").toFloat();
float dx2 = server.arg("dx2").toFloat();
float dy2 = server.arg("dy2").toFloat();

// Calculate the motor speeds based on the joystick data
int motor1Speed = int(dy1 * 255);
int motor2Speed = int(dy1 * 255);
int motor3Speed = int(dy2 * 255);
int motor4Speed = int(dy2 * 255);

// Send the motor speeds to the motor shield
if (dy1 > 0 && dy2 > 0)
{
  // Move forward
  motorShield.setMotorSpeeds(motor1Speed, motor2Speed, motor3Speed, motor4Speed);
}
else if (dy1 < 0 && dy2 < 0)
{
  // Move backward
  motorShield.setMotorSpeeds(-motor1Speed, -motor2Speed, -motor3Speed, -motor4Speed);
}
else if (dy1 > 0 && dy2 < 0)
{
  // Turn right
  motorShield.setMotorSpeeds(motor1Speed, -motor2Speed, motor3Speed, -motor4Speed);
}
else if (dy1 < 0 && dy2 > 0)
{
  // Turn left
  motorShield.setMotorSpeeds(-motor1Speed, motor2Speed, -motor3Speed, motor4Speed);
}
else
{
  // Stop
  motorShield.setMotorSpeeds(0, 0, 0, 0);
}

// Send a response back to the client
server.send(200, "text/plain", "OK");
}