# 🚗 Sistema de Gestão de Portagens

Sistema integrado para **gestão de portagens rodoviárias** desenvolvido em **C**, permitindo o controlo de veículos, donos, passagens e análise de dados estatísticos/rankings através de estruturas de dados adequadas.  

Aplicação de consola desenvolvida no âmbito da unidade curricular **Estrutura de Dados**, da **Licenciatura em Engenharia Informática – ESTGV**, no **2º semestre do ano letivo 2024/2025**.  

📌 **Nota final obtida:** 19,63 valores  

---

## ✨ Funcionalidades Principais

### 1️⃣ Gestão de Donos
- Registar novos proprietários  
- Listar donos por ordem alfabética  
- Listar donos por NIF  

### 2️⃣ Gestão de Veículos
- Registar novos veículos  
- Alterar proprietário  
- Listar veículos por:
  - Matrícula  
  - Marca  
  - Modelo  
- Consultar veículos por período de circulação  

### 3️⃣ Passagens / Viagens
- Registar passagens em portagens  
- Consultar histórico de viagens  

### 4️⃣ Estatísticas e Rankings
- Ranking de KMs por período  
- Ranking de KMs por marca  
- Ranking de infrações  
- Velocidades médias:
  - Por condutor  
  - Por marca  
  - Por código postal  

### 5️⃣ Sistema de Infrações
- Consulta de veículos com infrações  
- Análise de infrações por período  

### 6️⃣ Análise de Dados
- Monitorização da memória utilizada em tempo real  
- Estatísticas detalhadas de marcas de veículos  

### 7️⃣ Exportação de Dados
- Formatos suportados:
  - **CSV**  
  - **XML**  
  - **HTML**  
  - **TXT** (apenas para listagens)  

### 8️⃣ Configurações
- Autosave configurável  
- Personalização de listagens  
- Gestão de ficheiros de dados  
- Backup e restauro de dados  

---

## ⚙️ Requisitos Técnicos 
- **Sistemas operativos:** Windows / Unix  
- **Compilador:** GCC compatível (recomendado C23)  
- **Codificação:** UTF-8 (para caracteres especiais)  
- **Armazenamento:** ≥ 300 MB (1 GB recomendado)  
- **Memória RAM:** ≥ 1 GB (8–16 GB recomendado para funcionamento fluido)  

---

## 🛠️ Compilação

### 💻 Em Windows
  ```bash
  gcc -Wall -Wextra -g -O0 -std=c23 -o FILENAME main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c
  ```

  ✅ **Testado em:**
  - Windows 11 Home 23H2 (64 bits)  
  - GCC em modo **C23**  

  🖥️ **Especificações da máquina de teste:**
  - 16 GB RAM DDR4 (3200MHz)  
  - Processador Intel i5-12450H 2.00GHz, 12MB Cache  
  - SSD 512 GB  

### 🐧 Em Linux
  ```bash
  gcc -std=c2x -Wall -Wextra -o FILENAME main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c -D_XOPEN_SOURCE=700
  ```

✅ **Testado em:**
  - Ubuntu 20.04.6 LTS
  - GCC versão 13.1.0 (C23)  

  🖥️ **Especificações da VM de teste:**
  - 8 GB RAM  
  - 7 CPUs (VirtualBox)  

---

## 👥 Autores

- **Hugo Afonso** (NMec 30032)  
- **Mateus Silva** (NMec 29989)  
- **Guilherme Pais** (NMec 30007)  

---

## 🙏 Agradecimentos
Gostaríamos de expressar os nossos agradecimentos ao **professor Carlos Simões** e, em especial, ao **Francisco Morgado**, pelo apoio prestado durante a realização deste projeto.  
