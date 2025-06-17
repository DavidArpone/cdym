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

int main(void){
	
	//Setear valores del timer para interrumpir cada 1 segundo 
	
	while (1) {
		
		
		if(FLAG_INT == ON){
			
			state_ON();
			FLAG_INT = OFF;	//Limpia FLAG de temporizador
			
		}
		
				
		switch (getState()){
			case START:
				state_START()
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

ISR(TWI_vect){}
ISR(USART_RX_vect){
	
	uint8_t dato_recibido = UDR0; //Limpiamos registro para nueva interrupcion
	uint8_t siguiente = (rx_posicion.indice_escritura + 1) % TAMANIO_RX;
	if(!(siguiente == rx_posicion.indice_lectura)){
		RX_BUFFER[rx_posicion.indice_escritura] = dato_recibido; //Descarta el dato si el buffer esta lleno
		rx_posicion.indice_escritura = siguiente
	}
	
	if(flag != KEEP){
		updateMef();
	}
	
}


ISR(USART_UDRE_vect){
	
	//Interrupme cuando se termina de enviar un mensaje por UART
	
	UDR0 = TX_BUFFER[tx_posicion.indice_lectura];
	tx_posicion.indice_lectura = (tx_posicion.indice_lectura + 1) % TAMANIO_TX;
	if(flag == NEXT){
		//Entramos aca cuando finaliza state_Start() y state_INIT()
		updateMef();
		UCSR0B &= ~(1 << UDRIE0); //Limpiamos interrupcion
	}

}
	
ISR(TIMER0_COMPA_vect){
	
	if(FLAG_ON == ENABLED){
		if(FLAG_INT == OFF){
			FLAG_INT = ON;
		}
	}
	
}

