/*
 * I2C.h
 *
 * Created: 3/1/2025 6:11:43 PM
 *  Author: Britany Barillas
 */ 


#ifndef I2C_H_
#define I2C_H_

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <stdint.h>


//*****************************************************************************
// Funcion para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler);
//*****************************************************************************
// Funcion de inicio de la comunicacion I2C
//*****************************************************************************
uint8_t I2C_Master_Start(void);
//*****************************************************************************
// Funcion de parada de la comunicacion I2C
//*****************************************************************************
void I2C_Master_Stop(void);
//*****************************************************************************
//Funcion de transmision de datos del maestro al esclavo
//esta funcion devolvera un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
uint8_t I2C_Master_Write(uint8_t  dato);
//*****************************************************************************
//Funcion de recepcion de datos enviados por el esclavo al maestro
//esta funcion es para leer los datos que estan en el esclavo
//*****************************************************************************
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t  ack);
//*****************************************************************************
// Funcion para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address);
#endif /* I2C_H_ */