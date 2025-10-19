#ifndef WEBSOCKET_HTML_H 
#define WEBSOCKET_HTML_H 
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>ESP32 WebSocket Server</title>

    <!-- Подключаем Cascadia Mono -->
    <style>
      @font-face {
        font-family: "Cascadia Mono";
        src: url("https://github.com/microsoft/cascadia-code/raw/main/releases/v2405.23/ttf/CascadiaMono.ttf")
          format("truetype");
        font-weight: normal;
        font-style: normal;
      }

      body {
        background: #222;
        color: #f0f0f0;
        font-family: "Cascadia Mono", monospace;
        margin: 0;
        padding: 20px;
        text-align: center;
      }

      .status {
        padding: 15px;
        text-align: center;
        font-size: 16px;
        font-weight: bold;
        background: #4a0404;
        border-radius: 8px;
        margin-bottom: 15px;
        transition: background 0.3s ease;
      }

      .status.connected {
        background: #0a5300;
      }

      #log {
        background: #333;
        padding: 10px;
        border-radius: 8px;
        text-align: left;
        max-width: 500px;
        margin: 0 auto;
        font-size: 14px;
        height: 250px;
        overflow-y: auto;
      }

      .input-area {
        margin-top: 25px; 
        margin-bottom: 15px;
        text-align: center;
      }

      input[type="text"] {
        width: 60%;
        padding: 10px;
        border-radius: 6px;
        border: none;
        font-size: 14px;
        outline: none;
        background: #333;
        color: #fff;
        font-family: "Cascadia Mono", monospace;
      }

      button {
        background: #0a5300;
        color: white;
        border: none;
        padding: 10px 20px;
        border-radius: 6px;
        font-size: 14px;
        margin-left: 8px;
        cursor: pointer;
        transition: background 0.2s ease;
        font-family: "Cascadia Mono", monospace;
      }

      button:hover {
        background: #0e7000;
      }
    </style>
  </head>
  <body>
    <div id="status" class="status">disconnected</div>

    <div class="input-area">
      <input id="msgInput" type="text" placeholder="message" />
      <button id="btnSend">Send</button>
    </div>

    <div id="log"></div>

    <script>
      const statusEl = document.getElementById("status");
      const logEl = document.getElementById("log");
      const inputEl = document.getElementById("msgInput");
      const sendBtn = document.getElementById("btnSend");
      let ws;
      let reconnectTimer;

      function log(msg) {
        const time = new Date().toLocaleTimeString();
        logEl.innerHTML += `[${time}] ${msg}<br>`;
        logEl.scrollTop = logEl.scrollHeight;
      }

      function setStatus(connected) {
        if (connected) {
          statusEl.textContent = "Connected";
          statusEl.classList.add("connected");
        } else {
          statusEl.textContent = "Disconnected";
          statusEl.classList.remove("connected");
        }
      }

      function connect() {
        const wsUrl = `ws://${window.location.hostname}:81/`;
        log(`Connecting to ${wsUrl} ...`);
        ws = new WebSocket(wsUrl);

        ws.onopen = () => {
          setStatus(true);
          log("WebSocket connected");
          ws.send("hello from browser");
        };

        ws.onmessage = (event) => {
          log(`${event.data}`);
        };

        ws.onclose = () => {
          setStatus(false);
          log("WebSocket disconnected. Reconnecting in 2s...");
          clearTimeout(reconnectTimer);
          reconnectTimer = setTimeout(connect, 2000);
        };

        ws.onerror = (err) => {
          log("WebSocket error: " + err.message);
          ws.close();
        };
      }

      sendBtn.addEventListener("click", () => {
        const msg = inputEl.value.trim();
        if (msg === "") {
          log("Message is empty");
          return;
        }

        if (ws && ws.readyState === WebSocket.OPEN) {
          ws.send(msg);
          log(`Sent: ${msg}`);
          inputEl.value = "";
        } else {
          log("Not connected!");
        }
      });

      inputEl.addEventListener("keypress", (e) => {
        if (e.key === "Enter") sendBtn.click();
      });

      // Запуск подключения
      connect();
    </script>
  </body>
</html>
)rawliteral";
#endif