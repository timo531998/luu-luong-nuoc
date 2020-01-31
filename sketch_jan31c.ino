byte statusLed    = 13;

byte sensorInterrupt1 = 0;
byte sensorPin1       = 2;

byte sensorInterrupt2 = 1;
byte sensorPin2       = 3;


float calibrationFactor = 7.5;

volatile byte pulseCount1;  
float flowRate1;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;
unsigned long oldTime1;

volatile byte pulseCount2; 
float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;
unsigned long oldTime2;

void setup() {
  Serial.begin(9600);
   pinMode(sensorPin1, INPUT);
  digitalWrite(sensorPin1, HIGH);

  pulseCount1        = 0;
  flowRate1          = 0.0;
  flowMilliLitres1   = 0;
  totalMilliLitres1  = 0;
  oldTime1           = 0;
  attachInterrupt(sensorInterrupt1, pulseCount1, FALLING);

  pinMode(sensorPin2, INPUT);
  digitalWrite(sensorPin2, HIGH);
  
  pulseCount2        = 0;
  flowRate2          = 0.0;
  flowMilliLitres2   = 0;
  totalMilliLitres2  = 0;
  oldTime2           = 0;

  attachInterrupt(sensorInterrupt2, pulseCount2, FALLING);
}
void loop() {
  delay(2000);
 
   if((millis() - oldTime1) > 1000)    
  { 
    detachInterrupt(sensorInterrupt1);

    flowRate1 = ((1000.0 / (millis() - oldTime1)) * pulseCount1 ) / calibrationFactor;
    oldTime1 = millis();
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    totalMilliLitres1 += flowMilliLitres1;
      
    unsigned int frac;
   
    Serial.print("Flow rate1: ");
    Serial.print(int(flowRate1)); 
    Serial.print("L/min");
    Serial.print("\n");     
    Serial.print("Output Liquid Quantity1: ");        
    Serial.print(totalMilliLitres1);
    Serial.println("mL"); 
    Serial.print("\t");      
    Serial.print(totalMilliLitres1/1000);
    Serial.print("L");
    Serial.print("\n");
    Serial.print("\n");
   
  
    pulseCount1 = 0;
 
    attachInterrupt(sensorInterrupt1, pulseCount1, FALLING);
    }
  
  delay (1000);
  
  if((millis() - oldTime2) > 1000)   
  { 
    detachInterrupt(sensorInterrupt2);
   
    flowRate2 = ((1000.0 / (millis() - oldTime2)) * pulseCount2) / calibrationFactor;
    oldTime2 = millis();
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    totalMilliLitres2 += flowMilliLitres2;
      
    unsigned int frac;

    Serial.print("Flow rate2: ");
    Serial.print(int(flowRate2));  
    Serial.print("L/min");
    Serial.print("\n");     
    Serial.print("Output Liquid Quantity2: ");        
    Serial.print(totalMilliLitres2);
    Serial.println("mL"); 
    Serial.print("\n");      
    Serial.print(totalMilliLitres2/1000);
    Serial.print("L");
    Serial.print("\n");
  
    pulseCount2 = 0;

    attachInterrupt(sensorInterrupt2, pulseCount2, FALLING);
  }}
 void pulseCounter1()
{
  pulseCount1++;
}
void pulseCounter2()
{
  pulseCount2++;
}
