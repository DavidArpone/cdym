#include "mef.h"

enum stateMachineStates{
	START, INIT, WAIT, ON, OFF, SET_TIME, SET_ALARM
} sms;

enum interruptTimerStateFlag{
	ENABLED, DISABLED
} itsf;

static volatile sms st = START;
volatile sflg flag = KEEP;
static volatile itsf FLAG_ON = DISABLED;
volatile tiflg FLAG_INT = OFF;

void updateMef(){
	
	switch(st){
		case START:
			
			if(flag == NEXT){
				flag = KEEP;
				st = INIT;
			}
			break;
			
		case INIT:
			
			if(flag == NEXT){
				flag = KEEP;
				st = WAIT;
			}
			break;
			
		case WAIT:
			
			switch(flag){
				case NEXT_ON:
					flag = KEEP;
					st = ON;
					break;
				case NEXT_OFF:
					flag = KEEP;
					st = OFF;
					break;
				case NEXT_ST:
					flag = KEEP;
					st = SET_TIME;
					break;
				case NEXT_SA:
					flag = KEEP;
					st = SET_ALARM;
					break;				
				default:
					flag = KEEP;
					break;
			}
			
			break;
			
		case ON:
			
			if(FLAG_ON == DISABLED){
				FLAG_ON = ENABLED;
			}
			
			st = WAIT;
			
			break;
			
		case OFF:
			
			if(FLAG_ON == ENABLED){
				FLAG_ON = DISABLED;
			}
			
			st = WAIT;
			
			break;
			
		case SET_TIME:
			
			if(flag == NEXT){
				flag = KEEP;
				st = WAIT;
			}
			
			break;
			
		case SET_ALARM:
			
			if(flag == NEXT){
				flag = KEEP;
				st = WAIT;
			}
			
			break;
	}
	
}

sms getState(){
	
	return st;
	
}

void state_START(){
	UART_INIT();
	i2c_init();
	sei();
	flag = NEXT;
	enviar_mensaje("Bienvenido :D\r\n");
}

void state_INIT(){
	enviar_mensaje("Seleccione una opcion\r\n");
	enviar_mensaje("ON\r\n");
	enviar_mensaje("SET TIME\r\n");
	enviar_mensaje("SET ALARM\r\n");
	flag = NEXT;
	enviar_mensaje("OFF\r\n");
}

void state_WAIT(){
	
	int termine=0;
	
	UCSR0B|=(1<<RXCIE0);
	
	while (!termine){
		if((RX_BUFFER[rx_posicion.indice_lectura ]=='O')&&(RX_BUFFER[(rx_posicion.indice_lectura+1)%TAMANIO_RX]=='N')&&(RX_BUFFER[(rx_posicion.indice_lectura+2)%TAMANIO_RX]=='\r')){
			termine=1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 3) % TAMANIO_RX;
			flag = NEXT_ON;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura ]=='O')&&(RX_BUFFER[(rx_posicion.indice_lectura+1)%TAMANIO_RX]=='F')&&(RX_BUFFER[(rx_posicion.indice_lectura+2)%TAMANIO_RX]=='F')&&(RX_BUFFER[(rx_posicion.indice_lectura+3)%TAMANIO_RX]=='\r')){
			termine=1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 4) % TAMANIO_RX;
			flag = NEXT_OFF;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura] == 'S') &&(RX_BUFFER[(rx_posicion.indice_lectura + 1) % TAMANIO_RX] == 'E') &&(RX_BUFFER[(rx_posicion.indice_lectura + 2) % TAMANIO_RX] == 'T') &&(RX_BUFFER[(rx_posicion.indice_lectura + 3) % TAMANIO_RX] == ' ') &&(RX_BUFFER[(rx_posicion.indice_lectura + 4) % TAMANIO_RX] == 'A') && (RX_BUFFER[(rx_posicion.indice_lectura + 5) % TAMANIO_RX] == 'L') &&(RX_BUFFER[(rx_posicion.indice_lectura + 6) % TAMANIO_RX] == 'A') && (RX_BUFFER[(rx_posicion.indice_lectura + 7) % TAMANIO_RX] == 'R') && (RX_BUFFER[(rx_posicion.indice_lectura + 8) % TAMANIO_RX] == 'M') && (RX_BUFFER[(rx_posicion.indice_lectura + 9) % TAMANIO_RX] == '\r')){
			termine = 1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura + 10) % TAMANIO_RX;
			flag = NEXT_SA;
		}
		else if((RX_BUFFER[rx_posicion.indice_lectura] == 'S') &&(RX_BUFFER[(rx_posicion.indice_lectura + 1) % TAMANIO_RX] == 'E') &&(RX_BUFFER[(rx_posicion.indice_lectura + 2) % TAMANIO_RX] == 'T') &&(RX_BUFFER[(rx_posicion.indice_lectura + 3) % TAMANIO_RX] == ' ') &&(RX_BUFFER[(rx_posicion.indice_lectura + 4) % TAMANIO_RX] == 'T') && (RX_BUFFER[(rx_posicion.indice_lectura + 5) % TAMANIO_RX] == 'I') &&(RX_BUFFER[(rx_posicion.indice_lectura + 6) % TAMANIO_RX] == 'M') && (RX_BUFFER[(rx_posicion.indice_lectura + 7) % TAMANIO_RX] == 'E') && (RX_BUFFER[(rx_posicion.indice_lectura + 8) % TAMANIO_RX] == '\r')){
			termine = 1;
			rx_posicion.indice_lectura=(rx_posicion.indice_lectura +9) % TAMANIO_RX;
			flag = NEXT_ST;
		}	
		else{
			rx_posicion.indice_lectura = (rx_posicion.indice_lectura + 1) % TAMANIO_RX;
		}
	}
	
	UCSR0B &= ~(1<<RXCIE0);
};



void state_ON(){
	
	char mensaje[20];
	uint8_t s,min,h,d,mes,a;
	
	if(FLAG_INT){
		get_time(&s,&min,&h,&d,&mes,&a);
		build_msg(mensaje,&s,&min,&h,&d,&mes,&a);
		enviar_mensaje(mensaje);
		FLAG_INT = OFF;
	}
}


void state_SET_TIME(){
	
	flag = NEXT;
}

void state_SET_ALARM(){
	
	flag = NEXT;
}

