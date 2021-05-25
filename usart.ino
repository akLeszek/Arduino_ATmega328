enum m{
	OUT, 
	IN
};
enum s{
	HI, 
	LO
};

static volatile uint8_t toggle = 0;

void _pinMode(uint8_t pin, uint8_t m) {
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
void _digitalWrite(uint8_t pin, uint8_t s){ 
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
		return ((PIND & (1 << pin)) >> pin); 
	else if (pin < 14)
		return ((PINB & (1 << (pin-8))) >> (pin-8)); 
}

// Inicjalizacja interfejsu USART.
// Brak kontroli parzystosci, 1 bit stopu, 8 bitow danych
void serialInit(uint16_t baud){
	uint8_t ubrbaud = (16000000/16/baud-1);
	UBRR0H = (ubrbaud >> 8);
	UBRR0L = ubrbaud;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	// Włączenie perwania odbioru danych.
	UCSR0B |= (1 << RXCIE0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}
// funkcja przeslania pojedynczego znaku.
void serialWrite(uint8_t data){
	while (!(UCSR0A) & (1 << UDRE0));
	UDR0 = data;
}
// funkcja przeslania ciagu znakow.
void serialPrintStr(char *text){
	while(*text){
		serialWrite(*text++);
	}
}
// funkcja przeslania wartosci dziesitnej
void serialPrintInt(uint16_t value){
	uint8_t dig1 = '0', dig2 = '0', dig3 = '0', dig4 = '0';
    while(value >= 10000){
        value -= 10000;
        dig1++;
    }
    while(value >= 1000){
        value -= 1000;
        dig2++;
    }
    while(value >= 100){
        value -= 100;
        dig3++;
    }
    while(value >= 10){
        value -= 10;
        dig4++;
    }
    uint8_t prevPrinted = 0;
    if(dig1 != '0') {serialWrite(dig1); prevPrinted = 1;}
    if(prevPrinted || (dig2 != '0')) {serialWrite(dig2); prevPrinted = 1;}
    if(prevPrinted || (dig3 != '0')) {serialWrite(dig3); prevPrinted = 1;}
    if(prevPrinted || (dig4 != '0')) {serialWrite(dig4); prevPrinted = 1;}
    serialWrite(value + '0');
}
// funkcja pomocnicza, wypisujaca stan portu za pomoca USART.
void serialPortStatusPrint(uint8_t port){
	serialPrintStr("Stan portu ");
	serialPrintInt(port);
	serialWrite(':');
	serialPrintInt(_digitalRead(port));
	serialPrintStr("\n");
}

void setup(){
	_pinMode(13, OUT);
	serialInit(9600);
	// globalne wlaczenie przerwan
	sei();
}

void loop(){
	// sprawdzenie obslugi przerwan przy pomocy diody LED i wypisania stanu za pomoca interfejsu USART.
	if (toggle){
		_digitalWrite(13, HI);
		serialPortStatusPrint(13);
		delay(1000);
	}
	_digitalWrite(13, LO);
	serialPortStatusPrint(13);
	delay(1000);
	toggle = 0;
}
// obsluga przerwan odbioru bajtu danych.
ISR(USART_RXC_vect){
	uint8_t r = UDR0;
	UDR0 = r;
	toggle = 1;
}
