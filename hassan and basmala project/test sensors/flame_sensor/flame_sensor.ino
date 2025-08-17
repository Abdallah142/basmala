#define flame 
int Value=0;  
void setup() {
  // put your setup code here, to run once:
pinMode(flame,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Value=digitalRead(flame);
Serial.println(Value);
}
