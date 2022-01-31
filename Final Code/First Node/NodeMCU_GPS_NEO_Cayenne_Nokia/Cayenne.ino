// WiFi network info.
//char ssid[] = "KaRo0oMa";
char ssid[] = "Mina Karam";
char wifiPassword[] = "123123123";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "d2d37920-cb53-11ea-b767-3f1a8f1211ba";
  

void Cayenne_Setup(void){
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void Cayenne_Loop(void){
}
