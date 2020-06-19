void time1()
{

  display.setCursor(2, 24);
  display.print(ntc[4]);
  
  display.display();
  display.setCursor(16, 24);
  display.print(ntc[5]);
  
  display.display();
  display.setCursor(30, 24);
  display.print(ntc[6]);
 
  display.display();
  
 
  display.setCursor(2, 40);
  display.print(ntc[2]);
 
  display.display();
  display.setCursor(18, 40);
  display.print(ntc[1]);
     
  display.display();
  display.setCursor(33, 40);
  display.print(ntc[0]);
   
  display.display();
  
  timeClient.update();
  timestamp=timeClient.getEpochTime();
  getDateTime(timestamp);

  ntc[4]<10?Serial.print("0"+String(ntc[4])):Serial.print(ntc[4]);
  Serial.print("/");
  ntc[5]<10?Serial.print("0"+String(ntc[5])):Serial.print(ntc[5]);
  Serial.print("/");
  ntc[6]<10?Serial.print("0"+String(ntc[6])):Serial.print(ntc[6]);
  Serial.print(" ");
  
  switch(ntc[3])
  {

  case 0:{ display.setCursor(3, 6);
    display.print("SUN");display.display();
    break;
     
   } 
  case 1:{ display.setCursor(3, 6);
    display.print("MON"); display.display();break; 
   } 
  case 2:{ display.setCursor(3, 10);
    display.print("TUE");display.display();break;} 
  case 3:{ display.setCursor(3, 10);
    display.print("WED");display.display();break;} 
  case 4:{ display.setCursor(3, 10);
    display.print("THU");display.display();break;} 
  case 5:{ display.setCursor(3, 10);
    display.print("FRI");display.display();break;} 
  case 6:{ display.setCursor(3, 10);
    display.print("SAT");display.display();break;}  
  }
  Serial.print(" ");
  
  ntc[2]<10?Serial.print("0"+String(ntc[2])):Serial.print(ntc[2]);
  Serial.print(":");
  ntc[1]<10?Serial.print("0"+String(ntc[1])):Serial.print(ntc[1]);
  Serial.print(":");
  ntc[0]<10?Serial.print("0"+String(ntc[0])):Serial.print(ntc[0]);
  Serial.println();

  delay(1000);
}

void getDateTime(unsigned long int timestamp)
{
  int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  ntc[0]=timestamp%60;
  ntc[1]=(timestamp%3600)/60;
  ntc[2]=(timestamp%86400)/3600;
  ntc[3]=((timestamp/86400)+4)%7;
  ntc[6]=timestamp/86400/365; //years since 1970
  ntc[4]=timestamp/86400-365*ntc[6]-(ntc[6]+1)/4;  //leap year start at 1972
  if(ntc[4]<0)
  {
    ntc[6]--;
    ntc[4]=timestamp/86400-365*ntc[6]-(ntc[6]+1)/4;  //days for current year
  }
  ntc[6]=(1970+ntc[6])%100;

  ntc[5]=0;
  while(ntc[4]>=days[ntc[5]])
  {
    if(((ntc[6]%4)==0)&(ntc[5]==1)) ntc[4]--; //if leap year and February
    ntc[4]-=days[ntc[5]];
    ntc[5]++;
  }
  ntc[4]++;  //1st day of Month is 0 days
  ntc[5]++;  //get value 1-12  
  if((ntc[4]==0)&(ntc[5]==3))
  {
    ntc[4]=29;
    ntc[5]=2;
  }
}

void draw_text(byte x_pos, byte y_pos, char *text)
{
  display.setCursor(x_pos, y_pos);
  display.print(text);
  display.display();
}
