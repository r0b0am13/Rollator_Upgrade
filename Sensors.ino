
//Threshold Variables and Definitions
#define dis_arrlen 5 // Sets the averaging size of previous distance values for smoother transitions and to avoid random errors
#define vol_arrlen 5 // Sets the averaging size of previous voltage values for smoother transitions and to avoid random errors

#define OutputPin 13 // Trigger Pin of Ultrasonic Sensor 1 

#define SensorPin1 A0 // Sensor Pin of Force Sensor 1
#define SensorPin2 A5 // Sensor Pin of Force Sensor 2

const int trigPin1 = 9; // Trigger Pin of Ultrasonic Sensor 1 
const int echoPin1 = 10; // Echo Pin of Ultrasonic Sensor 1
const int trigPin2 = 5;// Trigger Pin of Ultrasonic Sensor 2
const int echoPin2 = 6; // Echo Pin of Ultrasonic Sensor 2

const int distance_threshold1 = 50;
const int distance_threshold2 = 50;
const float voltage_threshold1 = 1.0;
const float voltage_threshold2 = 0;
const int pulse_delay = 100;
//Code Variables
float distance1=0;
float distance2=0;
float voltage2=0;
float voltage1=0;
bool isClose;
bool isHold;
bool Brakes;

void setup() {
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT);
  pinMode(OutputPin,OUTPUT); 
  Serial.begin(9600); 
}
void loop() {
  
  isClose=DistanceThreshold(distance_threshold1,distance_threshold2);
  isHold=VoltageThreshold(voltage_threshold1,voltage_threshold2);
  distance1 = avgDistance1();
  voltage2 = avgVoltage2();
  distance2 = avgDistance2();
  voltage1 = avgVoltage1();

  PrintValues(0);
  PrintStatus(1);
  
  delay(pulse_delay);

}



bool DistanceThreshold(int distance_threshold1,int distance_threshold2){
  return((avgDistance1()<distance_threshold1) && (avgDistance2()<distance_threshold2));
}
bool VoltageThreshold(int voltage_threshold1,int voltage_threshold2){
  return((avgVoltage1()>voltage_threshold1) && (avgVoltage2()>voltage_threshold2));
}
float avgDistance1(){
  static int dis1[dis_arrlen]= {0};
  for(char i = dis_arrlen-1; i >= 1 ; i--) dis1[i]=dis1[i-1];

  dis1[0]=measureDistance(trigPin1,echoPin1);

  float distance = 0;
  for(char i = 0; i<dis_arrlen; i++){
    distance+=float(dis1[i])/dis_arrlen;
  }
  return distance;
}
float avgVoltage1(){

  static float vol1[vol_arrlen]= {0};
  for(char i = vol_arrlen-1; i >= 1 ; i--) vol1[i]=vol1[i-1];

  vol1[0]=analogRead(SensorPin1) * (5.0 / 1023.0);

  float voltage = 0;
  for(char i = 0; i<vol_arrlen; i++){
    voltage+=float(vol1[i])/vol_arrlen;
  }
  return voltage;
}
float avgDistance2(){
  static int dis2[dis_arrlen]= {0};
  for(char i = dis_arrlen-1; i >= 1 ; i--) dis2[i]=dis2[i-1];

  dis2[0]=measureDistance(trigPin2,echoPin2);

  float distance = 0;
  for(char i = 0; i<dis_arrlen; i++){
    distance+=float(dis2[i])/dis_arrlen;
  }
  return distance;
}
float avgVoltage2(){

  static float vol2[vol_arrlen]= {0};
  for(char i = vol_arrlen-1; i >= 1 ; i--) vol2[i]=vol2[i-1];

  vol2[0]=analogRead(SensorPin2) * (5.0 / 1023.0);

  float voltage = 0;
  for(char i = 0; i<vol_arrlen; i++){
    voltage+=float(vol2[i])/vol_arrlen;
  }
  return voltage;
}
int measureDistance(int trigPin, int echoPin){
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  return duration * 0.034 / 2;
}

void PrintValues(bool newline){
  Serial.print(distance1);
  Serial.print("cm ");
  Serial.print(distance2);
  Serial.print("cm ");
 
  Serial.print(voltage1);
  Serial.print("V ");
  Serial.print(voltage2);
  Serial.print("V ");
  if(newline)
  Serial.println("");
  else
  Serial.print("");
}

void PrintStatus(bool newline){
  Serial.print(isClose ? "CLOSE" : "FAR");
  Serial.print(" ");
  Serial.print(isHold ? "HOLDING" : "NOT_HOLDING");
  Serial.print(" ");
  Serial.print(!(isHold&&isClose) ? "BRAKES_ON" : "BRAKES_OFF");
  if(newline)
  Serial.println("");
  else
  Serial.print("");
}
