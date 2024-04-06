#include <DHT.h>
#include <ArduinoJson.h>
 
#define DHTPIN 7    
#define DHTTYPE DHT22  
#define LDRPIN A0    
#define LED_VERMELHO 10
#define LED_AMARELO 9
#define LED_VERDE 8    
 
DHT dht(DHTPIN, DHTTYPE); // Inicializa o sensor DHT
 
void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  dht.begin();        // Inicializa o sensor DHT
 
  pinMode(LED_VERMELHO, OUTPUT); // Define os pinos dos LEDs como saída
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
}
 
void loop() {
  StaticJsonDocument<200> jsonDocument; // Cria um documento JSON estático
  JsonObject jsonObject = jsonDocument.to<JsonObject>(); // Cria um objeto JSON
  float umidade = dht.readHumidity(); // Lê a umidade do sensor DHT
  float temperatura = dht.readTemperature(); // Lê a temperatura do sensor DHT (em Celsius)
  int luminosidade = analogRead(LDRPIN); // Lê a luminosidade do sensor LDR
  float luminosidade_lux = map(luminosidade, 0, 1023, 0, 1000); // Converte o valor da luminosidade para lux
 
  if (isnan(umidade) || isnan(temperatura)) { // Verifica se houve erro na leitura do sensor DHT
    Serial.println("Erro ao ler dados do sensor DHT!");
    return;
  }
 
  // Adiciona os dados dos sensores ao objeto JSON
  jsonObject["umidade"] = umidade;
  jsonObject["temperatura"] = temperatura;
  jsonObject["luminosidade"] = luminosidade_lux;
 
  // Serializa o objeto JSON para uma string
  String jsonString;
  serializeJson(jsonObject, jsonString);
 
  // Envia a string JSON via porta serial
  Serial.println(jsonString);
 
  // Verifica as condições de temperatura e umidade e acende os LEDs correspondentes
  if (umidade < 40 || temperatura > 30) {
    digitalWrite(LED_VERMELHO, HIGH); // Acende o LED vermelho
    digitalWrite(LED_AMARELO, LOW);   // Desliga o LED amarelo
    digitalWrite(LED_VERDE, LOW);     // Desliga o LED verde
  } else if (umidade < 60 || temperatura > 25) {
    digitalWrite(LED_VERMELHO, LOW);  // Desliga o LED vermelho
    digitalWrite(LED_AMARELO, HIGH);  // Acende o LED amarelo
    digitalWrite(LED_VERDE, LOW);     // Desliga o LED verde
  } else {
    digitalWrite(LED_VERMELHO, LOW);  // Desliga o LED vermelho
    digitalWrite(LED_AMARELO, LOW);   // Desliga o LED amarelo
    digitalWrite(LED_VERDE, HIGH);    // Acende o LED verde
  }
 
  delay(2000); // Aguarda 2 segundos antes de fazer a próxima leitura
}