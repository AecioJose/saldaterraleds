# Sal da Terra LEDs <img alt="Logo Sal da terra" height="50px"  src="https://saldaterrapirapitinga.com.br/wp-content/uploads/2023/10/WhatsApp-Image-2023-10-24-at-18.36.44-150x150.jpeg"/>
Projeto de Iluminação com fita de LED usado na Igreja **Sal Da Terra Pirapitinga** de Ituiutaba <a href="https://www.instagram.com/saldaterrapirapitinga/">@saldaterrapirapitinga</a><br>
📍Endereço: Av. Jandiro Vilela de Freitas 3926 | Bairro Pirapitinga
<br><br>

## Possíveis Problemas⚠️ (LEIA AQUI SE ALGO NÃO ESTIVER FUNCIONANDO)
### 1° - Se os leds não mudam de cor ou você não consegue acessar o site 192.168.0.107
1. Certifique-se de que tem internet na rede 2.4G
2. Tente primeiramente desligar(por pelo menos 1 min) e ligar o servidor(aquela caixa preta com um botão on/off, que fica na parede em cima da mesa branca).
3. Caso não volte, terá que desligar o disjuntor de iluminação no quadro de distribuição(para os LEDS do teto); Leds do palco e da Lateral tem um botão de ligar e desligar neles, em uma caixinha preta(É Necessário que a internet esteje ligada antes dos leds e do servidor, se você ligar primeiro os leds e depois o roteador, não irão funcionar a mudança de cor, ele vai ficar na ultima cor escolhida anteriormente. Por isso o roteador deve está ligado e com internet antes de você ligar os leds).

### 2° - Se a SENHA DO WIFI foi alterada
1. Certifique-se de que tudo está ligado(todos os controladores, a caixa de servidores, e a rede wifi).
2. Cada controlador e os servidores ao não se conectarem a uma rede já cadastrada ele gera uma rede própria, você deve se conectar a elas uma por uma:
   * SSID: LEDs Broker Server - senha: comunica2024IOT
   * SSID: LEDs Pagina WEB - senha: comunica2024IOT
   * SSID: LEDs TETO - senha: comunica2024IOT
   * SSID: LEDs PALCO - senha: comunica2024IOT
   * SSID: não implementado ainda - senha: comunica2024IOT -> espaço reservado a implementação futura dos leds da lateral
   * SSID: não implementado ainda - senha: comunica2024IOT -> espaço reservado a implementação futura da controladora de cores das fitas
3. Ao se conectar você entrará na página web da esp(em alguns celulares abre a pagina logo ao se conectar, mas você consegue ver o IP dela nas configurações da conexão no seu celular e acessar a página via web, é bem parecido quando você quer acessar aquela página de configurações de um roteador), e nessa pagina vai ter as opções para configurar uma nova rede wifi, basta buscar as redes, selecionar a rede wifi que você quer se conectar e colocar a senha, depois ele vai se desconectar automaticamente do seu celular e conectar a rede que você configurou.
4. Vídeo de ajuda <a href="https://youtu.be/KnED-ZTHh3U?t=347">AQUI</a> caso tenha alguma dificuldade.
5. LEMBRE-SE, todos os controladores e servidores devem estar na mesma rede wifi, além do servidor broker e o servidor web com IP fixo definido no roteador.
6. Se você configurou a rede errada, tem que desligar o wifi que a esp se conectou e conectar na rede certa com o wifi antigo/errado desligado e o novo/correto ligado.


### 3° - Se forem trocados os roteadores
- Primeiro conecte todos os dispositivos na mesma rede como citado no tópico acima.
- Depois você precisa de definir IP fixo OBRIGATORIAMENTE para o servidor broker e o servidor web nos seguintes IPs.
- Servidor Broker MAC: IP: 192.168.0.110
- Servidor Web MAC: IP: 192.168.0.107 -> aqui não é obrigatório esse IP, porém se alterar, terá que avisar a todos o novo IP para acessar os controles das cores.
- Caso no futuro a igreja cresça e tenha muitos dispositivos conectados a uma rede e a rede interna deixe de ser tipo C, utilize um roteador dedicado para as esps, não fiz testes, mas acredito que nem precise de internet, apenas um roteador, mas para mudar as cores terá que se conectar a esse roteador.

<br>

## Como utilizar o sistema? ⚙️
1. Para acessar o controle de cores dos LEDs você precisa estar conectado a rede da comunicação.
2. Depois acesse pelo navegador o endereço 192.168.0.107
3. Ao acessar ele mostrará a seguinte pagina como na imagem abaixo, na imagem abaixo tem 3 capturas de tela explicando o que cada função faz.
![capturas de tela de um celular com explicações o que cada botão faz](https://github.com/AecioJose/saldaterraleds/blob/main/sources/LEDs%20frontend%20explicado.png?raw=true)
4. Existe uma página chamada "TODOS os LEDs", nela a cor que você escolher irá para todas as fitas de LED do sistema.

<br>

## Como funciona❓
- Este projeto utiliza esp32 para controlar fitas de led <br>
- Nas esps que ficam diretamente ligadas as fitas de led a qual são as do teto e as do palco elas tem conectado a elas 3 Módulo Mosfet PWM de Alta Potência 30A 400w 5v a 36v DC, uma fonte 12V para alimentar a fita e uma 5v para alimentar a esp.<br>
- Tem um servidor WEB que serve uma página e nessa pagina você manda requisições para esse servidor, e esse servidor envia para outro servidor chamado broker que utiliza um protocolo de comunicação chamado mqtt<br>
- O Servidor Broker, gerencia as comunicações entre as controladoras, o servidor web envia para o broker e o broker envia a mensagem para esp certa<br>
- Ao total são 4 Esp32, 1 broker server, 1 web server, 1 controlador leds do teto, 1 controlador leds palco<br>
<br>
Detalhes e explicação do código e ligação do hardware no <a href="https://github.com/AecioJose/saldaterraleds/wiki/Funcionamento-Detalhado-do-software-e-hardware-desse-projeto">WikiGitHub</a>

<br>

## Aprendizados 💡
Neste projeto a principal habilidade que acredito ter se desenvolvido, foi a de aprender do zero em 5 dias e por em prática, além de ter lidado muito bem com a pressão de entregar no prazo, eu aprendi sobre:
- Formas de comunicar as esps via wifi. (A mais viável que encontrei foi através do protocolo mqtt, e depois de diversos testes, porque mqtt server é pago, descobri que dava para fazer meu próprio servidor mqtt("fazer" no caso apenas utilizei a biblioteca com o código base padrão)).
- Como fazer um servidor web com o esp32.
- A pagina web eu já tinha experiencia com front-end, porém tive dificuldade para fazer a roda de cores de selecionar as cores, para isso tive a ajuda de um amigo chamado Icaro, que me ajudou demais pesquisando e pensando em formas de fazer essa roda de cores e sem ele essa roda de cores não seria mesma, estaria até hoje na versão inicial que era muito ruim!
- Fitas de led funcionam controlando o terminal negativo e não o positivo(eu passei muita raiva com as fitas de led quando tentei conectar o negativo e diminuir a corrente pelo positivo, no caso o correto é ligar o positivo em paralelo na fita de led e os pinos das cores é o negativo, tem explicado melhor no wiki do projeto <a href="https://github.com/AecioJose/saldaterraleds/wiki/Funcionamento-Detalhado-do-software-e-hardware-desse-projeto">WikiGitHub</a>)


<br>

## Agradecimentos especiais 🙏
A <a href="https://www.instagram.com/saldaterrapirapitinga/">@saldaterrapirapitinga</a> pela oportunidade de desenvolver esse projeto.<br>
Ao <a href="https://github.com/IcaroRubem">@icaroRubem</a>, por ter me ajudado no front-end com dicas e conselhos para implementação do círculo de selecionar a cor, sem ele acredito que o código do servidor Web não sairia da versão beta

<br>

## Relato
Este projeto foi realizado em diversas partes, a 1° parte foi realizada no final do mês de setembro de 2023.<br>
Na primeira parte foram instalados os leds do teto e os do palco, com a implementação IOT apenas nos leds do teto.<br>
Ao decorrer dos meses foram implementados o sistema IOT para os outros LEDs, ao final de outubro de 2023 foi implementado os LEDs do palco, e em março de 2024 foram implementados os LEDs das laterais.<br>

### Antes
![Imagem da igreja durante o processo de mudança](https://github.com/AecioJose/saldaterraleds/blob/main/sources/SDT%20antes.png?raw=true)
<br>

### Durante

![Imagem da igreja durante o processo de mudança](https://github.com/AecioJose/saldaterraleds/blob/main/sources/SDT%20DURANTE.png?raw=true)
<br>

### Depois
![Imagem da igreja durante o processo de mudança](https://github.com/AecioJose/saldaterraleds/blob/main/sources/SDT%20depois.png?raw=true)
<br>

### Obrigado por acompanhar este projeto!
Estou aberto a, sugestões de melhorias ou reporte de bugs.<br>
Minhas informações de contato estão no meu perfil aqui do <a href="https://www.github.com/AecioJose">GitHub</a> 🙃
