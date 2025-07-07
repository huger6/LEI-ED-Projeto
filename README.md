# Sistema de Gestão de Portagens

Sistema integrado para gestão de portagens rodoviárias desenvolvido em C, permitindo o controlo de veículos, donos, passagens e análise de dados estatísticos e rankings com as estruturas de dados devidas.

Aplicação de consola desenvolvida no âmbito da cadeira "Estrutura de Dados" da licenciatura de Engenharia Informática da ESTGV no 2º semestre do ano letivo 2024-2025

## Funcionalidades Principais

### 1. Gestão de Donos
- Registar novos proprietários
- Listar donos por ordem alfabética
- Listar donos por NIF

### 2. Gestão de Veículos
- Registar novos veículos
- Mudar proprietário
- Listar veículos por:
  - Matrícula
  - Marca
  - Modelo
- Consultar veículos por período de circulação

### 3. Passagens/Viagens
- Registar passagens em portagens
- Consultar histórico de viagens

### 4. Estatísticas e Rankings
- Ranking de KMs por período
- Ranking de KMs por marca
- Ranking de infrações
- Velocidades médias:
  - Por condutor
  - Por marca
  - Por código postal

### 5. Sistema de Infrações
- Consulta de veículos com infrações
- Análise de infrações por período

### 6. Análise de Dados
- Monitorização da memória utilizada em tempo real
- Outras estatísticas de marcas de veículos

### 7. Exportação de Dados
- Formato CSV
- Formato XML  
- Formato HTML
- Formato TXT (apenas em listagens)

### 8. Configurações
- Autosave configurável
- Personalização de listagens
- Gestão de ficheiros de dados
- Backup e restauro de dados

## Requisitos Técnicos
- Sistema operativo: Windows/Unix
- Compilador C compatível (Recomendado GCC, usando o C23)
- Suporte a UTF-8 para caracteres especiais
- 300MB de espaço mínimo em disco (1GB, pelo menos, recomendado)
- 1GB RAM no mínimo (Recomenado 8 ou 16 GB para um funcionamento fluido)

## Compilação

### Em Windows
- Compilar com: gcc -Wall -Wextra -g -O0 -std=c23 -o **filename** main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c

- Testado em ambiente Windows 11 Home 23H2 (64 bits) com o compilador GCC em C23
- Especificações do computador utilizado:
    - 16GB RAM DDR4 (3200MHz)
    - Processador Intel i5-12450H 2.00GHz 12MB Cache
    - SSD 512GB

### Em Linux
- Compilar com: gcc -std=c2x -Wall -Wextra -o **FILENAME** main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c -D_XOPEN_SOURCE=700

- Testado em ambiente Linux Ubuntu 20.04.6 LTS (Garantir que estamos a usar gcc13 (C23) - Testado na versão 13.1.0)
- Especificações do computador (VM):
    - 8GB RAM
    - 7 CPU's no VirtualBox


## Autores

- Hugo Afonso (Número mecanográfico 30032)
- Mateus Silva (Número mecanográfico 29989)
- Guilherme Pais (Número mecanográfico 30007)

- Todas as versões do programa estão disponíveis no seguinte repositório do github: https://github.com/huger6/ProjetoED
- Notar que o repositório apenas estará público durante o período de entrega do projeto

## Agradecimentos

- Gostávamos de expressar os nossos agradecimentos ao professor Carlos Simões, e especialmente ao Francisco Morgado, por toda a ajuda durante a realização deste projeto.

*Nota: 19,63*