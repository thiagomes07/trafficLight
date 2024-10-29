// Definições de pinos e bibliotecas
const int redInterval = 6000;      // 6 segundos para o LED vermelho
const int yellowInterval = 2000;   // 2 segundos para o LED amarelo
const int greenInterval = 4000;    // 4 segundos para o LED verde

class TrafficLight {
  private:
    // Atributos do semáforo
    int *greenPin, *yellowPin, *redPin, *buttonPin, *buzzerPin;
    int trafficLightState;
    unsigned long previousTime;
    unsigned long lastButtonTime;
    const unsigned long debounceDelay = 50;
    bool buttonPressed;

  public:
    // Construtor para inicializar os atributos
    TrafficLight(int *green, int *yellow, int *red, int *button, int *buzzer)
      : greenPin(green), yellowPin(yellow), redPin(red), 
        buttonPin(button), buzzerPin(buzzer), previousTime(0), 
        lastButtonTime(0), buttonPressed(false) {
        
      trafficLightState = 0; 

      // Definir os pinos como saídas ou entrada
      pinMode(*greenPin, OUTPUT);
      pinMode(*yellowPin, OUTPUT);
      pinMode(*redPin, OUTPUT);
      pinMode(*buttonPin, INPUT_PULLUP);
      pinMode(*buzzerPin, OUTPUT);
    }

    // Método para atualizar o semáforo
    void update() {
      manageButton();
      unsigned long currentTime = millis();

      switch (trafficLightState) {
        case 0: // Estado do LED Vermelho
          digitalWrite(*redPin, HIGH);
          digitalWrite(*yellowPin, LOW);
          digitalWrite(*greenPin, LOW);

          if (currentTime - previousTime >= redInterval) {
            changeToYellow();
          }
          break;

        case 1: // Estado do LED Amarelo (pós-vermelho)
          digitalWrite(*redPin, LOW);
          digitalWrite(*yellowPin, HIGH);
          digitalWrite(*greenPin, LOW);

          if (currentTime - previousTime >= yellowInterval) {
            changeToGreen();
          }
          break;

        case 2: // Estado do LED Verde
          digitalWrite(*greenPin, HIGH);
          digitalWrite(*yellowPin, LOW);
          digitalWrite(*redPin, LOW);

          if (currentTime - previousTime >= greenInterval) {
            changeToYellowBeforeRed();
          }
          break;

        case 3: // Estado do LED Amarelo (pré-vermelho)
          digitalWrite(*redPin, LOW);
          digitalWrite(*yellowPin, HIGH);
          digitalWrite(*greenPin, LOW);

          if (currentTime - previousTime >= yellowInterval) {
            changeToRed();
          }
          break;
      }
    }

    // Método para gerenciar o botão
    void manageButton() {
      int buttonReading = digitalRead(*buttonPin);
      if (buttonReading == LOW && (millis() - lastButtonTime > debounceDelay)) {
        buttonPressed = true;
        lastButtonTime = millis();
        Serial.println("Botao pressionado");
        
        // Ajustar o intervalo conforme o estado do semáforo
        if (trafficLightState == 0) {
          Serial.println("Tempo do LED Vermelho reduzido por botao");
        } else if (trafficLightState == 2) {
          Serial.println("Tempo do LED Verde aumentado por botao");
        }
      } else {
        buttonPressed = false;
      }
    }

    // Métodos para transições de estado
    void changeToYellow() {
      trafficLightState = 1;
      previousTime = millis();
      Serial.println("Mudou para o LED Amarelo (pós-vermelho)");
    }

    void changeToGreen() {
      trafficLightState = 2;
      previousTime = millis();
      buzz(1); // Disparar o buzzer uma vez ao mudar para verde
      Serial.println("Mudou para o LED Verde");
    }

    void changeToYellowBeforeRed() {
      trafficLightState = 3;
      previousTime = millis();
      Serial.println("Mudou para o LED Amarelo (pré-vermelho)");
    }

    void changeToRed() {
      trafficLightState = 0;
      previousTime = millis();
      buzz(3); // Disparar o buzzer três vezes ao mudar para vermelho
      Serial.println("Mudou para o LED Vermelho");
    }

    // Método para o buzzer
    void buzz(int times) {
      for (int i = 0; i < times; i++) {
        tone(*buzzerPin, 1000, 100);
        delay(200); // Pausa entre os apitos
      }
      noTone(*buzzerPin);
      Serial.print("Buzzer tocou ");
      Serial.print(times);
      Serial.println(" vez(es)");
    }
};

// Instância da classe TrafficLight
int greenPin = 13, yellowPin = 12, redPin = 11, buttonPin = 7, buzzerPin = 6;
TrafficLight trafficLight(&greenPin, &yellowPin, &redPin, &buttonPin, &buzzerPin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Atualiza o estado do semáforo
  trafficLight.update();
}
