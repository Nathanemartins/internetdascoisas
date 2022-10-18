#include "DHTesp.h"
#include "WiFi.h"
#include "PubSubClient.h"

const char* ssid = "Wokwi-GUEST";
const char* senha = "";

//configuração do mqtt broker
int port = 1883;
const char* server = "broker.hivemq.com";
const char* topico = "temp_umidade";

DHTesp dhtSensor;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Setup");

  WiFi.begin(ssid, senha);
  dhtSensor.setup(15, DHTesp::DHT22);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a rede Wifi");

  // conexão com broker
  mqttClient.setServer(server,port);
  char* clientId = "nathisenai";

  Serial.println("Conectando ao Broker");
  mqttClient.connect(clientId);
  mqttClient.subscribe("temp_umidade");

  while(!mqttClient.connected()){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Broker conectado");


}

void loop() {

//capturando temperatura e humidade
float temperatura = dhtSensor.getTemperature();
float umidade = dhtSensor.getHumidity();

//imprimindo dados capturados
Serial.println("Temperatura :" + String (temperatura) + "ºC");
Serial.println("Umidade :" + String (umidade) + "%");
Serial.println("----------------");

int msg = mqttClient.publish(topico, "{\"temperatura\": 35.0,\"umidade\" : 55}");
if(!msg){
  Serial.println("Problemas ao enviar mensagem");
  mqttClient.connect("nathisenai");
  mqttClient.subscribe(topico, 0);
}else{
  Serial.println("Mensagem enviada com sucesso");
}

delay(3000);

digitalWrite(14, HIGH);
digitalWrite(27, HIGH);
delay(1000);

digitalWrite(14, LOW);
digitalWrite(27, LOW);
delay(1000);



}
 

 // {
 // "temperatura": xx,
 // "umidade": 45
 // }