# Simulador de colisões elásticas

Este projeto requer a biblioteca **Raylib** para funcionar. Siga as instruções abaixo para instalar as dependências, compilar e executar o projeto.

## Dependências Necessárias

* **Raylib**: Biblioteca principal para renderização e gerenciamento da janela.

## Instalação

### Arch Linux

Para instalar a Raylib no Arch Linux, execute o comando:

```bash
sudo pacman -S raylib
```

## Compilação

Para compilar o código, execute o comando:

```bash
make
```

## Execução

Para executar o simulador, é necessário fornecer os parâmetros de largura, altura e número de partículas:

```bash
./colision [width] [height] [num particles]
```

**Exemplo de uso:**
```bash
./colision 800 600 10
```
