#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "GNC";
const char* password = "gnc81982832";
ESP8266WebServer server(80);
const int output5 = 5; // D1
IPAddress local_IP(192, 168, 1, 169); // Troque o IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

bool paymentStarted = false;
unsigned long remainingTime = 0;
unsigned long endTime = 0;

void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Simulação de Pagamento</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      min-height: 100vh;
      background-color: #f4f4f4;
    }

    main {
      flex-grow: 1;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      padding: 20px;
    }

    #paymentScreen, #timerScreen {
      text-align: center;
      width: 100%;
      max-width: 400px;
      padding: 20px;
      background-color: #fff;
      border-radius: 8px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }

    input[type="number"] {
      width: 50px;
      padding: 8px;
      font-size: 16px;
    }

    button {
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
      background-color: #3498db;
      color: #fff;
      border: none;
      border-radius: 4px;
    }

    button:hover {
      background-color: #2980b9;
    }

    #timer {
      font-size: 24px;
      font-weight: bold;
    }

    footer {
      display: flex;
      justify-content: center;
      align-items: center;
      padding: 10px 0;
      background-color: #ccc;
      color: #333;
    }

    footer a {
      margin-left: 5px;
      color: #333;
      text-decoration: none;
      display: flex;
      align-items: center;
    }

    footer img {
      width: 24px;
      height: 24px;
      margin-right: 5px;
    }

    strong {
      padding-left: 4px;
    }

    @media screen and (max-width: 600px) {
      #paymentScreen, #timerScreen {
        width: 90%;
      }
    }
  </style>
</head>
<body>
  <main>
    <div id="paymentScreen">
      <h1>Simulação de Pagamento</h1>
      <label for="hoursInput">Escolha a duração do tempo (em horas):</label>
      <input type="number" id="hoursInput" min="1" step="1" value="1" aria-label="Escolha a duração do tempo em horas">
      <button onclick="startPayment()">Pagar</button>
    </div>
    <div id="timerScreen" style="display: none;">
      <h1>Tempo Restante</h1>
      <p id="timer"></p>
      <button onclick="cancelPayment()">Cancelar</button> <!-- Botão de Cancelar -->
    </div>
  </main>
  <footer>
    Desenvolvido por <strong>alfajor-iot</strong>
    <a href="https://github.com/Matheus-Bertol/Alfajor-iot" target="_blank" rel="noopener noreferrer">
      <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/github/github-original.svg" alt="GitHub" />
    </a>
  </footer>
  <script>
    let remainingTime;
    let intervalId;

    function startPayment() {
      const hours = parseInt(document.getElementById('hoursInput').value);
      const amount = hours * 0.5; // Valor a ser pago (R$2 por hora)

      alert(`Você pagou R$ ${amount.toFixed(2)}. O tempo está contando!`);

      toggleLED('on'); // Ligar o relé quando o pagamento começa

      document.getElementById('paymentScreen').style.display = 'none';
      document.getElementById('timerScreen').style.display = 'block';

      remainingTime = hours * 60 * 60; // Converter horas em segundos
      startTimer();
    }

    function toggleLED(status) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          console.log("Resposta do servidor: " + this.responseText);
        }
      };

      if (status === 'on') {
        xhttp.open("GET", "/5/on", true);
      } else if (status === 'off') {
        xhttp.open("GET", "/5/off", true);
      }
      xhttp.send();
    }

    function startTimer() {
      const timerDisplay = document.getElementById('timer');
      updateTimerDisplay(timerDisplay);

      intervalId = setInterval(() => {
        remainingTime--;
        updateTimerDisplay(timerDisplay);

        if (remainingTime <= 0) {
          clearInterval(intervalId);
          alert('Tempo acabou! Retornando para a tela de pagamento.');
          toggleLED('off'); // Desligar o relé quando o tempo expirar
          resetPayment();
        }
      }, 1000);
    }

    function updateTimerDisplay(displayElement) {
      const hours = Math.floor(remainingTime / 3600);
      const minutes = Math.floor((remainingTime % 3600) / 60);
      const seconds = remainingTime % 60;

      displayElement.textContent = `Tempo restante: ${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
    }

    function resetPayment() {
      clearInterval(intervalId);
      document.getElementById('paymentScreen').style.display = 'block';
      document.getElementById('timerScreen').style.display = 'none';
    }

    function cancelPayment() {
      clearInterval(intervalId); // Interrompe o contador de tempo
      toggleLED('off'); // Desliga o LED
      alert('Pagamento cancelado!'); // Exibe um alerta informando sobre o cancelamento do pagamento
      resetPayment(); // Retorna à tela de pagamento
    }
  </script>
</body>
</html>

  <script>
    let remainingTime;
    let intervalId;

    function startPayment() {
      const hours = parseInt(document.getElementById('hoursInput').value);
      const amount = hours * 0.5; // Valor a ser pago (R$2 por hora)

      alert(`Você pagou R$ ${amount.toFixed(2)}. O tempo está contando!`);

      toggleLED('on'); // Ligar o LED quando o pagamento começa

      document.getElementById('paymentScreen').style.display = 'none';
      document.getElementById('timerScreen').style.display = 'block';

      remainingTime = hours * 60 * 60; // Converter horas em segundos
      startTimer();
    }

    function toggleLED(status) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          console.log("Resposta do servidor: " + this.responseText);
        }
      };

      if (status === 'on') {
        xhttp.open("GET", "/5/on", true);
      } else if (status === 'off') {
        xhttp.open("GET", "/5/off", true);
      }
      xhttp.send();
    }

    function startTimer() {
      const timerDisplay = document.getElementById('timer');
      updateTimerDisplay(timerDisplay);

      intervalId = setInterval(() => {
        remainingTime--;
        updateTimerDisplay(timerDisplay);

        if (remainingTime <= 0) {
          clearInterval(intervalId);
          alert('Tempo acabou! Retornando para a tela de pagamento.');
          toggleLED('off'); // Desligar o relé quando o tempo expirar
          resetPayment();
        }
      }, 1000);
    }

    function updateTimerDisplay(displayElement) {
      const hours = Math.floor(remainingTime / 3600);
      const minutes = Math.floor((remainingTime % 3600) / 60);
      const seconds = remainingTime % 60;

      displayElement.textContent = `Tempo restante: ${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
    }

    function resetPayment() {
      clearInterval(intervalId);
      document.getElementById('paymentScreen').style.display = 'block';
      document.getElementById('timerScreen').style.display = 'none';
    }

    function cancelPayment() {
      clearInterval(intervalId); // Interrompe o contador de tempo
      toggleLED('off'); // Desliga o relé
      alert('Pagamento cancelado!'); // Exibe um alerta informando sobre o cancelamento do pagamento
      resetPayment(); // Retorna à tela de pagamento
    }
  </script>
</body>
</html>

)=====";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(output5, LOW); 
  server.send(200, "text/plain", "RELÉ Ligado!"); 
  Serial.println("RELÉ ligado");
}

void handleOff() {
  digitalWrite(output5, HIGH); 
  server.send(200, "text/plain", "RELÉ Desligado!"); 
  Serial.println("RELÉ ligado");
}

void setup() {
  Serial.begin(9600);
  pinMode(output5, OUTPUT);
  digitalWrite(output5, HIGH);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/5/on", HTTP_GET, handleOn);
  server.on("/5/off", HTTP_GET, handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}