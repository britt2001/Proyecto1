/*
 * GccApplication6.c
 *
 * Created: 2/27/2025 7:59:08 PM
 * Author : Britany Barillas
 */ 

 #define F_CPU 16000000  // Define la frecuencia del microcontrolador
 #include <avr/io.h>
 #include <util/delay.h>
 #include <stdlib.h>  // Para usar itoa()
 #include <string.h>  // Para usar strcpy()
 #include "LCD_8b/LCD_8b.h"  // Incluir la librería LCD


 // --- Configuración UART ---
 void UART_init(uint16_t baudrate) {
	 uint16_t ubrr = F_CPU / 16 / baudrate - 1;
	 UBRR0H = (ubrr >> 8);
	 UBRR0L = ubrr;
	 UCSR0B = (1 << TXEN0);   // Habilita transmisión
	 UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Modo de 8 bits
 }

 void UART_sendChar(char c) {
	 while (!(UCSR0A & (1 << UDRE0))); // Esperar buffer vacío
	 UDR0 = c;
 }

 void UART_sendString(const char* str) {
	 while (*str) {
		 UART_sendChar(*str++);
	 }
 }

 // --- Definir pines ---
 #define HUMEDAD_ANALOG PC1  // A1 en Arduino (sensor de humedad)
 #define LUZ_ANALOG PC2      // A2 en Arduino (sensor LDR analógico)

 int main(void) {
	 // Configurar UART
	 UART_init(115200);

	 // Inicializar el LCD
	 initLCD8();
	 LCD8_Clear();  // Limpiar pantalla al inicio

	 // Mostrar mensaje inicial en LCD
	 LCD8_Set_Cursor(0, 0);
	 LCD8_Write_String("Iniciando...");
	 _delay_ms(2000);  // Esperar 2 segundos

	 // Configurar A1 y A2 como entradas analógicas
	 DDRC &= ~((1 << HUMEDAD_ANALOG) | (1 << LUZ_ANALOG));

	 // Habilitar ADC
	 ADMUX = (1 << REFS0); // Referencia en AVCC
	 ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler de 128

	 while (1) {
		 // --- Leer Humedad (analógica en A1) ---
		 ADMUX = (ADMUX & 0xF0) | (HUMEDAD_ANALOG & 0x0F); // Seleccionar canal PC1 (A1)
		 ADCSRA |= (1 << ADSC); // Iniciar conversión
		 while (ADCSRA & (1 << ADSC)); // Esperar resultado
		 uint16_t humedad = ADC;

		 // Determinar SECO o HÚMEDO
		 char estado_humedad[10];
		 if (humedad > 700) {
			 strcpy(estado_humedad, "SECO");
			 } else {
			 strcpy(estado_humedad, "HÚMEDO");
		 }

		 // --- Leer Luz (analógica en A2) ---
		 ADMUX = (ADMUX & 0xF0) | (LUZ_ANALOG & 0x0F); // Seleccionar canal PC2 (A2)
		 ADCSRA |= (1 << ADSC); // Iniciar conversión
		 while (ADCSRA & (1 << ADSC)); // Esperar resultado
		 uint16_t luz = ADC;

		 // --- Depuración: Mostrar valor de luz en UART ---
		 char buffer_luz[10];
		 itoa(luz, buffer_luz, 10);
		 UART_sendString("Valor luz (A2) crudo: ");
		 UART_sendString(buffer_luz);
		 UART_sendString("\r\n");

		 // Determinar CLARO u OSCURO según el nuevo umbral (800)
		 char estado_luz[10];
		 if (luz > 800) {  // Umbral ajustado a 800
			 strcpy(estado_luz, "OSCURO");
			 } else {
			 strcpy(estado_luz, "CLARO");
		 }

		 // --- Mostrar en LCD ---
		 LCD8_Clear();  // Limpiar pantalla
		 LCD8_Set_Cursor(0, 0);  // Primera línea
		 LCD8_Write_String("Humedad: ");
		 LCD8_Write_String(estado_humedad);  // Mostrar SECO o HÚMEDO

		 LCD8_Set_Cursor(0, 1);  // Segunda línea
		 LCD8_Write_String("Luz: ");
		 LCD8_Write_String(estado_luz);  // Mostrar CLARO u OSCURO

		 // --- Enviar por UART ---
		 char buffer[10];
		 UART_sendString("Humedad Tierra (A1): ");
		 itoa(humedad, buffer, 10);
		 UART_sendString(buffer);
		 UART_sendString("\r\n");

		 UART_sendString("Estado Humedad: ");
		 UART_sendString(estado_humedad);
		 UART_sendString("\r\n");

		 UART_sendString("Luz (A2): ");
		 itoa(luz, buffer, 10);
		 UART_sendString(buffer);
		 UART_sendString("\r\n");

		 UART_sendString("Estado Luz: ");
		 UART_sendString(estado_luz);
		 UART_sendString("\r\n");

		 UART_sendString("----------------------\r\n");
		 _delay_ms(2000);
	 }
 }
