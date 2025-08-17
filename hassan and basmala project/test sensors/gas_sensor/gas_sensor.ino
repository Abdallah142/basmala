#define gas 
int Value=0;  
void setup() {
  // put your setup code here, to run once:
pinMode(gas,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Value=analogRead(gas);
Serial.println(Value);
}
