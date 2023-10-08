#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>


// Variáveis para armazenar os valores RGB e página que o enviou
int red = 0, green = 0, blue = 0, page = 0;


WebServer server(80);



//Funções
void controlLEDs(int red, int green, int blue, int page) {
  // Se o RGB foi enviado da página `/`, liga os LEDs vermelho, verde e azul
  if (page == 1) {
    Serial.println("");
    Serial.print("Pagina 0 -> ");Serial.print("R: ");Serial.print(red);Serial.print(" G: ");Serial.print(green);Serial.print(" B: ");Serial.println(blue);
  }

  // Se o RGB foi enviado da página `/pagina2`, liga os LEDs verde e azul
  else if (page == 2) {
    Serial.println("");
    Serial.print("Pagina 2 -> ");
    
    Serial.print("R: ");
    Serial.print(red);

    Serial.print(" G: ");
    Serial.print(green);

    Serial.print(" B: ");
    Serial.println(blue);
  }

  // Se o RGB foi enviado da página `/pagina3`, liga os LEDs vermelho e azul
  else if (page == 3) {
    Serial.println("");
    Serial.print("Pagina 3 -> ");
    
    Serial.print("R: ");
    Serial.print(red);

    Serial.print(" G: ");
    Serial.print(green);

    Serial.print(" B: ");
    Serial.println(blue);
  }

  else if (page == 4) {
    Serial.println("");
    Serial.print("Pagina 4 -> ");
    
    Serial.print("R: ");
    Serial.print(red);

    Serial.print(" G: ");
    Serial.print(green);

    Serial.print(" B: ");
    Serial.println(blue);
  }
}

void handleRoot() {
  String html = R"=====(<!DOCTYPE html>
  <html lang="pt-br">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Roboto:wght@100;400;500;700&display=swap">
      <title>Sal da Terra - LEDs</title>
      <style>
          * {
              padding: 0px;
              margin: 0px;
              box-sizing: border-box;
              font-family: 'Roboto', sans-serif;
          }
          body {
              text-align: center;
              margin: 0;
              padding: 0;
              color: #ffffff;
              background-color: #191B21;
          }

          a {
              text-decoration: none;
              color: yellow;font-size: 150%;
              background-color: #2f333d;
              padding: 2px 10px 2px 10px;
              margin: 0 20px 0 20px;
          }
      </style>
  </head>
  <body>
      <header></header>
      <main style="display: flex; flex-direction: column; width: 50vw; margin: auto;">
          <div style="margin: 100px 0 0 0;">
              <a href="/pagina4"><</a>
              <a href="/pagina2">></a>
          </div>
          <div style="margin: 50px 0 0 0; display: flex; justify-content: center; align-items: center;">
              <input style="border-style: hidden;" type="color" id="colorPicker">
              <button style="margin: 0 5px 0 5px;" id="sendButton">Enviar</button>
          </div>
          
      </main>
      <footer></footer>
      <script>
          document.getElementById("sendButton").addEventListener("click", function() {
              // Obtém o valor selecionado no campo de entrada de cor
              var colorPicker = document.getElementById("colorPicker");
              var selectedColor = colorPicker.value;
              
              // Extrai os componentes RGB do valor hexadecimal
              var red = parseInt(selectedColor.slice(1, 3), 16);
              var green = parseInt(selectedColor.slice(3, 5), 16);
              var blue = parseInt(selectedColor.slice(5, 7), 16);
              
              // Crie um objeto JSON com os valores
              var jsonData = {
                  r: red,
                  g: green,
                  b: blue,
                  page: 1
              };
              
              // Envia o JSON via POST para o servidor
              fetch("/receber-json", {
                  method: "POST",
                  body: JSON.stringify(jsonData),
                  headers: {
                      "Content-Type": "application/json"
                  }
              })
              .then(function(response) {
                  if (response.ok) {
                      console.log("JSON enviado com sucesso");
                  } else {
                      console.error("Erro ao enviar JSON");
                  }
              })
              .catch(function(error) {
                  console.error("Erro ao enviar JSON:", error);
              });
          });
      </script>
  </body>
  </html>)=====";
  server.send(200, "text/html", html);
}

void handlePagina2() {
  String html = R"=====(<!DOCTYPE html>
  <html lang="pt-br">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Roboto:wght@100;400;500;700&display=swap">
      <title>Sal da Terra - LEDs</title>
      <style>
          * {
              padding: 0px;
              margin: 0px;
              box-sizing: border-box;
              font-family: 'Roboto', sans-serif;
          }
          body {
              text-align: center;
              margin: 0;
              padding: 0;
              color: #ffffff;
              background-color: #191B21;
          }

          a {
              text-decoration: none;
              color: yellow;font-size: 150%;
              background-color: #2f333d;
              padding: 2px 10px 2px 10px;
              margin: 0 20px 0 20px;
          }
      </style>
  </head>
  <body>
      <header></header>
      <main style="display: flex; flex-direction: column; width: 50vw; margin: auto;">
          <div style="margin: 100px 0 0 0;">
              <a href="/"><</a>
              <a href="/pagina3">></a>
          </div>
          <div style="margin: 50px 0 0 0; display: flex; justify-content: center; align-items: center;">
              <input style="border-style: hidden;" type="color" id="colorPicker">
              <button style="margin: 0 5px 0 5px;" id="sendButton">Enviar</button>
          </div>
          
      </main>
      <footer></footer>
      <script>
          document.getElementById("sendButton").addEventListener("click", function() {
              // Obtém o valor selecionado no campo de entrada de cor
              var colorPicker = document.getElementById("colorPicker");
              var selectedColor = colorPicker.value;
              
              // Extrai os componentes RGB do valor hexadecimal
              var red = parseInt(selectedColor.slice(1, 3), 16);
              var green = parseInt(selectedColor.slice(3, 5), 16);
              var blue = parseInt(selectedColor.slice(5, 7), 16);
              
              // Crie um objeto JSON com os valores
              var jsonData = {
                  r: red,
                  g: green,
                  b: blue,
                  page: 2
              };
              
              // Envia o JSON via POST para o servidor
              fetch("/receber-json", {
                  method: "POST",
                  body: JSON.stringify(jsonData),
                  headers: {
                      "Content-Type": "application/json"
                  }
              })
              .then(function(response) {
                  if (response.ok) {
                      console.log("JSON enviado com sucesso");
                  } else {
                      console.error("Erro ao enviar JSON");
                  }
              })
              .catch(function(error) {
                  console.error("Erro ao enviar JSON:", error);
              });
          });
      </script>
  </body>
  </html>)=====";
  server.send(200, "text/html", html);
}

void handlePagina3() {
  String html = R"=====(<!DOCTYPE html>
  <html lang="pt-br">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Roboto:wght@100;400;500;700&display=swap">
      <title>Sal da Terra - LEDs</title>
      <style>
          * {
              padding: 0px;
              margin: 0px;
              box-sizing: border-box;
              font-family: 'Roboto', sans-serif;
          }
          body {
              text-align: center;
              margin: 0;
              padding: 0;
              color: #ffffff;
              background-color: #191B21;
          }

          a {
              text-decoration: none;
              color: yellow;font-size: 150%;
              background-color: #2f333d;
              padding: 2px 10px 2px 10px;
              margin: 0 20px 0 20px;
          }
      </style>
  </head>
  <body>
      <header></header>
      <main style="display: flex; flex-direction: column; width: 50vw; margin: auto;">
          <div style="margin: 100px 0 0 0;">
              <a href="/pagina2"><</a>
              <a href="/pagina4">></a>
          </div>
          <div style="margin: 50px 0 0 0; display: flex; justify-content: center; align-items: center;">
              <input style="border-style: hidden;" type="color" id="colorPicker">
              <button style="margin: 0 5px 0 5px;" id="sendButton">Enviar</button>
          </div>
          
      </main>
      <footer></footer>
      <script>
          document.getElementById("sendButton").addEventListener("click", function() {
              // Obtém o valor selecionado no campo de entrada de cor
              var colorPicker = document.getElementById("colorPicker");
              var selectedColor = colorPicker.value;
              
              // Extrai os componentes RGB do valor hexadecimal
              var red = parseInt(selectedColor.slice(1, 3), 16);
              var green = parseInt(selectedColor.slice(3, 5), 16);
              var blue = parseInt(selectedColor.slice(5, 7), 16);
              
              // Crie um objeto JSON com os valores
              var jsonData = {
                  r: red,
                  g: green,
                  b: blue,
                  page: 3
              };
              
              // Envia o JSON via POST para o servidor
              fetch("/receber-json", {
                  method: "POST",
                  body: JSON.stringify(jsonData),
                  headers: {
                      "Content-Type": "application/json"
                  }
              })
              .then(function(response) {
                  if (response.ok) {
                      console.log("JSON enviado com sucesso");
                  } else {
                      console.error("Erro ao enviar JSON");
                  }
              })
              .catch(function(error) {
                  console.error("Erro ao enviar JSON:", error);
              });
          });
      </script>
  </body>
  </html>)=====";
  server.send(200, "text/html", html);
}

void handlePagina4() {
  String html = R"=====(<!DOCTYPE html>
  <html lang="pt-br">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Roboto:wght@100;400;500;700&display=swap">
      <title>Sal da Terra - LEDs</title>
      <style>
          * {
              padding: 0px;
              margin: 0px;
              box-sizing: border-box;
              font-family: 'Roboto', sans-serif;
          }
          body {
              text-align: center;
              margin: 0;
              padding: 0;
              color: #ffffff;
              background-color: #191B21;
          }

          a {
              text-decoration: none;
              color: yellow;font-size: 150%;
              background-color: #2f333d;
              padding: 2px 10px 2px 10px;
              margin: 0 20px 0 20px;
          }
      </style>
  </head>
  <body>
      <header></header>
      <main style="display: flex; flex-direction: column; width: 50vw; margin: auto;">
          <div style="margin: 100px 0 0 0;">
              <a href="/pagina3"><</a>
              <a href="/">></a>
          </div>
          <div style="margin: 50px 0 0 0; display: flex; justify-content: center; align-items: center;">
              <input style="border-style: hidden;" type="color" id="colorPicker">
              <button style="margin: 0 5px 0 5px;" id="sendButton">Enviar</button>
          </div>
          
      </main>
      <footer></footer>
      <script>
          document.getElementById("sendButton").addEventListener("click", function() {
              // Obtém o valor selecionado no campo de entrada de cor
              var colorPicker = document.getElementById("colorPicker");
              var selectedColor = colorPicker.value;
              
              // Extrai os componentes RGB do valor hexadecimal
              var red = parseInt(selectedColor.slice(1, 3), 16);
              var green = parseInt(selectedColor.slice(3, 5), 16);
              var blue = parseInt(selectedColor.slice(5, 7), 16);
              
              // Crie um objeto JSON com os valores
              var jsonData = {
                  r: red,
                  g: green,
                  b: blue,
                  page: 4
              };
              
              // Envia o JSON via POST para o servidor
              fetch("/receber-json", {
                  method: "POST",
                  body: JSON.stringify(jsonData),
                  headers: {
                      "Content-Type": "application/json"
                  }
              })
              .then(function(response) {
                  if (response.ok) {
                      console.log("JSON enviado com sucesso");
                  } else {
                      console.error("Erro ao enviar JSON");
                  }
              })
              .catch(function(error) {
                  console.error("Erro ao enviar JSON:", error);
              });
          });
      </script>
  </body>
  </html>)=====";
  server.send(200, "text/html", html);
}

void handleReceiveJSON() {
  if (server.hasArg("plain")) {
    String json = server.arg("plain");

    // Parse do JSON recebido
    DynamicJsonDocument doc(1024); // Ajuste o tamanho conforme necessário
    deserializeJson(doc, json);

    red = doc["r"];
    green = doc["g"];
    blue = doc["b"];
    page = doc["page"];

    // Faça o que desejar com os valores RGB e página
    controlLEDs(red, green, blue, page);

    server.send(200, "text/plain", "JSON recebido com sucesso");
  } else {
    server.send(400, "text/plain", "Requisição inválida");
  }
}




void setup() {
  Serial.begin(115200);

  // Configura a conexão WiFi
  WiFi.begin("Aécio's Labs", "presentedopai2");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado ao WiFi");
  Serial.println(WiFi.localIP());

  // Define rotas para as páginas web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/pagina2", HTTP_GET, handlePagina2);
  server.on("/pagina3", HTTP_GET, handlePagina3);
  server.on("/pagina4", HTTP_GET, handlePagina4);

  // Define rota para receber JSON
  server.on("/receber-json", HTTP_POST, handleReceiveJSON);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}
