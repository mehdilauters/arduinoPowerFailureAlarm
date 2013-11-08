int powerOnPin = 11;
int powerLevelPin = A11;
int ledPin = 5;
int phoneCommand = 7;

const float R1 = 320;
const float R2 = 320;
const float resistorFactor = 1024 / (R2/(R1 + R2));
const int referenceVolts = 5;

int val = 1;
int sign = 1;
void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(powerOnPin, INPUT);  
  pinMode(powerLevelPin, INPUT);  
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(val == 255 || val == 0)
  {
    sign = sign * -1;
  }
 analogWrite(ledPin, val); 
 int powerStatus= digitalRead(powerOnPin);
 int  powerLevel = analogRead(powerLevelPin);
 float volts = (powerLevel / resistorFactor) * referenceVolts *2 *2 ;
 Serial.print("(" +String(val)+ " )Power is "+String(powerStatus) + " level is " + String(powerLevel) + " => "); 
 Serial.println(volts);
 delay(500);
 val += sign;
}
