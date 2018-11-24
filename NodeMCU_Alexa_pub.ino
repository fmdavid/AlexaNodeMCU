#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

#define VELOCIDAD_PUERTO_SERIE 115200

/* Network credentials */
#define WIFI_SSID "TU_SSID"
#define WIFI_PASS "TU_PASS"    

fauxmoESP fauxmo;

bool estadoDispositivos[7]= {false, false, false, false, false, false, false};

#define PIN_1 D5
#define PIN_2 D6
#define PIN_3 D7
#define PIN_4 D8

#define DISPOSITIVO_1 "Ventilador"
#define DISPOSITIVO_2 "Luz roja"
#define DISPOSITIVO_3 "Luz verde"
#define DISPOSITIVO_4 "Luz azul"
#define GRUPO_1 "Luces"
#define TODO "TODO"

void setup() {
  // Iniciamos puerto serie
  Serial.begin(VELOCIDAD_PUERTO_SERIE);

  // Iniciamos wifi
  wifiSetup();

  // Configuramos como pines de salida
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_3, OUTPUT);
  pinMode(PIN_4, OUTPUT);

  // Habilitamos la librería para el descubrimiento y cambio de estado
  // de los dispositivos
  fauxmo.enable(true);  

  // Damos de alta los diferentes dispositivos y grupos
  fauxmo.addDevice(DISPOSITIVO_1);   //ID0
  fauxmo.addDevice(DISPOSITIVO_2);   //ID1
  fauxmo.addDevice(DISPOSITIVO_3);   //ID2
  fauxmo.addDevice(DISPOSITIVO_4);   //ID3
  fauxmo.addDevice(GRUPO_1);         //ID4
  fauxmo.addDevice(TODO);            //ID6
  
  // Decimos cuales van a ser nuestras funciones para obtener estado
  // y para establecerlo...
  fauxmo.onSetState(establecerEstado);
  fauxmo.onGetState(obtenerEstado);
}


void loop() {
  // Nos quedamos esperando peticiones todo el tiempo...
  fauxmo.handle();
}

void establecerEstado(unsigned char idDispositivo, const char * nombreDispositivo, bool estado){
    Serial.printf("Dispositivo #%d (%s) estado: %s\n", idDispositivo, nombreDispositivo, estado ? "encendido" : "apagado");
    
    // Establecemos el estado del dispositivo concreto
    estadoDispositivos[idDispositivo] = estado;
    
    // En función del dispositivo recibido...
    switch (idDispositivo) {
      case 0:
        {
          digitalWrite(PIN_1, estado);
        }
        break;
      case 1:
        {
          digitalWrite(PIN_2, estado);
        }
        break;
      case 2:
        {
          digitalWrite(PIN_3, estado);
        }
        break;
      case 3:
        {
          digitalWrite(PIN_4, estado);
        }
        break;
      case 4:
        {
          digitalWrite(PIN_2, estado);
          digitalWrite(PIN_3, estado);
          digitalWrite(PIN_4, estado);
        }
        break;
      case 5:
        {
          digitalWrite(PIN_1, estado);
          digitalWrite(PIN_2, estado);
          digitalWrite(PIN_3, estado);
          digitalWrite(PIN_4, estado);
        }
        break;
    }
  delay(50);
 }

bool obtenerEstado(unsigned char idDispositivo, const char * nombreDispositivo){
  return estadoDispositivos[idDispositivo];
}

void wifiSetup() {
  // Configuramos modo de wifi (estación)
   WiFi.mode(WIFI_STA);

   // Realizamos la conexión
   Serial.println ();
   Serial.printf("[WIFI] Conectando con %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Esperamos a que la conexión esté lista
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }

   // Conexión realizada
   Serial.println(" [WIFI] Conexión realizada con éxito" );
   Serial.println();
   Serial.printf("[WIFI] Modo estación, SSID: %s, Dirección IP asignada: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
