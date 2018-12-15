
#include <Keyboard.h>
#include <Chrono.h>
//#include <conio.h>
//#include <LightChrono.h>
//#include <Filters.h>
//#include <microsmooth.h>


#include "RunningMedian.h"
// Sensor pin - GND
// Sensor pin - Analog In 0, with 10K resistor to +5V

Chrono myChrono, myChrono2, myChrono3 , chronoCalib;
int enablePin = 9;
int in1Pin = 3;
int in2Pin = 8;
int ledPin = 13;
int SensorPin1 = A0 , SensorPin2 = A1 ;// Sensor connected to analog pin A0
int positionPin = A2; //motor position pin
int i = 0, buttonState = 0, motorPin1 = 8, motorPin2 = 7;
double sum1 = 0.0, avg1 = 0, sum2 = 0, avg2 = 0;
double a = 0, b = 0 , c = 0, d = 0;// used for motor not reaching issue
int calibTime = 5000;

int minPosn = 200, maxPosn = 600; //variables for motorpositions
double rawNew1, rawNew2, filtered1 = 0, filtered2 = 0, alpha = 0.1, filteredMapped, sensorMaximum,sensorMinimum , startTime = 0, Th = 8 ;
double mapPotandSen = 0;
bool track = true, track2 = true , flag = true;
double findMax( double );
double findMin( double );

void setup()
{
  // initialize serial communications
  Serial.begin(9600);
//  Keyboard.begin();
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(SensorPin1 , INPUT);
  pinMode(SensorPin2 , INPUT);
  pinMode(positionPin , INPUT);
  pinMode(ledPin , OUTPUT);
  digitalWrite(enablePin, HIGH);
//  centerMotor(400);
}

void loop()
{
 
//Serial.println (c);
  // read the voltage from the voltage divider (sensor plus resistor)
  //digitalWrite(enablePin, HIGH);
  int sensor1 = analogRead(SensorPin1);
  int sensor2 = analogRead(SensorPin2);
  //int sensor = sensor1+sensor2;

  if (i < 1000)
  {
    sum1 = sum1 + sensor1;
    //Serial.println (sum);
    sum2 = sum2 + sensor2;
    i++;
  }

  else if (i == 1000)
  {
    avg1 = sum1 / 1000.0;
    avg2 = sum2 / 1000.0;
    //Serial.print("average is");
    //Serial.println (avg);
    //Serial.println("Noise done");
    delay(2000);
    digitalWrite(ledPin , HIGH);
    delay (1000);
    digitalWrite(ledPin , LOW);
    
    i = 1005;
    chronoCalib.restart();
  }

  else if ( i == 1005)
  {
    rawNew1 = (sensor1 - avg1);
    rawNew2 = (sensor2 - avg2);
    //Serial.print("raw value");

    filtered1 = (1 - alpha) * filtered1 + alpha * rawNew1;
    filtered2 = (1 - alpha) * filtered2 + alpha * rawNew2;
    //////////////// calibaration to set maximum and minimum/////////////////////////////////
    if(chronoCalib.elapsed() < calibTime)
    {
     int ledInterval = chronoCalib.elapsed();
     if ( (ledInterval % 200) > 0 && (ledInterval % 200)  < 50)
     {
      digitalWrite(ledPin , HIGH);
     } 
     else 
     {
      digitalWrite(ledPin , LOW);
     }
     
     sensorMaximum = findMax(filtered1+filtered2);
     sensorMinimum = findMin(filtered1+filtered2);
      
    }

    //    sample1.add(rawNew1);
    //    sample2.add(rawNew2);
    //Serial.print(rawNew);
    /*Serial.print(sample1.getMedian());
      Serial.print(",");
      Serial.print(sample2.getMedian());
      Serial.print(",");*/
    //Serial.println((sample1.getMedian()+ sample1.getMedian()))
  

        Serial.print(filtered1);
        Serial.print( ",");
        Serial.print(filtered2);
        Serial.print( ",");
        Serial.println(filtered2+filtered1);
    //thresholdCheck (5 ,30, filtered1 , filtered1+filtered2);
    //    Serial.print( ",");
    //Serial.println(slopeSensor((sample1.getMedian()+ sample1.getMedian())));
    //Serial.println(slopeSensor(filtered2+filtered1));
//    else
//    {
//     
//    mapPotandSen = map (filtered1+filtered2 , sensorMinimum, sensorMaximum, minPosn, maxPosn);
//    
//      if (mapPotandSen > maxPosn)
//        {
//          mapPotandSen = maxPosn;
//        }
//      if (mapPotandSen <minPosn)
//        {
//          mapPotandSen =minPosn;
//        }
//      Serial.print(sensorMaximum);
//      Serial.print( ",");
//      Serial.println(sensorMinimum);
       
    //Serial.println(mapPotandSen);
    //    if ((mapPotandSen > minPosn) && (mapPotandSen < maxPosn))
    //    {
    //
//        categoriseSlopeRunMotor(slopeSensor(filtered1+filtered2), -2 , 5 , 25, 25);
    //    }
    
    //Serial.println(analogRead(positionPin));
    //findMax (filtered);
    //clickCheck (25 , filtered , 1000);
    //delay(10);
//    }




  //  if (buttonState == HIGH)
  //  {
  //    i = 0;
  //    sum1 = 0;
  //    sum2 = 0;
  //    avg1 = 0;
  //    avg2 = 0;
  //    flag= true;
  //    centerMotor(410);
  //
  //
  //  }
// }

}



////////////////////////////////////////////////////////////////////functions used///////////////////////////////////////////////////////////////////////////////////////////////

double findMax( double x)
{
  static double maxi = x;
  
  if (x > maxi)
  {
    maxi = x;

  }
  
  return maxi;
}

double findMin( double x)
{
  static double mini = x;
  if (x< mini)
  {
    x= mini;
  }
  return mini;
}

//void thresholdCheck( double lower, double higher, double channel_1 , double channel_12) // just checks the threshold
//{
//  if ( channel_1 > lower)
//  {
//    //Serial.println("run the motor");
//    digitalWrite(in1Pin ,HIGH);
//    digitalWrite(in2Pin ,LOW);
//  }
//   if (channel_12 > higher)
//  {
//    //Serial.println("reverse the motor");
//    in1Pin = 8;
//    in2Pin=4;
//  }
//  if (channel_1 < lower)
//  {
//    digitalWrite(in1Pin ,LOW);
//    digitalWrite(in2Pin ,LOW);
//    //Serial.println("stop the motor");
//  }
//}






void updateSensorValue( void )
{
  //rawNew = (analogRead(SensorPin1) - avg);
  //filtered = (1 - alpha) * filtered + alpha * rawNew;
}

double slopeSensor( double value)
{
  //static double value1 = 0;
  //static double slope = 0;
  static double slope = 0, sum1 = 0, sum2 = 0, sum3 = 0, initialValue = 0;
  static int count1 = 0, count2 = 0, count3 = 0;
  if (flag == true)
  {
    //initialValue = value;
    myChrono.restart();
    flag = false;
  }

  if (myChrono.elapsed() <= 67)
  {
    sum1 = value + sum1;
    count1++;
  }

  if (myChrono.elapsed() > 67 && myChrono.elapsed() <= 134)
  {
    sum2 = value + sum2;
    count2++;
  }

  if (myChrono.elapsed() > 134 && myChrono.elapsed() <= 200)
  {
    sum3 = value + sum3;
    count3++;
  }


  if ( myChrono.elapsed() > 200 &&  myChrono.elapsed() <= 225)
  {
    slope = ((((sum2 / count2) - (sum1 / count1)) / 0.134) + (((sum3 / count3) - (sum2 / count2)) / 0.134)) / 2.0 ;
    //slope = ((value - initialValue )/0.225);

    myChrono.stop();
    sum1 = 0, sum2 = 0, sum3 = 0, count1 = 0, count2 = 0, count3 = 0; // initialise all the variables
    flag = true;


  }

  return slope;
}
/////////////////////////////////////////////////////////////////////////////////////
void categoriseSlopeRunMotor( double x , double negThresh , double posThresh , int negMotRelief , int posMotRelief )
{
   
  int pot2 = analogRead(positionPin);
  if (pot2 > (maxPosn + posMotRelief) || pot2 < (minPosn - negMotRelief))
  {
    return;
  }

  if (x <= posThresh && x >= negThresh)
  {
       digitalWrite(in1Pin ,LOW);
       digitalWrite(in2Pin ,LOW);
    //Serial.println(" in the halt the motor");
  }

  if ((x > posThresh) )
  {
   
  
        pot2 = analogRead(positionPin);
        myChrono2.restart();
        while( pot2 < (mapPotandSen) && (pot2 <(maxPosn+posMotRelief) && pot2 >(minPosn-negMotRelief)))
        {
        digitalWrite(in1Pin ,HIGH);
        digitalWrite(in2Pin, LOW);
        pot2 = analogRead(positionPin);
        if (myChrono2.elapsed()>=200 && myChrono2.elapsed()<=250) // for breaking the loop if the motor is struck
        {
          b=a;
          a= pot2;
          myChrono2.restart();
          if ( a>=b-5 && a <= b+5)
          {
          break;
          }
        }

        }
        digitalWrite(in1Pin , LOW);
        digitalWrite (in2Pin, LOW);
        if (myChrono.elapsed() > 225)
        {
          flag = true;
        }
  //    Serial.print(pot2);
  //    Serial.print(" , in forward ,");
  //    Serial.println(mapPotandSen);
 }





//Serial.println("forward");


 if (x <= negThresh )
{
//  Keyboard.write(KEY_UP_ARROW);

     pot2 = analogRead(positionPin);
      //Serial.print(" , in reverse , ");
      myChrono3.restart();
      while(pot2 > (mapPotandSen) && (pot2 <(maxPosn+posMotRelief) && pot2 >(minPosn-negMotRelief)))
      {
       digitalWrite(in2Pin ,HIGH);
       digitalWrite(in1Pin, LOW);
       pot2 = analogRead(positionPin);
      if (myChrono3.elapsed()>=200 && myChrono3.elapsed()<=250)
      {
        c=d;
        c= pot2;
        myChrono3.restart();
        if ( c>=d-5 && c <= d+5)
        {
        break;
        }
      }

     }
     digitalWrite(in1Pin , LOW);
     digitalWrite(in2Pin, LOW);
      if (myChrono.elapsed() > 225)
      {
        flag = true;
      }
}
//     Serial.print(pot2);
//     Serial.print(" , in reverse , ");
//     Serial.println(mapPotandSen);




//Serial.println(" reverse");
}

///////////////////////////////////////////////////////////////////////////////
void centerMotor(int value)
{
  int pot = analogRead(positionPin);
  while (pot < (value - 10))
  {
    digitalWrite(in1Pin , HIGH);
    digitalWrite(in2Pin , LOW);
    pot = analogRead(positionPin);
  }
  while (pot > (value + 10))
  {
    digitalWrite(in2Pin , HIGH);
    digitalWrite(in1Pin , LOW);
    pot = analogRead(positionPin);
  }

  if (pot >= (value - 10) && pot <= (value + 10))
  {
    digitalWrite(in1Pin , LOW);
    digitalWrite(in2Pin , LOW);
  }
}

