#define analogPin A0
#include <util/delay.h>
//SWITCH CD4053 as 4:1 MUX
int S1=6; //A
int S2=5; //B
int S3=7;//B
int S4=8;//C
int m=1;
int k=4;
float g=1;
float ad=4.887;
double T;
unsigned long V0,V1;
String str;
boolean toginv=true;
%For Moving Average
const int numReadings = 20;
float readings[numReadings];// the readings from the analog input
int readIndex = 0;          // the index of the current reading
float total = 0;            // the running total
float average = 0.000;      // the average
float voltage = 0.000;

void regdwn(int p)
 {switch (p)
  {
   case 4:
   digitalWrite(S2,LOW);
   digitalWrite(S1,HIGH);//Regime 4 TO 3
   digitalWrite(S3,HIGH);
   k=3;
   g=0.1;
   //Serial.println("Regime Shift 4 to 3");
   break;
   case 3:
   digitalWrite(S2,HIGH);//Regime 3 TO 2
   digitalWrite(S1,LOW);
   digitalWrite(S3,LOW);
   k=2;
   g=0.01;
  //Serial.println("Regime Shift 3 to 2");
   break;
   case 2:
   digitalWrite(S2,HIGH);
   digitalWrite(S1,HIGH);//Regime 2 TO 1
   digitalWrite(S3,HIGH);
   k=1;
   g=0.001;
  // Serial.println("Regime Shift 2 to 1");
   break;
   case 1:
   Serial.println(str +" OR Low,"+ V0); 
   //Serial.println("OR Low");//current below IL
   break;
  }
}
void regup(int p)
{switch (p)
  {
   case 1://Regime 1 TO 2
   digitalWrite(S2,HIGH);
   digitalWrite(S3,LOW);
   digitalWrite(S1,LOW);
   k=2;
   g=0.01;
   //Serial.println("Regime Shift 1 to 2");
   break;
   case 2:
   digitalWrite(S2,LOW);//Regime 2 TO 3
   digitalWrite(S1,HIGH);
   digitalWrite(S3,HIGH);
   k=3;
   g=0.1;
   //Serial.println("Regime Shift 2 to 3");
   break;
   case 3:
   digitalWrite(S1,LOW);//Regime 3 TO 4
   digitalWrite(S2,LOW);
   digitalWrite(S3,LOW);
   k=4;
   g=1;
   //Serial.println("Regime Shift 3 to 4");
   break;
   case 4:
   Serial.println("OR High");//current above Ih
   return;
   break;
  }
}

void setup() 
{
 Serial.begin(115200);
 pinMode(S1, OUTPUT);// Switch 1 
 pinMode(S2, OUTPUT);// Switch 2
 pinMode(S3, OUTPUT);// Switch 2
 pinMode(S4, OUTPUT);// Switch 3
 digitalWrite(S4,toginv);
 digitalWrite(S1,LOW);
 digitalWrite(S2,LOW); 
 digitalWrite(S3,LOW);
 k=4;
 g=1;
}
void loop() 
{//delay(10); 
 proc1:
 digitalWrite(S4,toginv);
 V0=analogRead(analogPin);
 //delay(1000);
 //Serial.println(str +"ADC Intermediate Read :"+ V0 );
  if (V0<99)
  {if (V0==0 & k==1)
      {toginv = !toginv;
       if(toginv==true){m=-1;}
       else{m=1;}
      //Serial.println("Polarity shift");
      goto proc1;
      }
   else{   
   regdwn(k);
   //if(k==1){goto proc2;}
   goto proc1;}
  }
  else if(V0==1023)
  {regup(k);
   goto proc1;
  } 
  else if(99<V0<1023)
  {
  proc2:
  voltage=V0*ad*g;
  total = total - readings[readIndex];
  readings[readIndex] = voltage;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
  readIndex = 0;
  }
  average = (total/ numReadings);
//  Serial.println(str +"Im="+ average + " Range " + k + " Polarity " + m);
//  current=average*ad/(m*g);
  T=micros(); // for Profile test
  Serial.println(str + T +","+ average +","+ m); // for Profile test 
//  Serial.println(average); // for FOM
  goto proc1;}
}
  
 
