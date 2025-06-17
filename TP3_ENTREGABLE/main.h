
#ifndef main_H
#define main_H

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#define TAMANIO_RX 18
#define TAMANIO_TX 32

#include "I2C.h"
#include "UART.h"
#include "mef.h"

struct Buffer {
	uint8_t indice_escritura;
	uint8_t indice_lectura;
};

#endif