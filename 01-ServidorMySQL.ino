
 
// Inclui a biblioteca DHT para interagir com o sensor DHT11
#include "DHT.h"

// Inclui a biblioteca WiFi para permitir a comunicação Wi-Fi
#include <WiFi.h>

// Inclui a biblioteca time para permitir a captura de data e hora
#include <HTTPClient.h>

#define DHTPIN 4  //Define o pino de conexão do sensor DHT
#define DHTTYPE DHT11 // Define o tipo de sensor como DHT11

//Constantes que terão o dados da Rede WiFi
const char* ssid = "INTELBRAS";
const char* password = "Lika8081!@";

// URL do servidor PHP
const char* serverName = "http://10.0.0.187/Estacionamento/dadosSensores/inserir";

// Cria um objeto DHT com o pino e tipo especificados
DHT dht(DHTPIN, DHTTYPE); 

// Variáveis para armazenar Umidade e Temperatura ambiente
float umidade, temperatura;
String luminosidade;

void setup(void){
  // Inicia a comunicação serial com uma taxa de transmissão de 115200 bps
  Serial.begin(115200); 

  // Inicia a conexão Wi-Fi com as credenciais fornecidas
  WiFi.begin(ssid, password); 

  // Inicializa o sensor DHT
  dht.begin(); 

  // Aguarda conexão Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP de acesso de qualquer browser: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  temperatura = dht.readTemperature(); // Lê o valor da temperatura 
  umidade = dht.readHumidity();        // Lê o valor da umidade do ar 

  // Verifica se a leitura falhou
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler o DHT11");
    delay(2000);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C | Umidade: ");
  Serial.print(umidade);
  Serial.print(" % | Luminosidade: ");
  Serial.println(" Baixa");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Cria os dados da requisição
    String httpRequestData = "temperatura=" + String(temperatura) + "&umidade=" + String(umidade)
                             + "&luminosidade=Baixa";

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String resposta = http.getString();
      Serial.println("Resposta do servidor: " + resposta);
    } else {
      Serial.print("Erro no envio POST. Código: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  
  // Espera 10 segundos antes da próxima leitura
  delay(10000); 
}
