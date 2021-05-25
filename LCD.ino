enum m{
	OUT, 
	IN
};
enum s{
	HI, 
	LO
};

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

#define RS  2
#define E   3
#define DB4 4
#define DB5 5
#define DB6 6
#define DB7 7

// Zdefiniowanie znaku specjalnego
byte heart[8] = {
  B00000,
  B00000,
  B01010,
  B10101,
  B10001,
  B01110,
  B00100,
  B00000
};

// Metoda zapisująca znak specjalny do pamięci wyświetlacza lub CG RAM
void lcdCreateChar(uint8_t address, uint8_t *chars){
	lcdWriteCmd(0x40); // ustawienie adresu pamięci generatora znaków
	for (uint8_t i = 0; i < 8; i++){
		lcdWriteData(chars[i]);
	}
}

void lcdShiftLeft(){
	lcdWriteCmd(0x18); // Przesuwana zawartość okna, kierunek lewo
}

void lcdShiftRight(){
	lcdWriteCmd(0x1c); // Przesuwana zawartość okna, kierunek prawo
}

void  _lcdOutNibble(uint8_t nibble){
	if(nibble & 0x01){
		_digitalWrite(DB4, HI);
	}
	else{
		_digitalWrite(DB4, LO);
	}
	if(nibble & 0x02){
		_digitalWrite(DB5, HI);
	}
	else{
		_digitalWrite(DB5, LO);
	}
	if(nibble & 0x04){
		_digitalWrite(DB6, HI);
	}else{
		_digitalWrite(DB6, LO);
	}
	if(nibble & 0x08){
		_digitalWrite(DB7, HI);
	}else{ 
		_digitalWrite(DB7, LO);
	}
}
void _lcdWrite(uint8_t data) {
	_digitalWrite(E, HI);
	_lcdOutNibble(data >> 4);
	_digitalWrite(E, LO);
	
	_digitalWrite(E, HI);
	_lcdOutNibble(data);
	_digitalWrite(E, LO);
	
	_delay_us(50);
}
void lcdWriteCmd(uint8_t command){
	_digitalWrite(RS, LO);
	_lcdWrite(command);
}
void lcdWriteData(uint8_t data){
	_digitalWrite(RS, HI);
	_lcdWrite(data);
}
void lcdGoto(uint8_t x, uint8_t y){
	lcdWriteCmd( 0x80 | (x + (0x40 * y)));
}

// Inicjalizacja wyświetlacza LCD
void lcdInit(){
	_pinMode(DB7, OUT);
	_pinMode(DB6, OUT);
	_pinMode(DB5, OUT);
	_pinMode(DB4, OUT);
	_pinMode(RS, OUT);
	_pinMode(E, OUT);
	_delay_ms(20); 
	
	_digitalWrite(RS, LO);
	_digitalWrite(E, LO);
	
	uint8_t i;
	for(i = 0; i < 2; i++) 
	{
		_digitalWrite(E, HI);
		_lcdOutNibble(0x03); 
		_digitalWrite(E, LO);
		_delay_ms(5); 
	}
	_digitalWrite(E, HI);
	_lcdOutNibble(0x02);  // Tryb 4-bitowy
	_digitalWrite(E, LO);
	
	_delay_ms(1); 
	
	lcdWriteCmd(0x28); // Tryb 4-bitowy, 2 linie, matryca 5x7
	lcdWriteCmd(0x08); // Wył. wyświetlacza, wył. kursora, wył. migania kursora
	lcdWriteCmd(0x01); // Wyczyść wyświetlacz
	
	_delay_ms(2);
	
	lcdWriteCmd(0x0C); // Wł. wyświetlacza, wył. kursora, wył. migania kursora
}
void lcdPrintStr(const char *text){
	while(*text) lcdWriteData(*text++);
}

void setup(){
	lcdInit();
	lcdCreateChar(0, heart);
}

void loop(){
	lcdGoto(6,0);
	lcdPrintStr("MILEGO DNIA ZYCZA STUDENCI US");	
	
	lcdGoto(0,1);
	for (uint8_t i = 0; i < 20; i++){
		lcdPrintStr(" ");
		_lcdWrite(0);
	}
	
	// lcdShiftLeft();
	lcdShiftRight();
	
	delay(100);
}
