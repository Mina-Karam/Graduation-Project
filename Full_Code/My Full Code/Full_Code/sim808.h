/*
 * sim808.h
 *  Created on: NOV 5, 2019
 *      Author: Mina Karam
 */

#ifndef __SIM808_H__
#define __SIM808_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

#define DEFAULT_TIMEOUT     		 5   //Seconds
#define DEFAULT_INTERCHAR_TIMEOUT 1500   //Milliseconds

enum DataType {
    CMD     = 0,
    DATA    = 1,
};

void  sim808_init(void * uart_device, char num );
//void  sim808_init(void * uart_device, uint32_t baud);
int   sim808_check_readable();
int   sim808_wait_readable(int wait_time);
void  sim808_flush_serial();
void  sim808_read_buffer(char* buffer,int count,  unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
void  sim808_clean_buffer(char* buffer, int count);
void  sim808_send_byte(uint8_t data);
void  sim808_send_char(const char c);
void  sim808_send_cmd(const char* cmd);
void  sim808_send_cmd(const __FlashStringHelper* cmd);
void  sim808_send_cmd_P(const char* cmd);
boolean  sim808_send_AT(void);
void  	  sim808_send_End_Mark(void);
boolean  sim808_wait_for_resp(const char* resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
boolean  sim808_check_with_cmd(const char* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT*5);
boolean  sim808_check_with_cmd(const __FlashStringHelper* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);

#endif
