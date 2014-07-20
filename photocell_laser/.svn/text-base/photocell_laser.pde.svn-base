int photocellPin;
int photocellReading;
int milisegundos;
int diffMili;
boolean estado;
boolean primero;

void setup(void) {
  Serial.begin(9600);
  photocellPin = 0;
  estado = true;
  primero = true;
}

/*Funciones*/
void salidaInterrupcion(int valorPhotocell){
  //Serial.println("====INTERRUPCION=====");
  //Serial.print("Valor Photocell = ");
  //Serial.println(valorPhotocell);
  //Serial.println("=====================");
  Serial.print(1, DEC);
}

/*Main loop*/
void loop(void) {
  photocellReading = analogRead(photocellPin); 
  

  if(photocellReading < 880){
    if(estado){
      if(primero){
        milisegundos = millis();
        primero = false;
      }
      if((millis() - milisegundos) > 200){
        salidaInterrupcion(photocellReading);
        primero = true;
        estado = false;
      }
    }
  }
  else{
    estado = true;
    primero = true;
  }
 
  delay(100);
}


