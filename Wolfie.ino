/* tg_wolfie 
*
*  Wolfie interfaces to a current sensor and reports
*  current readings via serial port polling.  It is expected that
*  an Xbee is attached to the serial port */

#define NUMBER_OF_SAMPLES 10

int sampleI;
double sumI, Irms; 
long  previousMillis;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
  // Serial.print(255);
  analogReference(DEFAULT);
  previousMillis = millis();
}


// the loop routine runs over and over again forever:
void loop() {
  
  unsigned long currentMillis = millis();
  double voltage;
  while( Serial.available()>0)
  {
    char cmnd = Serial.read();
   // read current command is ?8, respond with !80 or !81 for off or on
   delay(1);  // needed to allow next char to be received. It takes 173.6 us per char
   char ch = Serial.read();
   // do an address check, flush and quit if no match
   if (ch != 0x38) 
      {
      Serial.flush();
      cmnd = 0;           // force a bailout
      }
   switch (cmnd)
   {
   case '?':
     
        voltage = getSample();
        Serial.print("!8");
        // Serial.println(voltage, DEC);
        if (voltage > 100) 
           Serial.println("1");
        else 
           Serial.println("0");
      break;
    case '#':
        voltage = getSample();
        Serial.print("!8V");
        Serial.println(voltage, DEC);
       
      break;
    }
    
  Serial.flush();
  
  }
}

// this A/D reading is an average of  the number of samples which
// is set as a program define at the top of this sketch.
// Samples are read and accumulated. when the total number of A/D
// samples has been read, the accumulated value is divided by the number
// of samples yielding an average.
double getSample(void)
{
   for (int n = 0; n < NUMBER_OF_SAMPLES; n++)
  {
      sampleI = analogRead(A0);
      delay(2);
      sumI += sampleI;
      
  }
  Irms = sumI/(NUMBER_OF_SAMPLES);
  sumI = 0;
  return Irms;
  }
  

