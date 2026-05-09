# Triângulos por Clique — OpenGL

Aplicação interativa em C++ com OpenGL que permite desenhar triângulos coloridos na tela clicando com o mouse.

## Como funciona

Clique em **3 pontos** na janela para formar um triângulo. Cada triângulo recebe uma cor aleatória. Repita quantas vezes quiser.

## Dependências

- [GLEW](http://glew.sourceforge.net/)
- [GLFW](https://www.glfw.org/)
- [GLM](https://github.com/g-truc/glm)
- OpenGL 3.3+


```bash
## Controles

| Ação | Resultado |
|------|-----------|
| Clique esquerdo | Adiciona um vértice |
| 3 cliques | Desenha um triângulo com cor aleatória |
| `Esc` | Fecha a janela |

## Estrutura do código

- **`mouseCallback`** — captura cliques e converte coordenadas de tela para NDC
- **`createVAO`** — envia os vértices de um triângulo para a GPU
- **`main`** — loop principal: cria VAOs, desenha triângulos e processa eventos