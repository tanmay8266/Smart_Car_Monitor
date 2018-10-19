
// * Connections for Communication:
// * ESP01   ->  Arduino
// * Vcc     ->  3.3V
// * GND     ->  GND
// * TxD     ->  Rx1 (Pin 19)
// * RxD     ->  Tx1 (Pin 18)
// * CH_PD   ->  3.3V 
String apiKey = "JGKG9JJ3IZO6IPR1";  //Change this key to your "Write API key"
int incomingByte=0;
void setup() {
  Serial.begin(9600);      // PC Arduino Serial Monitor
  Serial1.begin(115200);   // Arduino to ESP01 Communication
  connectWiFi();  // To connect to Wifi
 } 

void loop() {
  int SENSOR_VALUE=analogRead(A0); 
  int SENSOR_VALUE1=analogRead(A1);
  if (SENSOR_VALUE >=650)
  {
    SENSOR_VALUE=1;
  }
  else{
    SENSOR_VALUE=0;
  }
  SENSOR_VALUE1=(SENSOR_VALUE1/1023.0)*100;
  Serial1.println("AT+CIPMUX=0\r\n");      // To Set MUX = 0
  delay(1000);                             // Wait for 1 sec
  // TCP connection 
  String cmd = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n\r\n";   // TCP connection with https://thingspeak.com server
  Serial1.println(cmd);                    // Display above Command on PC
  Serial.println(cmd);                     // Send above command to Rx1, Tx1
  delay(1000);                            // Wait for 1 Sec
  if(Serial1.find("ERROR"))                // If returns error in TCP connection
  { 
    Serial.println("AT+CIPSTART error");   // Display error msg to PC 
  }
  
  // prepare GET string 
  String getStr = "GET /update?api_key="+ apiKey +"&field1="+ SENSOR_VALUE+ "&field2="+SENSOR_VALUE1+"\r\n\r\n"; // Add apiKey and Sensor Value
  Serial.println(getStr);                 // Display GET String on PC
  cmd = "AT+CIPSEND="+String(getStr.length())+"\r\n";                    // send data length 
  Serial.println(cmd);                   // Display Data length on PC
  Serial1.println(cmd);                  // Send Data length command to Tx1, Rx1
  delay(3000);                           // wait for 3 sec
  if(Serial1.find(">"))                  // If prompt opens //verify connection with cloud
  {
    Serial.println("connected to Cloud");  // Display confirmation msg to PC
    Serial1.print(getStr);                 // Send GET String to Rx1, Tx1
  }
  else
  { 
    Serial1.println("AT+CIPCLOSE\r\n");    // Send Close Connection command to Rx1, Tx1
    Serial.println("AT+CIPCLOSE");         // Display Connection closed command on PC
  } 
  // thingspeak free version needs 16 sec delay between updates 
  delay(16000);                            // wait for 16sec

 }
boolean connectWiFi() {               // Connect to Wifi Function
  Serial1.println("AT+CWMODE=1\r\n"); // Setting Mode = 1 
  delay(1000);                         // wait for 100 mSec
  
  String cmd = "AT+CWJAP=\"Mini\",\"Jumbo@264\"\r\n";         // Connect to WiFi
  Serial.println(cmd);                // Display Connect Wifi Command on PC
  Serial1.println(cmd);               // send Connect WiFi command to Rx1, Tx1 
  delay(5000);                       // wait for 10 sec
  Serial1.println("AT+CWJAP?");       // Verify Connected WiFi
  if(Serial1.find("+CWJAP"))        
  {
    Serial.println("OK, Connected to WiFi.");         // Display Confirmation msg on PC
    return true;
  }
  else
  {
    Serial.println("Can not connect to the WiFi.");   // Display Error msg on PC
    return false;
  }
}
