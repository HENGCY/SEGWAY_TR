/* 
 * File:   lib_serial.h
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */



#ifndef LIB_SERIAL_H
#define LIB_SERIAL_H


#include "../includes.h"

#define MESSAGE_SERIAL_LENGTH 7
#define MESSAGE_TIMEOUT_NS 2000

typedef struct
{
  char label;
  float value;
} message_serial;


#ifdef	__cplusplus
extern "C" {
#endif
int init_serial();
message_serial read_from_serial();
int send_float_to_serial(float fl_value, char tag);
float bytes_to_float(unsigned char * bytes);
int bytes_to_message(unsigned char * mesg, message_serial * m);

int close_serial();
#ifdef	__cplusplus
}
#endif

#endif
