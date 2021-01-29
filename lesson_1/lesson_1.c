
/*
 * lesson_1.c
 *
 * Created: 09.12.2018 12:39:13
 *  Author: Sergei
 */ 
#define	F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//------------------------------------------
unsigned char R1, R2, R3, R4, n_count, MSB, LSB, perm_butt=0;
short int AC1, AC2, AC3, B1, B2, MB, MC, MD;
unsigned int AC4, AC5, AC6, butcount1=0, butcount2=0, butcount3=0, sel_mode=0, butcount6=0, butcount7=0;
long int UT, UP, X1, X2, B5, T, k, B6, X3, B3, p, contr_altit_1, contr_altit_2, contr_altit_3, altit;
unsigned long int B4, B7;

//------------------------------------------
void TWI_MasterInit ()
{
	TWBR = 0x2A;
	TWSR &= ~(1 << 0) & ~(1 << 1);
}

void TWI_Start ()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void TWI_Stop ()
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

//------------------------------------------
void init (void)
{	
	DDRB |= (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);
	DDRB &= ~(1<<0) & ~(1<<6) & ~(1<<7);
	PORTB |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<6) | (1<<7);
	DDRD = 0xFF;
	PORTD = 0xFF;
	_delay_ms(2000);
	PORTB &= ~(1<<1) & ~(1<<2) & ~(1<<3) & ~(1<<4) & ~(1<<5);
}

//------------------------------------------
void segment (unsigned char seg)
{
	switch (seg)
	{
		case 0: PORTD = 0x3F; break;
		case 1: PORTD = 0x06; break;
		case 2: PORTD = 0x5B; break;
		case 3: PORTD = 0x4F; break;
		case 4: PORTD = 0x66; break;
		case 5: PORTD = 0x6D; break;
		case 6: PORTD = 0x7D; break;
		case 7: PORTD = 0x07; break;
		case 8: PORTD = 0x7F; break;
		case 9: PORTD = 0x6F; break;
		case 10: PORTD = 0x40; break; // минус
		case 11: PORTD = 0x5E; break; // d
		case 12: PORTD = 0x5C; break; // o
		case 13: PORTD = 0x7d; break; // G
		case 14: PORTD = 0x50; break; // r
		case 15: PORTD = 0x79; break; // E
	}
}
//------------------------------------------

void timer_init (void)
{
	TCCR1B |= (1<<WGM12); 
	TIMSK |= (1<<OCIE1A);
	OCR1AH = 0b01001110;
	OCR1AL = 0b00100000;
	TCCR1B |= (1<<0);
	
// 	TCCR0 |= (1<<1) | (1<<0);
// 	TIMSK |= (1<<0);
// 	TCNT0 = 0x0F;	
}

//------------------------------------------

void show_k (void)
{
	if (k == 1111)
	{
		if (n_count==0) { PORTB |= (1<<1); PORTB &= ~(1<<2) & ~(1<<3) & ~(1<<4); segment(R1);}
		if (n_count==1) { if (!(R2==0 && R3==0 && R4==0)) {PORTB |= (1<<2); PORTB &= ~(1<<1) & ~(1<<3) & ~(1<<4); segment(R2);}}
		if (n_count==2) { if (!(R3==0 && R4==0)) {PORTB |= (1<<3); PORTB &= ~(1<<1) & ~(1<<2) & ~(1<<4); segment(R3);}}
		if (n_count==3) { if (!(R4==0)) {PORTB |= (1<<4); PORTB &= ~(1<<1) & ~(1<<2) & ~(1<<3); segment(R4);}}
		n_count++;
		if (n_count>3) {n_count = 0;}
	} 
	else
	{
		R4 = k/1000;
		R3 = k%1000/100;
		R2 = k%100/10;
		R1 = k%10;
		// 		R3 = k/100;
		// 		R2 = k%100/10;
		// 		R1 = k%10;

		if (n_count==0) { PORTB |= (1<<1); PORTB &= ~(1<<2) & ~(1<<3) & ~(1<<4); segment(R1);}
		if (n_count==1) { if (!(R2==0 && R3==0 && R4==0)) {PORTB |= (1<<2); PORTB &= ~(1<<1) & ~(1<<3) & ~(1<<4); segment(R2);}}
		if (n_count==2) { if (!(R3==0 && R4==0)) {PORTB |= (1<<3); PORTB &= ~(1<<1) & ~(1<<2) & ~(1<<4); segment(R3);}}
		if (n_count==3) { if (!(R4==0)) {PORTB |= (1<<4); PORTB &= ~(1<<1) & ~(1<<2) & ~(1<<3); segment(R4);}}
		n_count++;
		if (n_count>3) {n_count = 0;}
	}
}
//------------------------------------------


ISR (TIMER1_COMPA_vect)
{
	if (k == 0)
	{ PORTB &= ~(1<<1) & ~(1<<2) & ~(1<<3) & ~(1<<4);} 
	else { show_k();}
	
// 	if (~PINB & (1<<6))
// 	{
// 		if (butcount<5) {butcount++;}
// 		else {sel_mode++; if (sel_mode>2) {sel_mode = 1;}}
// 	}
// 	else {if (butcount>0) {butcount--;}}
		
}

//------------------------------------------

// ISR (TIMER0_OVF_vect)
// {
// // 	if (~PINB & (1<<6))
// // 	{
// // 		if (butcount<5) {butcount++;}
// // 		else {sel_mode++; if (sel_mode>3) {sel_mode = 1;}}
// // 	}
// // 	else {if (butcount>0) {butcount--;}}
// 
// 	if (~PINB & (1<<7))
// 	{
// 		if (butcount<5) {butcount++;}
// 		else {sel_mode++; perm_butt = 0; if (sel_mode>3) {sel_mode = 1;}}
// 	}
// 	else {if (butcount>0) {butcount--;}}
// 	
// }

//------------------------------------------



unsigned char TWI_R_B (unsigned char addr_r_b)
{	
	_delay_ms(80);
	TWI_Start();
	
	TWDR = 0xEE;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWDR = addr_r_b;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWI_Start();
	
	TWDR = 0xEF;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	TWI_Stop();
	
	return TWDR;
}
//------------------------------------------

unsigned short TWI_R_W (unsigned char addr_r_w)
{
	unsigned short n;
	_delay_ms(80);
	TWI_Start();
	
	TWDR = 0xEE;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWDR = addr_r_w;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWI_Start();
	
	TWDR = 0xEF;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWCR = (1<<TWEA) | (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	MSB = TWDR;
		
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	LSB = TWDR;

	TWI_Stop();
	
	return n = (MSB<<8) + LSB; 
}
//------------------------------------------


void TWI_W (unsigned char addr_w, unsigned char data_w)
{
	_delay_ms(80);
	TWI_Start();
	
	TWDR = 0xEE;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWDR = addr_w;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	TWDR = data_w;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	
	TWI_Stop();
}
//------------------------------------------

void r_cal_data (void)
{
unsigned short check_twi;
	AC1 = TWI_R_W(0xAA);
	AC2 = TWI_R_W(0xAC);
	AC3 = TWI_R_W(0xAE);
	AC4 = TWI_R_W(0xB0);
	AC5 = TWI_R_W(0xB2);
	AC6 = TWI_R_W(0xB4);
	B1 = TWI_R_W(0xB6);
	B2 = TWI_R_W(0xB8);
	MB = TWI_R_W(0xBA);
	MC = TWI_R_W(0xBC);
	MD = TWI_R_W(0xBE);

	check_twi = TWI_R_B(0xD0);
	if (check_twi == 85) { R4 = 13; R3 = 12; R2 = 12; R1 = 11; k = 1111;}
	else { R3 = 15; R2 = 14; R1 = 14; k = 1111;} 
	_delay_ms(1000);
		
	k = 0;
	
}
//------------------------------------------

void prov (long z)
{
	if (z<0)
	{
		z = z-1;
		z = ~z;
		R4 = 10;
	}
	else {R4 = 0;}
	k = z/1000;
	_delay_ms(2000);
	k = z%1000;
	_delay_ms(2000);
	
}

//------------------------------------------


int main(void)
{

	init();
	timer_init();
	sei();
	
	TWI_MasterInit();
	_delay_ms(1000);
	r_cal_data();
	
	
	
	
	while(1)
    {
				TWI_W(0xF4, 0x2E);
				_delay_ms(4.5);
				UT = TWI_R_W(0xF6);

				TWI_W(0xF4, 0x34);
				_delay_ms(4.5);
				UP = TWI_R_W(0xF6);
				X1 = (UT-AC6)*AC5/32768;
				X2 = (long)MC*2048/(X1+MD);
				B5 = X1+X2;
				T = (B5+8)/16;
		if (T>250)
		{
			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);
			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

		}
				
		
		if ((~PINB & (1<<0)) && (perm_butt == 0))
		{
			if (butcount1<5) {butcount1++;}
			else 
			{
				
				contr_altit_1 = T;
				k = T;
				_delay_ms(1000);
				perm_butt = 1;
			}
		}
		else {if (butcount1>0) {butcount1--;}}

		if ((~PINB & (1<<0)) && (perm_butt == 1))
		{
			if (butcount2<5) {butcount2++;}
			else
			{
				contr_altit_2 = T;
				k = T;
				_delay_ms(2000);
				perm_butt = 2;
			}
		}
		else {if (butcount2>0) {butcount2--;}}

		if ((~PINB & (1<<0)) && (perm_butt == 2))
		{
			if (butcount3<5) {butcount3++;}
			else
			{
				
				contr_altit_3 = T;
				altit = (contr_altit_1+contr_altit_2+contr_altit_3)/3;
				k = T;
				_delay_ms(2000);
				k = 8888;
				_delay_ms(5000);
				k = altit;
				_delay_ms(2000);
				
				perm_butt = 0;
			}
		}
		else {if (butcount3>0) {butcount3--;} }
			
		if (~PINB & (1<<6))
		{
			if (butcount6<5) {butcount6++;}
			else
			{
				while (~PINB & (1<<6))
				{
				TWI_W(0xF4, 0x2E);
				_delay_ms(4.5);
				UT = TWI_R_W(0xF6);

				TWI_W(0xF4, 0x34);
				_delay_ms(4.5);
				UP = TWI_R_W(0xF6);
				X1 = (UT-AC6)*AC5/32768;
				X2 = (long)MC*2048/(X1+MD);
				B5 = X1+X2;
				T = (B5+8)/16;
					
				k = T;
				
		if (T>250)
		{
			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);
			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

			PORTB |= (1<<5);
			_delay_ms(100);
			PORTB &= ~(1<<5);
			_delay_ms(100);

		}
				
				}
			}
		}
		else {if (butcount6>0) {butcount6--;}}
			
			

		if (~PINB & (1<<7))
		{
			if (butcount7<5) {butcount7++;}
			else
			{
				while (~PINB & (1<<7))
				{
					TWI_W(0xF4, 0x2E);
					_delay_ms(4.5);
					UT = TWI_R_W(0xF6);

					TWI_W(0xF4, 0x34);
					_delay_ms(4.5);
					UP = TWI_R_W(0xF6);
					X1 = (UT-AC6)*AC5/32768;
					X2 = (long)MC*2048/(X1+MD);
					B5 = X1+X2;
					
					T = (B5+8)/16; // для измерения давления эта строка лишняя
					perm_butt = 1;
					
					B6 = B5-4000;
					X1 = (B2*(B6*B6/4096))/2048;
					X2 = AC2*B6/2048;
					X3 = X1+X2;
					B3 = ((AC1*4+X3)+2)/4;
					X1 = AC3*B6/8192;
					X2 = (B1*(B6*B6/4096))/65536;
					X3 = ((X1+X2)+2)/4;
					B4 = AC4*(unsigned long)(X3+32768)/32768;
					B7 = ((unsigned long)UP-B3)*(50000>>0x00);
					if (B7<0x80000000)
					{
						p = (B7*2)/B4;
					}
					else
					{
					p = (B7/B4)*2;
					}
					X1 = (p/256)*(p/256);
					X1 = (X1*3038)/65536;
					X2 = (-7357*p)/65536;
					p = p+(X1+X2+3791)/16;
					k = p/100;
	
				}
			}
		}
		else {if (butcount7>0) {butcount7--;}}

			
			k = 0; 
		
    }
} 


/*

// 		if (sel_mode == 1)
// 		{
// 			TWI_W(0xF4, 0x2E);
// 			_delay_ms(4.5);
// 			UT = TWI_R_W(0xF6);
//
// 			TWI_W(0xF4, 0x34);
// 			_delay_ms(4.5);
// 			UP = TWI_R_W(0xF6);
// 			X1 = (UT-AC6)*AC5/32768;
// 			X2 = (long)MC*2048/(X1+MD);
// 			B5 = X1+X2;
// 			T = (B5+8)/16;
//
// 			k = T;
// 		}

if ((sel_mode == 1) && (perm_butt == 0))
{
	TWI_W(0xF4, 0x2E);
	_delay_ms(4.5);
	UT = TWI_R_W(0xF6);

	TWI_W(0xF4, 0x34);
	_delay_ms(4.5);
	UP = TWI_R_W(0xF6);
	X1 = (UT-AC6)*AC5/32768;
	X2 = (long)MC*2048/(X1+MD);
	B5 = X1+X2;
	T = (B5+8)/16;
	
	contr_altit_1 = T;
	k = T;
	perm_butt = 1;
	
}


if ((sel_mode == 2) && (perm_butt == 0))
{
	TWI_W(0xF4, 0x2E);
	_delay_ms(4.5);
	UT = TWI_R_W(0xF6);

	TWI_W(0xF4, 0x34);
	_delay_ms(4.5);
	UP = TWI_R_W(0xF6);
	X1 = (UT-AC6)*AC5/32768;
	X2 = (long)MC*2048/(X1+MD);
	B5 = X1+X2;
	
	T = (B5+8)/16; // для измерения давления эта строка лишняя
	perm_butt = 1;
	
	// 			B6 = B5-4000;
	// 			X1 = (B2*(B6*B6/4096))/2048;
	// 			X2 = AC2*B6/2048;
	// 			X3 = X1+X2;
	// 			B3 = ((AC1*4+X3)+2)/4;
	// 			X1 = AC3*B6/8192;
	// 			X2 = (B1*(B6*B6/4096))/65536;
	// 			X3 = ((X1+X2)+2)/4;
	// 			B4 = AC4*(unsigned long)(X3+32768)/32768;
	// 			B7 = ((unsigned long)UP-B3)*(50000>>0x00);
	// 			if (B7<0x80000000)
	// 			{
	// 				p = (B7*2)/B4;
	// 			}
	// 			else
	// 			{
	// 				p = (B7/B4)*2;
	// 			}
	// 			X1 = (p/256)*(p/256);
	// 			X1 = (X1*3038)/65536;
	// 			X2 = (-7357*p)/65536;
	// 			p = p+(X1+X2+3791)/16;
	
	// 			k = p/100;
	contr_altit_2 = T;
	k = T;
}

if ((sel_mode == 3) && (perm_butt == 0))
{
	k = 0;
	_delay_ms(1000);
	k = (contr_altit_1 + contr_altit_2) / 2;
	perm_butt = 1;
}
*/
