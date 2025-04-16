const char simpleb_html[] PROGMEM = R"rawliteral(
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
    
<body data-bs-theme="dark">
    <app-navbar />

    <div class="container-sm">
        <h1 class="mt-2">Disparo BULB</h1>
        <p class="lead">Selecciona un modo para configurar</p>

        <div class="d-grid gap-3 mb-3 ">

            <form class="d-grid gap-3 mb-3" action="/pgrm/load" method="GET">
                <div hidden>
                    <label class="form-label" for="mode">Programa</label>
                    <input class="form-control" type="number" id="mode" name="mode" value="1" required>
                </div>

                <div class="">
                    <label class="form-label" for="exp_t">Tiempo de Exposicion</label>
                    <div class="row-gap-3">
                        <!-- <input class="form-range" type="range" id="exp_t" name="exp_t" min="1000" max="180000"
                            step="1000" value="1000" required oninput="if(this.value >= 60000){ 
                                this.nextElementSibling.value = `${(this.value / 60000).toFixed(2).toString()} Minutos`;
                            } else {
                                this.nextElementSibling.value = `${(this.value / 1000).toFixed(2).toString()} Segundos`;
                            }">
                        <input class="form-control" disabled> -->

                    </div>

                    <select id="exp_t" class="form-select" name="exp_t" required>
                        <option selected disabled value="">Seleccione un tiempo</option>
                    </select>

                    <script>
                        const select = document.getElementById('exp_t');
                        for (let segundos = 1; segundos <= 5; segundos += 1) {
                            const option = document.createElement('option');
                            option.value = segundos * 1000; // Convertir segundos a milisegundos
                            option.textContent = `${segundos} segundos`;
                            select.appendChild(option);
                        }
                        for (let segundos = 10; segundos <= 60; segundos += 10) {
                            const option = document.createElement('option');
                            option.value = segundos * 1000; // Convertir segundos a milisegundos
                            option.textContent = `${segundos} segundos`;
                            select.appendChild(option);
                        }
                        for (let minutos = 1; minutos <= (5); minutos += 1) {
                            const option = document.createElement('option');
                            option.value = minutos * 60000; // Convertir minutos a milisegundos
                            option.textContent = `${minutos} minutos`;
                            select.appendChild(option);
                        }
                    </script>
                </div>

                <div hidden>
                    <label class="form-label" for="burst_c">Numero de disparos</label>
                    <input class="form-control" list="datalistOptions" type="" id="burst_c" name="burst_c" value="0"
                        required>
                </div>

                <div hidden>
                    <label class="form-label" for="reburst_c">Numero de repeticiones</label>
                    <input class="form-control" list="datalistOptions" type="" id="reburst_c" name="reburst_c" value="0"
                        required>
                </div>

                <div hidden>
                    <label class="form-label" for="burst_tb">Numero de repeticiones</label>
                    <input class="form-control" list="datalistOptions" type="" id="burst_tb" name="burst_tb" value="0"
                        required>
                </div>

                <input class="btn btn-secondary w-100" type="submit" value="Enviar">
            </form>
        </div>
    </div>
    <script src="/js/bootstrap.min.js"></script>
</body>

    
</html>
)rawliteral";