var WebSocketServer = require("websocket").server;
var http = require("http");

//Porta que o server irá escutar
const port = 8080;

//Cria o server
var server = http.createServer();

//Server irá escutar na porta definida em 'port'
server.listen(port, () => {
  //Server está pronto
  console.log(`Server está executando na porta ${port}`);
});

//Cria o WebSocket server
wsServer = new WebSocketServer({
  httpServer: server,
});

//Chamado quando um client deseja conectar
wsServer.on("request", (request) => {
  //Estado do led: false para desligado e true para ligado
  let state = false;

  //Aceita a conexão do client
  let client = request.accept(null, request.origin);

  //Chamado quando o client envia uma mensagem
  client.on("message", (message) => {
    //Se é uma mensagem string utf8
    if (message.type === "utf8") {
      //Mostra no console a mensagem
      let touchRead = parseInt(message.utf8Data);
      if (isNaN(touchRead)) {
        console.log(message.utf8Data);
      } else {
        if (touchRead < 40) {
          console.log("Touch foi tocado");
        } else {
          console.log("Touch não foi tocado");
        }
      }
    }
  });

  //Cria uma função que será executada a cada 1 segundo (1000 millis) para enviar o estado do led
  let interval = setInterval(() => {
    //Envia para o client "ON" ou "OFF" dependendo do estado atual da variável state
    client.sendUTF(state ? "ON" : "OFF");
    //Inverte o estado
    state = !state;
  }, 2500); //Tempo entre chamadas => 1000 millis = 1 segundo

  //Chamado quando a conexão com o client é fechada
  client.on("close", () => {
    console.log("Conexão fechada");
    //Remove o intervalo de envio de estado
    clearInterval(interval);
  });
});
