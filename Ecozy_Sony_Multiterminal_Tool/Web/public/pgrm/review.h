const char review_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html>
    
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Formulario ESP32</title>
        <link href="/css/bootstrap.min.css" rel="stylesheet">
        <script src="/js/bootstrap.bundle.min.js"></script>
        <script src="/components/app-navbar.js"></script>
    </head>
        
<body data-bs-theme="dark" >
    <app-navbar />

    <div class="d-flex flex-column gap-3 p-3">
        <h1 class="mt-2">Control</h1>
        <!-- <p class="lead">Configura los parametros</p> -->

        <button class="btn btn-danger flex-fill rounded-4 shadow " style="height: 200px;"
            onclick="fetch('/pgrm/start')">
            <!-- Ejecutar programa -->
        </button>
    </div>
    <script src="/js/bootstrap.min.js"></script>
</body>

        
    </html>
    )rawliteral";