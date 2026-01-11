#ifndef HTML_PAGES_H
#define HTML_PAGES_H

#include <Arduino.h>

// Página: data/login.html
const char HTML_LOGIN[] PROGMEM = R"=====(<!DOCTYPE html>
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
                    window.location.href = '/';
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
const char HTML_INDEX[] PROGMEM = R"=====(<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{{DEVICE_NAME}}</title>
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
            color: #fff;
        }

        .container {
            text-align: center;
            background: #232D3F;
            padding: 50px 40px;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.5);
            border: 1px solid #005B41;
            max-width: 500px;
            width: 100%;
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

        h1 {
            color: #008170;
            margin-bottom: 10px;
            font-size: 2.5em;
            font-weight: 600;
        }

        .subtitle {
            color: #888;
            margin-bottom: 40px;
            font-size: 1.1em;
        }

        .button-group {
            display: flex;
            flex-direction: column;
            gap: 15px;
        }

        .btn {
            display: block;
            padding: 18px 30px;
            font-size: 1.1em;
            font-weight: 600;
            text-decoration: none;
            color: white;
            border: 2px solid #005B41;
            border-radius: 12px;
            cursor: pointer;
            transition: all 0.3s ease;
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
        }

        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(0, 129, 112, 0.4);
            border-color: #008170;
        }

        .btn:active {
            transform: translateY(0);
        }

        .btn-logs {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
        }

        .btn-config {
            background: linear-gradient(135deg, #004d3d 0%, #006b5a 100%);
        }

        .btn-config:hover {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
        }

        @media (max-width: 480px) {
            .container {
                padding: 30px 20px;
            }
            h1 {
                font-size: 2em;
            }
            .btn {
                padding: 15px 25px;
                font-size: 1em;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>{{DEVICE_NAME}}</h1>
        <p class="subtitle">ESP8266 Gateway</p>
        <div class="button-group">
            <a href="/logs.html" class="btn btn-logs">📊 Ver Logs</a>
            <a href="/config.html" class="btn btn-config">⚙️ Configurações</a>
        </div>
    </div>
</body>
</html>
)=====";

// Página: data/dashboard.html
const char HTML_DASHBOARD[] PROGMEM = R"=====(<!DOCTYPE html>
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
const char HTML_CONFIG[] PROGMEM = R"=====(<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configurações - {{DEVICE_NAME}}</title>
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
            max-width: 900px;
            margin: 0 auto;
        }

        .header {
            text-align: center;
            margin-bottom: 30px;
        }

        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
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
            display: flex;
            align-items: center;
            gap: 10px;
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
            outline: none;
            background: #0F0F0F;
            color: #fff;
        }

        .form-input:focus {
            border-color: #008170;
            box-shadow: 0 0 0 3px rgba(0, 129, 112, 0.2);
        }

        .form-input::placeholder {
            color: #666;
        }

        .form-input:disabled {
            background: #1a1a1a;
            cursor: not-allowed;
            opacity: 0.6;
        }

        .checkbox-group {
            display: flex;
            align-items: center;
            gap: 10px;
        }

        .checkbox-input {
            width: 20px;
            height: 20px;
            cursor: pointer;
        }

        .form-help {
            font-size: 0.85em;
            color: #888;
            margin-top: 5px;
        }

        .btn {
            padding: 12px 30px;
            border: none;
            border-radius: 8px;
            font-size: 1em;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        .btn-primary {
            background: linear-gradient(135deg, #005B41 0%, #008170 100%);
            color: white;
            width: 100%;
        }

        .btn-primary:hover:not(:disabled) {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(0, 129, 112, 0.4);
        }

        .btn-primary:disabled {
            opacity: 0.6;
            cursor: not-allowed;
        }

        .btn-danger {
            background: linear-gradient(135deg, #f44336 0%, #e91e63 100%);
            color: white;
            width: 100%;
        }

        .btn-danger:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(244, 67, 54, 0.4);
        }

        .alert {
            padding: 15px 20px;
            border-radius: 8px;
            margin-bottom: 20px;
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

        .alert-success {
            background: rgba(46, 125, 50, 0.2);
            border: 1px solid #2e7d32;
            color: #a5d6a7;
        }

        .alert-error {
            background: rgba(211, 47, 47, 0.2);
            border: 1px solid #d32f2f;
            color: #ef9a9a;
        }

        .alert-warning {
            background: rgba(245, 124, 0, 0.2);
            border: 1px solid #f57c00;
            color: #ffcc80;
        }

        .divider {
            height: 1px;
            background: rgba(0, 91, 65, 0.3);
            margin: 25px 0;
        }

        .info-box {
            background: rgba(0, 129, 112, 0.1);
            border-left: 3px solid #008170;
            padding: 15px;
            border-radius: 8px;
            margin-bottom: 20px;
        }

        .info-box p {
            color: #ccc;
            line-height: 1.6;
            margin: 0;
        }

        .button-group {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
            margin-top: 25px;
        }

        @media (max-width: 768px) {
            body {
                padding: 10px;
            }

            .header h1 {
                font-size: 1.8em;
            }

            .card {
                padding: 20px;
            }

            .button-group {
                grid-template-columns: 1fr;
            }
        }

        .password-toggle {
            position: relative;
        }

        .password-toggle-btn {
            position: absolute;
            right: 10px;
            top: 50%;
            transform: translateY(-50%);
            background: none;
            border: none;
            color: #888;
            cursor: pointer;
            padding: 5px 10px;
            font-size: 0.9em;
        }

        .password-toggle-btn:hover {
            color: #008170;
        }

        .loading {
            display: none;
            text-align: center;
            padding: 20px;
        }

        .loading.active {
            display: block;
        }

        .spinner {
            border: 3px solid rgba(0, 129, 112, 0.3);
            border-top: 3px solid #008170;
            border-radius: 50%;
            width: 40px;
            height: 40px;
            animation: spin 1s linear infinite;
            margin: 0 auto;
        }

        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="container">
        <a href="/" class="back-button">
            ← Voltar ao Início
        </a>

        <div class="header">
            <h1>⚙️ Configurações do Sistema</h1>
        </div>

        <div id="alertContainer"></div>

        <!-- Configurações do Sistema -->
        <div class="card">
            <div class="card-title">
                🖥️ Sistema
            </div>

            <div class="form-group">
                <label class="form-label" for="deviceName">Nome do Dispositivo (Hostname)</label>
                <input type="text" id="deviceName" class="form-input" placeholder="esp-gateway" maxlength="31">
                <div class="form-help">Usado para identificação na rede (mDNS) e interface</div>
            </div>

            <div class="form-group">
                <label class="form-label" for="updateInterval">Intervalo de Atualização (ms)</label>
                <input type="number" id="updateInterval" class="form-input" min="500" max="10000" step="100" value="2000">
                <div class="form-help">Tempo entre atualizações automáticas do dashboard</div>
            </div>
        </div>

        <!-- Configurações do Access Point -->
        <div class="card">
            <div class="card-title">
                📡 Access Point (AP)
            </div>

            <div class="info-box">
                <p>O ESP8266 criará uma rede WiFi própria com estas configurações. Outros dispositivos poderão se conectar a ela.</p>
            </div>

            <div class="form-group">
                <label class="form-label" for="apSsid">Nome da Rede (SSID)</label>
                <input type="text" id="apSsid" class="form-input" placeholder="ESP8266-SETUP" maxlength="31">
                <div class="form-help">Nome visível da rede WiFi criada pelo ESP8266</div>
            </div>

            <div class="form-group">
                <label class="form-label" for="apPassword">Senha da Rede</label>
                <div class="password-toggle">
                    <input type="password" id="apPassword" class="form-input" placeholder="Mínimo 8 caracteres" minlength="8" maxlength="63">
                    <button type="button" class="password-toggle-btn" onclick="togglePassword('apPassword')">👁️</button>
                </div>
                <div class="form-help">Senha para conectar ao AP (mínimo 8 caracteres)</div>
            </div>

            <div class="form-group">
                <label class="form-label" for="apMaxConn">Máximo de Conexões</label>
                <input type="number" id="apMaxConn" class="form-input" min="1" max="4" value="4">
                <div class="form-help">Número máximo de dispositivos conectados simultaneamente</div>
            </div>

            <div class="form-group">
                <div class="checkbox-group">
                    <input type="checkbox" id="apHidden" class="checkbox-input">
                    <label class="form-label" for="apHidden" style="margin: 0;">Rede Oculta</label>
                </div>
                <div class="form-help">Se ativado, a rede não aparecerá na lista de redes disponíveis</div>
            </div>
        </div>

        <!-- Configurações de WiFi Cliente -->
        <div class="card">
            <div class="card-title">
                📶 Conexão WiFi (Cliente)
            </div>

            <div class="info-box">
                <p>Conecte o ESP8266 à sua rede WiFi existente. O dispositivo funcionará como cliente e terá acesso à internet se disponível.</p>
            </div>

            <div class="form-group">
                <label class="form-label">Escanear Redes Disponíveis</label>
                <button type="button" class="btn btn-primary" onclick="scanWiFi()" id="scanBtn" style="width: 100%; margin-bottom: 15px;">
                    🔍 Escanear Redes WiFi
                </button>
            </div>

            <div class="form-group" id="networkSelectGroup" style="display: none;">
                <label class="form-label" for="networkSelect">Selecione uma Rede</label>
                <select id="networkSelect" class="form-input" onchange="selectNetwork()">
                    <option value="">-- Selecione --</option>
                </select>
                <div class="form-help">Redes WiFi encontradas próximas</div>
            </div>

            <div class="form-group">
                <label class="form-label" for="wifiSsid">Nome da Rede (SSID)</label>
                <input type="text" id="wifiSsid" class="form-input" placeholder="Deixe vazio para não conectar" maxlength="31">
                <div class="form-help">SSID da rede WiFi à qual deseja conectar</div>
            </div>

            <div class="form-group">
                <label class="form-label" for="wifiPassword">Senha da Rede</label>
                <div class="password-toggle">
                    <input type="password" id="wifiPassword" class="form-input" placeholder="Senha da rede WiFi">
                    <button type="button" class="password-toggle-btn" onclick="togglePassword('wifiPassword')">👁️</button>
                </div>
                <div class="form-help">Senha para conectar à rede WiFi</div>
            </div>
        </div>

        <!-- Botões de Ação -->
        <div class="card">
            <div class="button-group">
                <button class="btn btn-primary" onclick="saveConfig()" id="saveBtn">
                    💾 Salvar Configurações
                </button>
                <button class="btn btn-danger" onclick="resetConfig()">
                    🔄 Restaurar Padrões
                </button>
            </div>
        </div>

        <div class="loading" id="loading">
            <div class="spinner"></div>
            <p style="margin-top: 15px; color: #888;">Salvando configurações...</p>
        </div>
    </div>

    <script>
        let currentConfig = {};
        let scannedNetworks = [];

        window.onload = function() {
            loadConfig();
        };

        function scanWiFi() {
            const scanBtn = document.getElementById('scanBtn');
            const networkSelectGroup = document.getElementById('networkSelectGroup');
            const networkSelect = document.getElementById('networkSelect');
            
            scanBtn.disabled = true;
            scanBtn.textContent = '🔄 Escaneando...';
            
            fetch('/api/wifi/scan')
                .then(response => response.json())
                .then(data => {
                    scannedNetworks = data.networks || [];
                    networkSelect.innerHTML = '<option value="">-- Selecione --</option>';
                    
                    if (scannedNetworks.length === 0) {
                        showAlert('Nenhuma rede WiFi encontrada', 'warning');
                    } else {
                        scannedNetworks.forEach((network, index) => {
                            const option = document.createElement('option');
                            option.value = index;
                            const signal = network.rssi > -60 ? '📶' : network.rssi > -70 ? '📡' : '📉';
                            const security = network.encryption === 'open' ? '🔓' : '🔒';
                            option.textContent = `${signal} ${security} ${network.ssid} (${network.rssi} dBm)`;
                            networkSelect.appendChild(option);
                        });
                        networkSelectGroup.style.display = 'block';
                        showAlert(`✅ ${scannedNetworks.length} rede(s) encontrada(s)`, 'success');
                    }
                    
                    scanBtn.disabled = false;
                    scanBtn.textContent = '🔍 Escanear Redes WiFi';
                })
                .catch(error => {
                    console.error('Erro ao escanear:', error);
                    showAlert('❌ Erro ao escanear redes WiFi', 'error');
                    scanBtn.disabled = false;
                    scanBtn.textContent = '🔍 Escanear Redes WiFi';
                });
        }

        function selectNetwork() {
            const networkSelect = document.getElementById('networkSelect');
            const wifiSsidInput = document.getElementById('wifiSsid');
            const selectedIndex = networkSelect.value;
            
            if (selectedIndex !== '' && scannedNetworks[selectedIndex]) {
                wifiSsidInput.value = scannedNetworks[selectedIndex].ssid;
                wifiSsidInput.focus();
            }
        }

        function loadConfig() {
            showLoading(true);
            fetch('/api/config')
                .then(response => response.json())
                .then(data => {
                    currentConfig = data;
                    
                    // Sistema
                    document.getElementById('deviceName').value = data.system?.name || 'esp-gateway';
                    document.getElementById('updateInterval').value = data.system?.updateInterval || 2000;
                    
                    // Access Point
                    document.getElementById('apSsid').value = data.ap?.ssid || 'ESP8266-SETUP';
                    document.getElementById('apPassword').value = '';
                    document.getElementById('apMaxConn').value = data.ap?.maxConnections || 4;
                    document.getElementById('apHidden').checked = data.ap?.hidden || false;
                    
                    // WiFi Cliente
                    document.getElementById('wifiSsid').value = data.wifi?.ssid || '';
                    document.getElementById('wifiPassword').value = '';
                    
                    showLoading(false);
                })
                .catch(error => {
                    console.error('Erro ao carregar configurações:', error);
                    showAlert('Erro ao carregar configurações', 'error');
                    showLoading(false);
                });
        }

        function saveConfig() {
            const deviceName = document.getElementById('deviceName').value.trim();
            const apSsid = document.getElementById('apSsid').value.trim();
            const apPassword = document.getElementById('apPassword').value;
            const wifiSsid = document.getElementById('wifiSsid').value.trim();

            // Validações
            if (!deviceName) {
                showAlert('Nome do dispositivo é obrigatório', 'error');
                return;
            }

            if (!apSsid) {
                showAlert('Nome da rede AP é obrigatório', 'error');
                return;
            }

            if (apPassword && apPassword.length < 8) {
                showAlert('Senha do AP deve ter no mínimo 8 caracteres', 'error');
                return;
            }

            const config = {
                system: {
                    name: deviceName,
                    updateInterval: parseInt(document.getElementById('updateInterval').value)
                },
                ap: {
                    ssid: apSsid,
                    maxConnections: parseInt(document.getElementById('apMaxConn').value),
                    hidden: document.getElementById('apHidden').checked
                },
                wifi: {
                    ssid: wifiSsid
                }
            };

            // Adicionar senhas apenas se preenchidas
            if (apPassword) {
                config.ap.password = apPassword;
            }

            const wifiPassword = document.getElementById('wifiPassword').value;
            if (wifiPassword) {
                config.wifi.password = wifiPassword;
            }

            showLoading(true);
            document.getElementById('saveBtn').disabled = true;

            fetch('/api/config', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(config)
            })
            .then(response => response.json())
            .then(data => {
                showLoading(false);
                document.getElementById('saveBtn').disabled = false;

                if (data.success) {
                    showAlert('✅ Configurações salvas com sucesso!', 'success');
                    
                    // Recarregar configurações
                    setTimeout(() => {
                        loadConfig();
                    }, 1000);
                } else {
                    showAlert('❌ Erro ao salvar configurações: ' + (data.error || 'desconhecido'), 'error');
                }
            })
            .catch(error => {
                console.error('Erro ao salvar:', error);
                showAlert('❌ Erro de comunicação ao salvar configurações', 'error');
                showLoading(false);
                document.getElementById('saveBtn').disabled = false;
            });
        }

        function resetConfig() {
            if (!confirm('⚠️ Tem certeza que deseja restaurar todas as configurações para os valores padrão?\n\nIsso irá resetar:\n- Nome do dispositivo\n- Configurações de AP\n- Configurações de WiFi')) {
                return;
            }

            showLoading(true);

            fetch('/api/config/reset', {
                method: 'POST'
            })
            .then(response => response.json())
            .then(data => {
                showLoading(false);
                if (data.success) {
                    showAlert('✅ Configurações restauradas! Redirecionando para login...', 'success');
                    setTimeout(() => {
                        window.location.href = '/login';
                    }, 2000);
                } else {
                    showAlert('❌ Erro ao restaurar configurações', 'error');
                }
            })
            .catch(error => {
                console.error('Erro ao resetar:', error);
                showAlert('❌ Erro de comunicação ao resetar configurações', 'error');
                showLoading(false);
            });
        }

        function togglePassword(fieldId) {
            const field = document.getElementById(fieldId);
            field.type = field.type === 'password' ? 'text' : 'password';
        }

        function showAlert(message, type) {
            const container = document.getElementById('alertContainer');
            const alert = document.createElement('div');
            alert.className = `alert alert-${type}`;
            alert.textContent = message;
            
            container.innerHTML = '';
            container.appendChild(alert);
            
            // Auto-remover alertas de sucesso após 5 segundos
            if (type === 'success') {
                setTimeout(() => {
                    alert.remove();
                }, 5000);
            }
        }

        function showLoading(show) {
            const loading = document.getElementById('loading');
            if (show) {
                loading.classList.add('active');
            } else {
                loading.classList.remove('active');
            }
        }
    </script>
</body>
</html>
)=====";

// Página: data/logs.html
const char HTML_LOGS[] PROGMEM = R"=====(<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Logs HTTP - {{DEVICE_NAME}}</title>
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
            max-width: 1400px;
            margin: 0 auto;
        }

        .header {
            text-align: center;
            margin-bottom: 30px;
        }

        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
            color: #008170;
        }

        .header p {
            color: #888;
            font-size: 1.1em;
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

        .filter-label {
            font-weight: 600;
            color: #ccc;
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
            max-height: 700px;
            overflow-y: auto;
            border: 1px solid #005B41;
        }

        .log-entry {
            padding: 20px;
            border-bottom: 1px solid rgba(0, 91, 65, 0.2);
            transition: background 0.2s ease;
        }

        .log-entry:hover {
            background: rgba(0, 129, 112, 0.05);
        }

        .log-entry:last-child {
            border-bottom: none;
        }

        .log-header {
            display: flex;
            align-items: center;
            gap: 15px;
            margin-bottom: 15px;
            flex-wrap: wrap;
        }

        .log-method {
            padding: 6px 12px;
            border-radius: 6px;
            font-size: 0.85em;
            font-weight: 700;
            font-family: 'Courier New', monospace;
            min-width: 70px;
            text-align: center;
        }

        .log-method.GET {
            background: #e3f2fd;
            color: #1976d2;
        }

        .log-method.POST {
            background: #e8f5e9;
            color: #2e7d32;
        }

        .log-method.PUT {
            background: #fff3e0;
            color: #f57c00;
        }

        .log-method.DELETE {
            background: #ffebee;
            color: #c62828;
        }

        .log-method.PATCH {
            background: #f3e5f5;
            color: #7b1fa2;
        }

        .log-uri {
            flex: 1;
            font-family: 'Courier New', monospace;
            color: #008170;
            font-weight: 600;
            font-size: 1.1em;
            word-break: break-all;
        }

        .log-timestamp {
            font-size: 0.85em;
            color: #888;
            white-space: nowrap;
            font-family: 'Courier New', monospace;
        }

        .log-details {
            display: grid;
            gap: 15px;
            margin-top: 15px;
        }

        .log-section {
            background: #0F0F0F;
            padding: 15px;
            border-radius: 8px;
            border-left: 3px solid #005B41;
        }

        .log-section-title {
            font-weight: 600;
            color: #008170;
            margin-bottom: 10px;
            font-size: 0.9em;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        .log-section-content {
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
            color: #ccc;
            line-height: 1.6;
            white-space: pre-wrap;
            word-break: break-all;
        }

        .log-meta {
            display: flex;
            gap: 20px;
            flex-wrap: wrap;
            padding: 10px 0;
        }

        .log-meta-item {
            display: flex;
            align-items: center;
            gap: 8px;
            font-size: 0.9em;
        }

        .log-meta-label {
            color: #888;
            font-weight: 600;
        }

        .log-meta-value {
            color: #fff;
            font-family: 'Courier New', monospace;
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
            body {
                padding: 10px;
            }

            .header h1 {
                font-size: 1.8em;
            }

            .log-header {
                flex-direction: column;
                align-items: flex-start;
            }

            .log-uri {
                font-size: 0.95em;
            }

            .log-meta {
                flex-direction: column;
                gap: 10px;
            }

            .stats {
                grid-template-columns: repeat(2, 1fr);
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <a href="/" class="back-button">
            ← Voltar ao Início
        </a>

        <div class="header">
            <h1>� Monitor de Requisições HTTP</h1>
            <p>Captura em tempo real - Método, URL, Headers e Body</p>
        </div>

        <div class="stats">
            <div class="stat-item">
                <div class="stat-value" id="totalRequests">0</div>
                <div class="stat-label">Total de Requisições</div>
            </div>
            <div class="stat-item">
                <div class="stat-value" id="getCount">0</div>
                <div class="stat-label">GET</div>
            </div>
            <div class="stat-item">
                <div class="stat-value" id="postCount">0</div>
                <div class="stat-label">POST</div>
            </div>
            <div class="stat-item">
                <div class="stat-value" id="otherCount">0</div>
                <div class="stat-label">Outros</div>
            </div>
        </div>

        <div class="controls">
            <div class="auto-refresh">
                <input type="checkbox" id="autoRefresh" checked onchange="toggleAutoRefresh()">
                <label class="filter-label" for="autoRefresh">Auto-atualizar (1s)</label>
            </div>
            <button class="btn btn-primary" onclick="refreshLogs()">🔄 Atualizar</button>
            <button class="btn btn-danger" onclick="clearLogs()">🗑️ Limpar Logs</button>
        </div>

        <div class="logs-container" id="logsContainer">
            <div class="empty-logs">
                <div class="empty-logs-icon">📡</div>
                <p>Aguardando requisições HTTP...</p>
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
                    updateStats(logs);
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

            container.innerHTML = logs.map(log => {
                let headersHtml = '';
                if (log.headers && log.headers.trim()) {
                    headersHtml = `
                        <div class="log-section">
                            <div class="log-section-title">📋 Headers</div>
                            <div class="log-section-content">${escapeHtml(log.headers)}</div>
                        </div>
                    `;
                }

                let bodyHtml = '';
                if (log.body && log.body.trim()) {
                    bodyHtml = `
                        <div class="log-section">
                            <div class="log-section-title">📦 Body (${log.contentLength} bytes)</div>
                            <div class="log-section-content">${escapeHtml(log.body)}</div>
                        </div>
                    `;
                }

                return `
                    <div class="log-entry">
                        <div class="log-header">
                            <div class="log-method ${log.method}">${log.method}</div>
                            <div class="log-uri">${escapeHtml(log.uri)}</div>
                            <div class="log-timestamp">${formatTimestamp(log.timestamp)}</div>
                        </div>
                        
                        <div class="log-meta">
                            <div class="log-meta-item">
                                <span class="log-meta-label">Protocolo:</span>
                                <span class="log-meta-value">${log.protocol}</span>
                            </div>
                            <div class="log-meta-item">
                                <span class="log-meta-label">Cliente IP:</span>
                                <span class="log-meta-value">${log.clientIP}</span>
                            </div>
                            <div class="log-meta-item">
                                <span class="log-meta-label">Content-Length:</span>
                                <span class="log-meta-value">${log.contentLength} bytes</span>
                            </div>
                        </div>

                        <div class="log-details">
                            ${headersHtml}
                            ${bodyHtml}
                        </div>
                    </div>
                `;
            }).join('');
        }

        function updateStats(logs) {
            let getCount = 0;
            let postCount = 0;
            let otherCount = 0;

            logs.forEach(log => {
                if (log.method === 'GET') getCount++;
                else if (log.method === 'POST') postCount++;
                else otherCount++;
            });

            document.getElementById('totalRequests').textContent = logs.length;
            document.getElementById('getCount').textContent = getCount;
            document.getElementById('postCount').textContent = postCount;
            document.getElementById('otherCount').textContent = otherCount;
        }

        function showEmptyState() {
            const container = document.getElementById('logsContainer');
            container.innerHTML = `
                <div class="empty-logs">
                    <div class="empty-logs-icon">📡</div>
                    <p>Aguardando requisições HTTP...</p>
                </div>
            `;
            
            document.getElementById('totalRequests').textContent = '0';
            document.getElementById('getCount').textContent = '0';
            document.getElementById('postCount').textContent = '0';
            document.getElementById('otherCount').textContent = '0';
        }

        function clearLogs() {
            if (confirm('Deseja limpar todos os logs?')) {
                fetch('/api/logs', { method: 'DELETE' })
                    .then(() => {
                        refreshLogs();
                    })
                    .catch(error => {
                        console.error('Erro ao limpar logs:', error);
                        alert('Erro ao limpar logs');
                    });
            }
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
            refreshInterval = setInterval(refreshLogs, 1000); // 1 segundo para tempo real
        }

        function stopAutoRefresh() {
            if (refreshInterval) {
                clearInterval(refreshInterval);
                refreshInterval = null;
            }
        }

        function formatTimestamp(timestamp) {
            const date = new Date(timestamp);
            const now = Date.now();
            const diff = now - timestamp;
            
            if (diff < 60000) {
                return 'há ' + Math.floor(diff / 1000) + 's';
            } else if (diff < 3600000) {
                return 'há ' + Math.floor(diff / 60000) + 'm';
            } else {
                return date.toLocaleTimeString('pt-BR');
            }
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
