#include <WiFiManager.h>
#include <RCSwitch.h>
#include <ESP8266WiFi.h>          
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          
#include <EEPROM.h>
#include <ESP8266WebServer.h>

RCSwitch mySwitch = RCSwitch();
unsigned long  value;

//*************************************************************
// CAMBIAR ESTOS DATOS POR LOS LEIDOS DE TU MANDO A DISTANCIA //
//*************************************************************
#define boton1 5592325
#define boton2 5592328
#define boton3 5592331
#define boton4 5592334
//*************************************************************
//*************************************************************
//*************************************************************

IPAddress server(192, 168, 0, 56); // IP de la raspberry Pi
const char* host = "MandoRF"; // nombre del entorno
int contador=0;
unsigned long tiempo;
unsigned long tiempo_pulsaciones;
int pulsador = 0;
boolean estado_pulsador = LOW;
boolean estado_button1=0;
boolean estado_button2=0;
boolean estado_button3=0;
boolean estado_button4=0;
boolean leyendo_codigos=LOW;
#define BUFFER_SIZE 100

WiFiClient wclient;
PubSubClient client(wclient, server);


void callback(const MQTT::Publish& pub) {
  // Serial.println (pub.payload_string());
    if(pub.payload_string() == "boton1on")
    {
      estado_button1=1;
      Serial.print("El estado del boton 1 es : ");
      Serial.println(estado_button1);
      EEPROM.write(0,estado_button1);
    }
    if(pub.payload_string() == "boton1off")
    {
      estado_button1=0;
      Serial.print("El estado del boton 1 es : ");
      Serial.println(estado_button1);      
      EEPROM.write(0,estado_button1);
    }  
    if(pub.payload_string() == "boton2on")
    {
      estado_button2=1;
      Serial.print("El estado del boton 2 es : ");
      Serial.println(estado_button2);     
      EEPROM.write(1,estado_button2);
    }
    if(pub.payload_string() == "boton2off")
    {
      estado_button2=0;
      Serial.print("El estado del boton 2 es : ");
      Serial.println(estado_button2);       
      EEPROM.write(1,estado_button2);
    }
    if(pub.payload_string() == "boton3on")
    {
      estado_button3=1;
      Serial.print("El estado del boton 3 es : ");
      Serial.println(estado_button3);      
      EEPROM.write(2,estado_button3);
    }
    if(pub.payload_string() == "boton3off")
    {
      estado_button3=0;
      Serial.print("El estado del boton 3 es : ");
      Serial.println(estado_button3);      
      EEPROM.write(2,estado_button3);
    }
    if(pub.payload_string() == "boton4on")
    {
      estado_button4=1;
      Serial.print("El estado del boton 4 es : ");
      Serial.println(estado_button4);      
      EEPROM.write(3,estado_button4);
    }
    if(pub.payload_string() == "boton4off")
    {
      estado_button4=0;
      Serial.print("El estado del boton 4 es : ");
      Serial.println(estado_button4);      
      EEPROM.write(3,estado_button4);
    }
  EEPROM.commit();  
}

void setup() {
  pinMode(pulsador,INPUT);
  Serial.begin(115200);
  mySwitch.enableReceive(2);  
  EEPROM.begin(512);
  delay(10);
  client.set_callback(callback);


if (WiFi.status() != WL_CONNECTED) {
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
  Serial.print("NO WIFI");
  }
  else
  {
    Serial.print("Conectado a ");
    Serial.println(WiFi.SSID());
  }
  delay(3000);
}


  estado_button1=EEPROM.read(0);
  estado_button2=EEPROM.read(1);
  estado_button3=EEPROM.read(2);
  estado_button4=EEPROM.read(3); 
    
  delay(200);

}



void loop() {

comprobar_pulsador();
delay(50);

if (mySwitch.available()) {
    value = int (mySwitch.getReceivedValue());
    Serial.println(value);
    mySwitch.resetAvailable();
  }

  if ( value == boton1)
  {
      Serial.println("Has pulsado el boton 1");
      if (estado_button1==LOW)
      {
       estado_button1=HIGH;
       }
       else
       {
         estado_button1=LOW;
       }
       client.publish("Button1",String(estado_button1)); 
       Serial.print("Button1 = ");
       Serial.println(estado_button1);
       EEPROM.write(0,estado_button1);
       EEPROM.commit(); 
    delay(1000);
    value=0;
    mySwitch.resetAvailable();
  }
  else if (value == boton2)
  {
      Serial.println("Has pulsado el boton 2");
      if (estado_button2==LOW)
      {
       estado_button2=HIGH;
       }
       else
       {
         estado_button2=LOW;
       }
       client.publish("Button2",String(estado_button2));
       Serial.print("Button2 = ");
       Serial.println(estado_button2);        
        EEPROM.write(1,estado_button2);
        EEPROM.commit();
    delay(1000);
    value=0;
    mySwitch.resetAvailable();
  } 
   else if ( value == boton3)
  {
      Serial.println("Has pulsado el boton 3");
      if (estado_button3==LOW)
      {
       estado_button3=HIGH;
       }
       else
       {
         estado_button3=LOW;
       }
       client.publish("Button3",String(estado_button3));
       Serial.print("Button3 = ");
       Serial.println(estado_button3);       
       EEPROM.write(2,estado_button3);
       EEPROM.commit();  
    delay(1000);
    value=0;
    mySwitch.resetAvailable();
  }
    else if (value == boton4)
  {
      Serial.println("Has pulsado el boton 4");
      if (estado_button4==LOW)
      {
       estado_button4=HIGH;
       }
       else
       {
         estado_button4=LOW;
       }
       client.publish("Button4",String(estado_button4));
       Serial.print("Button4 = ");
       Serial.println(estado_button4);       
       EEPROM.write(3,estado_button4);
       EEPROM.commit(); 
       delay(1000);
       value=0;
       mySwitch.resetAvailable();
  } 

if (WiFi.status() == WL_CONNECTED)
{

{
  if (!client.connected()) 
  {
    if (client.connect("ESP8266: MandoRF"))  
    {   
      client.subscribe(host+(String)"/#");
    }
  }
    client.loop();
  }
  delay(1);
} 
delay(50); 


 
}


void comprobar_pulsador()
{
estado_pulsador=digitalRead(pulsador);
if (estado_pulsador==LOW)
  {
   contador++; 
   delay(200);
   tiempo=millis();
   tiempo_pulsaciones = tiempo+1000;
   while (tiempo_pulsaciones>=tiempo)
   {
    tiempo=millis();
    estado_pulsador=digitalRead(pulsador);
    if (estado_pulsador==LOW)
    {
      contador++;
      tiempo_pulsaciones = tiempo+1000;
      delay(200);
    }
   }
   Serial.print("El total de pulsaciones ha sido ");
   Serial.println(contador);
  }
  
if (contador==3)
{ 
 contador=0;
 leyendo_codigos=HIGH;
 leer_codigos_mando();   
}
else if (contador==4)
{ 
 contador=0;
 conexion_wifi(); 
}
else
{
  contador=0;
}
}

void leer_codigos_mando()
{
  Serial.println("INICIANDO LECTURA DE CODIGOS");
  while(leyendo_codigos==HIGH)
  {
    if (mySwitch.available()) {
    value = int (mySwitch.getReceivedValue());
    Serial.println(value);
    mySwitch.resetAvailable();
    delay(10);
    }
    delay(50);
    value=0;
    estado_pulsador=digitalRead(pulsador);
    if (estado_pulsador==LOW)
    {
      leyendo_codigos=LOW;
      delay(500);
      Serial.println("SALIENDO DE LA LECTURA DE CODIGOS");
    }
  }
}

void conexion_wifi()
{
    WiFiManager wifiManager;  
    if (!wifiManager.startConfigPortal("MandoRFSetup")) { // SSID 
    delay(3000);
    ESP.reset();
    delay(5000);
    }
  Serial.print("connected to ");
  Serial.println(WiFi.SSID());
  delay(2000);  
}




