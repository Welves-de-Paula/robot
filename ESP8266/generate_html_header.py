#!/usr/bin/env python3
"""
Script para gerar o arquivo html_pages.h com todas as páginas HTML
Lê os arquivos HTML da pasta data e gera strings PROGMEM
"""

import os

# Arquivos HTML para processar
html_files = {
    "HTML_LOGIN": "data/login.html",
    "HTML_INDEX": "data/index.html",
    "HTML_DASHBOARD": "data/dashboard.html",
    "HTML_CONFIG": "data/config.html",
    "HTML_LOGS": "data/logs.html",
}


def read_html_file(filepath):
    """Lê arquivo HTML e retorna conteúdo"""
    with open(filepath, "r", encoding="utf-8") as f:
        return f.read()


def generate_header():
    """Gera o arquivo html_pages.h"""
    output = []
    output.append("#ifndef HTML_PAGES_H")
    output.append("#define HTML_PAGES_H")
    output.append("")
    output.append("#include <Arduino.h>")
    output.append("")

    for var_name, filepath in html_files.items():
        if os.path.exists(filepath):
            print(f"Processando {filepath}...")
            html_content = read_html_file(filepath)

            # Adiciona comentário
            output.append(f"// Página: {filepath}")
            output.append(f'const char {var_name}[] PROGMEM = R"=====(')
            output.append(html_content)
            output.append(')=====";')
            output.append("")
        else:
            print(f"AVISO: {filepath} não encontrado!")

    output.append("#endif")

    # Escreve arquivo
    with open("html_pages.h", "w", encoding="utf-8") as f:
        f.write("\n".join(output))

    print("\nArquivo html_pages.h gerado com sucesso!")
    print(f"Total de páginas: {len(html_files)}")


if __name__ == "__main__":
    generate_header()
