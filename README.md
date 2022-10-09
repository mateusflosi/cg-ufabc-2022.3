# Campo Minado

Mateus Flosi Molero - 11201811546

### Regras do jogo

Um jogo muito conhecido que possui regras simples, mas mesmo assim pode ser muito diverdido.

O jogador precisa identificar quais quadrados estão com bombas. No começo do jogo ele precisa pressionar um quadrado no escuro, mas depois disso os quadrados irão revelar quantas bombas tem ao seu redor.

Quando o jogador se sentir confiante ele pode marcar um quadrado como bomba, para isso basta pressionar o botão no topo do tabuleiro.

Quando o jogador pressiona um quadrado com bomba ele perde o jogo, e, consequentemente, quando ele consegue revelar todos os quadrados, menos as bombas, ele vence o jogo.

O jogo pode ser acessado no link abaixo:

https://mateusflosi.github.io/cg-ufabc-2022.3/pages/campominado/

### Implementação

O jogo foi feito com base no exemplo "TicTacToe" visto em aula, com um Menu que pode reiniciar o jogo no topo da aplicação, um "Game Board" e um botão de "Restart" na parte inferior da tela.

Mas, em questão de layout apresenta logo em cima do "Game Board" o número de bombas que ainda não foram identificadas e o botão de "Flag" que pode ser usado pelo jogador quando ele quiser marcar um quadrado como bomba. Além disso, quando o jogador ganha, ou perde, uma nova janela é exibida no meio da aplicação.

O jogo começa preenchendo o tabuleiro com caracteres vazios e depois seleciona 8 quadrados, aleatoriamente, para ser as casas com bombas.

<img src="/pages/campominado/CampoMinado1.png" alt="My cool logo"/>

Depois disso o jogador precisa ir pressionando os quadrados do "Gama Board" para revelar quantas bombas tem ao redor da casa pressionada. Quando o quadrado pressionado não apresentads bombas ao seu redor, o jogo, automaticamente, revela o número de bombas dos quadrados ao redor, de forma recursiva, até que seja encontrando quadrados com bombas próximas. Esse movimento é para o jogo não ficar repetitivo e chato em seu inicio. 

<img src="/pages/campominado/CampoMinado2.png" alt="My cool logo"/>

Quando o jogador está confiante que sabe onde estão as bombas ele pode flega-lo. O quadrado flegado quando pressionado não executa nada. Todos os quadrados podem ser flegados e desflegados em qualquer momento do jogo, contudo só pode haver 8 quadrados flegados simultaneamente.

Caso o jogador perca o jogo, todas as bombas são reveladas, exceto aqueles que já haviam sido identifcadas. Além disso, caso o jogador tenha flegado uma quadrado equivocadamente também é dado um feedback.

Demais comentários e explicações da implmenetação podem ser encontrados no próprio código.
