#include "mef.h"

static enum stateMachineStates st = START;
enum stateFlag flag = KEEP;
enum interruptTimerStateFlag FLAG_ON = DISABLED;
enum timerInterruptFlag FLAG_INT = OFF;

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
					st = ST_ON;
					break;
				case NEXT_OFF:
					flag = KEEP;
					st = ST_OFF;
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
			
		case ST_ON:
			
			if(FLAG_ON == DISABLED){
				FLAG_ON = ENABLED;
				timer1_init();
			}
			st = WAIT;
			
			break;
			
		case ST_OFF:
			
			if(FLAG_ON == ENABLED){
				FLAG_ON = DISABLED;
				timer1_init();
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

enum stateMachineStates getState(){
	
	return st;
	
}

void state_START(){
	UART_INIT();
	i2c_init();
	timer1_config();
	sei();
	enviar_mensaje("Bienvenido :D\r\n");
	flag = NEXT;
}

void state_INIT(){
	enviar_mensaje("Seleccione una opcion\r\n");
	enviar_mensaje("ON\r\n");
	enviar_mensaje("SET TIME\r\n");
	enviar_mensaje("SET ALARM\r\n");
	
	enviar_mensaje("OFF\r\n");
	flag = NEXT;
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
	
	
	get_time(&s,&min,&h,&d,&mes,&a);
	build_msg(mensaje,&s,&min,&h,&d,&mes,&a);
	enviar_mensaje(mensaje);
	FLAG_INT = OFF;
	
	
}


void state_SET_TIME(){
	
	flag = NEXT;
}

void state_SET_ALARM(){
	
	flag = NEXT;
}

void timer1_config(){
	
	TCCR1B |= (1 << WGM12);					//Modo CTC (Clear Timer on Compare Match)
	TCCR1B |= (1 << CS12) | (1 << CS10);	//Prescaler 1024
	OCR1A = 15624;							//Valor de comparación para 1 segundo
	TCNT1 = 0;								// Inicializar contador
}

void timer1_init() {
		
	if(FLAG_INT == ON){
		TIMSK1 |= (1 << OCIE1A);			//Habilitar interrupción por comparación A
	}
	if(FLAG_INT == OFF){
		
		TIMSK1 &= ~(1 << OCIE1A);			//Deshabilito la interrupcion por comparacion
		
	}
	
	
}