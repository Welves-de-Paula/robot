#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os

# Diretório contendo os arquivos HTML
data_dir = "data"
output_file = "html_pages.h"

# Mapeamento de arquivos HTML para nomes de variáveis
html_files = {
    "login.html": "HTML_LOGIN",
    "index.html": "HTML_INDEX",
    "dashboard.html": "HTML_DASHBOARD",
    "config.html": "HTML_CONFIG",
    "logs.html": "HTML_LOGS",
}


def generate_header():
    with open(output_file, "w", encoding="utf-8") as out:
        # Cabeçalho
        out.write("#ifndef HTML_PAGES_H\n")
        out.write("#define HTML_PAGES_H\n\n")
        out.write("#include <Arduino.h>\n\n")

        # Processar cada arquivo HTML
        for html_file, var_name in html_files.items():
            html_path = os.path.join(data_dir, html_file)

            if not os.path.exists(html_path):
                print(f"Aviso: {html_path} não encontrado, pulando...")
                continue

            print(f"Processando {html_file}...")

            # Ler o arquivo HTML
            with open(html_path, "r", encoding="utf-8") as f:
                html_content = f.read()

            # Escrever no header
            out.write(f"// Página: {data_dir}/{html_file}\n")
            out.write(f'const char {var_name}[] PROGMEM = R"=====(')
            out.write(html_content)
            out.write(')=====";\n\n')

        # Rodapé
        out.write("#endif\n")

    print(f"\n✓ Arquivo {output_file} gerado com sucesso!")


if __name__ == "__main__":
    generate_header()
