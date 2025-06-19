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

enum interruptFlagTimer1 FLAG_TIMER = TIMER_OFF;

int main(){
	
	rx_posicion.indice_escritura = 0;
	rx_posicion.indice_lectura = 0;
	tx_posicion.indice_lectura = 0;
	tx_posicion.indice_escritura = 0;
	
	task_INIT();
	task_PRINT();
	
	while (1) {
		
		
		if(FLAG_TIMER == TIMER_ON){
			
			task_ON();
			FLAG_TIMER = TIMER_OFF;	//Limpia FLAG de temporizador
			
		}
		if(FLAG == WAIT){
			
			task_WAIT();
			
		}
		if(FLAG == ON || FLAG == OFF){
			
			timer1_ENABLE();
			FLAG = DEFAULT;
			
		}
		if(FLAG == SET_ALARM){
			
			task_SET_ALARM();
			FLAG = DEFAULT;
			
		}
		if(FLAG == SET_TIME){
			
			task_SET_TIME();
			FLAG = DEFAULT;
			
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
	
	if(dato_recibido == '\n'){
		FLAG = WAIT;
	}
	
}


ISR(USART_UDRE_vect){
	
	UDR0 = TX_BUFFER[tx_posicion.indice_lectura];
	tx_posicion.indice_lectura = (tx_posicion.indice_lectura + 1) % TAMANIO_TX;
	if (tx_posicion.indice_lectura==tx_posicion.indice_escritura){
		UCSR0B &= ~(1<<UDRIE0); // cuando mandas el ultimo byte tenes que deshabilitar la interrupcion de buffer vacio
	}
	

}
	
ISR(TIMER1_COMPA_vect){
	
	if(FLAG_TIMER == TIMER_OFF){
		FLAG_TIMER = TIMER_ON;
	}
	
}
