# Tetris Stack — Nível Mestre (C)

Projeto acadêmico: implementação de estruturas de dados (fila circular + pilha) simulando a mecânica de peças futuras e reserva do jogo "Tetris Stack".

---

## Visão Geral

Este projeto implementa a lógica de gerenciamento de peças do jogo Tetris Stack utilizando:
- **Fila circular** (tamanho 5) para as peças futuras.
- **Pilha** (tamanho 3) para reservas.
- **Histórico de jogadas** para desfazer ações.
- Operações: jogar, reservar, usar reservada, trocar topo/ frente, desfazer, inverter estrutura.

Linguagem: **C**  
Arquivo principal: `tetris_stack_mestre.c`.

---

## Requisitos

- GCC (ou outro compilador C compatível)
- Sistema: Linux / macOS / Windows (com MinGW ou MSYS2)

---

## Compilação

No Linux/macOS / WSL / Git Bash:

```bash
gcc tetris_stack_mestre.c -o tetris_stack
