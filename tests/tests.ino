void setup()
{
  pinMode(13, OUTPUT); // Configura el Pin 13 como salida
}

void loop()
{
  digitalWrite(13, LOW); // Apaga el Pin 13
  delay(3000);           // Espera 3 segundos

  digitalWrite(13, HIGH); // Enciende el Pin 13
  delay(3000);            // Espera 3 segundos
}