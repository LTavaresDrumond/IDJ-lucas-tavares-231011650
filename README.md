# Introdução ao Desenvolvimento de Jogos (IDJ) - Trabalho 1

**Aluno:** Lucas Tavares Drumond  
**Matrícula:** 231011650  
**Turma:** 2025/2  
**Status:** Fase Um Concluída ✅

## Descrição do Projeto

Este repositório contém o código-fonte da primeira fase da engine construída na disciplina de IDJ (Introdução ao Desenvolvimento de Jogos).

A Fase Um consiste na configuração inicial do repositório, implementação da estrutura de compilação cruzada (Windows/Linux via Makefile) e na implementação das quatro classes fundamentais baseadas na biblioteca SDL2:

- `Game`: Engine principal gerenciada através do padrão de projeto Singleton (inclui Window e Renderer).
- `State`: A estrutura responsável por gerenciar a lógica do jogo (nesta etapa: carregar a imagem de fundo e colocar a música para tocar).
- `Sprite`: Classe responsável pelo gerenciamento e exibição de texturas usando a extensão `SDL_image`.
- `Music`: Classe responsável pelo controle e execução de áudios BGM (Background Music) usando a extensão `SDL_mixer`.

## Como Executar

### 1. Windows (Via MSYS2/MinGW)

**Pré-requisitos:**
1. Tenha o `g++` e `make` instalados e configurados na variável de ambiente do seu Windows (ex: adicionando `C:\msys64\ucrt64\bin` ao `Path`).
2. Tenha a pasta `C:\SDL2` configurada contendo as bibliotecas de desenvolvimento do MinGW para a SDL2, com as pastas `include`, `lib` e `bin`.
3. Certifique-se de que os arquivos `.dll` do SDL2 estejam na mesma pasta raiz do `JOGO.exe`. *(Elas não estão no Github para manter o repositório leve)*.

**Execução:**
1. Abra o terminal na pasta raiz do projeto.
2. Compile o código com o comando:
   ```bash
   mingw32-make
   ```
3. Execute o jogo:
   ```bash
   .\JOGO.exe
   ```

### 2. Linux (Ex: GitHub Codespaces)

**Pré-requisitos:**
Instalar as bibliotecas da SDL2 via gerenciador de pacotes da sua distribuição. No Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install -y build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

**Execução:**
1. Abra o terminal na pasta raiz.
2. Compile executando:
   ```bash
   make
   ```
3. Rode o executável:
   ```bash
   ./JOGO
   ```

> **Aviso para rodar no Codespaces Headless:**
> Se o servidor não tiver interface gráfica instalada, rodar `./JOGO` diretamente irá falhar pois não há "Display" para abrir a janela. Caso queira apenas testar se não há erros na inicialização lógica do binário, rode usando variáveis de ambiente de Dummy Devices:
> `SDL_AUDIODRIVER=dummy SDL_VIDEODRIVER=dummy ./JOGO`
> *(Aperte `Ctrl+C` para encerrar já que nenhuma janela irá aparecer).*
