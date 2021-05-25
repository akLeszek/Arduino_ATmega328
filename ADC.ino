enum m{
	OUT, 
	IN
};
enum s{
	HI, 
	LO
};

#define przycisk1 1014
#define przycisk2 1014
#define przycisk3 1014


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
void serialPrintInt(uint16_t value){
	const char buffer[6];                       
    itoa(value, buffer, 10);             
	serialPrintStr(buffer);    
}


void adcInit(){
    ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0)|(1 << ADEN);	
}
uint16_t _analogRead(uint8_t channel){
    ADMUX = (1 << REFS0) | (channel & 0x0f);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

uint8_t button(){
	if (ADCval <= 1024 || ADCval >= 1004){
		serialPrintStr("Przycisk 1");
	}
	if (ADCval <= 722 || ADCval >= 702){
		serialPrintStr("Przycisk 2");
	}
	if (ADCval <= 623 || ADCval >= 593){
		serialPrintStr("Przycisk 3");
	}
}


void setup(){
	serialInit(9600);
	adcInit();
	sei();
}

void loop(){
	uint16_t ADCval = _analogRead(0);
	uint16_t ADCv = ((uint32_t)ADCval * 5000UL)/1024UL;
	
	serialPrintStr("ADC: ");
	serialPrintInt(ADCval);
	serialPrintStr(" Napiecie: ");
	serialPrintInt(ADCv / 1000);
	serialPrintStr(".");
	serialPrintInt(ADCv % 100);
	serialPrintStr("\n");
	
	button();
	delay(1000);
}