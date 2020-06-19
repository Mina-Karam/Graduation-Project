/*
 * RTC.h
 *
 *  Created on: Dec 8, 2019
 *      Author: MinaK
 */

#ifndef RTC_H_
#define RTC_H_

bool debounce (void);
void day_display(void);// function for display day of the week
void RTC_display(void);
void blink_parameter(void);
byte edit(byte x_pos, byte y_pos, byte parameter);
void draw_text(byte x_pos, byte y_pos, char *text); // print text on the LCD

#endif /* RTC_H_ */
