# Sistema Solar

Kaio Vinicius Souza Santos - 11201810069

Mateus Flosi Molero - 11201811546

## Animação

Este é um exemplo clássico do que é possível ser realizado com a biblioteca ABCg e OpenGL, a renderização contém todos os planetas do sistema solar, incluindo o sol, onde já há desenvolvido o movimento de translação, ou seja, os planetas giram em torno do sol.

O usuário ao ter acesso à cena irá conseguir mover a câmera para qualquer ponto da cena, movendo com os botões A,W,S,D, Q e E ou as setas do teclado.

O simulador pode ser visto acessando o link a seguir:

https://mateusflosi.github.io/cg-ufabc-2022.3/pages/SystemaSolar/

## Implementação

A animação teve como base o projeto de "LookAt" visto em aula, como, por exemplo, a organização e arquitetura do projeto. Uma imagem da animação pode ser visualizada a seguir:

<img src="/pages/SystemaSolar/Sistema Solar.jpeg" alt="My cool logo"/>

A animação apresenta, basicamente, os seguintes componentes: Planet e Camera.

### Planet

Cada astro presente no nosso sistema solar, vai ser representado por um objeto da classe planeta.
A classe planet é responsável por instanciar o tipo de planeta que queremos, esta classe possui as seguintes propriedades:
* m_translate : Propriedade responsável por estabelecer a posição no espaço na qual o planeta estará, essa propriedade é atualizada a cada update realizado pelo programa com a nova posição do planeta. 
* m_scale : Propriedade responsável por estabelecer o tamanho do planeta, com ela é possível estabelecer uma escala real entre os tamanhos dos planetas instanciados.
* m_color : Propriedade que determina a cor de cada planeta (posteriormente serão aplicadas texturas nos planetas).
* m_velocity : Propriedade que irá determinar a velocidade de órbita dos planetas, possuindo um valor pré-determinado para cada planeta instanciado.

### Camera

Este é o componente responsável por capturar a cena que estamos montando, ele consegue se movimentar livremente pelos eixos X e Y. Suas propriedades são:
* m_eye : Propriedade responsável por simular o olhar de um observador da cena.
* m_at : Propriedade responsável por direcionar através de um vetor a direção que o observador está olhando.
* m_up : Propriedade responsável por elevar o observador para uma unidade de medida acima do plano para ter uma visibilidade melhor da cena.

## Implementações Futuras:

* Adicionar texturização e iluminação no projeto;
* Adicionar objetos específicos para cada planeta;
* Adicionar escala ao sistema, tanto em relação ao tamanho, como também a distância entre planetas e também velocidade de translação;
* Movimento de rotação dos planetas, isto é, a movimentação em torno do próprio eixo.

## Contribuições

### Kaio 
* Confecção do código inicial da cena, sem abstração das classes;
* Adição dos arquivos de planetas na cena;
* Adição da lógica de rotação dos planetas.

### Mateus
* Ajustes nas configurações da câmera, posição inicial e perspectiva;
* Abstração de todos os planetas, que inicialmente estavam em objetos separados, para a classe Planeta. Passando toda a lógica de renderização referente aos planetas para dentro da classe, inicialmente estava parte no Window e parte nas classes individuais dos planetas.
