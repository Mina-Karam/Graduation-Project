#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

#define TINY_GSM_MODEM_SIM808
#include <CayenneMQTTGSM.h>

// GSM connection info.
char apn[] = ""; // Access point name. Leave empty if it is not needed.
char gprsLogin[] = ""; // GPRS username. Leave empty if it is not needed.
char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
char pin[] = ""; // SIM pin number. Leave empty if it is not needed.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "12ed7f80-decc-11e9-a4a3-7d841ff78abf";
char password[] = "a6b673aa48bf44aa6223f32c4bc57a3eb591d795";
char clientID[] = "7a2d2c30-abe6-11ea-b767-3f1a8f1211ba";

void Cayenne_Setup(void){
   Cayenne.begin(username, password, clientID, Serial3, apn, gprsLogin, gprsPassword, pin);
   
}
