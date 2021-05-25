// Kierunek lini portu
enum m{
	OUT = 0, 
	IN = 1
};
// Stan lini portu
enum s{
	HI, 
	LO
};

// Funkcja przyjmuje dwie wartosci pin oraz m. Sluzy do wyznaczenia kierunku lini portu.
// pin jest numerem pinu, który chcemy ustawic.
// m jest wartoscia okreslajaca kierunek lini portu.
void _pinMode(uint8_t pin, uint8_t m) {
    if (( pin < 8 ) && (m == 0)){ 
       DDRD |= (1 << pin); 
	}else if (( pin < 8 ) && (m == 1)){ 
       DDRD &= ~(1 << pin); 
    }else if (( pin < 14 ) && (m == 0)){
       DDRB |= (1 << (pin - 8));  
	}else if (( pin < 14 ) && (m == 1)){
       DDRB &= ~(1 << (pin - 8));  
	}
}

// Funkcja przyjmuje dwie wartosci pin oraz s. Sluzy do ustawienia stany okreslonej lini portu.
// pin jest numerem pinu, który chcemy ustawic.
// s jest wartoscia okreslajaca stan portu.
void _digitalWrite(uint8_t pin,uint8_t s){
	
	if (( pin < 8 ) && (s == 0)){ 
       PORTD |= (1 << pin); 
	}else if (( pin < 8 ) && (s == 1)){ 
       PORTD &= ~(1 << pin); 
    }else if (( pin < 14 ) && (s == 0)){
       PORTB |= (1 << (pin - 8));  
	}else if (( pin < 14 ) && (s == 1)){
       PORTB &= ~(1 << (pin - 8));  
	}
}

// Funkcja służy do zwrócenie stanu lini portu
// 0 - niski
// 1 - wysoki
uint8_t _digitalRead(uint8_t pin){
	if(pin < 8){
		return((PIND & (1 << pin)) >> pin);
	}else if (pin < 14) {
		return((PINB & (1 << (pin - 8))) >> (pin-8));
	}
}

void setup() {
	// Ustawienie pinu 13 jako OUT
	_pinMode(13, OUT);
}

void loop() {
	// Zapalenie diody na 1000ms
	_digitalWrite(13, HI);
	delay(1000);
	// Zgasezenie diody ma 1000ms
	_digitalWrite(13, LO);
	delay(1000);
}