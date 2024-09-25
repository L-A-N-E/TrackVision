<h1 align="center">TrackVision :racing_car:</h1> 

![banner](https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/153787379/132308ff-27a0-45e7-8323-80d9103f2390)

## Índice :page_with_curl:

* [Descrição do Projeto](#descrição-do-projeto-memo)
   * [Introdução](#introdução-1st_place_medal)
   * [TimeTracker](#timetracker-2nd_place_medal)
   * [Futuro do Projeto](#futuro-do-projeto-3rd_place_medal)
* [Acesso ao projeto](#acesso-ao-projeto-file_folder)
* [Ferramentas utilizadas](#ferramentas-utilizadas-hammer_and_wrench)
* [Bibliotecas utilizadas](#bibliotecas-utilizadas-books)
* [Componentes necessários](#componentes-necessários-toolbox)
* [Montagem](#montagem-wrench)
   * [Cuidados durante a montagem](#cuidados-durante-a-montagem-warning)
* [Reprodução](#reprodução-gear)
* [Pessoas Desenvolvedoras do Projeto](#pessoas-desenvolvedoras-do-projeto-globe_with_meridians)


## Descrição do projeto :memo:

<h3>Introdução :1st_place_medal:</h3>
<p>
Nós, da L.A.N.E., desenvolvemos um sistema de cronometragem de corridas utilizando o ESP32, um microcontrolador avançado com conectividade Wi-Fi embutida. O objetivo é criar um dispositivo IoT que captura os tempos de volta em tempo real e envia essas informações para a nuvem, permitindo monitoramento e análise de desempenho.
</p>

<h3>ESP32 :pager:</h3>
<p>
O ESP32 é um microcontrolador que facilita a conexão com a Internet, possibilitando a coleta e o envio de dados de maneira bidirecional. Utilizamos tecnologias como o Fiware para gerenciamento de dados e a plataforma Docker no Microsoft Azure para orquestração e execução de containers. O Fiware fornece uma infraestrutura robusta para construir aplicações inteligentes, enquanto o Docker permite o gerenciamento de ambientes escaláveis. A Microsoft Azure hospeda e escala esses serviços, garantindo alta disponibilidade e performance.
</p>

<p>
Para monitorar e visualizar o fluxo de dados do ESP32, utilizamos o Postman, uma ferramenta que facilita a execução de requisições HTTP e a análise de respostas. Com o Postman, testamos as APIs, verificamos a integração dos dados e garantimos que a comunicação entre o dispositivo e o servidor funcione conforme o esperado.
</p>

## Acesso ao projeto :file_folder:

Você pode acessar o [código do projeto](code/TrackVisionCode.ino) ou a [simulação feita no Wokwi](https://wokwi.com/projects/409286303085618177)

## Ferramentas utilizadas :hammer_and_wrench:

- `Arduino IDE`

## Bibliotecas utilizadas :books:

- ``LiquidCrystal_I2C``
- ``IRremote``
- ``Wire``
- ``RTClib``
- ``WiFi``
- ``PubSubClient``

## Componentes necessários :toolbox:

|   Componente   | Quantidade |
|:--------------:|:----------:|
|      Cabos     |     10     |
|   ESP32  |     1      |
| IR Receiver  |     1      |
| IR Remote  |     1      |
| LCD 16x2 com módulo I2C |     1      |
| Real Time Clock - RTC |     1     |
|    Cabo USB    |     1     |