#define water_level
int Value=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(water_level,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Value=digitalRead(water_level);
Serial.println(Value);
 }
