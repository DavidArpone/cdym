/*
 *
 * TP3_ENTREGABLE
 * Reloj Calendario con Alarma
 * Arduino UNO, RTC DS3231 
 * Circuitos Digitales y Microcontroladores
 * Arpone, David. Iloro, Gonzalo. Lacourrage, Santiago.
 * 24/06/2025
 *
 */ 

#include "main.h"

enum stateMachineStates sms;
int contador = 0;

int main(){
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler = 64

	OCR0A = 249; // Con 16 MHz y prescaler 64 ? interrupción cada 1 ms

	TIMSK0 |= (1 << OCIE0A); // Habilitar interrupción por comparación A
	
	rx_posicion.indice_escritura = 0;
	rx_posicion.indice_lectura = 0;
	tx_posicion.indice_lectura = 0;
	tx_posicion.indice_escritura = 0;
	
	while (1) {
		
		
		if(FLAG_INT == ON){
			
			state_ON();
			FLAG_INT = OFF;	//Limpia FLAG de temporizador
			
		}
		
			
			sms = getState();
			
			switch (sms){
			case START:
			state_START();
			break;
			case INIT:
			state_INIT();
			break;
			case WAIT:
			state_WAIT();
			break;
			case SET_ALARM:
			state_SET_ALARM();
			break;
			case SET_TIME:
			state_SET_TIME();
			break;
			default:
			break;
			
		}
			
		
    }
}

ISR(USART_RX_vect){
	
	uint8_t dato_recibido = UDR0; //Limpiamos registro para nueva interrupcion
	uint8_t siguiente = (rx_posicion.indice_escritura + 1) % TAMANIO_RX;
	if(!(siguiente == rx_posicion.indice_lectura)){
		RX_BUFFER[rx_posicion.indice_escritura] = dato_recibido; //Descarta el dato si el buffer esta lleno
		rx_posicion.indice_escritura = siguiente;
	}
	
	//if(flag != KEEP ){//PONER CONDICION DE ENTER
	if( dato_recibido == '\n'){
		updateMef();
		//banderita on
	}
	
}


ISR(USART_UDRE_vect){
	
	UDR0 = TX_BUFFER[tx_posicion.indice_lectura];
	tx_posicion.indice_lectura = (tx_posicion.indice_lectura + 1) % TAMANIO_TX;
	if (tx_posicion.indice_lectura==tx_posicion.indice_escritura){
		UCSR0B &= ~(1<<UDRIE0); // cuando mandas el ultimo byte tenes que deshabilitar la interrupcion de buffer vacio
		
	}
	if(flag == NEXT){
		//Entramos aca cuando finaliza state_Start() y state_INIT()
		updateMef();
	}

}
	
ISR(TIMER1_COMPA_vect){
	
	if(FLAG_ON == ENABLED){
		if(FLAG_INT == OFF){
			FLAG_INT = ON;
		}
	}
	
}

ISR(TIMER0_COMPA_vect){
	
	contador++;
	
	if (contador >= 100) {
		contador = 0;

		// Acción cada 100 ms
		updateMef();
	}
	
	
	
}

