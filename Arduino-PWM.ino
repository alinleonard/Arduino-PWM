/*
Conectica:
  Potentiometre: Analog Pin 0  , Analog Pin 1
  PWM - Digital pin 3 , Digital pin 11

*/
//min 337
//max 69
int pwm1 = 3;           // pwm pin 3 Digital
int pwm2 = 11;           // pwm pin 11 Digital

const int potentiometerPin1 = A0;  // Analog input pin that the potentiometer is attached to
int potValue1 = 0;        // value read from the pot
int outputValue1 = 0;        // value output to the PWM (analog out)

const int potentiometerPin2 = A1;  // Analog input pin that the potentiometer is attached to
int potValue2 = 0;        // value read from the pot
int outputValue2 = 0;        // value output to the PWM (analog out)


// Set pin 9's PWM frequency to 3906 Hz (31250/8 = 3906)
// Note that the base frequency for pins 3, 9, 10, and 11 is 31250 Hz

// the setup routine runs once when you press reset:
void setup()  { 
  Serial.begin(9600);
  // Set pin 9's PWM frequency to 208 Hz (31250/256 = 122 Hz)
  setPwmFrequency(pwm1, 128);
  // Set pin 10's PWM frequency to 208 Hz (31250/256 = 122 Hz)
  setPwmFrequency(pwm2, 128);
  // declare pin 9 to be an output:
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  //led
  pinMode(13, OUTPUT);    
  
  //sistem reglare pwm 1
  potValue1 =  analogRead(potentiometerPin1);  
  int i = 0;
  if(potValue1 > 10){
    do{
      // map it to the range of the analog out:
      outputValue1 = map(i, 0, 1023, 10, 135);  
      // change the analog out value for pwm1:
      analogWrite(pwm1, outputValue1);   
      
      i = i + 10;
      delay(50);
    }while(i < potValue1);
  }
  //sistem reclare pwm2
  potValue2 =  analogRead(potentiometerPin2);  
  int x = 0;
  if(potValue2 > 10){
    do{
      // map it to the range of the analog out:
      outputValue2 = map(x, 0, 1023, 10, 135);  
      // change the analog out value for pwm1:
      analogWrite(pwm2, outputValue2);   
      
      x = x + 10;
      delay(50);
    }while(x < potValue2);
  }
  
  
} 

// the loop routine runs over and over again forever:
void loop()  { 
  potValue1 =  analogRead(potentiometerPin1);  
  // map it to the range of the analog out:
  outputValue1 = map(potValue1, 0, 1023, 10, 135);  
  // change the analog out value for pwm1:
  analogWrite(pwm1, outputValue1);     

  potValue2 =  analogRead(potentiometerPin2);  
  // map it to the range of the analog out:
  outputValue2 = map(potValue2, 0, 1023, 10, 135);  
  // change the analog out value for pwm2:
  
  analogWrite(pwm2, outputValue2);

  
  //Serial.println(outputValue2);
  //daca un potentiometru este la maxim atunci led -ul se aprinde
  if(outputValue2 == 135 || outputValue1 == 135){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);                    
}

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

