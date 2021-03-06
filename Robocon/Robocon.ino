

#include "Sensor.h"
#include "Bot.h"
#include "Setup.h"
#include "Helper.h"



Bot bot;
Helper h;
unsigned long interval = 4000;
//unsigned long previousMillis = 0;

int gameMode;
int direction = 0;
int nodeCount = 1;
int delays[4] ={0,300,500,300}; 
//float speedFactor[4] = {1,0.5,0.1,0.5};
  
void setup() {
  //parameter is in se tup header.
  Initializer::initializePins();
   Serial.begin(9600);
  gameMode = EEPROM.read(0);
  bot.initializeBotSensor();

  while(0){
  h.SensorCalibration();
}

 

}

void loop() {
  //Serial.println(gameMode); 
 if(gameMode!='A'){
   bot.changeMode('A');
   bot.buzzer(1,1000);
  // unsigned long currentMillis = millis();
   //exit(0);
   while(0){
    //bot.moveForward(50,50);
    bot.testSensor(gameMode);
   }
   float RPM = 50, rightRPM, leftRPM;
   int error = 0;
   int Kp = 30;
   int leftCount = 0;
   int rightCount = 0;
   while(1){
    
    bot.startFan();
    if(bot.isDirection() == END){
      bot.stopMoving();
      bot.changeMode('A');
      bot.stopFan();
      bot.buzzer(15,80);
      exit(0);
       
   }


    error = bot.getError();
    leftRPM = (RPM - Kp * error);
    rightRPM = (RPM + Kp * error);
    
     bot.moveForward(leftRPM,rightRPM);
     if(millis()>=interval){           
     direction = bot.isDirection();
     }
     if(direction >0 && millis() >=interval){
      if(direction ==LEFT)leftCount+=1;
      if(direction ==RIGHT)rightCount+=1;
      if(direction == JUNCTION) {
        nodeCount+=1;
        //bot.stopMoving();
        //delay(1000);  
      bot.buzzer(1);

        continue;
      }
     }else {
      leftCount = 0;
      rightCount=0;
     }
     
     if(leftCount>=2){  
      bot.buzzer(1);
      nodeCount++;
      if(nodeCount ==1 || nodeCount>= 8){          
        leftCount = 0;
        rightCount = 0;
        continue;       
      }
      bot.stopFan();
      delay(delays[nodeCount%4]);
      bot.stopMoving();
      delay(1000);  
      bot.buzzer(1);
      
       bot.moveLeft();
      leftCount = 0;
     }
     
     if(rightCount >=2){      
      bot.buzzer(1);
      nodeCount++;
      if(nodeCount ==1 || nodeCount >= 8){
        leftCount = 0;
        rightCount = 0;
        continue;   
      }
      bot.stopFan();
      delay(delays[nodeCount%4]);
      bot.stopMoving();
      delay(1000);
      bot.buzzer(1); 
      
       bot.moveRight();
      rightCount  = 0;     
   }
  
  }
 }
  
  if(gameMode=='A'){                                                                                                                                                                             
    bot.changeMode('B');
    bot.buzzer(3);
    //exit(0);
    bot.moveBackward(35,35);
    delay(4000);
    bot.startFan();
    bot.startCleaner();
    bot.rotate180();
    delay(100);
    while(1){
          bot.moveForward(35,35);
          if(bot.detectWay()==NOWAY){
            bot.stopMoving();  
            bot.buzzer(5,100);
            bot.moveBackward(30,30);
            delay(1500);
            bot.rotate180();
            bot.stopMoving();
            delay(300);
          }        
          if(bot.detectWay()==LEFTLOCK){
            bot.stopMoving();  
            bot.buzzer(5,100);
            bot.moveBackward(30,30);
            delay(1500);
            bot.rotateRight(40);
            //delay(300);
            bot.stopMoving();
            delay(500);
          }
          if(bot.detectWay()==RIGHTLOCK){
            bot.stopMoving();  
            bot.buzzer(5,100);
            bot.moveBackward(30,30);
            delay(1500);
            bot.rotateLeft(40);
            //delay(300);
             bot.stopMoving();
            delay(500);
          }
        }

    }
   //}
 
  }  

