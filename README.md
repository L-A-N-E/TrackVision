<h1 align="center">TrackVision :racing_car:</h1> 

![banner](https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/153787379/132308ff-27a0-45e7-8323-80d9103f2390)

## Índice :page_with_curl:

* [Descrição do Projeto](#descrição-do-projeto-memo)
   * [Introdução](#introdução-1st_place_medal)
   * [ESP32](#ESP32-pager)
* [Objetivos do Projeto](#objetivos-do-projeto-dart)
* [Acesso ao projeto](#acesso-ao-projeto-file_folder)
* [Ferramentas utilizadas](#ferramentas-utilizadas-hammer_and_wrench)
* [Bibliotecas utilizadas](#bibliotecas-utilizadas-books)
* [Componentes necessários](#componentes-necessários-toolbox)
* [Montagem](#montagem-wrench)
   * [Cuidados durante a montagem](#cuidados-durante-a-montagem-warning)
* [Reprodução](#reprodução-gear)
* [Como utilizar o TrackVision](#como-utilizar-o-trackVision-pushpin)
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

## Objetivos do Projeto :dart:

<p>
O objetivo principal do projeto TrackVision é desenvolver um sistema de cronometragem de corridas que seja eficiente e fácil de usar. As metas específicas incluem:
</p>

- ``1.`` Captura de Voltas em Tempo Real:
    - ``1.1`` Implementar a capacidade de capturar os tempos de volta em tempo real, utilizando o ESP32 e sensores adequados.
- ``2.`` Transmissão de Dados: 
    - ``2.1`` Criar uma infraestrutura para enviar os dados de volta para a nuvem, permitindo o monitoramento em tempo real e a análise posterior.
- ``3.`` Interface Amigável: 
    - ``3.1`` Desenvolver uma interface de usuário que seja intuitiva e forneça informações claras sobre o desempenho dos corredores.

- ``4.`` Escalabilidade: 
    - ``4.1`` Garantir que o sistema possa ser facilmente escalado para atender a diferentes tipos de corridas e número de participantes.

- ``5.`` Robustez e Confiabilidade:
    - ``5.1`` Assegurar que o sistema funcione de maneira consistente em diferentes condições de corrida e ambientes.

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

## Montagem :wrench:

<details>
  <summary>Imagem da Montagem</summary>
  <img src="https://github.com/user-attachments/assets/d7734bf3-a731-49b7-b120-20c9a60e2b51" alt="imagem-montagem">
</details>

<h3>Cuidados durante a montagem :warning:</h3>

**Atenção!**
**Lembrar de conectar os terminais negativos e positivos com o esp32 para passar a corrente!**

- ``1.`` Conectando LCD:
    - ``1.1.`` **Atenção!** Estamos utilizando um LCD 16x2 com um módulo I2C!;
    - ``1.2.`` Conecte o VCC no terminal positivo (5V), GND no terminal negativo (GND), o SDA no pino 21 e o SCL no pino 22;
    - ``1.3.`` Teste para ver se o display está funcionando, se tiver problemas com o display, pode ser algumas dessas possibilidades: o LCD está quebrado, com mal contato ou o contraste está baixo;
        - ``1.3.1.`` Para aumentar o contraste do display basta girar o trimpot de ajuste do contraste no sentido anti-horário. Por sua vez, para diminuir o contraste gire no sentido horário.

        <details>
            <summary>Imagem de onde fica o trimpot de ajuste do contraste</summary>
            <img src="https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/101829188/50648d65-2402-4508-a47d-1d38bbf663e5" alt="Terminais do DHT11">
            </details>

- ``2.`` Conectando o RTC:
  - ``2.1.`` Conecte o SCL no pino 22 (junto com o SCL do LCD) do arduino, o SDA no pino 21 (junto com o SDA do LCD), o 5V no terminal positivo do breadboard e o GND no terminal negativo do breadboard.
  - ``2.2.`` O RTC é um componente delicado. Manipule com cuidado para evitar danos.
  - ``2.3.`` Certifique-se de que o RTC esteja configurado com a hora correta. Uma configuração incorreta pode levar a erros no funcionamento do seu projeto.
  - ``2.4.`` Verifique se o RTC está recebendo a alimentação adequada. Uma alimentação insuficiente ou excessiva pode danificar o RTC ou afetar seu funcionamento.

- ``3.`` Conectando o IR-receiver:
  - ``3.1.`` Conecte o 5V no terminal positivo do breadboard e o GND no terminal negativo do breadboard e o DATA conecte no pino 15.
  - ``3.2.`` O IR-receiver é um componente delicado manipule o com cuidado.
  - ``3.3.`` Verifique se o IR-receiver está recebendo a alimentação adequada. 

- ``4.`` Utilizando o IR-remote:
    - ``4.1.`` Confira os comandos que são enviados em cada botão do controle na documentação.

## Reprodução :gear:

- ``1.`` Após a montagem do projeto, é necessário inserir o código por meio de um computador que possui o programa Arduino IDE instalado;
- ``2.``Ao abrir a IDE, é necessário fazer algumas coisas para selecionar o ESP32:
  - ``2.1.`` Clique em **Arquivo** > **Preferências**.
    - ``2.1.1.`` No campo **URLs Adicionais para Gerenciadores de Placas**, adicione o seguinte link: *https://dl.espressif.com/dl/package_esp32_index.json*;
  - ``2.2.`` Clique em **Ferramentas** > **Placas** > **Gerenciar Placas**.
    - ``2.2.1.`` Na janela que se abre, digite *ESP32* na caixa de pesquisa;
    - ``2.2.2.`` Selecione a plataforma *esp32* da lista e clique em Instalar;
  - ``2.3.`` Após a instalação, vá novamente em **Ferramentas** > **Placas**.
    - ``2.3.1.`` Você verá uma nova opção para selecionar as placas ESP32. Escolha a placa específica que você está usando;
- ``3.`` Baixe as [bibliotecas necessárias](#bibliotecas-utilizadas-books) no Arduino IDE;
- ``4.`` Faça as devidas modificações no código disponível:
  
  ```cpp
    const char* default_SSID = "SUA_INTERNET"; // Nome da rede Wi-Fi 
    const char* default_PASSWORD = "SENHA_DA_SUA_INTERNET"; // Senha da rede Wi-Fi 
    const char* default_BROKER_MQTT = "IP_PÚBLICO"; // IP do Broker MQTT 
  ```

  - ``4.1.`` Substitua a "SUA_INTERNET" pelo nome de sua internet;
  - ``4.2.`` Substitua a "SENHA_DA_SUA_INTERNET" pela senha de sua internet;
  - ``4.3.`` Substitua o "IP_PÚBLICO" pelo ip do servidor do Cloud Service de sua preferência:
    - ``4.3.1.`` Não disponibilizamos o IP por motivos de segurança. Para testar este código, você precisará de um serviço de nuvem, como Azure ou AWS. Além disso, será necessário instalar o FIWARE e o Docker nesse serviço e, por fim, abrir as portas necessárias. 
- ``5.`` Transferir o código do computador para  o ESP32 por meio do Cabo USB;
- ``6.`` Teste o sistema para verificar se ele está recebendo instruções e enviando dados via Postman;
- ``7.`` Com tudo montado e pronto, é necessário levá-lo para o ambiente em que será implementado e ligá-lo á uma fonte;

<p align='center'><i>OBS: Se o ESP32 for uma versão mais antiga, pode ser necessário pressionar o botão BOOT na placa durante a transferência do código </i></p>

## Como utilizar o TrackVision :pushpin:

- ``1.`` Espere a conexão com a internet, checando o monitor serial.
- ``2.`` Clique no POWER, e espere a conexao com o MQTT.
- ``3.`` Cada clique no POWER contará uma volta e mostrará o tempo dela.
- ``4.`` Na penúltima volta, aperte o 0 uma vez, após apertar o 0, o proximo clique no POWER encerrará a corrida.
- ``5.`` Mensagem final será mostrada com o tempo total da corrida.
- ``6.`` Se por acaso, voce iniciou a corrida sem querer, para voltar ao MENU, aperte MENU no IR-Controller.


## Pessoas Desenvolvedoras do Projeto :globe_with_meridians:

| [<img src="https://avatars.githubusercontent.com/u/101829188?v=4" width=115><br><sub>Alice Santos Bulhões</sub>](https://github.com/AliceSBulhoes) |  [<img src="https://avatars.githubusercontent.com/u/163866552?v=4" width=115><br><sub>Eduardo Oliveira Cardoso Madid</sub>](https://github.com/EduardoMadid) |  [<img src="https://avatars.githubusercontent.com/u/148162404?v=4" width=115><br><sub>Lucas Henzo Ide Yuki</sub>](https://github.com/LucasYuki1) | [<img src="https://avatars.githubusercontent.com/u/153787379?v=4" width=115><br><sub>Nicolas Haubricht Hainfellner</sub>](https://github.com/NicolasHaubricht) |
| :---: | :---: | :---: | :---: |
| RM:554499 | RM:556349 | RM:554865 | RM:556259 |
