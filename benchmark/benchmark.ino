#include <MonteCarloPi.h>

MonteCarloPi myPi;
double pi;
const unsigned long myLoop = 30000;
const byte decP = 5; // Preciso até 5 casas decimais
char sChar;

void setup()
{
  Serial.begin(9600);

  // Aguarde o Serial estar pronto
  // Funciona em Arduino AVR (Uno, Leonardo etc), mas não no ESP32
  // Para o ESP32, pressione reset após abrir o Monitor Serial
  while (!Serial){}

  Serial.println(F("MonteCarloPi de cygig\nPi é 3.14159265359\n"));

  // Necessário para ESP32 ou os resultados não são consistentes
  // Reinicia os números aleatórios
  // Opcional para a maioria dos outros
  myPi.reseed();

  do
  { 
    // Execute o benchmark
    benchmark();

    // Limpe o buffer Serial
    while (Serial.available()){
      char a = Serial.read(); 
    }

    // Pergunte ao usuário se eles querem executar o benchmark novamente
    Serial.print(F("Executar novamente? S/N: "));

    // Continue esperando enquanto o usuário não inserir nada
    while (Serial.available() == 0){}

    // Assim que o usuário inserir algo, leia
    sChar = Serial.read();
    Serial.println('\n');
  }
  while (sChar == 'S' || sChar == 's'); // Repita se o usuário inserir 'S' ou 's'
}

void loop(){ /* NADA */ }

void benchmark()
{
  if(1) {
    Serial.print(F("Estimando pi em "));
    Serial.print(myLoop);
    Serial.println(F(" loops..."));
    // myPi.startTimer(); // Função de temporizador integrada
    Serial.println(F("INICIADO"));
    pi = myPi.piLoop(myLoop);
    Serial.println(F("FINALIZADO"));
    // myPi.stopTimer(); // Lembre-se de parar o temporizador

    Serial.print(F("Pi é estimado como "));
    Serial.println(pi, decP+1);
    
    Serial.print(myPi.getSquares()); // Número de quadrados é o mesmo que o número total de loops
    Serial.println(F(" loops.\n"));
  } else {
    Serial.print(F("Estimando pi com "));
    Serial.print(decP);
    Serial.println(F(" casa(s) decimal(is)..."));

    Serial.println(F("INICIADO::Para Decimal"));
    pi = myPi.piToDP(decP);
    Serial.println(F("FINALIZADO::Para Decimal"));

    Serial.print(F("Pi é estimado como "));
    Serial.print(pi, decP+1);
    Serial.print(F(", preciso até "));
    Serial.print(myPi.getDP());
    Serial.println(F(" casa(s) decimal(is)."));
    
    Serial.print(F("Levou "));
    Serial.print(myPi.getSquares());
    Serial.println(F(" loops.\n"));
  }
  myPi.reset();
  myPi.reseed();
}
