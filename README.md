# Hc Tracking

Este projeto utiliza um Arduino Uno para monitorar e controlar o ambiente onde a criança está jogando o HC Gaming no Instituto da Criança do Hospital das Clínicas. Ele utiliza um sensor de luminosidade (LDR) para medir a intensidade da luz e um sensor de temperatura e umidade (DHT22) para monitorar as condições ambientais. Com base nos dados coletados, o Arduino controla LEDs para indicar as condições de temperatura e umidade.


## Tecnologias e componentes utilizados

- Linguagem C#
- Arduino Uno
- Sensor de Luminosidade
- Sensor de Temperatura e Umidade
- LED's (Vermelho, Amarelo e Verde)
- Resistores
- Protoboard
- Fios Jumpers
- Fonte de alimentação para o Arduino


## Demonstração

Para simular o projeto é possível acessar o projeto através deste link [Wokwi](https://wokwi.com/projects/394281094943664129), além de poder montar o projeto da imagem abaixo com o  código a seguir:



![arduino](https://github.com/felpschneider/sprint-3-edge/assets/143764606/41db819a-5eab-4bb7-8bf2-3645a9acc97d)


- Código Arduino:
```bash
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


```
Basta copiar e colar o código a cima!



## Node Red

Foi criado um projeto para simulação no Node-Red, no qual foi utilizado um (Inject Json) para o recebimento de dados, uma (Function) para gerar valores aleatórios para temperatura, umidade e luminosidade, além de um (MQTT OUT) para mandar a simulação para o Tago.io.

Para a testar a simulação basta copiar o circuito e utilizar os códigos:

![node red](https://github.com/felpschneider/sprint-3-edge/assets/143764606/44529e6d-0b5a-405f-a553-bc65877e8de8)

- Inject Json:
```bash
    {
        "variable": "Temperatura",
        "value": 28,
        "unit": "ºC"
    },
    {
        "variable": "Umidade",
        "value": 18,
        "unit": "%"
    },
    {
        "variable": "Luminosidade",
        "value": 190,
        "unit": "lux"
    }


```
- Function:
```bash
var newTemperature = Math.random() * 40;
var newHumidity = Math.random() * 100;
var newLight = Math.random() * 500;
 
// Limita os valores de temperatura entre 21 e 22, com 25% de chance de ter um pico até 26
if (Math.random() < 0.25) {
    newTemperature = 26;
} else {
    newTemperature = Math.max(21, Math.min(22, newTemperature));
}
 
// Limita os valores de luminosidade entre 250 e 270, com 15% de chance de queda para até 170
if (Math.random() < 0.15) {
    newLight = Math.random() * 170;
} else {
    newLight = Math.max(250, Math.min(270, newLight));
}
 
// Limita os valores de umidade entre 50 e 55, com 20% de chance de um pico até 60
if (Math.random() < 0.2) {
    newHumidity = 60;
} else {
    newHumidity = Math.max(50, Math.min(55, newHumidity));
}
 
// Atribui os novos valores ao JSON
var json = msg.payload;
json[0].value = Math.round(newTemperature);
json[1].value = Math.round(newHumidity);
json[2].value = Math.round(newLight);
 
// Retorna o JSON modificado
return { payload: json };

```
## Tago.io

Abaixo está uma screenshot do funcionamento da simulação no [Tago.io](https://tago.io):

![tago](https://github.com/felpschneider/sprint-3-edge/assets/143764606/73d1d40d-7fc6-4e74-bcd7-4f42374f2f5e)




















## Autores

- Felipe Schneider - RM 552643
- Maria Julia - RM 553384
- Thiago Araujo - RM553477
- Vinícius Centurion - RM554063

