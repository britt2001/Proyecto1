/*
 * LCD_8b.h
 *
 * Created: 3/1/2025 4:45:08 PM
 *  Author: Britany Barillas
 */ 


#ifndef LCD_8B_H_
#define LCD_8B_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// inicializa la pantalla 16x02
void initLCD8(void);

// manda valor al puerto
void LCD8_PORT(uint16_t valor);

// manda un comando
void LCD8_CMD(uint16_t mascara);

// imprime el valor de una variable float
void LCD8_Variable(float v, uint8_t n);

void LCD8_Variable_U(uint8_t v);

void uint8_to_string(uint8_t num, char *buffer);

// convierte float a string
void float_to_string(float num, char *buffer, uint8_t decimales);

// imprime en pantalla un caracter
void LCD8_Write_Char(char a);

// imprime en pantalla una cadena
void LCD8_Write_String(char *a);

// posiciona el cursor
void LCD8_Set_Cursor(uint16_t c, uint16_t f);

// borra pantalla
void LCD8_Clear(void);

#endif /* LCD_8B_H_ */