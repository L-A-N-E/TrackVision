// Bibliotecas utilizadas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <RTClib.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PIN_RECEIVER 15  // Pino para o receptor IR
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

IRrecv receiver(PIN_RECEIVER); // Inicializa o receptor IR
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);  // Inicializa o LCD I2C
RTC_DS1307 rtc;  // Inicializa o RTC

// Variáveis para armazenar o tempo dos apertos
DateTime firstPressTime;
DateTime secondPressTime;
bool firstPress = false;
bool timeMeasured = false;
bool isLastLap = false;
int lapCount = 0;

unsigned long totalTime = 0;
unsigned long startMillis;
unsigned long currentMillis;
unsigned long elapsedMillis;

// Configurações - variáveis editáveis
const char* default_SSID = "NOME_DA_SUA_REDE"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "SENHA_DA_SUA_REDE"; // Senha da rede Wi-Fi 
const char* default_BROKER_MQTT = "IP_PÚBLICO"; // IP do Broker MQTT 
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/TRV027/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/TRV027/attrs/t"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/TRV027/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_027"; // ID MQTT // Grupo LANE 27
const int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico 
const char* topicPrefix = "lamp027";

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

WiFiClient espClient; // Inicia o Wifi
PubSubClient MQTT(espClient); // Inicia o MQTT

// Inicia o Wi-Fi
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

// Inicia a conexão com o MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

// Checa se o wifi esta conectado, se não tenta novamente
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}

// Verifica se está conectado com o MQTT e o Wifi
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

// Checa se o MQTT esta conectado, se não tenta novamente
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

// Envia ao MQTT o tempo exato e o tempo da volta, tudo formatado
void handleLapTime(unsigned long lapTimeMillis, DateTime exactLapTime) {
  if (timeMeasured) {
    // Formata o tempo exato da volta como "hh:mm:ss"
    String exactTimeMessage = String(exactLapTime.hour()) + ":" + String(exactLapTime.minute()) + ":" + String(exactLapTime.second());

    Serial.print("Enviando tempo exato da volta: ");
    Serial.println(exactTimeMessage.c_str());

    Serial.print("tempo da volta em ms: ");
    Serial.println(lapTimeMillis);

    // Envia o tempo exato da volta (hh:mm:ss) para o MQTT
    MQTT.publish(TOPICO_PUBLISH_1, exactTimeMessage.c_str());

    // Converte lapTimeMillis para string antes de enviar via MQTT
    String lapTimeArray = "[" + String(lapCount) + "," + (lapTimeMillis) + "]";
    MQTT.publish(TOPICO_PUBLISH_2, lapTimeArray.c_str());

    // Zera o estado de medição do tempo
    timeMeasured = false;
  }
}

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial
  clearSerialMonitor();
  lcd.init(); // Inicia o lcd
  lcd.backlight(); // Liga a luz de trás do lcd

  displayMenu();

  receiver.enableIRIn(); // Habilita o receptor IR

  initWiFi();
  initMQTT();
  delay(5000);
  

  if (!rtc.begin()) {
    Serial.println("RTC nao encontrado");
    while (1);
  }
}

void loop() {
  if (receiver.decode()) { // Verifica se houve decodificação do sinal recebido
    VerificaConexoesWiFIEMQTT();
    translateIR(); // Processa o comando IR
    receiver.resume(); // Prepara o IR receptor para a proxima leitura
    MQTT.loop(); // Mantém a conexão MQTT ativa
  }
}

// Traduz o comando Ir recebido
void translateIR() { 
  switch (receiver.decodedIRData.command) { // Verifica o comando IR recebido
    case 162: // Se o comando for 162
      handleTimeMeasurement(); // Chama a função para medir o tempo
      break;
    case 226: // Se o comando for 266
      resetRace();// Chama a função para resetar a corrida
      break;
    case 104: // Se o comando for 104
      isLastLap = true;
      break;
    default:
      lcd.clear();
      lcd.print("Aperte o POWER!");
      displayMenu();
  }
}

// Mede o tempo
void handleTimeMeasurement() {
  if (!firstPress) {
    // Inicializa a primeira medição de tempo
    firstPressTime = rtc.now();
    firstPress = true;
    lapCount = 0;
    totalTime = 0;

    // Contagem regressiva
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("3");
    delay(1000);

    lcd.setCursor(7, 0);
    lcd.print("2");
    delay(1000);

    lcd.setCursor(9, 0);
    lcd.print("1");
    delay(1000);

    lcd.setCursor(5, 1);
    lcd.print("START");
    delay(50);

    // Captura o tempo em milissegundos
    startMillis = millis();  
    lcd.clear();
    lcd.setCursor(3, 0);
    // Mostra o horário que começou a corrida no lcd
    lcd.print("INICIO:");
    lcd.setCursor(0, 3);
    lcd.print(firstPressTime.hour());
    lcd.print(":");
    lcd.print(firstPressTime.minute());
    lcd.print(":");
    lcd.println(firstPressTime.second());

    // Mostra o horário que começou a corrida no monitor serial
    Serial.print("Começo da Corrida: ");
    Serial.print(firstPressTime.hour());
    Serial.print(":");
    Serial.print(firstPressTime.minute());
    Serial.print(":");
    Serial.println(firstPressTime.second());

    delay(2000);
    timeMeasured = false; // Tempo não foi medido

  } else {
    // Captura o tempo exato da volta
    secondPressTime = rtc.now(); 
    // Captura o tempo atual em milissegundos
    currentMillis = millis();     
    lapCount++;

    // Calcula o tempo da volta em milissegundos
    unsigned long lapTimeMillis = currentMillis - startMillis;  

    // Exibe o tempo da volta em ms no Serial Monitor
    Serial.print("Volta ");
    Serial.print(lapCount);
    Serial.print(": ");
    Serial.print(lapTimeMillis);  
    Serial.println("ms");

    // Exibe o tempo da volta em ms no lcd
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Volta ");
    lcd.print(lapCount);

    lcd.setCursor(0, 1);
    lcd.print(lapTimeMillis);
    lcd.print("ms");

    // Acumula o tempo total da corrida
    totalTime += lapTimeMillis;

    // Tempo foi medido
    timeMeasured = true;

    // Se for a ultima lap
    if (isLastLap) {
      handleLapTime(lapTimeMillis, secondPressTime); // Envia o tempo para o MQTT
      displayEndMessage(); // Mostra o tempo total da corrida formatado
    } else {
      firstPressTime = secondPressTime;
      startMillis = currentMillis;  // Atualizar para a próxima volta
    }

    // Chama a função handleLapTime passando o tempo em milissegundos e o tempo exato (RTC)
    handleLapTime(lapTimeMillis, secondPressTime);
  }
}

// Mostra o menu no lcd
void displayMenu() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("TrackVision");
  delay(300);
}

// Reseta a corrida resetando todas as variavéis
void resetRace() {
  firstPress = false;
  timeMeasured = false;
  lapCount = 0;
  totalTime = 0;
  isLastLap = false;

  // Volta ao menu no lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltando ao Menu");
  delay(2000);

  displayMenu();
}

// Mostra o tempo total da corrida no lcd formatado e no monitor serial
void displayEndMessage() {
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("FIM");

  int hours, totalMinutes, totalSeconds, totalMilliseconds;
  convertToHMS(totalTime, &hours, &totalMinutes, &totalSeconds, &totalMilliseconds);

  lcd.setCursor(3, 1);
  lcd.print(hours);
  lcd.print("h ");
  lcd.print(totalMinutes);
  lcd.print("m ");
  lcd.print(totalSeconds);
  lcd.print("s ");
  lcd.print(totalMilliseconds);
  lcd.print("ms");

  Serial.print("FIM DA CORRIDA:");
  Serial.print(hours);
  Serial.print("h ");
  Serial.print(totalMinutes);
  Serial.print("m ");
  Serial.print(totalSeconds);
  Serial.print("s ");
  Serial.print(totalMilliseconds);
  Serial.print("ms");

  delay(5000);
  lcd.clear();
  resetRace();
}

// Transforma o valor total em milisegundos e transforma para horas minutos segundo e milisegundos
void convertToHMS(unsigned long totalMillis, int *hours, int *minutes, int *seconds, int *milliseconds) {
    *hours = totalMillis / 3600000;       // Converter para horas
    totalMillis %= 3600000;
    *minutes = totalMillis / 60000;       // Converter para minutos
    totalMillis %= 60000;
    *seconds = totalMillis / 1000;        // Converter para segundos
    *milliseconds = totalMillis % 1000;   // Pegar o resto como milissegundos
}

// Limpa o monitor serial, printando várias linhas em branco
void clearSerialMonitor() {
  // Número de linhas em branco para limpar o monitor serial
  int linesToPrint = 20;  // Ajuste conforme necessário
  for (int i = 0; i < linesToPrint; i++) {
    Serial.println();
  }
}