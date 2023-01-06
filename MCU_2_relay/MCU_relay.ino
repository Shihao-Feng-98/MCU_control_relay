// Control the relay via Seeeduino XIAO 
// Note: digital low -> LED 13 ON

#define PIN_RELAY 10 

String recv_str = ""; // 0000 -> OFF, 1111 -> ON
char recv_byte;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH); // RELAY ON
  digitalWrite(LED_BUILTIN, LOW); // LED ON
  
  // baud rate: 115200
  // data bit: 8 
  // parity bit: no
  // stop bit: 1
  Serial.begin(115200, SERIAL_8N1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    while (Serial.available() > 0 )
    {
      recv_str = Serial.readStringUntil('\n');
    }
    if (recv_str.length() > 0)
    {
      if (recv_str == "1111")
      {
        digitalWrite(PIN_RELAY, HIGH); // RELAY ON
        digitalWrite(LED_BUILTIN, LOW); // LED ON
        Serial.print("Relay ON!!");
      }
      else if (recv_str == "0000")
      {
        digitalWrite(PIN_RELAY, LOW); // RELAY OFF
        digitalWrite(LED_BUILTIN, HIGH); // LED OFF
        Serial.print("Relay OFF!");
      }
      recv_str = "";
    }
  }
}
