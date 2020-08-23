#include <WiFi.h>

// yes I know the ssid and password is on github, I changed to fake value so enjoy lil hacker bots
const char* ssid = "Newstone";
const char* password = "1122334455";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

String controlPanelHTML;
String controlPanelStaticHeader;
unsigned long controlPanelLastGenerated = 0;
//ms interval that the arduino should generate a new control panel with updated metrics
#define MS_GENERATE_CONTROL_PANEL 1000
void generateControlPanel();
void handleRequest(String header);
void generateStaticControlPanelHeader();



// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void webServerSetup() {
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  prntln("WiFi connected.");
  prntln("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  generateStaticControlPanelHeader();
}

void webServerLoop(){
  if ((millis() - controlPanelLastGenerated) > MS_GENERATE_CONTROL_PANEL || controlPanelLastGenerated == 0) {
    controlPanelLastGenerated = millis();
    generateControlPanel();
  }
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    prntln("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        #ifdef DEBUG4
          Serial.write(c);                    // print it out the serial monitor
        #endif
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            handleRequest(header);
            client.println(controlPanelHTML);
            // The HTTP response ends with another blank line
            client.println();
            
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    prntln("Client disconnected.");
  }
}
void generateControlPanel() {
  #ifdef DEBUG 3
    prntln("Generating new control Panel");
  #endif
  controlPanelHTML = controlPanelStaticHeader
+String("<div class=\"row\"><div class=\"col-xs-12\"><div class=\"btn-group btn-group-lg\" role=\"group\" aria-label=\"Control Panel\">");
if (currentDisplayMode == off) {
  controlPanelHTML +=String("<a href=\"/mode/on\"><button type=\"button\" class=\"btn btn-success\">TURN ON</button></a>");
} else {
  controlPanelHTML +=String("<a href=\"/mode/off\"><button type=\"button\" class=\"btn btn-danger\">TURN OFF</button></a>");
} 
controlPanelHTML +=String("<a href=\"/mode/change\"><button type=\"button\" class=\"btn btn-primary\">CHANGE MODE</button></a>")
+String("<a href=\"/routine/up\"><button type=\"button\" class=\"btn btn-primary\">NEXT ROUTINE</button></a>")
+String("<a href=\"/routine/down\"><button type=\"button\" class=\"btn btn-primary\">PREVIOUS ROUTINE</button></a>")
+String("<a href=\"/brightness/up\"><button type=\"button\" class=\"btn btn-primary\">BRIGHTNESS UP</button></a>")
+String("<a href=\"/brightness/down\"><button type=\"button\" class=\"btn btn-primary\">BRIGHTNESS DOWN</button></a>")
+String("</div></div></div>")
+String("<div class=\"row\"><table class=\"table table-dark\"><thead><tr><th scope=\"col\">Setting</th><th scope=\"col\">Status</th></tr></thead><tbody><tr><td>Mode</td><td>")
+String(getDisplayModeText())
+String("</td></tr><tr><td>Displaying Routine</td><td>#")
+String(getCurrentPatternNumber())
+String(": ")
+String(routineNames[getCurrentPatternNumber()])
+String("</td></tr>")
+String("<tr><td>Brightness (0-10)</td><td>")
+String(brightness)
+String("</td></tr>")
+String("<tr style=\"color:rgb(")
+String(color[0])
+String(",")
+String(color[1])
+String(",")
+String(color[2])
+String(");\"><td>Selected Color</td><td>(")
+String(color[0])
+String(",")
+String(color[1])
+String(",")
+String(color[2])
+String(")</td></tr>")
+String("<tr><td>Motion</td><td>");
if (motionDetected) {
  controlPanelHTML += String("Motion Detected");
} else {
  controlPanelHTML +=String("No Motion for ")
  +String(((millis() - motionLastDetected)/3600000)/1000)
  +String("h ")
  +String((((millis() - motionLastDetected)%3600000)/60)/1000)
  +String("m " )
  +String(((millis() - motionLastDetected)%60000)/1000)
  +String("s ");
}
controlPanelHTML += String("</td></tr>")
+String("</tbody></table></div>")
+String("</div></body></html>");
}

void handleRequest(String header) {
  if (header.indexOf("GET /mode/on") >= 0) {
    prntln("turning on");
    if (currentDisplayMode == off) { 
      currentDisplayMode = previousDisplayMode; 
    }
  } else if (header.indexOf("GET /mode/off") >= 0) {
    prntln("turning off");
    if (currentDisplayMode != off) { 
      previousDisplayMode = currentDisplayMode; 
      currentDisplayMode = off; 
     }
  } else if (header.indexOf("GET /mode/change") >= 0) {
    prntln("Change Mode");
    toggleDisplayMode();
  } else if (header.indexOf("GET /routine/up") >= 0) {
    prntln("Next Routine");
    nextPattern();
  } else if (header.indexOf("GET /routine/down") >= 0) {
    prntln("Previous Routine");
    previousPattern();
  } else if (header.indexOf("GET /brightness/up") >= 0) {
    prntln("Brightness up");
    brightnessUp();
  } else if (header.indexOf("GET /brightness/down") >= 0) {
    prntln("Brightness down");
    brightnessDown();
  } else {
    //if we got here, request not recognized
    return;
  }
  //generate new control panel since something changed
  generateControlPanel();
}

void generateStaticControlPanelHeader() {
  controlPanelStaticHeader = String("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">")
  +String("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}")
  +String(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;")
  +String("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}")
  +String(".button2 {background-color: #555555;}</style>")
  +String("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" crossorigin=\"anonymous\"></head>")
  
  // Web Page Heading
  +String("<body><div class=\"container\"><div class=\"row\"><div class=\"col-xs-12 text-center\"><h1>LED Kommander</h1></div></div>");
}

