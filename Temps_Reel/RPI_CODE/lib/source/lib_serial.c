/* 
 * File:   lib_serial.c
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */




#include "lib_serial.h"

/**
	Initialize the serial port in order to send and receive messages through it

	@returns: the file descriptor, or -1 if there was an error. 
	(Retreiving the file descriptor is not necessary as it is not an argument of any function of the library)

*/


int uart0_filestream;

int init_serial(){

	uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		rt_printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
		exit(EXIT_FAILURE);
	}

	//Configuration of the serial port
	// 115 520 Bauds
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR; // ignores bytes with bad parity
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	return uart0_filestream;
}

/**
	Takes an array of 7 bytes and tries to format it to a message structure

	@params unsigned char * mesg : an array of 7 bytes.
		the byte 0 should always be '<'
		the byte 1 should be an ascii char that is the label. It define what the data represent
		the bytes 2 to 5 are the float value
		the byte 6 should always be a '\n'
	@params message_serial * m : a pointer of a message. If there was an issue when reading the serial port
		the message takes 'e' (for error) as its label

	@returns 0 if there was an issue while reading the message else 1
*/ 

int bytes_to_message(unsigned char * mesg, message_serial * m){
	int ret =0;
	if(mesg[0] == '<' && mesg[6] == '\n' ){
		m->label = mesg[1];
		m->value = bytes_to_float(&mesg[2]);
		ret=1;
	}else{
		m->label = 'e';
		ret = 0;
	}
	return ret;
}


/**
	Converts a float into a byte sequence that is sent on the serial port 
	the message is '<'{tag}{the float encoded on 4 bytes}'\n

	@params float fl_value : The value to send
	@params char tag : the tag that define what the value represents.  
	
	@return 1 if the message was sent, else 0
*/
int send_float_to_serial(float fl_value, char tag){
	
	unsigned char * msg = (unsigned char *) &fl_value; 
	int ret_val = 0;
	unsigned char p_tx_buffer[MESSAGE_SERIAL_LENGTH];
	p_tx_buffer[0] = '<';
	p_tx_buffer[1] = tag;
	p_tx_buffer[2] = msg[0];
	p_tx_buffer[3] = msg[1];
	p_tx_buffer[4] = msg[2];
	p_tx_buffer[5] = msg[3];
	p_tx_buffer[6] = '\n';

			if (uart0_filestream != -1)
			{
					int count = write(uart0_filestream, &p_tx_buffer[0], MESSAGE_SERIAL_LENGTH);              //Filestream, bytes to write, number of bytes to write
					if (count < 0)
					{
							rt_printf("UART TX error\n");
					}else{
						ret_val = 1;
					}	
			}
	return ret_val;
}


/**
	Converts an array of 4 bytes into a float

	@params unsigned char * bytes : array of 4 bytes

	@return the float value
*/

float bytes_to_float(unsigned char * bytes){
	unsigned long value;
	value = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0]);
	return *(float *)&value;
}


/**
	Reads a message from the serial port. 
	The function is blocked until a suitable message is received. 

*/

message_serial read_from_serial(){
	int message_length =0;
	RTIME begin_time;
	unsigned char message_buffer[256];

	while(message_length<MESSAGE_SERIAL_LENGTH)
	{
		// Read up to 255 characters from the port if they are there
			unsigned char rx_buffer[256];
			/*
			if(message_length >0){
				
				SRTIME time_from_first_byte = rt_timer_ticks2ns(rt_timer_read() - begin_time);
				
				
				if(time_from_first_byte > MESSAGE_TIMEOUT_NS){
					message_length = 0;
					rt_printf("Dropped a message (timeout) %d\n", time_from_first_byte);
				}
				
			}
			*/
			
			int rx_length = read(uart0_filestream, (void*)rx_buffer, MESSAGE_SERIAL_LENGTH - message_length);		//Filestream, buffer to store in, number of bytes to read (max)
			
		
		if (rx_length < 0)
		{
			//rt_printf("Error when checking for rx bytes\n");
			//An error occured (will occur if there are no bytes)
		}
		else if (rx_length == 0)
		{
			//rt_printf("No data is available");
			//No data waiting
		}
		else
		{
			if(message_length == 0){
				begin_time = rt_timer_read();
				rt_printf("begin time = %d \n", begin_time);
			}


			int i;
			for (i = 0; i<rx_length; ++i) {
				message_buffer[message_length + i] = rx_buffer[i];
			}

					
			message_length = message_length + rx_length;

		}
	}
	
	message_serial m;
	bytes_to_message(message_buffer, &m);

	return m;

}

int close_serial(){
	return close(uart0_filestream);
}

