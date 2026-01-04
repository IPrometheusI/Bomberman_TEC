# Bomberman_TEC

Author: @MaickolFernandez | @JordiPerez

Description: This project determines how good we are at coding in C, we are going to do that by programming a cool game called Bomberman.

Date: 11/02/2024

Course: Digital Systems Design

## Instalacion

1. Instala gcc, make y SDL2 (ver `requirements.txt`).
   - Script automatico: `bash scripts/install_deps.sh`
   - Windows: usa MSYS2 (MinGW64) o WSL y ejecuta el script desde ahi.
   - macOS (Homebrew): `brew install sdl2`
   - Ubuntu/Debian: `sudo apt install libsdl2-dev`
   - Fedora: `sudo dnf install SDL2-devel`
   - Arch: `sudo pacman -S sdl2`
2. Verifica que `sdl2-config` este en tu PATH.
3. Desde la raiz del repo ejecuta: `make`

## Uso

- `make`: limpia, compila y ejecuta el juego.
- `make Build`: solo compila.
- `make build_run`: clean + build + run.
- `make clean`: elimina `salida` y archivos temporales.
- Ejecutable: `./salida` (desde la raiz; los assets se cargan desde `assets/`).
- Pantalla completa: `./salida -f` (opcional).

## Controles de juego

- Flechas: mover al jugador.
- `1`/`2`/`3`/`4`: colocar bombas (segun powerups disponibles).
- `Espacio`: iniciar desde el menu y volver al menu tras Game Over.
- `Esc`: salir.

## Estructura del proyecto

- `source/BBMAN.c`: logica principal del juego (SDL2).
- `assets/`: sprites y fondos `.bmp`.
- `Makefile`: tareas de build/limpieza/ejecucion.
- `salida`: binario generado por `make`.
- `requirements.txt`: lista referencial de dependencias del sistema.
- `README.md`: esta guia.
