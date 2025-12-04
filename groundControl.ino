#include <WiFi.h>
#include <WebServer.h>
#include <AccelStepper.h>

// --- WIFI ---
const char* ssid = "Prabal";
const char* password = "lucaswantsit";

IPAddress local_IP(192, 168, 1, 50);   // Desired static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// --- MOTOR PIN SETS ---
#define SET1_DIR  26
#define SET1_STEP 25
#define SET1_EN   27
#define SET2_DIR  32
#define SET2_STEP 18
#define SET2_EN   33
#define SET3_DIR  12
#define SET3_STEP 13
#define SET3_EN   14

AccelStepper driver1(AccelStepper::DRIVER, SET1_STEP, SET1_DIR);
AccelStepper driver2(AccelStepper::DRIVER, SET2_STEP, SET2_DIR);
AccelStepper driver3(AccelStepper::DRIVER, SET3_STEP, SET3_DIR);

// --- WEB SERVER ---
WebServer server(80);

// --- MOTOR SETTINGS ---
#define SPEED 300
long steps = 500;

void setup() {
  Serial.begin(115200);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Static IP config failed");
  }

  pinMode(SET1_EN, OUTPUT); digitalWrite(SET1_EN, HIGH);
  pinMode(SET2_EN, OUTPUT); digitalWrite(SET2_EN, HIGH);
  pinMode(SET3_EN, OUTPUT); digitalWrite(SET3_EN, HIGH);

  driver1.setMaxSpeed(SPEED); driver1.setAcceleration(200);
  driver2.setMaxSpeed(SPEED); driver2.setAcceleration(200);
  driver3.setMaxSpeed(SPEED); driver3.setAcceleration(200);

  driver2.setSpeed(-50);
for(int i=0;i<10;i++) driver2.runSpeed();  // tiny test movement

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP address: " + WiFi.localIP().toString());

  // Web server routes
  server.on("/", handleRoot);
  server.on("/forwardL", [](){ runOpposite(driver1, driver3, -steps); server.send(200, "text/plain", "forwardL"); });
  server.on("/forward", [](){ runOpposite(driver1, driver2, -steps); server.send(200, "text/plain", "forward"); });
  server.on("/forwardR", [](){ runOpposite(driver2, driver3, steps); server.send(200, "text/plain", "forwardR"); });
  server.on("/backwardL", [](){ runOpposite(driver2, driver3, -steps); server.send(200, "text/plain", "backwardL"); });
  server.on("/backward", [](){  runOpposite(driver1, driver2, steps); server.send(200, "text/plain", "backward"); });
  server.on("/backwardR", [](){ runOpposite(driver1, driver3, steps); server.send(200, "text/plain", "backwardR"); });
  server.on("/Clock", [](){ runAll(-steps); server.send(200, "text/plain", "Clock"); });
  server.on("/Anti", [](){ runAll(steps); server.send(200, "text/plain", "Anti"); });
  server.on("/stop", [](){ stopAll(); server.send(200, "text/plain", "Stop"); });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}

// --- WEB PAGE ---
void handleRoot() {
  String html = "<html><body>"
                "<h1>Ballbot Remote</h1>"
                "<table>"
                "<tr>"
                "<td><button onclick=\"location.href='/forwardL'\">F-L</button></td>"
                "<td><button onclick=\"location.href='/forward'\">F</button></td>"
                "<td><button onclick=\"location.href='/forwardR'\">F-R</button></td>"
                "</tr>"
                "<tr>"
                "<td><button onclick=\"location.href='/backwardL'\">B-L</button></td>"
                "<td><button onclick=\"location.href='/backward'\">B</button></td>"
                "<td><button onclick=\"location.href='/backwardR'\">B-R</button></td>"
                "</tr>"
                "<tr>"
                "<td><button onclick=\"location.href='/Clock'\">Clock</button></td>"
                "<td><button onclick=\"location.href='/Anti'\">Anti</button></td>"
                "</tr>"
                "<tr><td colspan='3'><button onclick=\"location.href='/stop'\">Stop</button></td></tr>"
                "</table>"
                "</body></html>";
  server.send(200, "text/html", html);
}

// --- MOTOR FUNCTIONS ---
void runOpposite(AccelStepper &mA, AccelStepper &mB, long steps) {
  mA.move(steps);
  mB.move(-steps);
  while(mA.distanceToGo() != 0 || mB.distanceToGo() != 0) {
    if(mA.distanceToGo() != 0) mA.run();
    if(mB.distanceToGo() != 0) mB.run();
  }
}

void runAll(long steps) {
  driver1.move(steps);
  driver2.move(steps);
  driver3.move(steps);
  while(driver1.distanceToGo() != 0 || driver2.distanceToGo() != 0 || driver3.distanceToGo() != 0) {
    if(driver1.distanceToGo() != 0) driver1.run();
    if(driver2.distanceToGo() != 0) driver2.run();
    if(driver3.distanceToGo() != 0) driver3.run();
  }
}

void stopAll() {
  driver1.stop(); driver2.stop(); driver3.stop();
}

