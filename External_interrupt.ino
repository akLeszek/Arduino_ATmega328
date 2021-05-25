enum m{
	OUT, 
	IN
};
enum s{
	HI, 
	LO
};

volatile uint8_t int0 = 0, int1 = 0;

void _pinMode(uint8_t pin, uint8_t m){
    if (( pin < 8 ) && (m == 0)){ 
       DDRD |= (1 << pin); 
	}else if (( pin < 8 ) && (m == 1)){ 
       DDRD &= ~(1 << pin); 
    }else if (( pin < 14 ) && (m == 0)){
       DDRB |= (1 << (pin - 8));  
	}else if (( pin<14 ) && (m == 1)){
       DDRB &= ~(1 << (pin - 8));  
	}
}
void _digitalWrite(uint8_t pin,uint8_t s){ 
	if (( pin < 8 ) && (s == 1)){ 
       PORTD &= ~(1 << pin); 
	}else if (( pin < 8 ) && (s == 0)){ 
       PORTD |= (1 << pin); 
	}else if (( pin < 14 ) && (s == 0)){  
       PORTB |= (1 << pin-8); 
	}else if (( pin < 14 ) && (s == 1)){ 
       PORTB &= ~(1 << pin-8); 
    }
}
uint8_t _digitalRead(uint8_t pin){
	if (pin < 8)
		return ((PIND & (1<<pin)) >> pin); 
	else if (pin < 14)
		return ((PINB & (1<<(pin-8))) >> (pin-8)); 
}

void serialInit(uint16_t baud){
	uint8_t _ubr = (16000000/16/baud-1); 
	UCSR0B |= (1<<RXEN0)  | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0H  = (_ubr >> 8);
	UBRR0L  = _ubr;
}
void serialWrite(uint8_t data){
	UDR0 = data;
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
}
void serialPrintStr(char *text){
	while(*text){
		serialWrite(*text++);
	}
}
//Obsluga przerwan zewnetrznych na liniach INT0 i INT1 
void externalInterruptInit(){
    digitalWrite(2, HIGH);
	EIMSK |= (1 << INT0);
	EIMSK |= (1 << INT1);
	EICRA |= (1 << ISC01) | (1 << ISC10);
}

void setup(){
	serialInit(9600);
	externalInterruptInit();
	sei();
}

void loop(){
	// Wypisanie przez USART informacji o stanie przerwan INT0 lub INT1.
	if (int0 > 0){
		serialPrintStr("INT0: 1\n");
		int0 = 0;
	}
	if (int1 > 0){
		serialPrintStr("INT1: 1\n");
		int1 = 0;
	}
}
// Inkrementacja wartosci int0 lub int1 w zaleznosci od wystepowanego przerwania.
ISR(INT0_vect){
	int0++;
}
ISR(INT1_vect){
	int1++;
}