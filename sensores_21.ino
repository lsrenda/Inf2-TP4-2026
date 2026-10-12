
#include <EEPROM.h>

// Estructura de datos para las lecturas en tiempo real
struct RegistroClimatico {
  float temperatura;
  int humedad;
  int luz;
};

// Estructura de datos para la persistencia en memoria
struct HistoricoClimatico {
  float maxTemp;
  float minTemp;
  int maxHum;
  int minHum;
  int maxLuz;
  int minLuz;
};

// Asignación de pines analógicos
const int pinTemp = A0;      
const int pinHumedad = A1;   
const int pinLuz = A2;       

// Dirección base de almacenamiento
const int dirEEPROM = 0;
HistoricoClimatico historicos;

void setup() {
  Serial.begin(9600); 
  
  // Inicialización y lectura de memoria persistente
  EEPROM.get(dirEEPROM, historicos);

  // Formateo inicial si la memoria se encuentra vacía o corrupta
  if (isnan(historicos.maxTemp) || historicos.minHum < 0 || historicos.minHum > 100) {
    historicos.maxTemp = -50.0;  historicos.minTemp = 150.0;
    historicos.maxHum = -1;      historicos.minHum = 101;
    historicos.maxLuz = -1;      historicos.minLuz = 101;
  }

  // Interfaz de arranque
  Serial.println("------------------------------------------"); 
  Serial.println("SISTEMA CLIMATICO INICIADO");
  Serial.println("Valores historicos recuperados:");
  Serial.print("Temp  -> Max: "); Serial.print(historicos.maxTemp); Serial.print(" C | Min: "); Serial.println(historicos.minTemp);
  Serial.print("Humedad-> Max: "); Serial.print(historicos.maxHum); Serial.print(" % | Min: "); Serial.println(historicos.minHum);
  Serial.print("Luz   -> Max: "); Serial.print(historicos.maxLuz); Serial.print(" % | Min: "); Serial.println(historicos.minLuz);
  Serial.println("------------------------------------------");
  
  delay(3000); 
}

void loop() {
  RegistroClimatico medicion; 
  bool hayCambios = false;
  
  // Adquisición y conversión de temperatura (TMP36)
  int lecturaTemp = analogRead(pinTemp); 
  float voltaje = lecturaTemp * (5.0 / 1023.0); 
  medicion.temperatura = (voltaje - 0.5) * 100.0; 
  
  // Adquisición y mapeo de humedad
  int lecturaHum = analogRead(pinHumedad); 
  medicion.humedad = map(lecturaHum, 0, 876, 0, 100); 
  if (medicion.humedad > 100) medicion.humedad = 100; 

  // Adquisición y mapeo de luminosidad
  int lecturaLuz = analogRead(pinLuz); 
  medicion.luz = map(lecturaLuz, 0, 1023, 0, 100); 

  // Evaluación de valores máximos y mínimos absolutos
  if (medicion.temperatura > historicos.maxTemp) { historicos.maxTemp = medicion.temperatura; hayCambios = true; }
  if (medicion.temperatura < historicos.minTemp) { historicos.minTemp = medicion.temperatura; hayCambios = true; }

  if (medicion.humedad > historicos.maxHum) { historicos.maxHum = medicion.humedad; hayCambios = true; }
  if (medicion.humedad < historicos.minHum) { historicos.minHum = medicion.humedad; hayCambios = true; }

  if (medicion.luz > historicos.maxLuz) { historicos.maxLuz = medicion.luz; hayCambios = true; }
  if (medicion.luz < historicos.minLuz) { historicos.minLuz = medicion.luz; hayCambios = true; }

  // Actualización condicional en memoria EEPROM
  if (hayCambios) {
    EEPROM.put(dirEEPROM, historicos);
  }

  // Transmisión de telemetría por puerto serie
  Serial.print("T: "); Serial.print(medicion.temperatura); Serial.print(" C (Max:"); Serial.print(historicos.maxTemp); Serial.print(" Min:"); Serial.print(historicos.minTemp); Serial.print(") | ");
  Serial.print("H: "); Serial.print(medicion.humedad); Serial.print(" % (Max:"); Serial.print(historicos.maxHum); Serial.print(" Min:"); Serial.print(historicos.minHum); Serial.print(") | ");
  Serial.print("L: "); Serial.print(medicion.luz); Serial.print(" % (Max:"); Serial.print(historicos.maxLuz); Serial.print(" Min:"); Serial.print(historicos.minLuz); Serial.println(")");

  delay(2000); 
}