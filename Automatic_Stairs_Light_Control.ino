int set_dis = 8;    // Set distance for human read
int set_step = 12;   // Set step for Stairs Limt
int set_shift = 500; // Set shifting time for step to step MilliSecond
int set_timer = 20;   // Set on stay time for light on Second

#define e_s1 A0 //echo pin
#define t_s1 A1 //Trigger pin

#define e_s2 A2 //echo pin
#define t_s2 A3 //Trigger pin

long dis_a=0,dis_b=0;
int flag1=0, flag2=0;
int i=0;
int Second = 0;
word MilliSecond = 0; 
bool timerStart = false;

int OutPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};


//**********************ultra_read****************************
void ultra_read(int pin_t,int pin_e,long &ultra_time){
long time;
digitalWrite(pin_t,LOW);
delayMicroseconds(2);
digitalWrite(pin_t,HIGH);
delayMicroseconds(10);
time=pulseIn (pin_e,HIGH);
ultra_time =  time / 29 / 2; 
}


void setup(){
Serial.begin(9600);// initialize serial communication at 9600 bits per second:

for (i=0; i<set_step; i++) {
    pinMode(OutPins [i], OUTPUT);
}

pinMode(e_s1,INPUT);
pinMode(t_s1,OUTPUT);

pinMode(e_s2,INPUT);
pinMode(t_s2,OUTPUT);

 noInterrupts();         // disable all interrupts
 TCCR1A = 0;             // set entire TCCR1A register to 0  //set timer1 interrupt at 1kHz  // 1 ms
 TCCR1B = 0;             // same for TCCR1B
 TCNT1  = 0;             // set timer count for 1khz increments
 OCR1A = 1999;           // = (16*10^6) / (1000*8) - 1
 //had to use 16 bit timer1 for this bc 1999>255, but could switch to timers 0 or 2 with larger prescaler
 // turn on CTC mode
 TCCR1B |= (1 << WGM12); // Set CS11 bit for 8 prescaler
 TCCR1B |= (1 << CS11);  // enable timer compare interrupt
 TIMSK1 |= (1 << OCIE1A);
 interrupts();           // enable
 
delay(1000); // Waiting for a while
}


void loop(){ 
data();
  
if(Second==0){
if(flag1==1){ flag1=0; timerStart = false;
delay(set_shift);
for(i=set_step-1; i>-1; i--){
digitalWrite(OutPins [i], LOW);
data();
out1();
delay(set_shift);
  }
 }
}

if(Second==0){
if(flag2==1){ flag2=0; timerStart = false;
delay(set_shift);
for(i=0; i<set_step; i++){
digitalWrite(OutPins [i], LOW);
data();
out2();
delay(set_shift);
  }
 }
}

}


void data(){
ultra_read(t_s1,e_s1,dis_b);
ultra_read(t_s2,e_s2,dis_a);

Serial.print("da:");Serial.println(dis_a);
Serial.print("db:");Serial.println(dis_b);  
Serial.print("time:");Serial.println(Second); 

if(dis_a<set_dis){
Second = set_timer;
delay(set_shift);
if(flag1==0 && flag2==0){ flag1=1;
for(i=set_step-1; i>-1; i--){
digitalWrite(OutPins [i], HIGH);
delay(set_shift);
}
timerStart = true;
}
}

if(dis_b<set_dis){
Second = set_timer;
delay(set_shift);
if(flag1==0 && flag2==0){ flag2=1;
for(i=0; i<set_step; i++){
digitalWrite(OutPins [i], HIGH);
delay(set_shift);
}
timerStart = true;
  }
 }
}

void out1(){
if(flag1==1 || flag2==1){i=0;}
}

void out2(){
if(flag1==1 || flag2==1){i=set_step-1;}
}


ISR(TIMER1_COMPA_vect){   
if(timerStart == true){MilliSecond++;
    if(MilliSecond >= 1000){MilliSecond = 0;
       Second--;
    }
  }  
}

