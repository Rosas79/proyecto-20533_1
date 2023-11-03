#include <SD.h>

#define sw2 17
#define sw1 31
#define BUZZER_PIN 19

bool guardarDatos = false;
File dataFile;

void setup() {
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT); // Configura el pin del buzzer como salida
  Serial.begin(115200);
  Serial2.begin(115200);

  if (!SD.begin(10)) {
    Serial.println("Error al inicializar la tarjeta SD");
  }
}

void loop() {
  if (Serial2.available()) {
    char tempera = Serial2.read();
    Serial.write(tempera);

    if (guardarDatos) {
      dataFile = SD.open("datos.txt", FILE_WRITE);

      if (dataFile) {
        dataFile.println(tempera);
        dataFile.close();
      } else {
        Serial.println("Error al abrir el archivo en la tarjeta SD.");
      }
    }
  }

  if (Serial.available()) {
    char tempera = Serial.read();

    if (tempera == 'p') {
      Serial.println("Medida de temperatura");
      Serial2.println('p');
      
      // Activa el buzzer para hacer un sonido cuando se realiza la medición con sw2
      tone(BUZZER_PIN, 2000, 500); // Frecuencia de 2000Hz durante 500ms
    }
  }

  int botonState2 = digitalRead(sw2);
  int botonState1 = digitalRead(sw1);

  if (botonState2 == LOW) {
    char tempera = Serial.read();
    Serial2.println('p');
    Serial.println("Medida de temperatura");
    
    // Activa el buzzer para hacer un sonido cuando se realiza la medición con sw2
    tone(BUZZER_PIN, 2000, 500); // Frecuencia de 2000Hz durante 500ms
    
    delay(500);
  }

  if (botonState1 == LOW) {
    if (!guardarDatos) {
      guardarDatos = true;
      Serial.println("Comenzando a guardar datos en la tarjeta SD.");
      
      // Activa el buzzer para hacer un sonido cuando se presiona sw1
      tone(BUZZER_PIN, 1000, 500); // Frecuencia de 1000Hz durante 500ms
    } else {
      guardarDatos = false;
      Serial.println("Deteniendo el guardado de datos en la tarjeta SD.");
    }
    delay(500);
  }
}
