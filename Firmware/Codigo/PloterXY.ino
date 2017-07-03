
/**********LIBRERIAS**********/
#include <SD.h>
#include <Servo.h>
#include "U8glib.h"

/**********ENDSTOP**********/

#define X_MIN_PIN          19
#define X_MAX_PIN          20
#define Y_MIN_PIN          18
#define Y_MAX_PIN          21

int finXmin = 0;			//flag que indica si se ha pulsado el final de carrera X_MIN.
int finYmin = 0;			//flag que indica si se ha pulsado el final de carrera Y_MIN.

/**********SD**********/
#define SS          	53
#define SCK         	52
#define MOSI        	51
#define MISO        	50
#define SDDETECT    	49

uint8_t ss = 53;
int8_t mosi = 51;
int8_t miso = 50;
int8_t sck = 52; 

File myFile;				//Objeto de la clase FILE
int UltimaPocicion=0;		//indica la ultima posicion leida en una linea del archivo G-Code
int pausa=1000;

/*********** LASER **********/
#define LASER_PIN      2

/*********** LASER **********/
#define BEEPER_PIN      37

int enciendeLaser = 0;

/**********SERVO**********/
#define SERVO_PIN 4

Servo servoMotor;
const int servoArriba = 105;		//levanta el servo para que NO pinte
const int servoAbajo = 85;			//baja el servo para que SI pinte
int servoAhora = 0;					//Marca el estado del servo en el instante
int servoAntes = 0;					//Marca el estado del servo en el instante previo
int servoDosAntes = 0;				//Marca el estado del servo en el dos veces previo

/**********FULL GRAPHIC SMART CONTROLLER**********/

#define RS          	16
#define ENABLE         	17
#define SCLK        	23

U8GLIB_ST7920_128X64_1X u8g(SCLK, ENABLE, RS);

/**********MOTOR**********/

#define MS1PINY 63
#define MS2PINY 59
#define MS3PINY 64

#define MS1PINX 40
#define MS2PINX 42
#define MS3PINX 44

#define DIRPIN 4
#define STEPPIN 5
#define ENABLEPIN 6
#define MOTOR_STEPS 200

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38


#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

/********** CONSTANTES DEL PROGRAMA **********/

const float pasos200 = 0.1839;			//medida en mm del avance en un paso de resolucion 200.
const float pasos400 = 0.09195;			//medida en mm del avance en un paso de resolucion 400.
const float pasos800 = 0.045975;			//medida en mm del avance en un paso de resolucion 800.
const float pasos1600 = 0.0229875;			//medida en mm del avance en un paso de resolucion 1600.
const float pasos3200 = 0.01149375;			//medida en mm del avance en un paso de resolucion 3200.

const int emergencia = 10;			//margen de seguridad de 1 centimetro en el origen (0,0) que posteriormente se ajustara con sumaEmergencia.
const float tiempoCoordenada = 0.25;			//tiempo que tarda en pintar una coordenada.

/**********MIRAR**********/

int finaliza = 0;

/********** VARIABLES GLOBALES PARA CONFIGURAR EL MOVIMIENTO **********/

//int numeroCoordenadas;			

float escala = 0;					//guarda el valor de la escala seleccionada.
int enMarcha = 0;				//indica si se ha comenzado a dibujar
int acabar = 0;					//indica si se ha acabado de pintar


/********** ENCODER Y PULSADOR **********/

#define ENCODER_NO_PULSADO			0
#define ENCODER_IZQUIERDA			31
#define ENCODER_DERECHA				33
#define ENCODER_PRESIONADO			35
#define PULSADOR					41

uint8_t uiEncoderIzquierda = 31;
uint8_t uiEncoderDerecha = 33;
uint8_t uiEncoderPresionado = 35;
uint8_t uiPulsador = 41;
uint8_t uiEncoderPrimero = ENCODER_NO_PULSADO;
uint8_t uiEncoderSegundo = ENCODER_NO_PULSADO;
uint8_t uiEncoder = ENCODER_NO_PULSADO;

/********** MENU DE LA ESCALA **********/

#define MENU_ITEMS 6
const char *menu_strings[MENU_ITEMS] = {"","4", "2", "1","1/2","1/4"};
uint8_t menu_current = 0;
uint8_t reescribirMenuEscala = 0;
uint8_t last_key_code = ENCODER_NO_PULSADO;
int seleccionarEscala = 0;			//guarda el valor de la escala seleccionada.

/***************************/
/********** SETUP **********/
/***************************/

/*** Configuracion del ENCODER ***/
void setupEncoder() { 
	pinMode(uiEncoderIzquierda, INPUT_PULLUP);           
	pinMode(uiEncoderDerecha, INPUT_PULLUP);           
	pinMode(uiEncoderPresionado, INPUT_PULLUP);           
	pinMode(uiPulsador, INPUT_PULLUP);          
}

/*** Configuracion de los MOTORES ***/
void setupMotor(){
	pinMode(MS1PINX,OUTPUT); 
	pinMode(MS2PINX,OUTPUT); 
	pinMode(MS3PINX,OUTPUT); 
	pinMode(MS1PINY,OUTPUT); 
	pinMode(MS2PINY,OUTPUT); 
	pinMode(MS3PINY,OUTPUT); 
	pinMode(X_DIR_PIN,OUTPUT);
	pinMode(X_STEP_PIN,OUTPUT);
	pinMode(X_ENABLE_PIN,OUTPUT);  
	pinMode(Y_DIR_PIN,OUTPUT);
	pinMode(Y_STEP_PIN,OUTPUT);
	pinMode(Y_ENABLE_PIN,OUTPUT);
	pinMode(X_MIN_PIN, INPUT);
	pinMode(X_MAX_PIN, INPUT);
	pinMode(Y_MIN_PIN, INPUT);
	pinMode(Y_MAX_PIN, INPUT);
}

/*** Configuracion de la SD ***/
void setupSD(){
						//abrir comunicacion serie con la tarjeta SD
	pinMode(SDDETECT,INPUT);
 // SD.begin(ss,mosi,miso,sck);
}

/*** Configuracion de las INTERRUPCIONES ***/
void setupInterrupciones(){
	attachInterrupt(digitalPinToInterrupt(X_MIN_PIN), minimoX, RISING);
	attachInterrupt(digitalPinToInterrupt(X_MAX_PIN), maximoX, RISING);
	attachInterrupt(digitalPinToInterrupt(Y_MIN_PIN), minimoY, RISING);
	attachInterrupt(digitalPinToInterrupt(Y_MAX_PIN), maximoY, RISING);
}

/*** Configuracion del SERVO ***/
void setupServo(){
	servoMotor.attach(SERVO_PIN);			//configurar el pin del SERVO.
   servoMotor.write(servoArriba);
}

/*** Configuracion el LASER ***/
void setupLaser(){
 pinMode(LASER_PIN,OUTPUT);
}

/*** Configuracion del BEEPER***/
void setupBeeper(){
 pinMode(BEEPER_PIN,OUTPUT);
}

/***Variables globales del programa***/
int resolucionX;      //indica al pololu que resolucion de pasos debe seleccionar del motor X.
int resolucionY;      //indica al pololu que resolucion de pasos debe seleccionar del motor Y.
int numeroCoordenadas = 0;    //almacena el numero de coordenadas que tiene el programa.
int sumaEmergencia = 0;     //determina un margen de seguridad en el origen (0,0) para desplazar el origen unos centimetros segun la escala deseada.
float tiempoTotal = 0;      //determina el tiempo total de la ejecucion.
float mueveteX;     //almacena el valor leido del archivo con los G-code del eje X.
float mueveteY;     //almacena el valor leido del archivo con los G-code del eje Y.
int j = 0;		//iteración para refrescar el LCD

/********** SETUP DEL FIRMWARE **********/

void setup(){
	Serial.begin(9600);			//abrir comunicacion serie con el PC.
	
	setupSD();				//configuracion de la SD. 
	setupEncoder();			//configuracion del ENCODER.                                
	setupMotor();			//configuracion de los MOTORES.      
	setupInterrupciones();	//configuracion de las INTERRUPCIONES.       
	setupServo();			//configuracion del SERVO       
	setupLaser();     //configuracion del LASER
	setupBeeper();    //configuracion del BEEPER
	reescribirMenuEscala = 1;			//refrescar el MENU ESCALA
	digitalWrite(X_ENABLE_PIN,HIGH);		//apagamos el MOTOR X (activo a nivel bajo).
	digitalWrite(Y_ENABLE_PIN,HIGH);		//apagamos el MOTOR Y (activo a nivel bajo).

}

/*******************************/
/********** FUNCIONES **********/
/*******************************/


/*************END STOP Y SEGURIDAD************/
/*Va a la posición inicial (0,0)*/
void posicionInicial(){
	digitalWrite(X_ENABLE_PIN,LOW);
	digitalWrite(Y_ENABLE_PIN,LOW);
	maximoX();
	maximoY();
	finXmin = 0; 
	finYmin = 0;
	digitalWrite(X_ENABLE_PIN,HIGH);
	digitalWrite(Y_ENABLE_PIN,HIGH);
  
}

/*Se ejecuta al producirse la interrupcion cuando se pulsa el endstop X_MIN_PIN*/
void minimoX(){
  finXmin = 1;
 if(enMarcha == 1){
    digitalWrite(LASER_PIN,LOW);
    servoMotor.write(servoArriba);
    digitalWrite(X_ENABLE_PIN,HIGH);
    digitalWrite(Y_ENABLE_PIN,HIGH);
    acabar = 1;
    myFile.close();
    SD.end();
    enMarcha = 0;
    finaliza = 1;
  }
}

/*Se ejecuta al producirse la interrupcion cuando se pulsa el endstop X_MAX_PIN*/
void maximoX(){
	if(enMarcha == 1){
    digitalWrite(LASER_PIN,LOW);
    servoMotor.write(servoArriba);
    digitalWrite(X_ENABLE_PIN,HIGH);
    digitalWrite(Y_ENABLE_PIN,HIGH);
    acabar = 1;
    myFile.close();
    SD.end();
    enMarcha = 0;
    finaliza = 1;
  }
 while(finXmin == 0){
    moviendoX(0);
 }
}

/*Se ejecuta al producirse la interrupcion cuando se pulsa el endstop Y_MIN_PIN*/
void minimoY(){
   finYmin = 1;
 if(enMarcha == 1){
    digitalWrite(LASER_PIN,LOW);
    servoMotor.write(servoArriba);
    digitalWrite(X_ENABLE_PIN,HIGH);
    digitalWrite(Y_ENABLE_PIN,HIGH);
    acabar = 1;
    myFile.close();
    SD.end();
    enMarcha = 0;
    finaliza = 1;
  }
}

/*Se ejecuta al producirse la interrupcion cuando se pulsa el endstop Y_MAX_PIN*/
void maximoY(){
   if(enMarcha == 1){
    digitalWrite(LASER_PIN,LOW);
    servoMotor.write(servoArriba);
    digitalWrite(X_ENABLE_PIN,HIGH);
    digitalWrite(Y_ENABLE_PIN,HIGH);
    acabar = 1;
    myFile.close();
    SD.end();
    enMarcha = 0;
    finaliza = 1;
  }
   while(finYmin == 0){
    moviendoY(0);
   }
}
/*************LOOP************/
void loop(){

	int startServo[numeroCoordenadas];		//Array con los valores de los servos
	float proporcion = 0;      //relacion entre el movimiento del motor X y el motor Y.
	float mueveX;     //determina lo que se tiene que mover el motor X.
	float mueveY;     //determina lo que se tiene que mover el motor Y.
	float aguantaX;     //almacena el valor leido del archivo con los G-code cuando se cambia de trazo cerrado del eje X.
	float aguantaY;     //almacena el valor leido del archivo con los G-code cuando se cambia de trazo cerrado del eje Y.
	float mueveXanterior; //almacena el valor anterior leido del archivo con los G-code del eje X.
	float mueveYanterior; //almacena el valor anterior leido del archivo con los G-code del eje X.
	int flagFinTrazo = 0; //flag que determina cuando se produce un final de trazo cerrado.
	int flagGuardaCoordenada = 0;       //flag que determina que valor tiene que guardar durante el cambio de trazo ya que se realizan dos paradas.
	float menorRestoX;    //determina que resolucion de pasos del motor X es mas precisa.
	float menorRestoY;      //determina que resolucion de pasos del motor Y es mas precisa.   
	int i = 0;        //iteraciones de las medidas leidas.

  if((acabar == 1) && (digitalRead(SDDETECT) == 0)){ 

    Serial.println("Se ha acabado");
    delay(1000);
    digitalWrite(BEEPER_PIN, LOW);
    servoMotor.detach();
    finXmin = 0; //0
	finYmin = 0;
    imprime();
    return;
  }
  
    imprime();
 
  if(digitalRead(SDDETECT) == 1){   
    Serial.println("NO DETECTADO");
 return;
  }
  
  acabar = 0;
  finaliza = 0;
  SD.begin(SS);

  posicionInicial();
  servoMotor.attach(SERVO_PIN);
  reescribirMenuEscala = 1;

  numeroCoordenadas = longArray();

  tiempoTotal = tiempoCoordenada * numeroCoordenadas;

  seleccionarEscala = 0;
  imprimeMenu();
  
 sumaEmergencia = emergencia * escala;
  while (i<numeroCoordenadas){
	j = i;
  mueveXanterior = mueveteX;
  mueveYanterior = mueveteY;
  mueveteX = ((leerValores()*escala) + sumaEmergencia);
  mueveteY = ((leerValores()*escala) + sumaEmergencia);
  
  imprimePosicionActual();
 if(i==3){
  enMarcha = 1; 
 }
  
 if(i == 0){
   
    Serial.println("EMPIEZA EL PROGRAMA");
  digitalWrite(X_ENABLE_PIN,LOW);
  digitalWrite(Y_ENABLE_PIN,LOW);
    startServo[i]=servoArriba;
    mueveX = mueveteX;
    mueveY =  mueveteY;
   
 }else if ((mueveteX == ((5*escala)+sumaEmergencia)) && (mueveteY== ((5*escala)+sumaEmergencia))){
       
       mueveX = 0;
       mueveY = 0;
       if (flagGuardaCoordenada==0){
       aguantaX = mueveXanterior;
       aguantaY = mueveYanterior;
       flagFinTrazo = 1;
       flagGuardaCoordenada++;
       }
       
      startServo[i]=servoArriba;
 
    }else{
      	  
       flagGuardaCoordenada = 0;
        if (flagFinTrazo == 1){

          mueveX = mueveteX-aguantaX;
          mueveY = mueveteY-aguantaY;
        
          flagFinTrazo = 0;
        }else{
          mueveX = mueveteX-mueveXanterior;
        mueveY = mueveteY-mueveYanterior;
        }
   
      startServo[i]=servoAbajo;
        
  }
   
    servoAhora = 0;
    servoAntes = 0;
    servoDosAntes = 0;

    servoAhora = startServo[i];
    servoAntes = startServo[--i];
    servoDosAntes = startServo[--i];
    i++;
    i++;


    if ((i==0)) {
		
       servoMotor.write(servoAhora);
       digitalWrite(LASER_PIN,LOW);
       enciendeLaser = 0;
    }else if ((servoAhora== servoAbajo) && (servoAntes==servoAbajo) && (servoDosAntes == servoArriba)){
        enciendeLaser = 1;
        digitalWrite(LASER_PIN,HIGH);
        servoMotor.write(servoAhora); //abajo

    }else if ((servoAhora== servoArriba) && (servoAntes==servoAbajo) && (servoDosAntes == servoAbajo)){
        digitalWrite(LASER_PIN,LOW);
        servoMotor.write(servoAhora); //Arriba
        enciendeLaser = 0;     
    }else if(enciendeLaser == 1){
      digitalWrite(LASER_PIN,HIGH);
    }else{
        digitalWrite(LASER_PIN,LOW);
    }
   /*****************************************************************************/
   
    if(mueveX<0){	//Accionar el sentido negativo del motor X
      digitalWrite(X_DIR_PIN,LOW);
      mueveX = abs(mueveX);
    }else{	//Accionar el sentido positivo del motor X
      digitalWrite(X_DIR_PIN,HIGH);
    }

    if(mueveY<0){	//Accionar el sentido negativo del motor Y
      digitalWrite(Y_DIR_PIN,LOW);
      mueveY = abs(mueveY);
    }else{	//Accionar el sentido positivo del motor Y
      digitalWrite(Y_DIR_PIN,HIGH);
    }
     /*VAN LAS DECLARACIONES DE RESTOS Y DE DIV*/   
int divX[5];         //array que guarda la division entera entre  mueveX/divX[].
int divY[5];        //array que guarda la division entera entre  mueveY/divY[].
float restosX[5];         //array que guarda los restos de mueveX-(divX[]*pasos).
float restosY[5];         //array que guarda los restos de mueveY-(divY[]*pasos).

    /**********PASOS DE 200**************/

    divX[0] = mueveX/pasos200; //div200 = pasos que tiene que dar el motor
    restosX[0] = mueveX -(divX[0]*pasos200);

    divY[0] = mueveY/pasos200; //div200 = pasos que tiene que dar el motor
    restosY[0] = mueveY -(divY[0]*pasos200);
    
    /*************PASOS DE 400*************/
    
    divX[1] = mueveX/pasos400;
    restosX[1] = mueveX -(divX[1]*pasos400);

    divY[1] = mueveY/pasos400;
    restosY[1] = mueveY -(divY[1]*pasos400);
    
    /*************PASOS DE 800**************/
    
    divX[2] = mueveX/pasos800;
    restosX[2] = mueveX -(divX[2]*pasos800);

    divY[2] = mueveY/pasos800;
    restosY[2] = mueveY -(divY[2]*pasos800);
    
    /*************PASOS DE 1600************/
    
    divX[3] = mueveX/pasos1600;
    restosX[3] = mueveX -(divX[3]*pasos1600);

    divY[3] = mueveY/pasos1600;
    restosY[3] = mueveY -(divY[3]*pasos1600);
    
    /***************PASOS DE 3200************/
    
    divX[4] = mueveX/pasos3200;
    restosX[4] = mueveX -(divX[4]*pasos3200);

    divY[4] = mueveY/pasos3200;
    restosY[4] = mueveY -(divY[4]*pasos3200);

    for (int r=0; r<5;r++){
      if (restosX[0] == 0){
        menorRestoX = 0;
        resolucionX = 0;
        break;
      }else if (restosX[r-1]>restosX[r]){
        menorRestoX = restosX[r];
        resolucionX = r;
      }
       
    }

      for (int t=0; t<5;t++){
		if (restosY[0] == 0){
        menorRestoY = 0;
        resolucionY = 0;
        break;
      }else if (restosY[t-1]>restosY[t]){
        menorRestoY = restosY[t];
        resolucionY = t;
      }
      
      }
 	
      int mayorpasos = divX[resolucionX]-divY[resolucionY];

      int flagPasos = 0;
      int steps = 0;      
      if (mayorpasos>0){
        proporcion = (float)divY[resolucionY]/divX[resolucionX];
        steps = divX[resolucionX];
        flagPasos = 0;
      }else{
        proporcion = (float)divX[resolucionX]/(divY[resolucionY]);
        steps = divY[resolucionY];
        flagPasos = 1;
          }

      movimiento(proporcion, steps, flagPasos);
     
		i++;
    Serial.println("UNA MAAAS");
    tiempoTotal -= tiempoCoordenada;
 //   enMarcha = 1;
    if (finaliza == 1){
      acabar = 1;
      digitalWrite(BEEPER_PIN, HIGH);
      return;
    }

  }
  enMarcha = 0;
  acabar = 1;

  posicionInicial();
  
  Serial.println("FINAAAAAL");
  
}


void moviendoX(int pasosVelocidad){
  combiMotorX(resolucionX);
  switch(pasosVelocidad){
    case 0: 
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(750); //MAXIMA
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(750);// MAXIMA
    break;

    case 1:
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(375);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(375);
    break;

    case 2:
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(187.5);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(187.5);
    break;

    case 3:
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(93.75);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(93.75);
    break;

    case 4:
      digitalWrite(X_STEP_PIN, HIGH);
      delayMicroseconds(46.875);
      digitalWrite(X_STEP_PIN, LOW);
      delayMicroseconds(46.875);
    break;
        
    }
}

void moviendoY(int pasosVelocidad){
   combiMotorY(resolucionY);
  switch(pasosVelocidad){
    case 0: 
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(750); //MAXIMA
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(750);// MAXIMA
    break;

    case 1:
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(375);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(375);
    break;

    case 2:
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(187.5);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(187.5);
    break;

    case 3:
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(93.75);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(93.75);
    break;

    case 4:
      digitalWrite(Y_STEP_PIN, HIGH);
      delayMicroseconds(46.875);
      digitalWrite(Y_STEP_PIN, LOW);
      delayMicroseconds(46.875);
    break;
        
    }
}
/******************************************LEER EL PRIMER VALOR****************************************/
float longArray (){

  myFile = SD.open("imprimir.txt");

  int totalBytes=myFile.size();
  Serial.println(totalBytes);
  String cadena="";
  
   if (myFile) {   
    
      //--Leemos una línea de la hoja de texto--------------
      while (myFile.available()) {
          
        char caracter=myFile.read();
          cadena=cadena+caracter;
          UltimaPocicion=myFile.position();
          if(caracter==10)//ASCII de nueva de línea
          {            
            break;
          }
      }
      //---------------------------------------------------
      myFile.close(); //cerramos el archivo
       } else {  
    delay(pausa);
    Serial.println("Error al abrir el archivooo");
  }     
        if (UltimaPocicion < 10){
         // Serial.print("El numero de lecturas de X es: ");
          numeroCoordenadas = (int)cadena.toFloat();
          return cadena.toFloat();
        }
         return cadena.toFloat();

}


/**********************************LEER VALORES************************************/

float leerValores(){
   myFile = SD.open("imprimir.txt");

  int totalBytes=myFile.size();
 
  String cadena="";
  
   if (myFile) {   
      if(UltimaPocicion>=totalBytes)   UltimaPocicion=0;
      myFile.seek(UltimaPocicion); 
      
      //--Leemos una línea de la hoja de texto--------------
      while (myFile.available()) {
          
        char caracter=myFile.read();
          cadena=cadena+caracter;
          UltimaPocicion=myFile.position();
          if(caracter==10)//ASCII de nueva de línea
          {            
            break;
          }
      }
      //---------------------------------------------------
      myFile.close(); //cerramos el archivo
       } else {  
    delay(pausa);
    Serial.println("Error al abrir el archivoooooooooooo");
   
  }
 
         return cadena.toFloat();

}

/*********************MOVIMIENTO*******************/

void movimiento(float razon, int numSteps, int flag) {
    float t = 0;
    float tant = 0;
    int r = 0;
    int y = 0;
    
   for(int paso = 0; paso <= numSteps; paso++){
     tant = t;
    r= (int)tant;
   
           t += razon;
          y =(int)t;
                   
      if((r != y) && (1<r)) {    
       
       moviendoX(resolucionX);
       moviendoY(resolucionY);
               
      }else{
     if (flag == 0){
      moviendoX(resolucionX);
     }else{
        moviendoY(resolucionY);
     } 
                    }
   }
}

void combiMotorX(int resolver){
   u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(2,35);
  u8g.print("pasos: ");
  
  switch(resolver){
    case 0:
      digitalWrite(MS1PINX, LOW);
      digitalWrite(MS2PINX, LOW);
      digitalWrite(MS3PINX, LOW);
      u8g.print(200);
    break;    

    case 1:
       digitalWrite(MS1PINX, HIGH);
       digitalWrite(MS2PINX, LOW);
       digitalWrite(MS3PINX, LOW);
       u8g.print(400);
    break;

    case 2:
      digitalWrite(MS1PINX, LOW);
      digitalWrite(MS2PINX, HIGH);
      digitalWrite(MS3PINX, LOW);
      u8g.print(800);
    break;

    case 3:
     digitalWrite(MS1PINX, HIGH);
     digitalWrite(MS2PINX, HIGH);
     digitalWrite(MS3PINX, LOW);
     u8g.print(1600);
    break;

    case 4:
      digitalWrite(MS1PINX, HIGH);
      digitalWrite(MS2PINX, HIGH);
      digitalWrite(MS3PINX, HIGH);
      u8g.print(3200);
    break;

    default:
    break;
  }
}

void combiMotorY(int resolver){
  u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(70,35);
  u8g.print("pasos: ");
  
  switch(resolver){
    case 0:
      digitalWrite(MS1PINY, LOW);
      digitalWrite(MS2PINY, LOW);
      digitalWrite(MS3PINY, LOW);
      u8g.print(200);
    break;    

    case 1:
      digitalWrite(MS1PINY, HIGH);
      digitalWrite(MS2PINY, LOW);
      digitalWrite(MS3PINY, LOW);
      u8g.print(400);
    break;

    case 2:
      digitalWrite(MS1PINY, LOW);
      digitalWrite(MS2PINY, HIGH);
      digitalWrite(MS3PINY, LOW);
      u8g.print(800);
    break;

    case 3:
      digitalWrite(MS1PINY, HIGH);
      digitalWrite(MS2PINY, HIGH);
      digitalWrite(MS3PINY, LOW);
      u8g.print(1600);
    break;

    case 4:
      digitalWrite(MS1PINY, HIGH);
      digitalWrite(MS2PINY, HIGH);
      digitalWrite(MS3PINY, HIGH);
      u8g.print(3200);
    break;
    
    default:
    break;
  }
    
}

/***Imprime la portada en el LCD***/ 
void imprime(){
  u8g.firstPage();  
  do {
      plotter();  //Imprime el titulo del proyecto PLOTTER XY
      autor();  //Imprime el nombre del autor
      unidades(); //Imprime las unidades de medida (mm)
      velocidad();  //Imprime la velocidad de los motores (RPM)
  } while( u8g.nextPage() );
  delay(3000);
}

void plotter(void) {
  u8g.setFont(u8g_font_profont22);
  u8g.drawStr( 5, 18, "PLOTTER XY");
  u8g.drawHLine(0,20,128);
  u8g.drawHLine(0,21,128);
}

void autor(void) {
  u8g.setFont(u8g_font_profont10);
  u8g.drawHLine(0,50,128);
  u8g.drawStr( 0, 60, "Gonzalo Calderon Lillo");
}

void unidades(){
  u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(2,32);
  u8g.print("Unidades: mm");
}

void velocidad(){
  u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(2,44);
  u8g.print("Velocidad: 200 RPM");
}

/***********************************/
void lasCoordenadas(void) {
   u8g.setFont(u8g_font_tpssb);
  u8g.setPrintPos(8,11);
  u8g.print("Coordenada Actual: ");
}

void coordenadaX(float coordenadaX){
  
  u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(2,25);
  u8g.print("X: ");
  u8g.print((coordenadaX- sumaEmergencia),4);
   

}
void coordenadaY(float coordenadaY){
u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(70,25);
  u8g.print("Y: ");
  u8g.print((coordenadaY - sumaEmergencia),4);
}

void proceso(int valor){
u8g.drawBox(20,40,valor,20);  
}

void imprimePosicionActual(){
  u8g.firstPage();  
  do {
  lasCoordenadas();
  coordenadaX(mueveteX);
  coordenadaY(mueveteY);
  combiMotorX(resolucionX);
  combiMotorY(resolucionY);
  u8g.drawHLine(0,15,128);
  u8g.drawHLine(0,14,128);
  u8g.drawHLine(0,40,128);
  u8g.drawHLine(0,39,128);
  u8g.drawVLine(61,15,24);
  u8g.drawVLine(62,15,24);
  u8g.drawVLine(63,15,24);
  tiempoEjecucion();
  unaDe();
  } while( u8g.nextPage() );
}

void unaDe(){
  
  u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(40,50);
  u8g.print(j);
  u8g.print(" de ");
  u8g.print(numeroCoordenadas);
}

void tiempoEjecucion(){
  int horas = tiempoTotal/3600;
  int minutos = tiempoTotal/60 - horas;
  int segundos = tiempoTotal - (horas*3600) - (minutos*60);
  u8g.setFont(u8g_font_profont10);
  u8g.setPrintPos(25,60);
  
  u8g.print(horas);
  u8g.print(" h ");
  u8g.print(minutos);
  u8g.print(" min ");
  u8g.print(segundos);
  u8g.print(" seg ");
 
}

void fin(void) {
 u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_courB14);
  u8g.drawStr( 0, 20, "FIN");
  } while( u8g.nextPage() );

  delay(5000);
}

/****************************MENU****************/

void uiStep(void) {
uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_helvB08);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
 
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  u8g.drawStr(25,0,"Factor de Escala");
  for( i = 1; i < MENU_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
      if ((menu_strings[i] == "4") && (digitalRead(uiEncoderPresionado) == LOW)){
     escala = 4;

      } if ((menu_strings[i] == "2") && (digitalRead(uiEncoderPresionado) == LOW)){
     escala = 2;

      } if ((menu_strings[i] == "1") && (digitalRead(uiEncoderPresionado) == LOW)){
    escala = 1;

      } if ((menu_strings[i] == "1/2") && (digitalRead(uiEncoderPresionado) == LOW)){
     escala = 0.5;

      } if ((menu_strings[i] == "1/4") && (digitalRead(uiEncoderPresionado) == LOW)){
     escala = 0.25;

      }
    }
    u8g.drawStr(d, i*h, menu_strings[i]);
  }
 
  uiEncoderSegundo = uiEncoderPrimero;
  if ( digitalRead(uiEncoderIzquierda) == LOW )
    uiEncoderPrimero = ENCODER_IZQUIERDA;

  else if ( digitalRead(uiEncoderDerecha) == LOW )
    uiEncoderPrimero = ENCODER_DERECHA;
  else if ( digitalRead(uiPulsador) == LOW ){
    uiEncoderPrimero = PULSADOR;
    seleccionarEscala = 1;

    
}else 
    uiEncoderPrimero = ENCODER_NO_PULSADO;
  
  if ( uiEncoderSegundo == uiEncoderPrimero )
    uiEncoder = uiEncoderPrimero;
  else
    uiEncoder = ENCODER_NO_PULSADO;   
}


void updateMenu(void) {
  if ( uiEncoder != ENCODER_NO_PULSADO && last_key_code == uiEncoder ) {
    return;
  }
  last_key_code = uiEncoder;
  
  switch ( uiEncoder ) {
    case ENCODER_DERECHA:
      menu_current++;
      if ( menu_current >= MENU_ITEMS )
        menu_current = 0;
      reescribirMenuEscala = 1;
      break;
    case ENCODER_IZQUIERDA:
      if ( menu_current == 0 )
        menu_current = MENU_ITEMS;
      menu_current--;
      reescribirMenuEscala = 1;
      break;
  }
}

void imprimeMenu() {  
  while(seleccionarEscala == 0){  
  uiStep();                                     // check for key press
    
  if (  reescribirMenuEscala != 0 ) {
    u8g.firstPage();
    do  {
      uiStep(); 
    } while( u8g.nextPage() );
    reescribirMenuEscala = 0;
  }

  updateMenu();                            // update menu bar
  }
}
