
  bool debounce ()
{
  byte count = 0;
  for(byte i = 0; i < 5; i++)
  {
    if ( !digitalRead(button1) )
      count++;
    delay(10);
  }
 
  if(count > 2)  return 1;
  else           return 0;
}
 
// function for display day of the week
void day_display()
{
  switch(w_day)
  {
    case 1:  draw_text(12, 16, "  SUNDAY  "); break;
    case 2:  draw_text(12, 16, "  MONDAY  "); break;
    case 3:  draw_text(15, 16, " TUESDAY ");  break;
    case 4:  draw_text(15, 16, "WEDNESDAY");  break;
    case 5:  draw_text(12, 16, " THURSDAY "); break;
    case 6:  draw_text(12, 16, "  FRIDAY  "); break;
    default : draw_text(12, 16, " SATURDAY "); 
  }
}
 
void RTC_display()
{
  // convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour   >> 4) * 10 + (hour   & 0x0F);
  day    = (day    >> 4) * 10 + (day    & 0x0F);
  month  = (month  >> 4) * 10 + (month  & 0x0F);
  year   = (year   >> 4) * 10 + (year   & 0x0F);
  // end conversion
 
  // update time array
  Time[7] = second % 10 + '0';
  Time[6] = second / 10 + '0';
  Time[4] = minute % 10 + '0';
  Time[3] = minute / 10 + '0';
  Time[1] = hour   % 10 + '0';
  Time[0] = hour   / 10 + '0';
 
  // update date array
  Date[9] = year   % 10 + '0';
  Date[8] = year   / 10 + '0';
  Date[4] = month  % 10 + '0';
  Date[3] = month  / 10 + '0';
  Date[1] = day    % 10 + '0';
  Date[0] = day    / 10 + '0';
 
  draw_text(12,  24, Date);  // print date
  draw_text(18, 40, Time);   // print time
}
 
void blink_parameter()
{
  byte j = 0;
  while(j < 100 && digitalRead(button1) && digitalRead(button2))
  {
    j++;
    delay(5);
  }
}
 
byte edit(byte x_pos, byte y_pos, byte parameter)
{
  char text[3];
  sprintf(text,"%02u", parameter);
  while(debounce());  // call debounce function (wait for B1 to be released)
 
  while(1)
  {
    while(!digitalRead(button2))
    {
      parameter++;
      if(i == 0 && parameter > 31)   // if date > 31 ==> date = 1
        parameter = 1;
      if(i == 1 && parameter > 12)   // if month > 12 ==> month = 1
        parameter = 1;
      if(i == 2 && parameter > 99)   // if year > 99 ==> year = 0
        parameter = 0;
      if(i == 3 && parameter > 23)   // if hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 4 && parameter > 59)   // if minutes > 59 ==> minutes = 0
        parameter = 0;
      if(i == 5 && parameter >60) 
      parameter =0 ;
      sprintf(text,"%02u", parameter);
      draw_text(x_pos, y_pos, text);
      
      delay(200);   // wait 200ms
    }
 
    draw_text(x_pos, y_pos, "  ");
    blink_parameter();
    
    draw_text(x_pos, y_pos, text);
    blink_parameter();
 
    if(!digitalRead(button1)) // if button B1 is pressed
    if(debounce())            // call debounce function (make sure if B1 is pressed)
    {
      i++;                // increment 'i' for the next parameter
      return parameter;   // return parameter value and exit
    }
  }
}
 
// print text on the LCD
void draw_text(byte x_pos, byte y_pos, char *text)
{
  display.setCursor(x_pos, y_pos);
  display.print(text);
  display.display();
}
 
// main loop
void RTCLCD()
{
  if(!digitalRead(button1))     // if button B1 is pressed
    if(debounce())   // call debounce function (make sure if B1 is pressed)
    {
      i = 0;
      while(debounce());  // call debounce function (wait for button B1 to be released)
      while(1)
      {
        while( !digitalRead(button2) )    // while button B2 pressed
        {
          w_day++;         // increment w_day
          if(w_day > 7)
            w_day = 1;
          day_display();   // call day_display function
          delay(500);   // wait 500 ms
        }
        draw_text(15, 16, "         ");
        blink_parameter();   // call blink_parameter function
 
        day_display();      // call day_display function
        blink_parameter();  // call blink_parameter function
        if( !digitalRead(button1) )  // if button B1 is pressed
          break;
      }
 
    day    = edit(12, 24, day);    // edit date
    month  = edit(30, 24, month);  // edit month
    year   = edit(60, 24, year);   // edit year
    hour   = edit(18, 40, hour);   // edit hours
    minute = edit(36, 40, minute); // edit minutes
 
    // convert decimal to BCD
    minute = ((minute / 10) << 4) + (minute % 10);
    hour   = ((hour   / 10) << 4) + (hour   % 10);
    day    = ((day    / 10) << 4) + (day    % 10);
    month  = ((month  / 10) << 4) + (month  % 10);
    year   = ((year   / 10) << 4) + (year   % 10);
    // end conversion
 
    while(debounce());  // call debounce function (wait for button B1 to be released)
 
    // write data to DS1307 RTC
    Wire.beginTransmission(0x68);  // start I2C protocol with DS1307 address
    Wire.write(0);       // send register address
    Wire.write(0);       // reset sesonds and start oscillator
    Wire.write(minute);  // write minute
    Wire.write(hour);    // write hour
    Wire.write(w_day);   // write day
    Wire.write(day);     // write date
    Wire.write(month);   // write month
    Wire.write(year);    // write year
    Wire.endTransmission();  // stop transmission and release the I2C bus
 
    delay(200); // wait 200ms
  }
 
  Wire.beginTransmission(0x68); // start I2C protocol with DS1307 address
  Wire.write(0);                // send register address
  Wire.endTransmission(false);  // I2C restart
  Wire.requestFrom(0x68, 7);    // request 7 bytes from DS1307 and release I2C bus at end of reading
  second = Wire.read();         // read seconds from register 0
  minute = Wire.read();         // read minuts from register 1
  hour   = Wire.read();         // read hour from register 2
  w_day  = Wire.read();         // read day from register 3
  day    = Wire.read();         // read date from register 4
  month  = Wire.read();         // read month from register 5
  year   = Wire.read();         // read year from register 6
 
  day_display();    // print day of the week
  RTC_display();    // print time & date
 
  delay(100);   // wait 100 ms
}
// end of code
  

