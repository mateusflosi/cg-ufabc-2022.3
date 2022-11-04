# Breakout

Mateus Flosi Molero - 11201811546

## Regras do jogo

Um clássico do Atari que foi lançado em 1976, que mesmo passados quase 50 anos do seu lançamento continua diverdindo pessoas de todas as idades, seja aquelas que jogaram o jogo no próprio Atari, quanto aquelas que nem sequer tiveram o console.

As ações do jogador são muito simples, ele precisa mexer a barra que fica na parte inferior das telas para esquerda/direita para rebater a bola que se movimenta pela tela de jogo. A bola, por sua vez, destroem os blocos, que ficam na parte intermediária da tela, quando entram em contato com os mesmos.

Dessa forma, a missão do jogador é muito simples, destruir todos os blocos da tela e não permitir que a bola atinja a parede inferior.

O jogo pode ser visualizado através do link abaixo, contudo estranhamente os objetos do jogador e os blocos ficaram transparentes na sua versão Web, mas é possível reparar que há interações. Alguns ajustes foram feitos na expectativa de corrigir esse bug, contudo nenhuma teve efeito positivo.

https://mateusflosi.github.io/cg-ufabc-2022.3/pages/breakout/

Por conta desse motivo, um vídeo no youtube está disponibilizado no link abaixo, para que o seu funcionamento esperado possa ser visto de forma mais prática, sem a necessidade de buildar e rodar o projeto:

https://youtu.be/NhFhkjGo7aM

## Implementação

O jogo teve como base o projeto de "Asteroids" visto em aula, como, por exemplo, a organização e arquitetura do projeto. Uma imagem do jogo pode ser visualizado a seguir:

<img src="/pages/breakout/Breakout.png" alt="My cool logo"/>

O jogo apresenta, basicamente, 3 tipos de componentes: Player, Block e Ball.

### Player

Esse é o único objeto que o jogador consegue interagir, sendo deslocado para esquerda ou direita dependendo da tecla que está apertada pelo usuário.

Ele foi constrúido com 4 pontos e 2 triângulos, o que formou um retangulo.

### Block

Os blocos podem ser considerados os "vilões" do jogo, uma vez que o jogador precisa destroir todos eles se quiser sair vitorioso.

A largura dos blocos são gerados aleatoriamente entre 0.20f e 0.25f, com excessão do último, cujo tamanho é calculado para que as fileiras fiquem alinhadas.

Podemos dividir os blocos em 3 níveis, o mais baixo (verde), o intermediário (laranja) e o mais alto (vermelho).

### Ball

Esse é, talvez, o objeto mais importante do jogo. A bola interage com todos os objetos da tela, o que incluem os próprios blocos e o player, como também com todos os limites da tela.

A bola muda de direção sempre que entra em contato com algum objeto do jogo, quer dizer, quase todos, uma vez que o jogador perde quando permite que a bola entre em contato com a parede inferior.

Ela troca o sentido em x ao entrar em contato com as laterais dos blocos, como também ao colidir com paredes da esquerda e da direita. Consequentemente, troca o sentido em y ao entrar em contato com a parede superior e também com a parte de cima e de baixo dos blocos.

Quando a bola entra em contato com os níveis mais altos, laranja e vermelho, pela primeira vez, sua velocidade é aumentada.

Demais comentários e explicações da implmenetação podem ser encontrados no próprio código.
