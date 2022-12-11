# Sistema Solar

Kaio Vinicius Souza Santos - 11201810069

Mateus Flosi Molero - 11201811546

## Animação

Este é um exemplo clássico do que é possível ser realizado com a biblioteca ABCg e OpenGL, a renderização contém todos os planetas do sistema solar, incluindo o sol, onde já há desenvolvido o movimento de translação, ou seja, os planetas giram em torno do sol.

O usuário ao ter acesso à cena irá conseguir mover a câmera para qualquer ponto da cena, movendo com os botões A,W,S,D, Q, E, R, F, T e G ou as setas do teclado.

O simulador pode ser visto acessando o link a seguir:

https://mateusflosi.github.io/cg-ufabc-2022.3/pages/SystemaSolar/

## Features

* Rotação da câmera sobre o eixo X (R e F);
* Movimento da câmera para cima e para baixo (T e G);
* Velocidade dos planetas está em escala com a realidade;
* Usuário pode setar a velocidade total do sistema;
* Usuário pode escolher se a distância entre os planetas estará ou não em escala;
* Sol como fonte de luz;
* Obj para cada astro;

<img src="/pages/SystemaSolar/Sistema Solar.jpeg" alt="My cool logo"/>

## Possíveis Melhorias

* Adicionar texturização;
* Movimento de rotação dos planetas, isto é, a movimentação em torno do próprio eixo.

## Contribuições

### Kaio 
* Confecção do código inicial da cena, sem abstração das classes;
* Adição dos arquivos de planetas na cena;
* Adição da lógica de rotação dos planetas;
* Adição dos obj para cada astro;

### Mateus
* Ajustes nas configurações da câmera, posição inicial e perspectiva;
* Abstração de todos os planetas, que inicialmente estavam em objetos separados, para a classe Planeta. Passando toda a lógica de renderização referente aos planetas para dentro da classe, inicialmente estava parte no Window e parte nas classes individuais dos planetas;
* Movimentos adicionais da camera;
* Adição de escala ao sistema;
* Configurações do usuário;
* Implementação do sol como fonte de luz;
