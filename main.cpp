#include <Arduino.h>
#include <ArduinoWebsockets.h>

const char* ssid = "Raoni's Galaxy A53 5G"; // Nome da rede
const char* password = "tomamoconta"; // Senha da rede
const char* websockets_server_host = "192.168.95.178"; // IP do servidor websocket
const int websockets_server_port = 8080; // Porta de conexão do servidor

// Utilizamos o namespace de websocket para podermos utilizar a classe WebsocketsClient
using namespace websockets;

// Objeto websocket client
WebsocketsClient client;

// Led
const int led = 2;

void setup() 
{
    // Iniciamos a serial com velocidade de 115200
    Serial.begin(921600);

    // Definimos o pino como saída
    pinMode(led, OUTPUT);
    pinMode(24,INPUT);

    // Conectamos o wifi
    WiFi.begin(ssid, password);

    // Enquanto não conectar printamos um "."
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    // Exibimos "WiFi Conectado"
    Serial.println("Connected to Wifi, Connecting to server.");

    // Tentamos conectar com o websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");

    // Se foi possível conectar
    if(connected) 
    {
        // Exibimos mensagem de sucesso
        Serial.println("Connected!");
        // Enviamos uma msg "Hello Server" para o servidor
        client.send(String(touchRead(4)));
        
    }   // Se não foi possível conectar
    else 
    {
        // Exibimos mensagem de falha
        Serial.println("Not Connected!");
        return;
    }
    
    // Iniciamos o callback onde as mesagens serão recebidas
    client.onMessage([&](WebsocketsMessage message)
    {        
        // Exibimos a mensagem recebida na serial
        Serial.print("Got Message: ");
        Serial.println(message.data());
        client.send(String(touchRead(4)));
        // Ligamos/Desligamos o led de acordo com o comando
        if(message.data().equalsIgnoreCase("ON"))
            digitalWrite(led, HIGH);
        else
        if(message.data().equalsIgnoreCase("OFF"))
            digitalWrite(led, LOW);
    });
}

void loop() 
{
    //  De tempo em tempo, o websockets client checa por novas mensagens recebidas
    if(client.available()) 
        client.poll();
        
    delay(300);
}