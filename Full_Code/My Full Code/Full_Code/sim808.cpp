/*
 * sim808.cpp
 *  Created on: NOV 5, 2019
 *      Author: Mina Karam
 */

#include "sim808.h"

Stream *serialSIM808 = NULL; // That's a class which contains >> HardwareSerial and SoftwareSerial to convert between them easily

void  sim808_init(void * uart_device, char num)
{
    if(num)
		serialSIM808 = (HardwareSerial*)uart_device;
	else
		serialSIM808 = (SoftwareSerial*)uart_device;
}


int sim808_check_readable()
{
    return serialSIM808->available(); // like Serial.available() in arduino code // we get that from Stream class above it's an public function in that class
}

int sim808_wait_readable (int wait_time)
{
    unsigned long timerStart;
    int dataLen = 0;
    timerStart = millis(); // millis() is a function that returns the amount of milliseconds that have passed since program start
    while((unsigned long) (millis() - timerStart) > wait_time * 1000UL)
    {
        delay(500);
        dataLen = sim808_check_readable();
        if(dataLen > 0){
            break;
        }
    }
    return dataLen;
}

void sim808_flush_serial() // to read the serial
{
    while(sim808_check_readable())
    {
        serialSIM808->read();
    }
}

void sim808_read_buffer(char *buffer, int count, unsigned int timeout, unsigned int chartimeout) // this is a memory which the data saved // check Serial.readBytes()
{
    int i = 0;
    unsigned long timerStart, prevChar;
    timerStart = millis();
    prevChar = 0;
    while(1) // Get into this loop till break;
    {
        while (sim808_check_readable())
        {
            char c = serialSIM808->read();
            prevChar = millis();
            buffer[i++] = c;
            if(i >= count)break;
        }
        if(i >= count)break;
        if ((unsigned long) (millis() - timerStart) > timeout * 1000UL) {
            break;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function. Not DO it if we dont recieve at least one char (prevChar <> 0)
        if (((unsigned long) (millis() - prevChar) > chartimeout) && (prevChar != 0)) {
            break;
        }
    }
}

void sim808_clean_buffer(char *buffer, int count) // clean the buffer memory
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void sim808_send_byte(uint8_t data)
{
	serialSIM808->write(data);
}

void sim808_send_char(const char c)
{
	serialSIM808->write(c);
}

void sim808_send_cmd(const char* cmd)
{
  for(uint16_t i=0; i<strlen(cmd); i++)
    {
        sim808_send_byte(cmd[i]);
    }
}

void sim808_send_cmd(const __FlashStringHelper* cmd)
{
  int i = 0;
  const char *ptr = (const char *) cmd;
  while (pgm_read_byte(ptr + i) != 0x00) {
    sim808_send_byte(pgm_read_byte(ptr + i++));
  }
}

void sim808_send_cmd_P(const char* cmd)
{
  while (pgm_read_byte(cmd) != 0x00)
    sim808_send_byte(pgm_read_byte(cmd++));
}

boolean sim808_send_AT(void)
{
    return sim808_check_with_cmd(F("AT\r\n"),"OK",CMD);
}

void sim808_send_End_Mark(void)
{
    sim808_send_byte((char)26);
}

boolean sim808_wait_for_resp(const char* resp, DataType type, unsigned int timeout, unsigned int chartimeout)
{
    int len = strlen(resp);
    int sum = 0;
    unsigned long timerStart, prevChar;    //prevChar is the time when the previous Char has been read.
    timerStart = millis();
    prevChar = 0;
    while(1) {
        if(sim808_check_readable()) {
            char c = serialSIM808->read();
            prevChar = millis();
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
        if ((unsigned long) (millis() - timerStart) > timeout * 1000UL) {
            return false;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function.
        if (((unsigned long) (millis() - prevChar) > chartimeout) && (prevChar != 0)) {
            return false;
        }

    }
    //If is a CMD, we will finish to read buffer.
    if(type == CMD) sim808_flush_serial();
    return true;
}


boolean sim808_check_with_cmd(const char* cmd, const char *resp, DataType type, unsigned int timeout, unsigned int chartimeout)
{
    sim808_send_cmd(cmd);
    return sim808_wait_for_resp(resp,type,timeout,chartimeout);
}

//HACERR que tambien la respuesta pueda ser FLASH STRING
boolean sim808_check_with_cmd(const __FlashStringHelper* cmd, const char *resp, DataType type, unsigned int timeout, unsigned int chartimeout)
{
    sim808_send_cmd(cmd);
    return sim808_wait_for_resp(resp,type,timeout,chartimeout);
}
