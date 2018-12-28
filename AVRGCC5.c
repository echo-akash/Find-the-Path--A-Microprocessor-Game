
 
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 4000000UL
#include <util/delay.h>
#define DataPort1	PORTA	// Using PortC as our Dataport
#define DataPort2	PORTB
#define DataDDR1	DDRA
#define DataDDR2	DDRB
int smile[8]=   {0b11000011,0b10100101,0b01011010,0b01111110,0b01111110,0b01011010,0b10111101,0b11000011};
int sad[8]  =   {0b11000011,0b10111101,0b01011010,0b01100110,0b01111110,0b01011010,0b10111101,0b11000011};
int obstacle[8]= {0b11000011,0b11111111,0b11000111,0b11111111,0b10001111,0b11111111,0b11100001,0b11111111};
int obstacle2[8]={0b10101010,0b11101010,0b10101010,0b10101010,0b10111010,0b10101110,0b10101010,0b10101011};
int row[8] = {0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000};
int ballcol;
int ballrow;
int lose=0;
 
int checkrow(int x,int r){
	if(x==r) return 1;
	return 0;
}	

int checkcol(int x,int c){
	int y=x|c;
	if(y!=0b11111111) return 1;
	return 0;
}	

void level2(){
	 ballrow=0b00000001;
	 ballcol=0b01111111;
	 int flag=0,lose=0;
	 while(1){
		for(int i=0;i<8;i++){
				   DataPort1=row[i];
				   DataPort2=obstacle2[i];	
				   _delay_ms(0.2);
				   if((checkcol(ballcol,obstacle2[i])==1 && checkrow(ballrow,row[i])==1)|| ballcol==0b11111111 || ballrow==0b100000000){
						lose=1;
						break;					   
				   }
				   DataPort1=ballrow;
				   DataPort2=ballcol;
				   _delay_ms(0.2);
				   if(ballrow==0b10000000 && ballcol==0b11111110){
						flag=1;
						break;
				   }					   
		 }
		if(flag==1)break;
		if(lose==1) break;
	 }		 
	 if(flag==1){
		put_a_smile();
		level1();
	 }		
	 if(lose==1)better_luck_nxt_time();
 }
  
void level1(){
	 ballrow=0b00000001;
	 ballcol=0b01111111;
	 int flag=0,lose=0;
	 while(1){
		for(int i=0;i<8;i++){
				   DataPort1=row[i];
				   DataPort2=obstacle[i];	
				   _delay_ms(0.2);
				   if((checkcol(ballcol,obstacle[i])==1 && checkrow(ballrow,row[i])==1)|| ballcol==0b11111111 || ballrow==0b100000000){
						lose=1;
						break;					   
				   }
				   DataPort1=ballrow;
				   DataPort2=ballcol;
				   _delay_ms(0.2);
				   if(ballrow==0b10000000 && ballcol==0b11111110){
						flag=1;
						break;
				   }					   
		 }
		if(flag==1)break;
		if(lose==1) break;
	 }		 
	 if(flag==1){
		put_a_smile();
		level2();
	 }		
	 if(lose==1)better_luck_nxt_time();
 }
//Interrupt Service Routine for INT0
ISR(INT0_vect)
{
	_delay_ms(0.1); // Software debouncing control
	int x,y,z;
	x=ballcol;
	ballcol=(x>>1)|((int)1<<7);
	DataPort2=ballcol;
}	

ISR(INT1_vect)
{
	_delay_ms(0.1); // Software debouncing control
	int x,y,z;
	x=ballrow;
	ballrow=(x<<1);
	DataPort1=ballrow;
}	


void put_a_smile(){
	lose=0;
	for(int j=0;j<100;j++){
		//_delay_ms(0.5);
		for(int i=0;i<8;i++){
			   DataPort1=row[i];
			   DataPort2=smile[i];
			   _delay_ms(0.5);
		   }	
	}
	ballrow=0b00000001;
	ballcol=0b01111111;
	DataPort1=0b00000001;
	DataPort2=0b01111111;	
	
}	

void better_luck_nxt_time(){
	for(int j=0;j<100;j++){
		//_delay_ms(0.5);
		for(int i=0;i<8;i++){
			   DataPort1=row[i];
			   DataPort2=sad[i];	
			   _delay_ms(0.5);
		 }
	}
	level1();
}	
int main(void)
{
		DDRD = 0x00;
	//PORTD = 1<<PD2;		// Enable PD2 pull-up resistor
	//PORTD = 1<<PD3;
 
	DataDDR1 = 0xFF;		// Configure Dataport as output
	DataDDR2=  0xFF;
	ballrow=0b00000001;
	ballcol=0b01111111;
	DataPort1=0b00000001;
	DataPort2=0b01111111;
	
 
	GICR = 1<<INT0 | 1<<INT1;					// Enable INT0
	MCUCR = (1<<ISC01 | 1<<ISC00) | (1<<ISC11 | 1<<ISC10);	// Trigger INT0 on rising edge
 
	sei();				//Enable Global Interrupt
	while(1){
		  	level1();  
    }
}