//Implementa função wichleds

#include <WiFi.h>
#include <Arduino.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>


// Variáveis para armazenar os valores RGB e página que o enviou
int red = 0, green = 0, blue = 0, page = 0;


//MQTT se conectando ao broker e se tornando publisher
const char* mqttServer = "192.168.0.110"; // IP do broker MQTT
const int mqttPort = 1883;
const char* ClientMQTTId = "ESP32TransmissorServer";
WiFiClient espClient;
PubSubClient ClientMQTT(espClient);


//Criando objeto server
WebServer server(80);




void setup() {
  Serial.begin(115200);

  // Configura a conexão WiFi
  WiFi.begin("Comunicacao_2G", "comunicacao2024");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(""); Serial.println("Conectado ao WiFi"); Serial.println(WiFi.localIP()); delay(10);

  // Configura Conexão com broker
  ClientMQTT.setServer(mqttServer, mqttPort);
  while (!ClientMQTT.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (ClientMQTT.connect(ClientMQTTId)) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.println("Falha na conexão ao servidor MQTT Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }


  // Define rotas para as páginas web
  server.on("/", HTTP_GET, handlePageHTML);
  server.on("/stage", HTTP_GET, handlePageHTML);
  server.on("/right-wall", HTTP_GET, handlePageHTML);
  server.on("/left-wall", HTTP_GET, handlePageHTML);
  server.on("/all", HTTP_GET, handlePageHTML);
  // Página de erro 404
  server.onNotFound(handlePageNotFound);
  // Receber JSON
  server.on("/receber-json", HTTP_POST, handleReceiveJSON);

  // Inicia sevidor HTTP
  server.begin();
  Serial.println("Servidor HTTP iniciado");


  // Set up mDNS
  if (!MDNS.begin("saldaterraleds")) {
      Serial.println("Error setting up MDNS responder!");
      while(1) {
          delay(1000);
      }
  }
  Serial.println("mDNS responder started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

}

void loop() {
  //Fica atento as ações do client
  server.handleClient();

  // Reconecta-se ao servidor BROKER MQTT
  while (!ClientMQTT.connected()) {
    Serial.println("Tentando reconectar ao BROKER MQTT...");
    if (ClientMQTT.connect(ClientMQTTId)) {
      Serial.println("Reconectado ao BROKER MQTT");
    } else {
      Serial.println("Falha na reconexão ao BROKER MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}




//Funções
void SendMessageMQTT(int Red, int Green, int Blue, const char* topic){
  // Crie uma string formatada com os valores das variáveis
  String message = String(Red) + "," + String(Green) + "," + String(Blue);

  // Publique a string no tópico desejado
  ClientMQTT.publish(topic, message.c_str());
}


void send_RGB_2_broker(int red, int green, int blue, int page) {
  switch (page){
    case 1: //LEDs Teto path`/`
      Serial.println("");
      Serial.print("Pagina 1 -> ");Serial.print("R: ");Serial.print(red);Serial.print(" G: ");Serial.print(green);Serial.print(" B: ");Serial.println(blue);
      //chama a função que enviar os dados RGB para o client certo
      SendMessageMQTT(red, green, blue, "ROOF");
      break;

    case 2: //LEDs Palco path`stage`
      Serial.println("");
      Serial.print("Pagina 2 -> ");Serial.print("R: ");Serial.print(red);Serial.print(" G: ");Serial.print(green);Serial.print(" B: ");Serial.println(blue);
      //chama a função que enviar os dados RGB para o client certo
      SendMessageMQTT(red, green, blue, "STAGE");
      break;

    case 3: //LEDs Direita path`right-wall`
      Serial.println("");
      Serial.print("Pagina 3 -> ");Serial.print("R: ");Serial.print(red);Serial.print(" G: ");Serial.print(green);Serial.print(" B: ");Serial.println(blue);
      //chama a função que enviar os dados RGB para o client certo
      SendMessageMQTT(red, green, blue, "RIGHT");// implementar logica para controlar individualmente
      break;

    case 4: //LEDs Esquerda path`left-wall`
      Serial.println("");
      Serial.print("Pagina 4 -> ");Serial.print("R: ");Serial.print(red);Serial.print(" G: ");Serial.print(green);Serial.print(" B: ");Serial.println(blue);
      //chama a função que enviar os dados RGB para o client certo
      SendMessageMQTT(red, green, blue, "LEFT");// implementar logica para controlar individualmente
      break;

    case 5: //Todas os LEDs path`all`
      Serial.println("");
      Serial.print("Pagina 5 -> ");Serial.print("R: ");Serial.print(red);Serial.print(" G: ");Serial.print(green);Serial.print(" B: ");Serial.println(blue);
      //chama a função que enviar os dados RGB para o client certo
      SendMessageMQTT(red, green, blue, "ROOF");
      SendMessageMQTT(red, green, blue, "STAGE");
      SendMessageMQTT(red, green, blue, "RIGHT");// implementar logica para controlar individualmente
      SendMessageMQTT(red, green, blue, "LEFT");// implementar logica para controlar individualmente
      break;
  }
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
    send_RGB_2_broker(red, green, blue, page);

    server.send(200, "text/plain", "JSON recebido com sucesso");
  } else {
    server.send(400, "text/plain", "Requisição inválida");
  }
}


void handlePageNotFound() {
  String html = R"=====(
  <!DOCTYPE html>
  <html lang="pt-br">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>PageNotFound</title>

        <style>
            * {
                color: #fff;
                font-family: 'Rubik', sans-serif;
                padding: 0px;
                margin: 0px;
                box-sizing: border-box;
            }
            body{
              width: 100vw;
              height: 100vh;
              display: flex; 
              flex-direction: column; 
              align-items: center; 
              justify-content: space-evenly; 
              color: #fff; 
              background-color: #1A1B20;
            }
            p {
                width: 100%;
                text-align: center;
            }
            a {
                color: #a0ffea;
            }
        </style>
    </head>
    <body>
        <h1>Página não encontrada</h1>
        <p>Verifique a URL digitada</p>
        <p>Para ir para pagina principal clique <a href='/'>aqui</a></p>
    </body>
  </html>
  )=====";
  server.send(404, "text/html", html);
}


void handlePageHTML() {
  String html = R"=====(
    <!DOCTYPE html>
    <html lang="pt-br">
        <head>
            <meta charset="UTF-8" />
            <meta name="viewport" content="width=device-width, initial-scale=1.0" />
            <link rel="preconnect" href="https://fonts.googleapis.com" />
            <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
            <link href="https://fonts.googleapis.com/css2?family=Rubik&display=swap" rel="stylesheet" />
            <title>Sal da Terra - LEDs</title>
            <style>
                * {
                    font-family: "Rubik", sans-serif;
                    padding: 0px;
                    margin: 0px;
                    box-sizing: border-box;
                }
                body {
                    text-align: center;
                    color: #fff;
                    background-color: #1a1b20;
                }

                header,
                main {
                    max-width: 1000px;
                    margin: 0 auto;
                }

                main {
                    min-height: 82vh;
                }

                header {
                    height: 80px;
                    width: 100%;
                    display: flex;
                    justify-content: space-between;
                    align-items: center;
                    padding: 30px 0;
                }
                .otherpage {
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    width: 40px;
                    height: 40px;
                    margin: 0 35px;
                    background-color: #212129;
                    text-decoration: none;
                    color: #fff;
                    font-size: 14pt;
                    border-radius: 20%;
                }
                h1 {
                    font-size: 17pt;
                }

                /*Color Wheel*/
                #ColorWheel {
                    display: flex;
                    align-items: center;
                    justify-content: center;
                }
                #container_ColorWheel {
                    height: 44vh;
                    aspect-ratio: 1/1;
                    display: flex;
                    align-items: center;
                    justify-content: center;
                }
                #outsideWheel {
                    width: 92%;
                    height: 92%;
                    border-radius: 50%;

                    display: flex;
                    align-items: center;
                    justify-content: center;
                    position: relative;
                }
                .radiusColor {
                    width: 4px;
                    height: 50%;
                    position: absolute;
                    bottom: 19.7vh;
                    clip-path: inset(0 0 51% 0);
                    z-index: 1;
                }
                #rail {
                    width: 80%;
                    height: 80%;
                    aspect-ratio: 1/1;
                    border-radius: 50%;
                    display: flex;
                    align-items: center;
                    justify-content: center;
                    border: none;
                }
                #insideWheel {
                    width: 60%;
                    height: 60%;

                    border-radius: 50%;
                    display: flex;
                    align-items: top;
                    justify-content: center;
                    background-color: transparent;
                    z-index: 3;
                }

                #draggable_Element {
                    top: 2vh; /*Inicializa com o elemento no trilho, sem ele o elemento inicializa no centro*/
                    border: solid 3px #fff;
                    border-radius: 50%;
                    width: 30px;
                    aspect-ratio: 1/1;
                    position: absolute;

                    /*desabilita o comportamento de arraste padrão do elemento */
                    touch-action: none;
                    user-drag: none;

                    /* Deixar a Bolinha encima dos outros elementos*/
                    z-index: 4;
                }
                #imglogo {
                    border-radius: 100%;
                    padding: 15px 0 0 0;
                }

                /*Cores pre selecionadas*/
                .container_precolors {
                    display: flex;
                    flex-direction: column;
                    align-items: center;
                    justify-content: space-around;
                    height: 120px;
                }

                .pre_colors {
                    width: 100%;
                    display: flex;
                    justify-content: space-evenly;
                }
                .circle {
                    width: 45px;
                    height: 45px;
                    border-radius: 50%;
                }

                /*Código Slider*/
                #intensityInput {
                    height: 17px;
                    width: 100%;
                    border-radius: 18px;

                    -webkit-appearance: none;
                }
                #intensityInput::-webkit-slider-thumb {
                    -webkit-appearance: none;
                    appearance: none;
                    width: 30px;
                    height: 30px;
                    border-radius: 50%;
                    background: #1a1b20;
                    border: solid 7px #fff;
                }
                .intensidade {
                    margin-top: 30px;
                    display: flex;
                    justify-content: center;
                }
                .containerSlider {
                    width: 80%;
                }

                .intensity {
                    display: flex;
                    padding-bottom: 10px;
                }
                p {
                    font-size: 13pt;
                }
                #slider-value {
                    color: #aaa;
                    margin: 0 1px 0 10px;
                }

                .buttonSelect {
                    width: 9vh;
                    height: 9vh;
                    border-radius: 20%;
                    margin: 6vh 0 10px 0;
                    font-size: 30pt;
                    display: flex;
                    align-items: center;
                    justify-content: center;

                    background-color: #212129;
                }
            </style>
        </head>
        <body>
            <header>
                <a class="otherpage" id="returnPage">&#10094;</a>
                <h1 id="title">LEDs</h1>
                <a class="otherpage" id="nextPage">&#10095;</a>
            </header>

            <main>
                <div id="ColorWheel">
                    <div id="container_ColorWheel">
                        <div id="outsideWheel">
                            <div id="rail">
                                <div id="insideWheel">
                                    <div id="imglogo">
                                        <svg
                                            id="svglogo"
                                            onclick="sendJson(this)"
                                            xmlns="http://www.w3.org/2000/svg"
                                            version="1.1"
                                            width="100%"
                                            height="100%"
                                            viewBox="0 0 500 500"
                                            style="shape-rendering: geometricPrecision; text-rendering: geometricPrecision; image-rendering: optimizeQuality; fill-rule: evenodd; clip-rule: evenodd; border-radius: 5px;"
                                        >
                                            <!--<g>-->
                                            <path
                                                d="M 412.5,9.5 C 415.167,9.5 417.833,9.5 420.5,9.5C 420.649,10.552 420.483,11.552 420,12.5C 398.386,27.3912 385.053,47.7246 380,73.5C 377.903,87.087 380.236,99.7537 387,111.5C 392.626,118.794 398.626,125.794 405,132.5C 428.377,153.199 448.043,176.865 464,203.5C 483.578,244.773 489.912,288.107 483,333.5C 475.323,374.377 456.157,408.544 425.5,436C 384.772,471.363 337.272,489.863 283,491.5C 244.909,490.013 207.742,483.513 171.5,472C 131.687,456.469 100.687,430.636 78.5,394.5C 63.5139,393.87 48.5139,393.037 33.5,392C 28.5037,390.014 24.337,386.847 21,382.5C 1.54014,351.936 -1.79319,319.602 11,285.5C 11.8141,283.718 12.9808,282.218 14.5,281C 27.6957,279.922 40.8624,279.089 54,278.5C 56.3375,250.145 62.0042,222.479 71,195.5C 74.9574,186.953 79.2908,178.62 84,170.5C 86.194,167.819 89.0273,166.319 92.5,166C 116.923,163.731 141.256,160.731 165.5,157C 177.477,155.762 189.477,154.928 201.5,154.5C 201.66,156.199 201.494,157.866 201,159.5C 194.875,170.202 189.708,181.369 185.5,193C 186,193.167 186.5,193.333 187,193.5C 203.943,177.938 223.443,166.438 245.5,159C 254.655,157.198 263.655,154.865 272.5,152C 259.173,144 250.34,132.5 246,117.5C 239.147,87.7271 247.313,62.8937 270.5,43C 279.187,35.9901 288.52,29.9901 298.5,25C 301.774,23.7977 305.107,22.7977 308.5,22C 343.239,17.2006 377.906,13.0339 412.5,9.5 Z M 308.5,166.5 C 309.097,165.861 309.93,165.528 311,165.5C 358.695,173.248 395.029,197.581 420,238.5C 450.091,301.785 441.425,359.119 394,410.5C 357.244,443.524 314.41,455.69 265.5,447C 249.908,442.361 234.908,436.361 220.5,429C 211.397,422.53 203.23,415.03 196,406.5C 194.36,404.421 193.194,402.088 192.5,399.5C 193.833,399.167 195.167,398.833 196.5,398.5C 231.98,400.556 267.48,401.889 303,402.5C 305.288,402.611 306.788,401.611 307.5,399.5C 304.336,391.17 300.502,383.17 296,375.5C 285.623,343.189 282.623,310.189 287,276.5C 285.178,274.955 283.011,274.288 280.5,274.5C 243.845,275.593 207.179,276.259 170.5,276.5C 171.92,260.646 174.42,244.98 178,229.5C 180.748,224.334 183.748,219.334 187,214.5C 189.067,211.93 191.567,209.93 194.5,208.5C 199.705,213.207 204.705,218.04 209.5,223C 214.385,226.297 219.718,228.631 225.5,230C 228.5,232.333 231.5,234.667 234.5,237C 247.869,237.142 261.202,237.809 274.5,239C 284.944,242.723 295.278,246.723 305.5,251C 309.656,253.882 311.656,257.882 311.5,263C 307.492,276.34 306.658,289.84 309,303.5C 310.046,305.09 311.213,306.59 312.5,308C 323.183,312.761 333.016,318.928 342,326.5C 344.983,340.358 346.316,354.358 346,368.5C 342.066,375.367 337.399,381.7 332,387.5C 328.074,393.351 325.241,399.684 323.5,406.5C 323.57,407.772 323.903,408.938 324.5,410C 329.949,410.896 335.282,410.563 340.5,409C 356.249,393.587 370.916,377.254 384.5,360C 390.299,356.953 396.299,354.286 402.5,352C 405.312,350.023 407.978,347.856 410.5,345.5C 417.161,336.847 423.327,327.847 429,318.5C 431.864,313.609 434.03,308.442 435.5,303C 435.129,299.545 433.795,296.545 431.5,294C 422.12,291.322 413.287,287.488 405,282.5C 404.235,279.804 403.068,277.304 401.5,275C 393.296,270.066 384.963,265.399 376.5,261C 374.141,254.925 371.975,248.759 370,242.5C 369.167,241.667 368.333,240.833 367.5,240C 353.94,237.683 340.273,237.016 326.5,238C 323.5,239.667 321.167,242 319.5,245C 318.391,245.638 317.225,246.138 316,246.5C 314.054,244.119 312.221,241.619 310.5,239C 303.077,235.914 295.41,233.581 287.5,232C 285.245,229.256 283.912,226.089 283.5,222.5C 281.926,221.369 280.259,221.202 278.5,222C 276.611,225.391 273.945,227.891 270.5,229.5C 267.241,227.425 264.075,225.092 261,222.5C 258.914,218.333 259.247,214.333 262,210.5C 266.719,203.777 273.052,199.444 281,197.5C 285.337,198.083 289.504,199.25 293.5,201C 295.969,206.817 299.302,212.151 303.5,217C 308.345,217.491 310.678,215.158 310.5,210C 307.249,201.085 306.083,191.919 307,182.5C 310.497,182.869 313.997,182.369 317.5,181C 318.745,173.673 315.745,168.84 308.5,166.5 Z"
                                            />
                                            <!--</path>-->
                                        </svg>
                                    </div>
                                </div>
                                <div id="draggable_Element"></div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="container_precolors">
                    <div class="pre_colors">
                        <div onclick="pickcolor(this)" class="circle circle_red" style="background-color: #f00;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_green" style="background-color: #0f0;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_blue" style="background-color: #00f;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_amarelo" style="background-color: #ffbc2f;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_rosa" style="background-color: #ff00bf;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_roxo" style="background-color: #ae00ff;"></div>
                    </div>
                    <div class="pre_colors">
                        <div onclick="pickcolor(this)" class="circle circle_red" style="background-color: #ffe100;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_green" style="background-color: #ff5100;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_blue" style="background-color: #00ffea;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_amarelo" style="background-color: #12a000;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_rosa" style="background-color: #000;"></div>
                        <div onclick="pickcolor(this)" class="circle circle_roxo" style="background-color: #fff;"></div>
                    </div>
                </div>

                <div class="intensidade">
                    <div class="containerSlider">
                        <div class="intensity">
                            <p style="margin-left: 3px;">Intensidade</p>
                            <p id="slider-value">0</p>
                            <p style="color: #aaaaaa;">%</p>
                        </div>
                        <input type="range" min="0" max="100" value="100" id="intensityInput" />
                    </div>
                </div>

                <div id="wichLEDS" style="width: 100%; display: none; justify-content: space-evenly;">
                    <div class="buttonSelect">1</div>
                    <div class="buttonSelect">2</div>
                    <div class="buttonSelect">3</div>
                </div>
            </main>

            <footer style="background-color: #000; height: 20vh; width: 100%; display: flex; justify-content: flex-end; align-items: center;">
                <!--Code Developed By Aécio Jose and Icaro Rubem-->
                <p style="font-size: 10pt; margin: 0 30px;">Code Developed by <a style="text-decoration: none; color: #aaa;" target="_blank" href="https://github.com/AecioJose/saldaterraleds">@AecioJose</a></p>
            </footer>

            <script>
                //identifica url e troca as coisa conforme a url
                var pageIndice = 0;
                switch (window.location.pathname) {
                    case "/":
                        //Roof
                        returnPage.href = "/all";
                        title.innerText = "LEDs TETO";
                        nextPage.href = "/stage";
                        pageIndice = 1;
                        break;
                    case "/stage":
                        //Stage
                        returnPage.href = "/";
                        title.innerText = "LEDs PALCO";
                        nextPage.href = "/right-wall";
                        pageIndice = 2;
                        break;
                    case "/right-wall":
                        //Right wall
                        returnPage.href = "/stage";
                        title.innerText = "LEDs DIREITA";
                        nextPage.href = "/left-wall";
                        pageIndice = 3;
                        wichLEDS.style.display = "flex";
                        break;
                    case "/left-wall":
                        //Left wall
                        returnPage.href = "/right-wall";
                        title.innerText = "LEDs ESQUERDA";
                        nextPage.href = "/all";
                        pageIndice = 4;
                        wichLEDS.style.display = "flex";
                        break;
                    case "/all":
                        //All
                        returnPage.href = "/left-wall";
                        title.innerText = "TODOS os LEDs";
                        nextPage.href = "/";
                        pageIndice = 5;
                        break;
                }

                //

                /*Cria circulo*/
                for (let n = 0; n < 360; n++) {
                    let div = document.createElement("div");
                    div.classList.toggle("radiusColor");
                    div.style.backgroundColor = `hsl(${n}, 100%, 55.1%, 1)`;
                    div.style.zIndex = 2;
                    div.style.transformOrigin = "50% 100%";
                    div.style.rotate = `${n}deg`;
                    document.querySelector("#outsideWheel").appendChild(div);
                }
                /*Fim circulo*/

                //Elemento Arrastavel, Trilho e Colisão
                const OutsideWheel = document.querySelector("#outsideWheel");
                const Rail = document.querySelector("#rail");
                const DraggableElement = document.getElementById("draggable_Element");
                const grids = document.querySelectorAll(".grid");

                let isDragging = false;

                DraggableElement.addEventListener("mousedown", (e) => {
                    isDragging = true;
                    e.preventDefault();
                });
                DraggableElement.addEventListener("touchstart", (e) => {
                    isDragging = true;
                });

                document.addEventListener("mousemove", (e) => {
                    if (!isDragging) return;

                    let raio = Rail.clientWidth / 2;

                    const wheelCenterX = Rail.offsetLeft + raio;
                    const wheelCenterY = Rail.offsetTop + Rail.clientHeight / 2;

                    const wheelX = OutsideWheel.offsetLeft + OutsideWheel.clientWidth / 2;
                    const wheelY = OutsideWheel.offsetTop + OutsideWheel.clientHeight / 2;

                    const positionX = e.clientX - wheelX;
                    const positionY = e.clientY - wheelY;

                    const angle = Math.atan2(positionY, positionX);

                    const radius = Rail.clientWidth / 2;

                    const clampedX = wheelCenterX + radius * Math.cos(angle) - DraggableElement.clientWidth / 2;
                    const clampedY = wheelCenterY + radius * Math.sin(angle) - DraggableElement.clientHeight / 2;

                    DraggableElement.style.left = `${clampedX}px`;
                    DraggableElement.style.top = `${clampedY}px`;

                    detectarColisaoComGrid(clampedX, clampedY, angle);
                });

                document.addEventListener("touchmove", (e) => {
                    if (!isDragging) return;

                    const touch = e.touches[0];

                    //Raio do Trilho que o draggabled element vai percorrer
                    let radius = Rail.clientWidth / 2;

                    //Encontra as coordenadas do centro do circulo do trilho
                    const wheelCenterX = Rail.offsetLeft + radius; //Soma a distancia da borda esquerda do parente superior mais proximo até ele |(pai do rail)| <distancia> |(rail)|
                    const wheelCenterY = Rail.offsetTop + Rail.clientHeight / 2; // faz a msm coisa porem para parte superior

                    //Encontra as cordenadas do centro do PAI do rail(trilho)
                    const wheelX = OutsideWheel.offsetLeft + OutsideWheel.clientWidth / 2; //radius do outsideWheel
                    const wheelY = OutsideWheel.offsetTop + OutsideWheel.clientHeight / 2;

                    //Pega as coordenadas do touch e subtrai pelo centro do outside
                    const positionX = touch.clientX - wheelX;
                    const positionY = touch.clientY - wheelY;

                    //Calcula o angulo baseado nas coordenadas
                    const angle = Math.atan2(positionY, positionX);

                    //Passa as coordenadas finais do elemento draggbled para atualizar a posição na tela
                    // apartir do centro do circulo do trilho ele vai somar o raio do trilho para ir para bordar e entt define o angulo que ele tem q esta
                    const clampedX = wheelCenterX + radius * Math.cos(angle) - DraggableElement.clientWidth / 2;
                    const clampedY = wheelCenterY + radius * Math.sin(angle) - DraggableElement.clientHeight / 2;

                    DraggableElement.style.left = `${clampedX}px`;
                    DraggableElement.style.top = `${clampedY}px`;

                    detectarColisaoComGrid(clampedX, clampedY, angle);
                });

                document.addEventListener("mouseup", () => {
                    isDragging = false;
                });

                DraggableElement.addEventListener("touchend", () => {
                    isDragging = false;
                });

                function detectarColisaoComGrid(x, y, angle) {
                    //PEGA UM ELEMENTO(div.radiusColor) COM BASE NA COORDENADA x E y, PELO FATO DE ELE ESTAR PROCURANDO NO document, PRECISA-SE SOMAR MAIS A DISTÂNCIA DA BORDA DA PÁGINA ATÉ O outsidewheel(left/top)

                    let element = document.elementFromPoint(x + outsideWheel.offsetLeft + rail.offsetLeft, y + outsideWheel.offsetTop);

                    //MUDA A COR DA LOGO E DO SLIDER  E APLICA PORCENTAGEM
                    const fillColor = element.style.backgroundColor;

                    const rgbaMatch = fillColor.match(/rgba?\((\d+), (\d+), (\d+)/);
                    const r = parseInt(rgbaMatch[1], 10);
                    const g = parseInt(rgbaMatch[2], 10);
                    const b = parseInt(rgbaMatch[3], 10);

                    Slider_n_SVG_ReloadColor(r, g, b);
                }

                /*slider*/
                //atualiza cor do slider e logo
                function Slider_n_SVG_ReloadColor(r, g, b) {
                    //Atualizando a cor no slider
                    document.getElementById("intensityInput").style.backgroundImage = `linear-gradient(to right, #00000A , rgb(${r},${g},${b}))`;

                    //Atualizando a cor no SVG
                    svglogo.style.fill = `rgb(${calcPorcent(r)},${calcPorcent(g)},${calcPorcent(b)})`;
                    localStorage.setItem("R", r);
                    localStorage.setItem("G", g);
                    localStorage.setItem("B", b);
                }

                Slider_n_SVG_ReloadColor(250, 250, 255); //Inicia o Slider com a cor branca
                var slider = document.getElementById("intensityInput");
                var output = document.getElementById("slider-value");

                output.innerHTML = slider.value;

                //ao mecher no slider ele altera o valor no output
                slider.oninput = function () {
                    output.innerText = this.value;

                    //ao mecher no slider ele muda a intensidade do svg

                    //Pegas as cores salvas em memoria
                    const R = localStorage.getItem("R");
                    const G = localStorage.getItem("G");
                    const B = localStorage.getItem("B");

                    const r = parseInt((R * this.value) / 100);
                    const g = parseInt((G * this.value) / 100);
                    const b = parseInt((B * this.value) / 100);

                    svglogo.style.fill = `rgb(${r},${g},${b})`;
                };

                /*fim slider*/

                //Função que calcula a intensidade
                function calcPorcent(number) {
                    const intensity = parseInt(document.querySelector("#intensityInput").value);

                    return parseInt((intensity * number) / 100);
                }

                //Pegar cor das bolinhas e atualiza no slider e na logo
                function pickcolor(me) {
                    // Obtém o background color atual do elemento
                    const backgroundColor = me.style.backgroundColor;

                    //atualiza a cor no slider
                    // A função rgba() retorna uma string no formato "rgba(r, g, b, a)".
                    // Vamos usar uma expressão regular para extrair os valores de r, g e b.
                    const rgbaMatch = backgroundColor.match(/rgba?\((\d+), (\d+), (\d+)/);

                    if (rgbaMatch) {
                        // O primeiro grupo capturado (índice 1) é o valor de R, o segundo (índice 2) é o valor de G e o terceiro (índice 3) é o valor de B.
                        const red = parseInt(rgbaMatch[1], 10);
                        const green = parseInt(rgbaMatch[2], 10);
                        const blue = parseInt(rgbaMatch[3], 10);

                        // Agora você tem os valores de R, G e B em variáveis separadas.
                        Slider_n_SVG_ReloadColor(red, green, blue);
                    }
                }

                //Pega a cor da logo e enviar para o JSON
                function sendJson(me) {
                    console.log("Sending Color");
                    const fillColor = me.style.fill;

                    const rgbaMatch = fillColor.match(/rgba?\((\d+), (\d+), (\d+)/);

                    if (rgbaMatch) {
                        const red = parseInt(rgbaMatch[1], 10);
                        const green = parseInt(rgbaMatch[2], 10);
                        const blue = parseInt(rgbaMatch[3], 10);
                        console.log("R ", red, "G ", green, "B ", blue, "PageIndice ", pageIndice);

                        //Enviando JSON para o broker
                        // Crie um objeto JSON com os valores
                        var jsonData = {
                            r: red,
                            g: green,
                            b: blue,
                            page: pageIndice,
                        };

                        //Envia o JSON via POST para o servidor
                        fetch("/receber-json", {
                            method: "POST",
                            body: JSON.stringify(jsonData),
                            headers: {
                                "Content-Type": "application/json",
                            },
                        })
                            .then(function (response) {
                                if (response.ok) {
                                    console.log("JSON enviado com sucesso");
                                } else {
                                    console.error("Erro ao enviar JSON");
                                }
                            })
                            .catch(function (error) {
                                console.error("Erro ao enviar JSON:", error);
                            });
                        console.log("Sended Color To Broker");
                    }
                }
            </script>
        </body>
    </html>

    )=====";
  server.send(200, "text/html", html);
}
