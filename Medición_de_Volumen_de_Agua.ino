// Configuramos los pines del sensor Trigger y Echo
const int PinTrig = 7;
const int PinEcho = 6;

// Constante velocidad sonido en cm/s
const float VelSon = 34000.0;

// Número de muestras
const int numLecturas = 1;

// Distancia a los 100 ml y vacío
const float distanciaVacio = 16.83;

float lecturas[numLecturas]; // Array para almacenar lecturas
int lecturaActual = 0; // Lectura en proceso
float total = 0; // Lecturas acumuladas
float media = 0; // Media para calcular el volumen de agua entre vacio y llenado
bool primeraMedia = false; // validar que ya se haya calculado al menos una

void setup()
{
  // Iniciamos el monitor serie para mostrar el resultado
  Serial.begin(9600);
  // Ponemos el pin Trig en modo salida
  pinMode(PinTrig, OUTPUT);
  // Ponemos el pin Echo en modo entrada
  pinMode(PinEcho, INPUT);

  // Inicializamos el array
  for (int i = 0; i < numLecturas; i++)
  {
    lecturas[i] = 0;
  }
}
void loop()
{
  // Eliminamos la última medida
  total = total - lecturas[lecturaActual];

  iniciarTrigger();

  // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
  unsigned long tiempo = pulseIn(PinEcho, HIGH);

  // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
  // por eso se multiplica por 0.000001
  float distancia = tiempo * 0.000001 * VelSon / 2.0;

  // Almacenamos la distancia en el array
  lecturas[lecturaActual] = distancia;

  // Añadimos la lectura al total
  total = total + lecturas[lecturaActual];

  // Avanzamos a la siguiente posición del array
  lecturaActual = lecturaActual + 1;

  // Comprobamos si hemos llegado al final del array
  if (lecturaActual >= numLecturas)
  {
    primeraMedia = true;
    lecturaActual = 0;
  }

  // Calculamos la media
  media = total / numLecturas;

  // Solo mostramos si hemos calculado por lo menos una media
  if (primeraMedia)
  {
    float distanciaLleno = distanciaVacio - media;
    //float cantidadLiquido = distanciaLleno * 100 / distancia100;

    Serial.print(media);
    Serial.println(" cm");

//100ML
    if (media >= 13.00 && media < 16.83) {
      float distancia100 = 3.83;
      float cantidadLiquido = distanciaLleno * 100 / distancia100;
      Serial.print(cantidadLiquido);
    Serial.println(" ml - 100ML");
    }

//200ml
    if (media >= 9.20 && media < 13.00) {
      float distancia100 = 3.80;
      float cantidadLiquido = distanciaLleno * 100 / distancia100;
      Serial.print(cantidadLiquido);
    Serial.println(" ml - 200ML");
    }

//300ml
    if (media >= 6.5 && media < 9.2) {
      float distancia100 = 2.70;
      float cantidadLiquido = distanciaLleno * 100 / distancia100;
      Serial.print(cantidadLiquido);
    Serial.println(" ml - 300ML");
    }


//400ml
    if (media > 4.00 && media < 6.5) {
      float distancia100 = 2.50;
      float cantidadLiquido = distanciaLleno * 100 / distancia100;
      Serial.print(cantidadLiquido);
    Serial.println(" ml - 400ML");
    }


//500ml
    if (media < 4.0) {
     float distancia100 = 2.40;
      float cantidadLiquido = distanciaLleno * 100 / distancia100;
      Serial.print(cantidadLiquido);
    Serial.println(" TINACO LLENO");
    }



    
    
  }

  delay(500);
}

// Método para iniciar la secuencia del Trigger para comenzar a medir
void iniciarTrigger()
{
  // Ponemos el Triiger en estado bajo y esperamos 2 ms
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);

  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);

  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(PinTrig, LOW);
}
