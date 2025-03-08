/*
 * LCD_8b.c
 *
 * Created: 3/1/2025 4:44:25 PM
 *  Author: Britany Barillas
 */ 
#include "LCD_8b.h"
// RS	E	D7... D0
unsigned char pines[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

void initLCD8(void){
	// salidas
	for (uint8_t i = 0; i < 10; i++) {
		uint8_t pin = pines[i];
		if (pin >= 8 && pin <= 13) {
			// Pines en PORTB
			DDRB |= (1 << (pin - 8));
			} else if (pin <= 7) {
			// Pines en PORTD
			DDRD |= (1 << pin);
		}
	}
	_delay_ms(15);
	// Function SET
	LCD8_PORT(0b0011100000);
	_delay_ms(5);
	// display ON/ OFF
	LCD8_PORT(0b0000110000);
	_delay_ms(10);
	// Entre mode
	LCD8_PORT(0b0000011000);
	_delay_ms(10);
	//Clear display
	LCD8_PORT(0b0000000100);
	_delay_ms(10);
}

// D7...D0 E RS
void LCD8_PORT(uint16_t mascara) {
	for(int i = 0; i<10; i++){ // verifica cada posición
		if(mascara&(1<<i)){	// enciende los pines
			uint8_t pin = pines[i];
			if(pin >= 8 && pin <= 13){
				PORTB |= (1 << (pin - 8));
				} else if (pin <= 7){
				PORTD |= (1 << pin);	// coloca el valor en el puerto
			}
			} else { // apaga los pines
			uint8_t pin = pines[i];
			if(pin >= 8 && pin <= 13){
				PORTB &= ~(1 << (pin - 8));
				} else if (pin <= 7){
				PORTD &= ~(1 << pin);
			}
		}
	}
	PORTD |= (1<<4);
	_delay_ms(3);
	PORTD &= ~(1<<4);
}


void LCD8_CMD(uint16_t mascara){
	for(int i = 0; i<10; i++){
		if(mascara&(1<<i)){
			uint8_t pin = pines[i];
			if(pin >= 8 && pin <= 13){
				PORTB |= (1 << (pin - 8));
				} else if (pin <= 7){
				PORTD |= (1 << pin);
			}
			} else {
			uint8_t pin = pines[i];
			if(pin >= 8 && pin <= 13){
				PORTB &= ~(1 << (pin - 8));
				} else if (pin <= 7){
				PORTD &= ~(1 << pin);
			}
		}
	}
	// agregando el Enable
	_delay_ms(3);
	PORTD |= (1<<4);
	_delay_ms(3);
	PORTD &= ~(1<<4);
}

void LCD8_Variable(float v, uint8_t n){
	char str[10];
	float_to_string(v, str, n);
	LCD8_Write_String(str); // envia la cadena convertida
}

void LCD8_Variable_U(uint8_t v){
	char str[4];
	uint8_to_string(v, str);
	LCD8_Write_String(str);
}

void LCD8_Write_Char(char c){
	uint16_t valor = (c << 2)|1; // un unico caracter
	LCD8_CMD(valor);
}


void LCD8_Write_String(char *a){
	for(int i = 0; a[i] != '\0'; i++){ // recorre la cadena
		LCD8_Write_Char(a[i]);
	}
}


void LCD8_Set_Cursor(uint16_t c, uint16_t f){
	uint16_t coordenada;
	if (f == 0){ // primera fila
		coordenada = (c << 2) + 512;
		LCD8_CMD(coordenada);
		} else if (f == 1){ // segunda fila
		coordenada = ((c + 64)<<2) + 512;
		LCD8_CMD(coordenada);
	}
}

void LCD8_Clear(void){
	LCD8_CMD(0b100); // comando para borrar todo
}

void float_to_string(float num, char *buffer, uint8_t decimales) {
	int parte_entera = (int)num;  // parte entera
	int parte_decimal = (int)((num - parte_entera) * 100);  // parte decimal
	
	if (parte_decimal < 0) parte_decimal *= -1;  // no negativo

	// Convertir a caracteres
	int i = 0;
	if (parte_entera == 0) {
		buffer[i++] = '0';
		} else {
		int temp = parte_entera;
		char temp_buffer[10];
		int j = 0;
		while (temp > 0) {  // Extraer dígitos
			temp_buffer[j++] = (temp % 10) + '0';
			temp /= 10;
		}
		while (j > 0) {  // Invertir los dígitos
			buffer[i++] = temp_buffer[--j];
		}
	}

	// punto decimal
	buffer[i++] = '.';

	// decimal a caracteres
	buffer[i++] = (parte_decimal / 10) + '0';
	buffer[i++] = (parte_decimal % 10) + '0';

	
	buffer[i] = '\0';
}

void uint8_to_string(uint8_t num, char *buffer) {
	uint8_t i = 0;
	char temp[4];  // Máximo 3 dígitos + '\0'
	uint8_t j = 0;

	if (num == 0) {
		buffer[i++] = '0';
		} else {
		while (num > 0) {
			temp[j++] = (num % 10) + '0';  // Extrae el último dígito
			num /= 10;
		}
		while (j > 0) {
			buffer[i++] = temp[--j];  // Invierte el orden
		}
	}

	buffer[i] = '\0';  // Termina el string
}