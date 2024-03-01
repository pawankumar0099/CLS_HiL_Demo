#include <SoftwareSerial.h>

#define DEBUG1

#define RPI Serial
#define LDR1_TH 350
#define LDR1_TH_RES 100
#define LDR1_TH_DEC 50

#define LDR2_TH	    270	 
#define LDR2_TH_RES 100
#define LDR2_TH_DEC 50

const int ldr1_pin = A0;
const int ldr2_pin = A1;

int ldr1_data = 0;
int ldr2_data = 0;

SoftwareSerial DUT(2, 3); // RX, TX
SoftwareSerial DEBUG(4, 5); // RX, TX

void setup() {
  DUT.begin(9600);
  RPI.begin(9600);
  #ifdef DEBUG1
    DEBUG.begin(9600);
  #endif
}
void fun(){
	delay(2000);
 	ldr1_data = readldr(ldr1_pin);

        if (ldr1_data > LDR1_TH)
        {
          //#ifdef DEBUG1
          //          DEBUG.println("LDR Data:");
          //          DEBUG.print(ldr1_data);
          //#endif
        
          RPI.write(0x10);
        }
        else
        {
          //#ifdef DEBUG1
          //          DEBUG.println("LDR Data:");
          //          DEBUG.print(ldr1_data);
          //#endif

          RPI.write(0x0F);
        }

}

void ldr2(){
	delay(1500);
	 ldr2_data = readldr(ldr2_pin);

        if (ldr2_data > LDR2_TH)
          RPI.write(0x01);

}

void Test_Case_MCU()
{
  //ldr1_data = readldr(ldr1_pin);

  if (RPI.available() > 0)
  {
    char data = RPI.read();

    //#ifdef DEBUG1
    //    DEBUG.println("Data:");
    //    DEBUG.print(data);
    //#endif

    switch (data)
    {
      case 0x10:
        DUT.write(0x10);
       	fun();
	//	delay(5000);
        /*ldr1_data = readldr(ldr1_pin);

        if (ldr1_data > LDR1_TH)
        {
          //#ifdef DEBUG1
          //          DEBUG.println("LDR Data:");
          //          DEBUG.print(ldr1_data);
          //#endif
        
          RPI.write(0x10);
        }
        else
        {
          //#ifdef DEBUG1
          //          DEBUG.println("LDR Data:");
          //          DEBUG.print(ldr1_data);
          //#endif

          RPI.write(0x0F);
        }
	*/
        break;

      case 0x00:
      
        DUT.write((uint8_t)0x00);
	//delay(1000);
        ldr1_data = readldr(ldr1_pin);

        if (ldr1_data > LDR1_TH_RES)
        {
          RPI.write(0x00);
        }

        delay(100);

        //   for(int i = 0; i <= 10 ; i++)
        //   RPI.write(0x00);

        if (ldr1_data > LDR1_TH_DEC)
        {
	  
	  delay(5000);
          RPI.write(0x00);
        }

        break;

      case 0x01:
        
        DUT.write(0x01);
	//delay(3000);
	ldr2();
        //ldr2_data = readldr(ldr2_pin);

        //if (ldr2_data > LDR2_TH)
          //RPI.write(0x01);
        break;
        
      case 0x11:
        DUT.write(0x11);
        
        ldr2_data = readldr(ldr2_pin);
        if (ldr2_data > LDR2_TH_RES)
          RPI.write(0x11);
        delay(50);

        if (ldr2_data > LDR2_TH_DEC)
          delay(5000);
	  RPI.write(0x11);
       	 
        break;

      case 0x19:  //25 degree temperature case
          DUT.write(0x19);
          RPI.write(0x19);          
        break;

      case 0x28: //40 degree
          DUT.write(0x19);
	  RPI.write(0x19);
        break;

      case 0x3C:  //60 degree
          DUT.write(0x3C);
	  RPI.write(0x3C);
        break;

      case 0x14: //-20 degree
          DUT.write(0x14);
	  RPI.write(0x14);
        break;
        
      case 0x1E: //-30 degree
          DUT.write(0x1E);
	  RPI.write(0x1E);
        break;
        
//      default:
         //RPI.write(data);
//#ifdef DEBUG1 
          //DEBUG.println("Inside Defualt Case:");
          //DEBUG.println(data);
//#endif
  //      break;
    }
  }
}

//Read LDR Data
//Paramters: ldrnum = ldr1_pin or ldr2_pin
int readldr(int ldrnum)
{
#ifdef DEBUG1
  DEBUG.println("LDR Data:");
  DEBUG.println(ldrnum);
  DEBUG.println(analogRead(ldrnum));
#endif
  return analogRead(ldrnum);
}

void loop() {
  //readldr(ldr1_pin);
  //delay(300);
  //readldr(ldr2_pin);
  Test_Case_MCU();
  //delay(200);
}
