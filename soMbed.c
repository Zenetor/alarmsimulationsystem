#include "mbed.h"
#include "rtos.h"
#include "stdio.h"
 
//Projeto Final - Sistemas Operacionais
//José de Faria Leite Neto
//Wilian Henrique Cavassin
//Fernando Levy Silvestre de Assis
 
 //Process struct
 struct prc
{
  int id;
  int pri; //Always starts at 0
  int press; //If button is pressed
  
};
 
//Interruptions
InterruptIn btn1(D7);
InterruptIn btn2(D6);
InterruptIn btn3(D5);
InterruptIn btn4(D4);
InterruptIn msin(D3);



//Outs
DigitalOut buzzer(D2);
DigitalOut ledb(LED_BLUE);
DigitalOut ledr(LED_RED);
DigitalOut ledg(LED_GREEN);

DigitalOut ground(D0);
DigitalOut MSout(D1);

//Buttons Threads
Thread threadb1, threadb2, threadb3, threadb4, threadms;

//Input controler thread
Thread inputCT;

//Scheduler Thread
Thread scdT;
//Scheduler, all priorities starts at 0
/*prc prca;
prca.pri = 0;
prca.id = -1;
prca.press = -1; */
prc scd[5] = {NULL, NULL, NULL, NULL, NULL};

//Buttons and motion sensor
prc b1, b2, b3, b4, ms;
    
 
     
//Button function 
//If pressed, put the process on queue
void button1_pressed() {
        scd[0].id = b1.id;
        scd[0].pri = b1.pri;
        //ledb = !ledb;
        
        }
        
void button2_pressed() {
        scd[1].id = b2.id;
        scd[1].pri = b2.pri;
        
        }
        
void button3_pressed() {
        scd[2].id = b3.id;
        scd[2].pri = b3.pri;
 
        }
        
void button4_pressed() {
        scd[3].id = b4.id;
        scd[3].pri = b4.pri;      
        }
        
void button5_pressed() {
        scd[4].id = ms.id;
        scd[4].pri = ms.pri;
        }

//Buttons threads
void button1_thread() {
    btn1.fall(&button1_pressed); // whenever the button falls, execute button_pressed function
 }
 
 void button2_thread() {
    btn2.fall(&button2_pressed); // whenever the button falls, execute button_pressed function
 }
 
 void button3_thread() {
    btn3.fall(&button3_pressed); // whenever the button falls, execute button_pressed function
 }
 
 void button4_thread() {
    btn4.fall(&button4_pressed); // whenever the button falls, execute button_pressed function
 }
 
  void button5_thread() {
    msin.fall(&button5_pressed); // whenever the button falls, execute button_pressed function
 }
 
 //Scheduler thread
void scheduler_thread() {
    int i, biggerP = 0, aux = 0;
    
    
    while (1) {
    
        for(i = 0; i < 5; i++) {
            if(scd[i].pri > biggerP) {
                biggerP = scd[i].pri;
                aux = i;
                }   
            }
            
            //Is it really a process? Which one?
            if(biggerP > 0){
                    //It is
                    
                    //Button 1 - priority 3 - execute the process and reset priority in priority queue
                    if(aux == 0) {
                        ledr = !ledr;
                        buzzer = !buzzer;
                        wait(3);
                        buzzer = !buzzer;
                        ledr = !ledr;
                        scd[0].id = -1;
                        scd[0].pri = -1;
                        biggerP = 0;
                        }
                        
                    //Button 2 - priority 2 - execute the process and reset priority in priority queue
                    if(aux == 1) {
                        ledb = !ledb;
                        buzzer = !buzzer;
                        wait(3);
                        buzzer = !buzzer;
                        ledb = !ledb;
                        scd[1].id = -1;
                        scd[1].pri = -1;
                        biggerP = 0;
                        }
                        
                    //Button 3 - priority 1 - execute the process and reset priority in priority queue
                    if(aux == 2) {
                        ledg = !ledg;
                        buzzer = !buzzer;
                        wait(3);
                        buzzer = !buzzer;
                        ledg = !ledg;
                        scd[2].id = -1;
                        scd[2].pri = -1;
                        biggerP = 0;
                        }
                       
                    //Button 4 - priority 3 - execute the process and reset priority in priority queue 
                    if(aux == 3) {
                        ledg = !ledg;
                        ledb = !ledb;
                        buzzer = !buzzer;
                        wait(3);
                        buzzer = !buzzer;
                        ledg = !ledg;
                        ledb = !ledb;
                        scd[3].id = -1;
                        scd[3].pri = -1;
                        biggerP = 0;
                        }
                        
                    //Movement sensor - priority 4 - execute the process and reset priority in priority queue
                    if(aux == 4) {
                        ledr = !ledr;
                        ledb = !ledb;
                        buzzer = !buzzer;
                        wait(3);
                        buzzer = !buzzer;
                        ledr = !ledr;
                        ledb = !ledb;
                        scd[4].id = -1;
                        scd[4].pri = -1;
                        biggerP = 0;
                        }
                    
                }
        
        
        }
 
 
 
 }
 
 

int main(int, char**) {
  
  
    //Setting buttons and motion sensor priorities and ids
    
    b1.id = 1;
    b1.pri = 3;
    b1.press = 0;
    
    b2.id = 2;
    b2.pri = 2;
    b2.press = 0;
    
    b3.id = 1;
    b3.pri = 1;
    b3.press = 0;
    
    b4.id = 4;
    b4.pri = 3;
    b4.press = 0;
    
    ms.id = 5;
    ms.pri = 4;
    ms.press = 0;
    
    
    
    prc prca;
    prca.pri = 0;
    prca.id = -1;
    prca.press = -1;
    scd[0] = prca;
    scd[1] = prca;
    scd[2] = prca;
    scd[3] = prca;
    scd[4] = prca;
  
  //Ground
  ground = 0;
  //Leds start at 0
  ledr = !ledr;
  ledg = !ledg;
  ledb = !ledb;
  
  //Threads starting
  threadb1.start(button1_thread);
  threadb2.start(button2_thread);
  threadb3.start(button3_thread);
  threadb4.start(button4_thread);
  threadms.start(button5_thread);
  scdT.start(scheduler_thread);

  while (1) {}
}