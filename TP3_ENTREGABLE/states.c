#include "states.h"

enum timerInterruptFlag FLAG_INT = DISABLED;
enum stateFlag FLAG = DEFAULT;

void task_INIT(){
	UART_INIT();
	i2c_init();
	timer1_CONFIG();
	sei();
	enviar_mensaje("Bienvenido :D\r\n");
}

void task_PRINT(){
	enviar_mensaje("Seleccione una opcion\r\n");
	enviar_mensaje("ON\r\n");
	enviar_mensaje("SET TIME\r\n");
	enviar_mensaje("SET ALARM\r\n");
	enviar_mensaje("OFF\r\n");
}

void task_WAIT(){
	
	int termine=0;
	
	UCSR0B|=(1<<RXCIE0);
	
	while (!termine){
		if((RX_BUFFER[rx_posicion.indice_lectura ]=='O')&&(RX_BUFFER[(rx_posicion.indice_lectura+1)%TAMANIO_RX]=='N')&&(RX_BUFFER[(rx_posicion.indice_lectura+2)%TAMANIO_RX]=='\r')){
			termine=1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 3) % TAMANIO_RX;
			FLAG_INT = ENABLED;
			FLAG = ON;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura ]=='O')&&(RX_BUFFER[(rx_posicion.indice_lectura+1)%TAMANIO_RX]=='F')&&(RX_BUFFER[(rx_posicion.indice_lectura+2)%TAMANIO_RX]=='F')&&(RX_BUFFER[(rx_posicion.indice_lectura+3)%TAMANIO_RX]=='\r')){
			termine=1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 4) % TAMANIO_RX;
			FLAG_INT = DISABLED;
			FLAG = OFF;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura] == 'S') &&(RX_BUFFER[(rx_posicion.indice_lectura + 1) % TAMANIO_RX] == 'E') &&(RX_BUFFER[(rx_posicion.indice_lectura + 2) % TAMANIO_RX] == 'T') &&(RX_BUFFER[(rx_posicion.indice_lectura + 3) % TAMANIO_RX] == ' ') &&(RX_BUFFER[(rx_posicion.indice_lectura + 4) % TAMANIO_RX] == 'A') && (RX_BUFFER[(rx_posicion.indice_lectura + 5) % TAMANIO_RX] == 'L') &&(RX_BUFFER[(rx_posicion.indice_lectura + 6) % TAMANIO_RX] == 'A') && (RX_BUFFER[(rx_posicion.indice_lectura + 7) % TAMANIO_RX] == 'R') && (RX_BUFFER[(rx_posicion.indice_lectura + 8) % TAMANIO_RX] == 'M') && (RX_BUFFER[(rx_posicion.indice_lectura + 9) % TAMANIO_RX] == '\r')){
			termine = 1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 10) % TAMANIO_RX;
			FLAG = SET_ALARM;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura] == 'S') &&(RX_BUFFER[(rx_posicion.indice_lectura + 1) % TAMANIO_RX] == 'E') &&(RX_BUFFER[(rx_posicion.indice_lectura + 2) % TAMANIO_RX] == 'T') &&(RX_BUFFER[(rx_posicion.indice_lectura + 3) % TAMANIO_RX] == ' ') &&(RX_BUFFER[(rx_posicion.indice_lectura + 4) % TAMANIO_RX] == 'T') && (RX_BUFFER[(rx_posicion.indice_lectura + 5) % TAMANIO_RX] == 'I') &&(RX_BUFFER[(rx_posicion.indice_lectura + 6) % TAMANIO_RX] == 'M') && (RX_BUFFER[(rx_posicion.indice_lectura + 7) % TAMANIO_RX] == 'E') && (RX_BUFFER[(rx_posicion.indice_lectura + 8) % TAMANIO_RX] == '\r')){
			termine = 1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura +9) % TAMANIO_RX;
			FLAG = SET_TIME;
		}
		else{
			rx_posicion.indice_lectura = (rx_posicion.indice_lectura + 1) % TAMANIO_RX;
		}
	}
	
	UCSR0B &= ~(1<<RXCIE0);
};

void task_ON(){
	
	char mensaje[20];
	uint8_t s,min,h,d,mes,a;
	get_time(&s,&min,&h,&d,&mes,&a);
	build_msg(mensaje,&s,&min,&h,&d,&mes,&a);
	enviar_mensaje(mensaje);	
	
}

void task_SET_TIME(){
	
	
}

void task_SET_ALARM(){
	
	
}

void timer1_CONFIG(){
	
	TCCR1B |= (1 << WGM12);					//Modo CTC (Clear Timer on Compare Match)
	TCCR1B |= (1 << CS12) | (1 << CS10);	//Prescaler 1024
	OCR1A = 15624;							//Valor de comparación para 1 segundo
	TCNT1 = 0;								// Inicializar contador
	
}

void timer1_ENABLE() {
	
	if(FLAG_INT == ENABLED){
		TIMSK1 |= (1 << OCIE1A);			//Habilitar interrupción por comparación A
	}
	if(FLAG_INT == DISABLED){
		TIMSK1 &= ~(1 << OCIE1A);			//Deshabilito la interrupcion por comparacion
	}
	
	
}