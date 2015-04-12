void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  String arrStr = "[50,50,50,50,50,50,50,50,50,50,50,50]";
  Serial.print(arrStr);
  delay(1000);
  if (Serial.available() > 0) {
    int incoming = Serial.read();
    if (incoming == 'f') Serial.print(0);
    if (incoming == 'b') Serial.print(1);
    if (incoming == 'l') Serial.print(2);
    if (incoming == 'r') Serial.print(3);
    if (incoming == 's') Serial.print(4);    
  }
}