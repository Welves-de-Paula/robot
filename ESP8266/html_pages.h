#ifndef HTML_PAGES_H
#define HTML_PAGES_H

#include <Arduino.h>

// Página: data/login.html
const char HTML_LOGIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ADMIN - {{DEVICE_NAME}}</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #0F0F0F;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }

        .login-container {
            background: #232D3F;
            border-radius: 12px;
            box-shadow: 0 10px 40px rgba(0, 0, 0, 0.5);
            width: 100%;
            max-width: 400px;
            padding: 40px 30px;
            animation: slideIn 0.5s ease-out;
            border: 1px solid #005B41;
        }

        @keyframes slideIn {
            from {
                opacity: 0;
                transform: translateY(-30px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .login-header {
            text-align: center;
            margin-bottom: 30px;
        }

        .login-header h1 {
            color: #008170;
            font-size: 28px;
            margin-bottom: 8px;
        }

        .login-header p {
            color: #ccc;
            font-size: 14px;
        }

        .form-group {
            margin-bottom: 20px;
        }

        .form-group label {
            display: block;
            color: #fff;
            font-weight: 500;
            margin-bottom: 8px;
            font-size: 14px;
        }

        .form-group input {
            width: 100%;
            padding: 12px 15px;
            border: 2px solid #005B41;
            border-radius: 8px;
            font-size: 15px;
            transition: all 0.3s ease;
            outline: none;
            background: #0F0F0F;
            color: #fff;
        }

        .form-group input:focus {
            border-color: #008170;
            box-shadow: 0 0 0 3px rgba(0, 129, 112, 0.2);
        }

        .form-group input::placeholder {
            color: #888;
        }

        .btn-login {
            width: 100%;
            padding: 14px;
            background: #008170;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            margin-top: 10px;
        }

        .btn-login:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 20px rgba(0, 129, 112, 0.5);
            background: #005B41;
        }

        .btn-login:active {
            transform: translateY(0);
        }

        .error-message {
            background: #232D3F;
            color: #ff6b6b;
            padding: 10px 15px;
            border-radius: 6px;
            font-size: 14px;
            margin-bottom: 20px;
            display: none;
            border-left: 4px solid #005B41;
        }

        .error-message.show {
            display: block;
            animation: shake 0.5s ease;
        }

        @keyframes shake {
            0%, 100% { transform: translateX(0); }
            25% { transform: translateX(-10px); }
            75% { transform: translateX(10px); }
        }

        /* Responsividade */
        @media (max-width: 480px) {
            .login-container {
                padding: 30px 20px;
            }

            .login-header h1 {
                font-size: 24px;
            }

            .form-group input {
                padding: 10px 12px;
            }

            .btn-login {
                padding: 12px;
            }
        }

        @media (max-width: 360px) {
            .login-container {
                padding: 25px 15px;
            }

            .login-header h1 {
                font-size: 20px;
            }
        }
    </style>
</head>
<body>
    <div class="login-container">
        <div class="login-header">
            <h1>  {{DEVICE_NAME}}</h1>
            <p>Faça login para continuar</p>
        </div>

        <div id="errorMessage" class="error-message"></div>

        <form id="loginForm" onsubmit="handleLogin(event)">
            <div class="form-group">
                <label for="username">Usuário</label>
                <input 
                    type="text" 
                    id="username" 
                    name="username" 
                    placeholder="Digite seu usuário"
                    required
                    autocomplete="username"
                >
            </div>

            <div class="form-group">
                <label for="password">Senha</label>
                <input 
                    type="password" 
                    id="password" 
                    name="password" 
                    placeholder="Digite sua senha"
                    required
                    autocomplete="current-password"
                >
            </div>

            <button type="submit" class="btn-login">Entrar</button>
        </form>
    </div>

    <script>
        async function handleLogin(event) {
            event.preventDefault();

            const username = document.getElementById('username').value.trim();
            const password = document.getElementById('password').value;
            const errorMessage = document.getElementById('errorMessage');

            try {
                const res = await fetch('/login', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ username, password })
                });

                if (res.ok) {
                    window.location.href = '/dashboard.html';
                } else {
                    const msg = await res.text();
                    errorMessage.textContent = msg || 'Login inválido';
                    errorMessage.classList.add('show');
                }
            } catch {
                errorMessage.textContent = 'Erro de comunicação';
                errorMessage.classList.add('show');
            }
        }

        // Limpar mensagem de erro ao digitar
        document.getElementById('username').addEventListener('input', () => {
            document.getElementById('errorMessage').classList.remove('show');
        });

        document.getElementById('password').addEventListener('input', () => {
            document.getElementById('errorMessage').classList.remove('show');
        });
    </script>
</body>
</html>

)=====";

// Página: data/index.html
const char HTML_INDEX[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Robot ESP8266</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
        }
        .container {
            text-align: center;
            background: white;
            padding: 40px;
            border-radius: 10px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }
        h1 {
            color: #333;
            margin: 0;
            font-size: 3em;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Olá! 👋</h1>
        <p>Bem-vindo ao Robot ESP8266</p>
    </div>
</body>
</html>

)=====";

// Página: data/dashboard.html
const char HTML_DASHBOARD[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #0F0F0F;
            min-height: 100vh;
            padding: 10px;
            overflow-x: hidden;
            color: #fff;
        }

        /* Menu Lateral */
        .sidebar {
            position: fixed;
            left: -280px;
            top: 0;
            width: 280px;
            height: 100vh;
            background: #232D3F;
            box-shadow: 2px 0 20px rgba(0,0,0,0.5);
            transition: left 0.3s ease;
            z-index: 1000;
            overflow-y: auto;
            border-right: 1px solid #005B41;
        }

        .sidebar.active {
            left: 0;
        }

        .sidebar-header {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
            color: white;
            padding: 25px 20px;
            text-align: center;
            border-bottom: 2px solid #005B41;
        }

        .sidebar-header h2 {
            font-size: 1.5rem;
            margin-bottom: 5px;
            font-weight: 600;
        }

        .sidebar-header p {
            font-size: 0.85rem;
            opacity: 0.95;
        }

        .sidebar-menu {
            padding: 20px 0;
        }

        .menu-item {
            display: flex;
            align-items: center;
            padding: 15px 20px;
            color: #ccc;
            text-decoration: none;
            transition: all 0.3s ease;
            cursor: pointer;
            border-left: 4px solid transparent;
        }

        .menu-item:hover {
            background: rgba(0, 129, 112, 0.1);
            border-left-color: #008170;
            color: #fff;
        }

        .menu-item.active {
            background: rgba(0, 129, 112, 0.15);
            border-left-color: #008170;
            color: #008170;
            font-weight: 600;
        }

        .menu-icon {
            font-size: 1.5rem;
            margin-right: 15px;
            width: 30px;
            text-align: center;
        }

        .menu-text {
            flex: 1;
            font-size: 1rem;
        }

        .menu-badge {
            background: #f44336;
            color: white;
            padding: 2px 8px;
            border-radius: 12px;
            font-size: 0.75rem;
            font-weight: 600;
        }

        .menu-divider {
            height: 1px;
            background: rgba(0, 91, 65, 0.3);
            margin: 10px 20px;
        }

        .menu-toggle {
            position: fixed;
            top: 20px;
            left: 20px;
            width: 50px;
            height: 50px;
            background: #232D3F;
            border-radius: 12px;
            display: flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
            box-shadow: 0 5px 15px rgba(0,0,0,0.3);
            z-index: 999;
            transition: all 0.3s ease;
            border: 1px solid #005B41;
        }

        .menu-toggle:hover {
            transform: scale(1.05);
            box-shadow: 0 8px 20px rgba(0, 129, 112, 0.4);
            background: #005B41;
        }

        .menu-toggle.active {
            left: 300px;
        }

        .hamburger {
            width: 24px;
            height: 2px;
            background: #008170;
            position: relative;
            transition: all 0.3s ease;
        }

        .hamburger::before,
        .hamburger::after {
            content: '';
            position: absolute;
            width: 24px;
            height: 2px;
            background: #008170;
            transition: all 0.3s ease;
        }

        .hamburger::before {
            top: -8px;
        }

        .hamburger::after {
            bottom: -8px;
        }

        .menu-toggle.active .hamburger {
            background: transparent;
        }

        .menu-toggle.active .hamburger::before {
            top: 0;
            transform: rotate(45deg);
        }

        .menu-toggle.active .hamburger::after {
            bottom: 0;
            transform: rotate(-45deg);
        }

        .overlay {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0,0,0,0.5);
            opacity: 0;
            visibility: hidden;
            transition: all 0.3s ease;
            z-index: 998;
        }

        .overlay.active {
            opacity: 1;
            visibility: visible;
        }

        .main-content {
            transition: margin-left 0.3s ease;
        }

        .container {
            max-width: 1400px;
            margin: 0 auto;
            padding: 0 5px;
        }

        .header {
            text-align: center;
            color: white;
            margin-bottom: 20px;
            padding: 10px;
        }

        .header h1 {
            font-size: clamp(1.5rem, 5vw, 2.5rem);
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }

        .header p {
            font-size: clamp(0.9rem, 3vw, 1.1rem);
            opacity: 0.9;
        }

        .dashboard-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 15px;
            margin-bottom: 20px;
        }

        .card {
            background: #232D3F;
            border-radius: 15px;
            padding: clamp(15px, 3vw, 25px);
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
            transition: transform 0.3s ease, box-shadow 0.3s ease;
            overflow: hidden;
            border: 1px solid #005B41;
        }

        .card:hover {
            transform: translateY(-5px);
            box-shadow: 0 15px 40px rgba(0, 129, 112, 0.3);
            border-color: #008170;
        }

        .card-header {
            display: flex;
            align-items: center;
            margin-bottom: 15px;
            padding-bottom: 12px;
            border-bottom: 2px solid rgba(0, 91, 65, 0.3);
            flex-wrap: wrap;
        }

        .card-icon {
            width: clamp(40px, 8vw, 50px);
            height: clamp(40px, 8vw, 50px);
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
            border-radius: 12px;
            display: flex;
            align-items: center;
            justify-content: center;
            margin-right: 12px;
            font-size: clamp(20px, 4vw, 24px);
            flex-shrink: 0;
        }

        .card-title {
            font-size: clamp(1.1rem, 3vw, 1.4rem);
            color: #fff;
            font-weight: 600;
        }

        .info-item {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 10px 0;
            border-bottom: 1px solid rgba(0, 91, 65, 0.2);
            gap: 10px;
            flex-wrap: wrap;
        }

        .info-item:last-child {
            border-bottom: none;
        }

        .info-label {
            color: #ccc;
            font-weight: 500;
            font-size: clamp(0.85rem, 2.5vw, 1rem);
            flex: 1;
            min-width: 100px;
        }

        .info-value {
            color: #fff;
            font-weight: 600;
            text-align: right;
            font-size: clamp(0.85rem, 2.5vw, 1rem);
            word-break: break-word;
        }

        .progress-bar {
            width: 100%;
            height: 8px;
            background: rgba(0, 91, 65, 0.2);
            border-radius: 4px;
            overflow: hidden;
            margin-top: 8px;
        }

        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #005B41 0%, #008170 100%);
            transition: width 0.5s ease;
        }

        .status-indicator {
            display: inline-block;
            width: 10px;
            height: 10px;
            border-radius: 50%;
            margin-right: 8px;
        }

        .status-online {
            background: #4caf50;
            box-shadow: 0 0 10px #4caf50;
        }

        .status-offline {
            background: #f44336;
        }

        .button-group {
            display: flex;
            gap: 10px;
            margin-top: 15px;
            flex-wrap: wrap;
        }

        .btn {
            flex: 1;
            min-width: 120px;
            padding: clamp(10px, 2vw, 12px) clamp(15px, 3vw, 20px);
            border: none;
            border-radius: 8px;
            font-size: clamp(0.75rem, 2.5vw, 0.875rem);
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
        }

        .btn-primary {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
            color: white;
        }

        .btn-primary:hover {
            transform: scale(1.05);
            box-shadow: 0 5px 15px rgba(0, 129, 112, 0.4);
        }

        .btn-danger {
            background: linear-gradient(135deg, #f44336 0%, #e91e63 100%);
            color: white;
        }

        .btn-danger:hover {
            transform: scale(1.05);
            box-shadow: 0 5px 15px rgba(244, 67, 54, 0.4);
        }

        .gpio-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(70px, 1fr));
            gap: 8px;
            margin-top: 15px;
        }

        .gpio-pin {
            background: rgba(0, 91, 65, 0.15);
            padding: clamp(10px, 2vw, 15px);
            border-radius: 8px;
            text-align: center;
            transition: all 0.3s ease;
            cursor: pointer;
            min-height: 60px;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            border: 1px solid rgba(0, 91, 65, 0.3);
        }

        .gpio-pin:hover {
            background: rgba(0, 129, 112, 0.25);
            transform: scale(1.05);
            border-color: #008170;
        }

        .gpio-pin.active {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
            color: white;
            border-color: #008170;
        }

        .gpio-label {
            font-size: clamp(0.7rem, 2vw, 0.75rem);
            font-weight: 600;
            margin-bottom: 5px;
            white-space: nowrap;
            color: #fff;
        }

        .gpio-state {
            font-size: clamp(0.6rem, 1.8vw, 0.625rem);
            opacity: 0.8;
            color: #ccc;
        }

        .last-update {
            text-align: center;
            color: white;
            margin-top: 20px;
            opacity: 0.8;
            font-size: clamp(0.75rem, 2.5vw, 0.875rem);
            padding: 10px;
        }

        /* Responsividade aprimorada */
        @media (max-width: 1200px) {
            .dashboard-grid {
                grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            }
        }

        @media (max-width: 768px) {
            body {
                padding: 5px;
            }

            .container {
                padding: 0 5px;
            }

            .header {
                padding: 15px 10px;
                margin-bottom: 15px;
            }

            .menu-toggle {
                top: 10px;
                left: 10px;
                width: 45px;
                height: 45px;
            }

            .menu-toggle.active {
                left: calc(100vw - 55px);
            }

            .sidebar {
                width: calc(100vw - 60px);
                left: calc(-100vw + 60px);
            }

            .sidebar.active {
                left: 0;
            }

            .sidebar-header h2 {
                font-size: 1.3rem;
            }

            .menu-item {
                padding: 12px 15px;
            }

            .menu-icon {
                font-size: 1.3rem;
                width: 25px;
            }

            .menu-text {
                font-size: 0.95rem;
            }

            .dashboard-grid {
                grid-template-columns: 1fr;
                gap: 12px;
            }

            .card {
                padding: 15px;
                border-radius: 12px;
            }

            .card-header {
                margin-bottom: 12px;
                padding-bottom: 10px;
            }

            .card-title {
                font-size: 1.2rem;
            }

            .info-item {
                padding: 8px 0;
                flex-wrap: nowrap;
            }

            .info-label,
            .info-value {
                font-size: 0.9rem;
            }

            .gpio-grid {
                grid-template-columns: repeat(4, 1fr);
                gap: 6px;
            }

            .gpio-pin {
                padding: 12px 8px;
                min-height: 65px;
            }

            .button-group {
                flex-direction: column;
                gap: 8px;
            }

            .btn {
                width: 100%;
                padding: 12px 15px;
                font-size: 0.85rem;
            }

            .last-update {
                font-size: 0.8rem;
                padding: 8px;
            }
        }

        @media (max-width: 480px) {
            body {
                padding: 3px;
                font-size: 14px;
            }

            .header {
                margin-bottom: 12px;
                padding: 10px 5px;
            }

            .header h1 {
                font-size: 1.5rem;
            }

            .header p {
                font-size: 0.85rem;
            }

            .menu-toggle {
                width: 40px;
                height: 40px;
                top: 8px;
                left: 8px;
            }

            .menu-toggle.active {
                left: calc(100vw - 48px);
            }

            .sidebar {
                width: calc(100vw - 48px);
                left: calc(-100vw + 48px);
            }

            .sidebar-header {
                padding: 20px 15px;
            }

            .sidebar-header h2 {
                font-size: 1.2rem;
            }

            .sidebar-header p {
                font-size: 0.8rem;
            }

            .menu-item {
                padding: 10px 12px;
            }

            .menu-icon {
                font-size: 1.2rem;
                width: 22px;
                margin-right: 10px;
            }

            .menu-text {
                font-size: 0.9rem;
            }

            .menu-badge {
                font-size: 0.7rem;
                padding: 2px 6px;
            }

            .hamburger,
            .hamburger::before,
            .hamburger::after {
                width: 20px;
            }

            .dashboard-grid {
                gap: 10px;
            }

            .card {
                border-radius: 10px;
                padding: 12px;
            }

            .card-header {
                margin-bottom: 10px;
                padding-bottom: 8px;
            }

            .card-icon {
                width: 36px;
                height: 36px;
                font-size: 18px;
                margin-right: 10px;
            }

            .card-title {
                font-size: 1.1rem;
            }

            .info-item {
                padding: 6px 0;
            }

            .info-label,
            .info-value {
                font-size: 0.85rem;
            }

            .progress-bar {
                height: 6px;
                margin-top: 6px;
            }

            .gpio-grid {
                grid-template-columns: repeat(4, 1fr);
                gap: 5px;
            }

            .gpio-pin {
                min-height: 60px;
                padding: 8px 5px;
            }

            .gpio-label {
                font-size: 0.65rem;
            }

            .gpio-state {
                font-size: 0.6rem;
            }

            .button-group {
                gap: 6px;
                margin-top: 12px;
            }

            .btn {
                padding: 10px 12px;
                font-size: 0.75rem;
            }

            .last-update {
                font-size: 0.75rem;
                margin-top: 15px;
            }
        }

        @media (max-width: 360px) {
            .card {
                padding: 10px;
            }

            .gpio-grid {
                grid-template-columns: repeat(2, 1fr);
            }

            .info-item {
                flex-direction: column;
                align-items: flex-start;
                gap: 5px;
            }

            .info-value {
                text-align: left;
            }
        }

        /* Landscape mode para dispositivos móveis */
        @media (max-height: 500px) and (orientation: landscape) {
            .header {
                margin-bottom: 10px;
            }

            .header h1 {
                font-size: 1.5rem;
                margin-bottom: 5px;
            }

            .header p {
                font-size: 0.9rem;
            }

            .dashboard-grid {
                grid-template-columns: repeat(2, 1fr);
            }
        }

        /* Impressão */
        @media print {
            body {
                background: white;
            }

            .btn {
                display: none;
            }

            .card {
                break-inside: avoid;
                box-shadow: none;
                border: 1px solid #ddd;
            }
        }

        .loading {
            display: inline-block;
            width: 14px;
            height: 14px;
            border: 2px solid rgba(255,255,255,0.3);
            border-radius: 50%;
            border-top-color: white;
            animation: spin 1s ease-in-out infinite;
        }

        @keyframes spin {
            to { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <!-- Botão Menu Toggle -->
    <div class="menu-toggle" onclick="toggleMenu()">
        <div class="hamburger"></div>
    </div>

    <!-- Overlay -->
    <div class="overlay" onclick="closeMenu()"></div>

    <!-- Menu Lateral -->
    <nav class="sidebar">
        <div class="sidebar-header">
            <h2>{{DEVICE_NAME}}</h2>
            <p>Menu de Navegação</p>
        </div>
        <div class="sidebar-menu">
            <a href="#" class="menu-item active" onclick="navigateTo('dashboard')">
                <span class="menu-icon">📊</span>
                <span class="menu-text">Dashboard</span>
            </a>
            <a href="#" class="menu-item" onclick="navigateTo('config')">
                <span class="menu-icon">⚙️</span>
                <span class="menu-text">Configurações</span>
            </a>
            <a href="#" class="menu-item" onclick="navigateTo('logs')">
                <span class="menu-icon">📝</span>
                <span class="menu-text">Logs</span>
                <span class="menu-badge" id="logsBadge">0</span>
            </a>
            <div class="menu-divider"></div>
     
            <a href="#" class="menu-item" onclick="navigateTo('logout')">
                <span class="menu-icon">🚪</span>
                <span class="menu-text">Sair</span>
            </a>
        </div>
    </nav>

    <!-- Conteúdo Principal -->
    <div class="main-content">
    <div class="container">
        <div class="header">
            <h1>Dashboard</h1>
            <p>Monitoramento em Tempo Real</p>
        </div>

        <div class="dashboard-grid">
            <!-- CPU Card -->
            <div class="card">
                <div class="card-header">
                    <div class="card-icon">⚙️</div>
                    <div class="card-title">CPU</div>
                </div>
                <div class="info-item">
                    <span class="info-label">Frequência</span>
                    <span class="info-value" id="cpuFreq">-- MHz</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Uso</span>
                    <span class="info-value" id="cpuUsage">-- %</span>
                </div>
                <div class="progress-bar">
                    <div class="progress-fill" id="cpuProgress" style="width: 0%"></div>
                </div>
                <div class="info-item">
                    <span class="info-label">Ciclos/Segundo</span>
                    <span class="info-value" id="cpuCycles">--</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Tempo Ativo</span>
                    <span class="info-value" id="uptime">--</span>
                </div>
            </div>

            <!-- Memória Card -->
            <div class="card">
                <div class="card-header">
                    <div class="card-icon">💾</div>
                    <div class="card-title">Memória RAM</div>
                </div>
                <div class="info-item">
                    <span class="info-label">Livre</span>
                    <span class="info-value" id="freeHeap">-- KB</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Total</span>
                    <span class="info-value" id="totalHeap">-- KB</span>
                </div>
                <div class="progress-bar">
                    <div class="progress-fill" id="memProgress" style="width: 0%"></div>
                </div>
                <div class="info-item">
                    <span class="info-label">Uso</span>
                    <span class="info-value" id="heapUsage">-- %</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Fragmentação</span>
                    <span class="info-value" id="heapFrag">-- %</span>
                </div>
            </div>

            <!-- Flash Card -->
            <div class="card">
                <div class="card-header">
                    <div class="card-icon">💿</div>
                    <div class="card-title">Flash</div>
                </div>
                <div class="info-item">
                    <span class="info-label">Tamanho Total</span>
                    <span class="info-value" id="flashSize">-- MB</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Usado</span>
                    <span class="info-value" id="flashUsed">-- KB</span>
                </div>
                <div class="progress-bar">
                    <div class="progress-fill" id="flashProgress" style="width: 0%"></div>
                </div>
                <div class="info-item">
                    <span class="info-label">Velocidade</span>
                    <span class="info-value" id="flashSpeed">-- MHz</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Chip ID</span>
                    <span class="info-value" id="chipId">--</span>
                </div>
            </div>

            <!-- Wi-Fi Card -->
            <div class="card">
                <div class="card-header">
                    <div class="card-icon">📶</div>
                    <div class="card-title">Wi-Fi</div>
                </div>
                <div class="info-item">
                    <span class="info-label">Status</span>
                    <span class="info-value">
                        <span class="status-indicator status-online" id="wifiStatus"></span>
                        <span id="wifiStatusText">Conectado</span>
                    </span>
                </div>
                <div class="info-item">
                    <span class="info-label">SSID</span>
                    <span class="info-value" id="ssid">--</span>
                </div>
                <div class="info-item">
                    <span class="info-label">IP Local</span>
                    <span class="info-value" id="localIP">--</span>
                </div>
                <div class="info-item">
                    <span class="info-label">RSSI</span>
                    <span class="info-value" id="rssi">-- dBm</span>
                </div>
                <div class="progress-bar">
                    <div class="progress-fill" id="rssiProgress" style="width: 0%"></div>
                </div>
                <div class="info-item">
                    <span class="info-label">Canal</span>
                    <span class="info-value" id="channel">--</span>
                </div>
                <div class="info-item">
                    <span class="info-label">MAC</span>
                    <span class="info-value" id="mac">--</span>
                </div>
            </div>

            <!-- Sistema de Arquivos Card -->
            <div class="card">
                <div class="card-header">
                    <div class="card-icon">📁</div>
                    <div class="card-title">Sistema de Arquivos</div>
                </div>
                <div class="info-item">
                    <span class="info-label">Total</span>
                    <span class="info-value" id="fsTotal">-- KB</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Usado</span>
                    <span class="info-value" id="fsUsed">-- KB</span>
                </div>
                <div class="progress-bar">
                    <div class="progress-fill" id="fsProgress" style="width: 0%"></div>
                </div>
                <div class="info-item">
                    <span class="info-label">Livre</span>
                    <span class="info-value" id="fsFree">-- KB</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Uso</span>
                    <span class="info-value" id="fsUsage">-- %</span>
                </div>
            </div>

            <!-- Reset/Boot Card -->
            <div class="card">
                <div class="card-header">
                    <div class="card-icon">🔄</div>
                    <div class="card-title">Reset / Boot</div>
                </div>
                <div class="info-item">
                    <span class="info-label">Motivo do Reset</span>
                    <span class="info-value" id="resetReason">--</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Boot Mode</span>
                    <span class="info-value" id="bootMode">--</span>
                </div>
                <div class="info-item">
                    <span class="info-label">Boot Version</span>
                    <span class="info-value" id="bootVersion">--</span>
                </div>
                <div class="button-group">
                    <button class="btn btn-primary" onclick="softReset()">Reiniciar</button>
                    <button class="btn btn-danger" onclick="hardReset()">Reset Total</button>
                </div>
            </div>

            <!-- GPIO Card -->
            <div class="card" style="grid-column: span 2;">
                <div class="card-header">
                    <div class="card-icon">🔌</div>
                    <div class="card-title">GPIO Pins</div>
                </div>
                <div class="gpio-grid">
                    <div class="gpio-pin" onclick="toggleGPIO(0)">
                        <div class="gpio-label">GPIO 0</div>
                        <div class="gpio-state" id="gpio0">LOW</div>
                    </div>
                    <div class="gpio-pin" onclick="toggleGPIO(2)">
                        <div class="gpio-label">GPIO 2</div>
                        <div class="gpio-state" id="gpio2">LOW</div>
                    </div>
                    <div class="gpio-pin" onclick="toggleGPIO(4)">
                        <div class="gpio-label">GPIO 4</div>
                        <div class="gpio-state" id="gpio4">LOW</div>
                    </div>
                    <div class="gpio-pin" onclick="toggleGPIO(5)">
                        <div class="gpio-label">GPIO 5</div>
                        <div class="gpio-state" id="gpio5">LOW</div>
                    </div>
                    <div class="gpio-pin" onclick="toggleGPIO(12)">
                        <div class="gpio-label">GPIO 12</div>
                        <div class="gpio-state" id="gpio12">LOW</div>
                    </div>
                    <div class="gpio-pin" onclick="toggleGPIO(13)">
                        <div class="gpio-label">GPIO 13</div>
                        <div class="gpio-state" id="gpio13">LOW</div>
                    </div>
                    <div class="gpio-pin" onclick="toggleGPIO(14)">
                        <div class="gpio-label">GPIO 14</div>
                        <div class="gpio-state" id="gpio14">LOW</div>
                    </div>
                    <div class="gpio-pin" onclick="toggleGPIO(15)">
                        <div class="gpio-label">GPIO 15</div>
                        <div class="gpio-state" id="gpio15">LOW</div>
                    </div>
                </div>
            </div>
        </div>

        <div class="last-update">
            Última atualização: <span id="lastUpdate">--</span>
        </div>
    </div>
    </div> <!-- Fim main-content -->

    <script>
        // Atualizar dados a cada 2 segundos
        setInterval(updateDashboard, 2000);
        
        // Atualizar imediatamente ao carregar
        updateDashboard();

        function updateDashboard() {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    // CPU
                    document.getElementById('cpuFreq').textContent = data.cpuFreq + ' MHz';
                    document.getElementById('cpuUsage').textContent = data.cpuUsage + ' %';
                    document.getElementById('cpuProgress').style.width = data.cpuUsage + '%';
                    document.getElementById('cpuCycles').textContent = data.cpuCycles;
                    document.getElementById('uptime').textContent = formatUptime(data.uptime);

                    // Memória
                    document.getElementById('freeHeap').textContent = (data.freeHeap / 1024).toFixed(2) + ' KB';
                    document.getElementById('totalHeap').textContent = (data.totalHeap / 1024).toFixed(2) + ' KB';
                    const heapUsage = ((data.totalHeap - data.freeHeap) / data.totalHeap * 100).toFixed(1);
                    document.getElementById('heapUsage').textContent = heapUsage + ' %';
                    document.getElementById('memProgress').style.width = heapUsage + '%';
                    document.getElementById('heapFrag').textContent = data.heapFrag + ' %';

                    // Flash
                    document.getElementById('flashSize').textContent = (data.flashSize / 1024 / 1024).toFixed(2) + ' MB';
                    document.getElementById('flashUsed').textContent = (data.flashUsed / 1024).toFixed(2) + ' KB';
                    const flashUsage = (data.flashUsed / data.flashSize * 100).toFixed(1);
                    document.getElementById('flashProgress').style.width = flashUsage + '%';
                    document.getElementById('flashSpeed').textContent = data.flashSpeed + ' MHz';
                    document.getElementById('chipId').textContent = data.chipId;

                    // Wi-Fi
                    document.getElementById('ssid').textContent = data.ssid;
                    document.getElementById('localIP').textContent = data.localIP;
                    document.getElementById('rssi').textContent = data.rssi + ' dBm';
                    const rssiPercent = Math.min(Math.max(2 * (data.rssi + 100), 0), 100);
                    document.getElementById('rssiProgress').style.width = rssiPercent + '%';
                    document.getElementById('channel').textContent = data.channel;
                    document.getElementById('mac').textContent = data.mac;

                    // Sistema de Arquivos
                    document.getElementById('fsTotal').textContent = (data.fsTotal / 1024).toFixed(2) + ' KB';
                    document.getElementById('fsUsed').textContent = (data.fsUsed / 1024).toFixed(2) + ' KB';
                    document.getElementById('fsFree').textContent = ((data.fsTotal - data.fsUsed) / 1024).toFixed(2) + ' KB';
                    const fsUsage = (data.fsUsed / data.fsTotal * 100).toFixed(1);
                    document.getElementById('fsUsage').textContent = fsUsage + ' %';
                    document.getElementById('fsProgress').style.width = fsUsage + '%';

                    // Reset/Boot
                    document.getElementById('resetReason').textContent = data.resetReason;
                    document.getElementById('bootMode').textContent = data.bootMode;
                    document.getElementById('bootVersion').textContent = data.bootVersion;

                    // GPIO
                    if (data.gpio) {
                        Object.keys(data.gpio).forEach(pin => {
                            const gpioElement = document.getElementById('gpio' + pin);
                            const gpioPin = gpioElement.parentElement;
                            if (data.gpio[pin]) {
                                gpioElement.textContent = 'HIGH';
                                gpioPin.classList.add('active');
                            } else {
                                gpioElement.textContent = 'LOW';
                                gpioPin.classList.remove('active');
                            }
                        });
                    }

                    // Última atualização
                    document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString('pt-BR');
                })
                .catch(error => {
                    console.error('Erro ao atualizar dashboard:', error);
                    document.getElementById('wifiStatus').classList.remove('status-online');
                    document.getElementById('wifiStatus').classList.add('status-offline');
                    document.getElementById('wifiStatusText').textContent = 'Desconectado';
                });
        }

        function formatUptime(seconds) {
            const days = Math.floor(seconds / 86400);
            const hours = Math.floor((seconds % 86400) / 3600);
            const minutes = Math.floor((seconds % 3600) / 60);
            const secs = seconds % 60;
            
            if (days > 0) {
                return `${days}d ${hours}h ${minutes}m`;
            } else if (hours > 0) {
                return `${hours}h ${minutes}m ${secs}s`;
            } else {
                return `${minutes}m ${secs}s`;
            }
        }

        function toggleGPIO(pin) {
            fetch('/api/gpio', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ pin: pin, action: 'toggle' })
            })
            .then(response => response.json())
            .then(data => {
                console.log('GPIO ' + pin + ' toggled:', data);
                updateDashboard();
            })
            .catch(error => console.error('Erro ao alternar GPIO:', error));
        }

        function softReset() {
            if (confirm('Deseja reiniciar o DEVICE_NAME?')) {
                fetch('/api/reset', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({ type: 'soft' })
                })
                .then(() => {
                    alert('ESP8266 reiniciando...');
                    setTimeout(updateDashboard, 5000);
                })
                .catch(error => console.error('Erro ao reiniciar:', error));
            }
        }

        function hardReset() {
            if (confirm('ATENÇÃO: Isso fará um reset completo do ESP8266. Continuar?')) {
                fetch('/api/reset', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({ type: 'hard' })
                })
                .then(() => {
                    alert('ESP8266 fazendo reset total...');
                    setTimeout(updateDashboard, 10000);
                })
                .catch(error => console.error('Erro ao fazer reset:', error));
            }
        }

        // Funções do Menu Lateral
        function toggleMenu() {
            const sidebar = document.querySelector('.sidebar');
            const overlay = document.querySelector('.overlay');
            const menuToggle = document.querySelector('.menu-toggle');
            
            sidebar.classList.toggle('active');
            overlay.classList.toggle('active');
            menuToggle.classList.toggle('active');
        }

        function closeMenu() {
            const sidebar = document.querySelector('.sidebar');
            const overlay = document.querySelector('.overlay');
            const menuToggle = document.querySelector('.menu-toggle');
            
            sidebar.classList.remove('active');
            overlay.classList.remove('active');
            menuToggle.classList.remove('active');
        }

        function navigateTo(page) {
            // Remove active de todos os itens
            document.querySelectorAll('.menu-item').forEach(item => {
                item.classList.remove('active');
            });
            
            // Adiciona active no item clicado
            event.target.closest('.menu-item').classList.add('active');
            
            // Fecha o menu
            closeMenu();
            
            // Navega para a página
            switch(page) {
                case 'dashboard':
                    // Já está no dashboard
                    console.log('Dashboard selecionado');
                    break;
                case 'config':
                    window.location.href = '/config.html';
                    break;
                case 'logs':
                    window.location.href = '/logs.html';
                    break;
                case 'gpio':
                    // Scroll para a seção GPIO
                    document.querySelector('.gpio-grid').scrollIntoView({ behavior: 'smooth' });
                    break;
                case 'wifi':
                    window.location.href = '/wifi.html';
                    break;
                case 'system':
                    window.location.href = '/system.html';
                    break;
                case 'about':
                    alert('ESP8266 Dashboard\nVersão 1.0\n\nDesenvolvido para monitoramento em tempo real');
                    break;
                case 'logout':
                    if (confirm('Deseja realmente sair?')) {
                        window.location.href = '/login.html';
                    }
                    break;
            }
        }

        // Atualizar contador de logs
        function updateLogsBadge() {
            fetch('/api/logs/count')
                .then(response => response.json())
                .then(data => {
                    const badge = document.getElementById('logsBadge');
                    if (data.count > 0) {
                        badge.textContent = data.count > 99 ? '99+' : data.count;
                        badge.style.display = 'inline-block';
                    } else {
                        badge.style.display = 'none';
                    }
                })
                .catch(error => console.error('Erro ao atualizar contador de logs:', error));
        }

        // Atualizar contador de logs a cada 10 segundos
        setInterval(updateLogsBadge, 10000);
        updateLogsBadge();

        // Fechar menu ao pressionar ESC
        document.addEventListener('keydown', function(event) {
            if (event.key === 'Escape') {
                closeMenu();
            }
        });
    </script>
</body>
</html>

)=====";

// Página: data/config.html
const char HTML_CONFIG[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configurações</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #0F0F0F;
            min-height: 100vh;
            padding: 20px;
            color: #fff;
        }

        .container {
            max-width: 800px;
            margin: 0 auto;
        }

        .header {
            text-align: center;
            color: white;
            margin-bottom: 30px;
        }

        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
            color: #008170;
        }

        .back-button {
            background: #232D3F;
            color: #008170;
            padding: 10px 20px;
            border-radius: 8px;
            text-decoration: none;
            display: inline-flex;
            align-items: center;
            gap: 8px;
            margin-bottom: 20px;
            font-weight: 600;
            transition: all 0.3s ease;
            border: 1px solid #005B41;
        }

        .back-button:hover {
            transform: translateX(-5px);
            box-shadow: 0 5px 15px rgba(0, 129, 112, 0.3);
            background: #005B41;
            color: #fff;
        }

        .card {
            background: #232D3F;
            border-radius: 15px;
            padding: 30px;
            margin-bottom: 20px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
            border: 1px solid #005B41;
            animation: fadeIn 0.5s ease-out;
        }

        @keyframes fadeIn {
            from {
                opacity: 0;
                transform: translateY(20px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .card-title {
            font-size: 1.5em;
            color: #008170;
            margin-bottom: 20px;
            padding-bottom: 15px;
            border-bottom: 2px solid rgba(0, 91, 65, 0.3);
            font-weight: 600;
        }

        .form-group {
            margin-bottom: 20px;
        }

        .form-label {
            display: block;
            color: #ccc;
            font-weight: 600;
            margin-bottom: 8px;
            font-size: 0.95em;
        }

        .form-input {
            width: 100%;
            padding: 12px 15px;
            border: 2px solid #005B41;
            border-radius: 8px;
            font-size: 1em;
            transition: all 0.3s ease;
            background: #0F0F0F;
            color: #fff;
            outline: none;
        }

        .form-input:focus {
            outline: none;
            border-color: #008170;
            box-shadow: 0 0 0 3px rgba(0, 129, 112, 0.2);
        }

        .form-input::placeholder {
            color: #888;
        }

        .form-select {
            width: 100%;
            padding: 12px 15px;
            border: 2px solid #005B41;
            border-radius: 8px;
            font-size: 1em;
            background: #0F0F0F;
            color: #fff;
            cursor: pointer;
            outline: none;
            transition: all 0.3s ease;
        }

        .form-select:focus {
            border-color: #008170;
            box-shadow: 0 0 0 3px rgba(0, 129, 112, 0.2);
        }

        .form-checkbox {
            display: flex;
            align-items: center;
            gap: 10px;
        }

        .form-checkbox input[type="checkbox"] {
            width: 20px;
            height: 20px;
            cursor: pointer;
        }

        .button-group {
            display: flex;
            gap: 15px;
            margin-top: 30px;
        }

        .btn {
            flex: 1;
            padding: 15px 30px;
            border: none;
            border-radius: 8px;
            font-size: 1em;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
        }

        .btn-primary {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
            color: white;
        }

        .btn-primary:hover {
            transform: scale(1.05);
            box-shadow: 0 5px 15px rgba(0, 129, 112, 0.4);
        }

        .btn-secondary {
            background: #0F0F0F;
            color: #ccc;
            border: 2px solid #005B41;
        }

        .btn-secondary:hover {
            background: #232D3F;
            border-color: #008170;
            color: #fff;
        }

        .alert {
            padding: 15px 20px;
            border-radius: 8px;
            margin-bottom: 20px;
            display: none;
            animation: slideDown 0.3s ease-out;
        }

        @keyframes slideDown {
            from {
                opacity: 0;
                transform: translateY(-10px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .alert.success {
            background: rgba(0, 129, 112, 0.2);
            color: #008170;
            border: 1px solid #005B41;
        }

        .alert.error {
            background: rgba(255, 107, 107, 0.2);
            color: #ff6b6b;
            border: 1px solid rgba(255, 107, 107, 0.5);
        }

        .info-text {
            font-size: 0.85em;
            color: #888;
            margin-top: 5px;
        }

        @media (max-width: 768px) {
            body {
                padding: 10px;
            }

            .container {
                padding: 0 10px;
            }

            .header h1 {
                font-size: 1.8rem;
            }

            .back-button {
                padding: 8px 15px;
                font-size: 0.9rem;
            }

            .card {
                padding: 20px;
                margin-bottom: 15px;
            }

            .card-title {
                font-size: 1.3em;
                margin-bottom: 15px;
            }

            .form-group {
                margin-bottom: 15px;
            }

            .form-label {
                font-size: 0.9em;
            }

            .form-input,
            .form-select {
                padding: 10px 12px;
                font-size: 0.95em;
            }

            .button-group {
                flex-direction: column;
                gap: 10px;
            }

            .btn {
                width: 100%;
                padding: 12px 20px;
                font-size: 0.9em;
            }
        }

        @media (max-width: 480px) {
            body {
                padding: 5px;
            }

            .container {
                padding: 0 5px;
            }

            .header {
                margin-bottom: 20px;
            }

            .header h1 {
                font-size: 1.5rem;
            }

            .back-button {
                padding: 8px 12px;
                font-size: 0.85rem;
                margin-bottom: 15px;
            }

            .card {
                padding: 15px;
                border-radius: 12px;
                margin-bottom: 12px;
            }

            .card-title {
                font-size: 1.2em;
                margin-bottom: 12px;
                padding-bottom: 12px;
            }

            .form-group {
                margin-bottom: 12px;
            }

            .form-label {
                font-size: 0.85em;
                margin-bottom: 6px;
            }

            .form-input,
            .form-select {
                padding: 10px;
                font-size: 0.9em;
                border-radius: 6px;
            }

            .form-checkbox {
                gap: 8px;
            }

            .form-checkbox input[type="checkbox"] {
                width: 18px;
                height: 18px;
            }

            .info-text {
                font-size: 0.8em;
            }

            .button-group {
                gap: 8px;
                margin-top: 20px;
            }

            .btn {
                padding: 12px 15px;
                font-size: 0.85em;
            }

            .alert {
                padding: 12px 15px;
                font-size: 0.9em;
            }
        }

        @media (max-width: 360px) {
            .card {
                padding: 12px;
            }

            .card-title {
                font-size: 1.1em;
            }

            .form-input,
            .form-select {
                font-size: 0.85em;
            }

            .btn {
                padding: 10px 12px;
                font-size: 0.8em;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <a href="/dashboard.html" class="back-button">
            ← Voltar ao Dashboard
        </a>

        <div class="header">
            <h1>⚙️ Configurações</h1>
        </div>

        <div id="alertMessage" class="alert"></div>

        <!-- Wi-Fi Settings -->
        <div class="card">
            <div class="card-title">📶 Configurações Wi-Fi</div>
            <form id="wifiForm">
                <div class="form-group">
                    <label class="form-label">SSID (Nome da Rede)</label>
                    <input type="text" class="form-input" id="ssid" placeholder="Nome da rede Wi-Fi">
                </div>
                <div class="form-group">
                    <label class="form-label">Senha</label>
                    <input type="password" class="form-input" id="password" placeholder="Senha da rede">
                </div>
            </form>
        </div>

        <!-- AP Settings -->
        <div class="card">
            <div class="card-title">📡 Configurações AP (Access Point)</div>
            <form id="apForm">
                <div class="form-group">
                    <label class="form-label">Nome do AP (SSID)</label>
                    <input type="text" class="form-input" id="apSSID" placeholder="ESP8266-SETUP">
                    <div class="info-text">Nome da rede que o ESP8266 irá criar</div>
                </div>
                <div class="form-group">
                    <label class="form-label">Senha do AP</label>
                    <input type="password" class="form-input" id="apPassword" placeholder="Senha do Access Point">
                    <div class="info-text">Mínimo de 8 caracteres (deixe vazio para AP aberto)</div>
                </div>
                <div class="form-group">
                    <label class="form-label">Número Máximo de Conexões</label>
                    <select class="form-select" id="apMaxConnections">
                        <option value="1">1 dispositivo</option>
                        <option value="2">2 dispositivos</option>
                        <option value="3">3 dispositivos</option>
                        <option value="4" selected>4 dispositivos</option>
                    </select>
                    <div class="info-text">Máximo de dispositivos conectados simultaneamente</div>
                </div>
                <div class="form-group">
                    <div class="form-checkbox">
                        <input type="checkbox" id="apHidden">
                        <label class="form-label" style="margin-bottom: 0;">Ocultar SSID (Rede invisível)</label>
                    </div>
                </div>
            </form>
        </div>

        <!-- System Settings -->
        <div class="card">
            <div class="card-title">💻 Configurações do Sistema</div>
            <form id="systemForm">
                <div class="form-group">
                    <label class="form-label">Nome do Dispositivo</label>
                    <input type="text" class="form-input" id="deviceName" placeholder="ESP8266-SETUP">
                </div>
                <div class="form-group">
                    <label class="form-label">Frequência da CPU</label>
                    <select class="form-select" id="cpuFreq">
                        <option value="80">80 MHz</option>
                        <option value="160">160 MHz</option>
                    </select>
                    <div class="info-text">Maior frequência = maior consumo de energia</div>
                </div>
                <div class="form-group">
                    <label class="form-label">Intervalo de Atualização (ms)</label>
                    <input type="number" class="form-input" id="updateInterval" placeholder="2000" min="500" max="10000">
                    <div class="info-text">Intervalo de atualização do dashboard</div>
                </div>
            </form>
        </div>

        <!-- GPIO Settings -->
        <div class="card">
            <div class="card-title">🔌 Configurações GPIO</div>
            <form id="gpioForm">
                <div class="form-group">
                    <div class="form-checkbox">
                        <input type="checkbox" id="gpio0">
                        <label class="form-label" style="margin-bottom: 0;">GPIO 0 - Boot Mode</label>
                    </div>
                </div>
                <div class="form-group">
                    <div class="form-checkbox">
                        <input type="checkbox" id="gpio2">
                        <label class="form-label" style="margin-bottom: 0;">GPIO 2 - LED Interno</label>
                    </div>
                </div>
                <div class="form-group">
                    <label class="form-label">Modo PWM Frequency</label>
                    <input type="number" class="form-input" id="pwmFreq" placeholder="1000" min="100" max="10000">
                    <div class="info-text">Frequência PWM para controle de velocidade</div>
                </div>
            </form>
        </div>

        <!-- Security Settings -->
        <div class="card">
            <div class="card-title">🔒 Segurança</div>
            <form id="securityForm">
                <div class="form-group">
                    <label class="form-label">Usuário Admin</label>
                    <input type="text" class="form-input" id="adminUser" placeholder="admin">
                </div>
                <div class="form-group">
                    <label class="form-label">Nova Senha</label>
                    <input type="password" class="form-input" id="newPassword" placeholder="Digite a nova senha">
                </div>
                <div class="form-group">
                    <label class="form-label">Confirmar Senha</label>
                    <input type="password" class="form-input" id="confirmPassword" placeholder="Confirme a senha">
                </div>
            </form>
        </div>

        <!-- Action Buttons -->
        <div class="button-group">
            <button class="btn btn-secondary" onclick="resetSettings()">Resetar</button>
            <button class="btn btn-primary" onclick="saveSettings()">Salvar Configurações</button>
        </div>
    </div>

    <script>
        // Carregar configurações ao iniciar
        window.onload = function() {
            loadSettings();
        };

        function loadSettings() {
            fetch('/api/config')
                .then(response => response.json())
                .then(data => {
                    // Wi-Fi
                    document.getElementById('ssid').value = data.wifi?.ssid || '';

                    // AP
                    document.getElementById('apSSID').value = data.ap?.ssid || 'ESP8266-SETUP';
                    document.getElementById('apMaxConnections').value = data.ap?.maxConnections || '4';
                    document.getElementById('apHidden').checked = data.ap?.hidden || false;

                    // System
                    document.getElementById('deviceName').value = data.system?.name || 'ESP8266';
                    document.getElementById('cpuFreq').value = data.system?.cpuFreq || '80';
                    document.getElementById('updateInterval').value = data.system?.updateInterval || '2000';

                    // GPIO
                    document.getElementById('pwmFreq').value = data.gpio?.pwmFreq || '1000';

                    // Security
                    document.getElementById('adminUser').value = data.security?.user || 'admin';
                })
                .catch(error => {
                    console.error('Erro ao carregar configurações:', error);
                    showAlert('Erro ao carregar configurações', 'error');
                });
        }

        function saveSettings() {
            const config = {
                wifi: {
                    ssid: document.getElementById('ssid').value,
                    password: document.getElementById('password').value
                },
                ap: {
                    ssid: document.getElementById('apSSID').value,
                    password: document.getElementById('apPassword').value,
                    maxConnections: parseInt(document.getElementById('apMaxConnections').value),
                    hidden: document.getElementById('apHidden').checked
                },
                system: {
                    name: document.getElementById('deviceName').value,
                    cpuFreq: parseInt(document.getElementById('cpuFreq').value),
                    updateInterval: parseInt(document.getElementById('updateInterval').value)
                },
                gpio: {
                    gpio0: document.getElementById('gpio0').checked,
                    gpio2: document.getElementById('gpio2').checked,
                    pwmFreq: parseInt(document.getElementById('pwmFreq').value)
                },
                security: {
                    user: document.getElementById('adminUser').value,
                    newPassword: document.getElementById('newPassword').value,
                    confirmPassword: document.getElementById('confirmPassword').value
                }
            };

            // Validar SSID do AP
            if (!config.ap.ssid) {
                showAlert('Por favor, defina um nome para o Access Point', 'error');
                return;
            }

            // Validar senha do AP (se fornecida, deve ter no mínimo 8 caracteres)
            if (config.ap.password && config.ap.password.length < 8) {
                showAlert('A senha do AP deve ter no mínimo 8 caracteres', 'error');
                return;
            }

            // Validar senhas de segurança
            if (config.security.newPassword && config.security.newPassword !== config.security.confirmPassword) {
                showAlert('As senhas não coincidem!', 'error');
                return;
            }

            fetch('/api/config', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(config)
            })
            .then(response => response.json())
            .then(data => {
                showAlert('Configurações salvas com sucesso!', 'success');
                setTimeout(() => {
                    if (config.wifi.ssid && config.wifi.password) {
                        alert('O ESP8266 será reiniciado para aplicar as novas configurações Wi-Fi.');
                    }
                }, 2000);
            })
            .catch(error => {
                console.error('Erro ao salvar configurações:', error);
                showAlert('Erro ao salvar configurações', 'error');
            });
        }

        function resetSettings() {
            if (confirm('Deseja realmente resetar todas as configurações para os valores padrão?')) {
                fetch('/api/config/reset', {
                    method: 'POST'
                })
                .then(response => response.json())
                .then(data => {
                    showAlert('Configurações resetadas!', 'success');
                    setTimeout(() => {
                        window.location.reload();
                    }, 2000);
                })
                .catch(error => {
                    console.error('Erro ao resetar configurações:', error);
                    showAlert('Erro ao resetar configurações', 'error');
                });
            }
        }

        function showAlert(message, type) {
            const alert = document.getElementById('alertMessage');
            alert.textContent = message;
            alert.className = 'alert ' + type;
            alert.style.display = 'block';
            
            setTimeout(() => {
                alert.style.display = 'none';
            }, 5000);
        }
    </script>
</body>
</html>

)=====";

// Página: data/logs.html
const char HTML_LOGS[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Logs</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #0F0F0F;
            min-height: 100vh;
            padding: 20px;
            color: #fff;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
        }

        .header {
            text-align: center;
            color: white;
            margin-bottom: 30px;
        }

        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
            color: #008170;
        }

        .back-button {
            background: #232D3F;
            color: #008170;
            padding: 10px 20px;
            border-radius: 8px;
            text-decoration: none;
            display: inline-flex;
            align-items: center;
            gap: 8px;
            margin-bottom: 20px;
            font-weight: 600;
            transition: all 0.3s ease;
            border: 1px solid #005B41;
        }

        .back-button:hover {
            transform: translateX(-5px);
            box-shadow: 0 5px 15px rgba(0, 129, 112, 0.3);
            background: #005B41;
            color: #fff;
        }

        .controls {
            background: #232D3F;
            border-radius: 15px;
            padding: 20px;
            margin-bottom: 20px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
            display: flex;
            gap: 15px;
            flex-wrap: wrap;
            align-items: center;
            border: 1px solid #005B41;
        }

        .filter-group {
            display: flex;
            gap: 10px;
            align-items: center;
        }

        .filter-label {
            font-weight: 600;
            color: #ccc;
        }

        .filter-select {
            padding: 8px 15px;
            border: 2px solid #005B41;
            border-radius: 8px;
            font-size: 0.95em;
            cursor: pointer;
            background: #0F0F0F;
            color: #fff;
            outline: none;
            transition: all 0.3s ease;
        }

        .filter-select:focus {
            border-color: #008170;
            box-shadow: 0 0 0 3px rgba(0, 129, 112, 0.2);
        }

        .search-input {
            flex: 1;
            min-width: 200px;
            padding: 8px 15px;
            border: 2px solid #005B41;
            border-radius: 8px;
            font-size: 0.95em;
            background: #0F0F0F;
            color: #fff;
            outline: none;
            transition: all 0.3s ease;
        }

        .search-input:focus {
            border-color: #008170;
            box-shadow: 0 0 0 3px rgba(0, 129, 112, 0.2);
        }

        .search-input::placeholder {
            color: #888;
        }

        .btn {
            padding: 8px 20px;
            border: none;
            border-radius: 8px;
            font-size: 0.95em;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
        }

        .btn-primary {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
            color: white;
        }

        .btn-primary:hover {
            transform: scale(1.05);
            box-shadow: 0 5px 15px rgba(0, 129, 112, 0.4);
        }

        .btn-danger {
            background: linear-gradient(135deg, #f44336 0%, #e91e63 100%);
            color: white;
        }

        .btn-danger:hover {
            transform: scale(1.05);
            box-shadow: 0 5px 15px rgba(244, 67, 54, 0.4);
        }

        .logs-container {
            background: #232D3F;
            border-radius: 15px;
            padding: 0;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
            max-height: 600px;
            overflow-y: auto;
            border: 1px solid #005B41;
        }

        .log-entry {
            padding: 15px 20px;
            border-bottom: 1px solid rgba(0, 91, 65, 0.2);
            display: flex;
            gap: 15px;
            align-items: flex-start;
            transition: background 0.2s ease;
        }

        .log-entry:hover {
            background: rgba(0, 129, 112, 0.1);
        }

        .log-entry:last-child {
            border-bottom: none;
        }

        .log-timestamp {
            font-size: 0.85em;
            color: #888;
            white-space: nowrap;
            min-width: 140px;
            font-family: 'Courier New', monospace;
        }

        .log-level {
            padding: 4px 10px;
            border-radius: 6px;
            font-size: 0.75em;
            font-weight: 600;
            text-transform: uppercase;
            white-space: nowrap;
            min-width: 70px;
            text-align: center;
        }

        .log-level.info {
            background: #e3f2fd;
            color: #1976d2;
        }

        .log-level.warning {
            background: #fff3e0;
            color: #f57c00;
        }

        .log-level.error {
            background: #ffebee;
            color: #c62828;
        }

        .log-level.debug {
            background: #f3e5f5;
            color: #7b1fa2;
        }

        .log-level.success {
            background: #e8f5e9;
            color: #2e7d32;
        }

        .log-message {
            flex: 1;
            color: #ccc;
            line-height: 1.5;
            word-break: break-word;
        }

        .log-source {
            font-size: 0.85em;
            color: #888;
            font-weight: 600;
            min-width: 100px;
        }

        .empty-logs {
            text-align: center;
            padding: 60px 20px;
            color: #888;
        }

        .empty-logs-icon {
            font-size: 4em;
            margin-bottom: 20px;
            opacity: 0.5;
        }

        .stats {
            background: #232D3F;
            border-radius: 15px;
            padding: 20px;
            margin-bottom: 20px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 20px;
            border: 1px solid #005B41;
        }

        .stat-item {
            text-align: center;
        }

        .stat-value {
            font-size: 2em;
            font-weight: 700;
            color: #008170;
            margin-bottom: 5px;
        }

        .stat-label {
            font-size: 0.85em;
            color: #888;
            text-transform: uppercase;
        }

        @media (max-width: 768px) {
            .container {
                padding: 0 10px;
            }

            .controls {
                flex-direction: column;
                align-items: stretch;
            }

            .filter-group {
                flex-direction: column;
                align-items: stretch;
            }

            .search-input {
                width: 100%;
            }

            .log-entry {
                flex-direction: column;
                gap: 8px;
            }

            .log-timestamp,
            .log-source {
                min-width: auto;
            }

            .stats {
                grid-template-columns: repeat(2, 1fr);
            }
        }

        @media (max-width: 480px) {
            body {
                padding: 5px;
            }

            .container {
                padding: 0 5px;
            }

            .header {
                margin-bottom: 20px;
            }

            .header h1 {
                font-size: 1.5rem;
            }

            .back-button {
                padding: 8px 12px;
                font-size: 0.85rem;
                margin-bottom: 15px;
            }

            .stats {
                grid-template-columns: repeat(2, 1fr);
                gap: 10px;
                padding: 12px;
                margin-bottom: 15px;
                border-radius: 12px;
            }

            .stat-item {
                padding: 8px 0;
            }

            .stat-value {
                font-size: 1.5em;
            }

            .stat-label {
                font-size: 0.75em;
            }

            .controls {
                padding: 12px;
                gap: 10px;
                border-radius: 12px;
                margin-bottom: 15px;
            }

            .filter-group {
                flex-direction: column;
                align-items: stretch;
                gap: 6px;
            }

            .filter-label {
                font-size: 0.85rem;
            }

            .filter-select {
                padding: 8px 10px;
                font-size: 0.85em;
            }

            .search-input {
                padding: 8px 10px;
                font-size: 0.85em;
            }

            .auto-refresh input[type="checkbox"] {
                width: 18px;
                height: 18px;
            }

            .btn {
                padding: 8px 12px;
                font-size: 0.8rem;
            }

            .logs-container {
                max-height: 400px;
                border-radius: 12px;
            }

            .log-entry {
                padding: 10px 12px;
                gap: 6px;
            }

            .log-timestamp {
                font-size: 0.75em;
            }

            .log-level {
                min-width: 55px;
                font-size: 0.65em;
                padding: 3px 6px;
            }

            .log-source {
                font-size: 0.8em;
            }

            .log-message {
                font-size: 0.85em;
                line-height: 1.4;
            }

            .empty-logs {
                padding: 40px 15px;
            }

            .empty-logs-icon {
                font-size: 2.5em;
                margin-bottom: 15px;
            }

            .empty-logs p {
                font-size: 0.9em;
            }
        }

        @media (max-width: 360px) {
            .stats {
                grid-template-columns: 1fr;
            }

            .stat-value {
                font-size: 1.3em;
            }

            .controls {
                padding: 10px;
            }

            .filter-select,
            .search-input {
                font-size: 0.8em;
            }

            .btn {
                font-size: 0.75rem;
            }

            .log-entry {
                padding: 8px 10px;
            }

            .log-timestamp,
            .log-source,
            .log-message {
                font-size: 0.75em;
            }

            .log-level {
                font-size: 0.6em;
                min-width: 50px;
            }
        }

        .auto-refresh {
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .auto-refresh input[type="checkbox"] {
            width: 20px;
            height: 20px;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <div class="container">
        <a href="/dashboard.html" class="back-button">
            ← Voltar ao Dashboard
        </a>

        <div class="header">
            <h1>📝 Logs do Sistema</h1>
        </div>

        <!-- Controles simples -->
        <div class="controls">
            <div class="auto-refresh">
                <input type="checkbox" id="autoRefresh" checked onchange="toggleAutoRefresh()">
                <label class="filter-label" for="autoRefresh">Auto-atualizar (2s)</label>
            </div>
            <button class="btn btn-primary" onclick="refreshLogs()">🔄 Atualizar agora</button>
        </div>

        <!-- Logs Container -->
        <div class="logs-container" id="logsContainer">
            <div class="empty-logs">
                <div class="empty-logs-icon">📋</div>
                <p>Nenhum log encontrado</p>
            </div>
        </div>
    </div>

    <script>
        let refreshInterval = null;

        window.onload = function() {
            refreshLogs();
            startAutoRefresh();
        };

        function refreshLogs() {
            fetch('/api/logs')
                .then(response => response.json())
                .then(data => {
                    const logs = data.logs || [];
                    displayLogs(logs);
                })
                .catch(error => {
                    console.error('Erro ao carregar logs:', error);
                    showEmptyState();
                });
        }

        function displayLogs(logs) {
            const container = document.getElementById('logsContainer');
            if (!logs.length) {
                showEmptyState();
                return;
            }

            container.innerHTML = logs.map(log => `
                <div class="log-entry">
                    <div class="log-timestamp">${formatTimestamp(log.timestamp)}</div>
                    <div class="log-level ${log.level}">${log.level}</div>
                    <div class="log-source">${log.source}</div>
                    <div class="log-message">${escapeHtml(log.message)}</div>
                </div>
            `).join('');
        }

        function showEmptyState() {
            const container = document.getElementById('logsContainer');
            container.innerHTML = `
                <div class="empty-logs">
                    <div class="empty-logs-icon">📋</div>
                    <p>Nenhum log encontrado</p>
                </div>
            `;
        }

        function toggleAutoRefresh() {
            if (document.getElementById('autoRefresh').checked) {
                startAutoRefresh();
            } else {
                stopAutoRefresh();
            }
        }

        function startAutoRefresh() {
            stopAutoRefresh();
            refreshInterval = setInterval(refreshLogs, 2000);
        }

        function stopAutoRefresh() {
            if (refreshInterval) {
                clearInterval(refreshInterval);
                refreshInterval = null;
            }
        }

        function formatTimestamp(timestamp) {
            const date = new Date(timestamp);
            return date.toLocaleString('pt-BR', {
                day: '2-digit',
                month: '2-digit',
                year: 'numeric',
                hour: '2-digit',
                minute: '2-digit',
                second: '2-digit'
            });
        }

        function escapeHtml(text) {
            const map = {
                '&': '&amp;',
                '<': '&lt;',
                '>': '&gt;',
                '"': '&quot;',
                "'": '&#039;'
            };
            return text.replace(/[&<>"']/g, m => map[m]);
        }
    </script>
</body>
</html>

)=====";

#endif