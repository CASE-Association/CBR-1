
#include <Arduino.h>
#include <PID_v1.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
 
int pwmA = 5;
int in1A = 3;
int in2A = 4;
 
int pwmB = 6;
int in1B = 7;
int in2B = 8;

int LED2 = 11;
int LED1 = 12;
 
int n1 = 0;
int n2 = 0;

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

/*********Tune these 4 values for your BOT*********/
double setpoint = 180; //set the value when the bot is perpendicular to ground using serial monitor. 

//###############################################
//Change those Values according to your Design
//#############################################
double Kp = 15; //Set this first      DEFAULT: 21
double Ki = 200; //Finally set this   DEFAULT: 140
double Kd = 0.6; //Set this secound   DEFAULT: 0.8
//##############################################

double input, output;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void dmpDataReady()
{
    mpuInterrupt = true;
}


void Forward() //Code to rotate the wheel forward 
{
    digitalWrite(LED1,0);
    digitalWrite(LED2,1);
    
    digitalWrite(in1A, HIGH);
    digitalWrite(in2A, LOW);
 
    digitalWrite(in1B, HIGH);
    digitalWrite(in2B, LOW);

    analogWrite(pwmA, output);
    analogWrite(pwmB, output);

    Serial.print("F"); //Debugging information 
}

void Reverse() //Code to rotate the wheel Backward  
{
    digitalWrite(LED1,1);
    digitalWrite(LED2,0);

    digitalWrite(in1A, LOW);
    digitalWrite(in2A, HIGH);
 
    digitalWrite(in1B, LOW);
    digitalWrite(in2B, HIGH);

    analogWrite(pwmA, -1*output);
    analogWrite(pwmB, -1*output);

    Serial.print("R");
}

void Stop() //Code to stop both the wheels
{
    digitalWrite(LED1,0);
    digitalWrite(LED2,0);

    digitalWrite(in1A, LOW);
    digitalWrite(in2A, LOW);
 
    digitalWrite(in1B, LOW);
    digitalWrite(in2B, LOW);

    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);

    Serial.print("S");
}

void setup(){

  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);


  //Serial.begin(9600);
  
  // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

     // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    devStatus = mpu.dmpInitialize();

    
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1688); 

      // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
        
        //setup PID
        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(10);
        pid.SetOutputLimits(-255, 255);  
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

void loop() {

  /*
  motorSpeed1 = map(analogRead(ASDASD), 0, 1023, 0, 255);
  motorSpeed2 = map(analogRead(50ASDASD0), 0, 1023, 0, 255);  
    */

// if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize)
    {
        //no mpu data - performing PID calculations and output to motors     
        pid.Compute();   
        
        //Print the value of Input and Output on serial monitor to check how it is working.
        Serial.print(input); Serial.print(" =>"); Serial.println(output);
               
        if (input>150 && input<200){//If the Bot is falling 
          
        if (output>0) //Falling towards front 
        Forward(); //Rotate the wheels forward 
        else if (output<0) //Falling towards back
        Reverse(); //Rotate the wheels backward 
        }
        else //If Bot not falling
        Stop(); //Hold the wheels still
        
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (mpuIntStatus & 0x02)
    {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer); //get value for q
        mpu.dmpGetGravity(&gravity, &q); //get value for gravity
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); //get value for ypr

        input = ypr[1] * 180/M_PI + 180;

   }
  
}