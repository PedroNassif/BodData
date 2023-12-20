const char* htmlCode = R"(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no"/>
  <title>Central de Controle - DicaLab</title>
  <style>
    *{
        font-family:"Quicksand", sans-serif;
    }
    body {
      text-align: center;
      margin: 20px;
      background-color: #333;
      color: #fff;
    }

    h2 {
      margin-bottom: 20px;
      color: #f3f3f3;
      font-size: xx-large;
    }

    button {
      border: 0;
      background-color: #1fa3ec;
      color: #fff;
      border-radius: 5px;
      width: 35%;
      padding: 10px;
      font-size: 1.2rem;

      cursor: pointer;
      margin-bottom: 15px;
      transition: background-color 0.3s;
    }

    button:active {
      background-color: #114b74;
    }

    button:hover {
      background-color: #155a8a;
    }

    button.D{
        background-color: #dc3630; 
        width: 20%;
    }

    button.D:hover{
        background-color: #9b1915
    }

    button.D:active{
        background-color: #b91d18
    }

    dl {
      margin-top: 20px;
    }

    dt {
      font-weight: bold;
      margin-top: 10px;
    }

    dd {
      margin: 0;
      padding: 0 0 0.5em 0;
    }

    table {
      width: 100%;
      margin-top: 20px;
      border-collapse: collapse;
    }

    th {
      border: 1px solid #444;
      padding: 15px;
      text-align: left;
      background-color: #444;
      color: #eee;
      font-weight: bold;
    }

    td {
      border: 1px solid #333;
      padding: 15px;
      text-align: left;
    }
    a {
        color: #fff;
        font-weight: 700;
        text-decoration: none;
    }
    a:hover {
        color: #000;
        text-decoration: underline;
    }
  </style>
</head>
<body>
    <div>
        <h2>CENTRAL DE CONTROLE - DICALAB</h2>
        <form action="/start" method="get"><button>Comece o Registro</button></form>
        <form action="/download" method="get"><button>Download do Arquivo</button></form>
        <form action="/reset" method="get"><button>Resete o Registro</button></form>
        <form action="/delete" method="get"><button>Delete o Arquivo</button></form>
        <form action="/exit" method="post"><button class="D">Sair</button></form>
           <Hr>

    <h3>Páginas Disponíveis</h3>
    <table>
      <thead><tr><th>Página do server</th><th>Função</th></tr></thead>
      <tbody>
        <tr><td><a href="/start">/start</a></td>
          <td>Cria o arquivo "datalogger.csv" com dados de temperatura, atualizando a cada 60 segundos.</td></tr>
        <tr><td><a href="/download">/download</a></td>
          <td>Baixa o arquivo "datalogger.csv" no seu dispositivo.</td></tr>
        <tr><td><a href="/reset">/reset</a></td>
          <td>Reinicia a coleta dos dados de temperatura, começando um novo arquivo "datalogger.csv"</td></tr>
        <tr><td><a href="/delete">/delete</a></td>
          <td>Deleta o arquivo "datalogger.csv" na memória do dispositivo.</td></tr>
        <tr><td><a href="/exit">/exit</a></td>
          <td>Sai do portal de registros.</td></tr>
      </tbody>
    </table>

    <hr>
    <h3>Dados do Dispositivo Controlador</h3>
    
    <dl>
        <dt>Modelo</dt><dd>Esp32 DevkitC v4</dd>
        <dt>Tipo do Chip</dt><dd>Espressif: ESP32-WROOM-32D</dd>
        <dt>Chip ID</dt><dd>123456</dd>
        <dt>SPI Flash</dt><dd>32 Mb</dd>
        <dt>Soft AP IP</dt><dd>192.168.4.1</dd>
    </dl>

</div>

<div id="esp32-info">
    <!-- Os dados do ESP32 serão inseridos aqui dinamicamente -->
  </div>
</body>
</html>
)";