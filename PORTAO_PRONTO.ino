
#include "IRremote.h"
#include <Servo.h>

// Controle remoto
const int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Servomotor
Servo myservo;  // objeto servo
// int val; // variavel recebe valor analogigo    

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); //começa a receber o valor hex do controle 
  myservo.attach(9); // servomotor no pino 9
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
   
    if (results.value == 0xFF906F){ // ESQUERDA
      Serial.println("ABRINDO PORTÃO"); 
      myservo.write(180);     // 
      delay(15);   
    }
    
    if (results.value == 0xFFE01F){ // DIREITA
      Serial.println("FECHANDO PORTÃO"); 
      myservo.write(0);      // 
      delay(15);   
    }
   
    irrecv.resume(); // Recebe o próximo valor
  }
}
