#include <Servo.h>

Servo servo1; // Servo que levanta o braço
Servo servo2; // Servo que empurra o braço
Servo servo3;
Servo servo4;

String inputString = "";         // Uma string para armazenar os dados recebidos da serial

void setup() {
  Serial.begin(9600);
  // Associa os pinos aos servos
  servo1.attach(11); // Pino do servo1
  servo2.attach(10); // Pino do servo2
  servo3.attach(9);
  servo4.attach(8);

  // Inicializa os servos nas posições iniciais
  servo1.write(180);
  servo2.write(10);
  servo3.write(95);
  servo4.write(90);

  // Espera um pouco para garantir que os servos estão na posição inicial
  delay(1000);

  inputString.reserve(200); // Reserva 200 bytes de espaço para a string

  // Mensagem de debug para indicar que o setup foi concluído
  Serial.println("Setup completo, aguardando comando...");
}

void bye() {
  // Levanta o braço
  servo1.write(50);
  delay(1000); // Aguarda 1 segundo para o braço levantar completamente

  // Acena 3 vezes
  for (int i = 0; i < 3; i++) {
    servo2.write(40); // Move o servo2 para a posição 60
    delay(500); // Espera 0,5 segundo
    servo2.write(10); // Move o servo2 para a posição 0
    delay(500); // Espera 0,5 segundo
  }

  // Retorna o braço à posição inicial
  servo1.write(180);
  delay(1000); // Aguarda 1 segundo para o braço descer completamente

  // Aguarda antes de repetir
  delay(3000); // Espera 3 segundos antes de acenar novamente
}

void yes() {
  servo3.write(75);
  delay(1000);
  for (int i = 0; i < 5; i++) {
    servo3.write(85);
    delay(100);
    servo3.write(75);
    delay(100);
  }
  delay(500);
  servo3.write(95);
}

void no(){
  for (int i = 0; i < 5; i++) {
    servo4.write(70);
    delay(100);
    servo4.write(110);
    delay(100);
  }
  servo4.write(90);
}

void dance() {
  while (true) {
    // Verifica se há dados disponíveis na porta serial
    if (Serial.available()) {
      // Lê o byte do buffer serial
      char inChar = (char)Serial.read();
      // Adiciona o byte na string de entrada
      inputString += inChar;
      // Verifica se o caractere de nova linha foi recebido
      if (inChar == '\n') {
        // Remove espaços em branco extras
        inputString.trim();

        // Mensagem de debug para indicar que a string foi recebida
        Serial.print("Comando recebido: ");
        Serial.println(inputString);

        if (inputString == "stop") {
          Serial.println("Comando 'stop' acionado, interrompendo dança");
          break; // Sai do loop da dança
        }
        
        // Limpa a string de entrada
        inputString = "";
      }
    }

    // Combinando movimentos dos servos para criar uma "dança"
    servo1.write(50);
    servo2.write(40);
    delay(500);
    servo1.write(180);
    servo2.write(10);
    delay(500);
    
    servo3.write(75);
    delay(500);
    servo3.write(95);
    delay(500);
    
    servo4.write(70);
    delay(500);
    servo4.write(90);
    delay(500);
  }
}

void idle() {


  int aux = 0;

  while (aux == 0) {
    // Verifica se há dados disponíveis na porta serial
    if (Serial.available()) {
      // Lê o byte do buffer serial
      char inChar = (char)Serial.read();
      // Adiciona o byte na string de entrada
      inputString += inChar;
      // Verifica se o caractere de nova linha foi recebido
      if (inChar == '\n') {
        // Remove espaços em branco extras
        inputString.trim();

        // Mensagem de debug para indicar que a string foi recebida
        Serial.print("Comando recebido: ");
        Serial.println(inputString);

        if (inputString == "stop") {
          Serial.println("Comando 'stop' acionado, interrompendo idle");
          aux = 1;
          break; // Sai do loop do idle
        }
        
        // Limpa a string de entrada
        inputString = "";
      }
    }

    // Movimentos aleatórios e espaçados dos servos 3 e 4
    int servo3Pos = random(85, 111); // Gera um valor aleatório entre 85 e 110 para o servo 3
    int servo4Pos = random(0, 181);  // Gera um valor aleatório entre 0 e 180 para o servo 4

    servo3.write(servo3Pos);
    delay(random(500, 1500)); // Espera um tempo aleatório entre 0,5 e 1,5 segundos
    servo4.write(servo4Pos);
    delay(random(500, 1500)); // Espera um tempo aleatório entre 0,5 e 1,5 segundos

    delay(3000);
  }
}

void loop() {
  // Verifica se há dados disponíveis na porta serial
  while (Serial.available()) {
    // Lê o byte do buffer serial
    char inChar = (char)Serial.read();
    // Adiciona o byte na string de entrada
    inputString += inChar;
    // Verifica se o caractere de nova linha foi recebido
    if (inChar == '\n') {
      // Remove espaços em branco extras
      inputString.trim();

      // Mensagem de debug para indicar que a string foi recebida
      Serial.print("Comando recebido: ");
      Serial.println(inputString);

      if (inputString == "bye") {
        Serial.println("Comando 'bye' acionado");
        bye();
      } else if (inputString == "yes") {
        Serial.println("Comando 'yes' acionado");
        yes();
      } else if (inputString == "no") {
        Serial.println("Comando 'no' acionado");
        no();
        
      } else if (inputString == "dance") {
        Serial.println("Comando 'dance' acionado");
        dance();
      } else if (inputString == "idle") {
        Serial.println("Comando 'idle' acionado");
        inputString = "";
        idle();
      }
        else {
        Serial.println("Erro ao comando");
      }
      
      // Limpa a string de entrada
      inputString = "";
    }
  }
}
