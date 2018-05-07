#include <CurieIMU.h>
#include <CurieBLE.h>

#define calibrateOffsets true           // variável de controle da calibração
#define deltaT           1             //tempo minimo da leitura

BLEPeripheral     blePeripheral;
BLEService        AccelService("4A981234-1CC4-E7C1-C757-F1267DD021E8");
BLECharacteristic AccelChar("4A981234-1CC4-E7C1-C757-F1267DD021E8", BLEWrite, 3);  

float oldAx = 0;
int i = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  
  blePeripheral.setLocalName("Aceleração");
  blePeripheral.setAdvertisedServiceUuid(AccelService.uuid());
  blePeripheral.addAttribute(AccelService);
  blePeripheral.addAttribute(AccelChar);
  
  blePeripheral.begin();
  Serial.println("BLE Accel Curie");

  if (calibrateOffsets) 
  {
    Serial.println("Valores de compensação do sensor (antes da calibração)");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS));
    Serial.print("\t"); 
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS));
    Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS));
    Serial.print("\t");
   
    Serial.println("Iniciando calibração. Posicione a placa em uma superfície plana");
    delay(5000); 
    //Delay de 5 segundos para posicionar a placa
    // A placa deve estar posicionada corretamente para a calibração funcionar
    
    Serial.print("Iniciando calibração do giroscópio");
    CurieIMU.autoCalibrateGyroOffset();
    Serial.println(" Finalizado");
    
    Serial.print("Iniciando calibração do sensor de aceleração");
    CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
    Serial.println(" Finalizado");
    
    Serial.println("Valores de compensação após a calibração");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS));
    Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS));
    Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS));
    Serial.print("\t");
  }

}

void loop() {
  float ax, ay, az; //Variáveis da aceleração em cada eixo
  
  BLECentral central = blePeripheral.central();
  if (central)
  {
    Serial.print("Conectado a central: ");
    //Mostra o MAC address do dispositivo conectado
    Serial.println(central.address());

    while (central.connected())
    {
      CurieIMU.readAccelerometerScaled(ax, ay, az); // Lê os valores da aceleração
      updateAccel(ax);

      i++;
      delay(deltaT);
    }
  }
  else
  {
    Serial.println("Procurando conexão");
  }

}

void updateAccel(float a)
{
    AccelChar.setValue(a, 3); 
    oldAx = i;
}

