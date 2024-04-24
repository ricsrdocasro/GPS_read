#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <axp20x.h>

#define USE_ALTITUDE


TinyGPSPlus gps; // Cria um objeto da classe TinyGPSPlus
HardwareSerial GPS(1); // RX: 34, TX: 12

AXP20X_Class axp; // Cria um objeto da classe AXP20X

void configureAXP()
{
  Wire.begin(21, 22); // configurado a comunicação com o AXP
  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) // configurado o endereço do AXP
  {
      Serial.println("AXP192 Begin PASS"); // se o axp for encontrado, ele imprime na tela
  }
  else
  {
      Serial.println("AXP192 Begin FAIL");
  }

  // Liga o LDO (GPS) e DCDC (LoRa)

  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON); 
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
}

void loopGPS()
{
  while (GPS.available())
    gps.encode(GPS.read()); // Enquanto houver dados disponíveis, ele irá ler e decodificar
}

void setup()
{
  Serial.begin(9600);

  configureAXP(); // Configura o AXP
  Serial.println("Axp OK");
  delay(300);

  GPS.begin(9600, SERIAL_8N1, 34, 12);  //34-TX 12-RX
  Serial.println("GPS OK");
  delay(300);
}

void loop()
{
  loopGPS();

  if (gps.location.isValid())
  {
    Serial.print("LAT=");
    Serial.println(gps.location.lat(), 6);
    Serial.print("LONG=");
    Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=");
    Serial.println(gps.altitude.meters());
  }

  delay(1000);
}