const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 WebSocket Demo</title>
</head>
<body>
  <h1>WebSocket Data</h1>
  <p id="data">Menunggu data...</p>

  <script>
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;

    window.addEventListener('load', onLoad);

    function onLoad(event) {
      initWebSocket();
    }

    function initWebSocket() {
      console.log('Mencoba menghubungkan ke WebSocket...');
      websocket = new WebSocket(gateway);
      websocket.onopen = onOpen;
      websocket.onclose = onClose;
      websocket.onmessage = onMessage;
    }

    function onOpen(event) {
      console.log('Terhubung ke WebSocket');
    }

    function onClose(event) {
      console.log('WebSocket terputus');
      setTimeout(initWebSocket, 2000);
    }

    function onMessage(event) {
      console.log('Pesan diterima: ', event.data);
      document.getElementById('data').innerHTML = 'Data: ' + event.data;
    }
  </script>
</body>
</html>
)rawliteral";
