class AppNavbar extends HTMLElement {
	connectedCallback() {
		this.innerHTML = `
        <nav class="navbar navbar-expand-lg bg-body-tertiary" >
        <div class="container-fluid">
            <a class="navbar-brand" href="/">Sony Multi-terminal Tool</a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse"
                data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false"
                aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
                <ul class="navbar-nav me-auto mb-2 mb-lg-0">
                    <li class="nav-item">
                        <a class="nav-link active" aria-current="page" href="/">Inicio</a>
                    </li>
                    <li class="nav-item dropdown">
                        <a class="nav-link dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown"
                            aria-expanded="false">
                            Modos
                        </a>
                        <ul class="dropdown-menu">
                            <li><a class="dropdown-item" href="/modes/simple">Disparo Simple</a></li>
                            <li><a class="dropdown-item" href="/modes/simpleb">Disparo Simple - BULB</a></li>
                            <li><a class="dropdown-item" href="/modes/burst">Ragafa Simple</a></li>
                            <li><a class="dropdown-item" href="/modes/burstb">Ragafa - BULB</a></li>
                            <li><a class="dropdown-item" href="/modes/reburst">Ragafa Repetitiva</a></li>
                        </ul>
                    </li>
                </ul>
            </div>
        </div>
    </nav>
      `;
	}
}
customElements.define('app-navbar', AppNavbar);
