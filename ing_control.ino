
// ******************************************************** //
//                                                          //
// Control por asignacion de polos                          //
//                                                          //
// Noviembre 16 de 2022                                     //
//                                                          //
// Instrucciones                                            //
//                                                          //
// Modificar las ganancias                                  //
//    Precompensador Kp                                     //
//                                                          //
// ******************************************************** //

//--  Librerias --//

#include <SoftwareSerial.h>

//---------- Ley de control --------//

// u = Kp*(Vref) - K1*(Vcap1) - K2*(Vcap2)





//--Precompensador --//
  #define Kp 4.0933

//---------- Definiciones --------//

  unsigned long TIC = 0;
  unsigned long TS = 50;  // Muestreo 50 miliseg
  unsigned long TOC = 0; 
  unsigned long TC = 0;
   
//----------  Señales control --------//

  float R = 0;
  float Y = 0;
  float X1 = 0;
  float U = 0;
  int Ui = 0;

//----------  Señales observador --------//

//-- Estados obs --//
  float XeR1 = 0; // Estado estimado 1 en k, Xe1[k]
  float XeR2 = 0; // Estado estimado 2 en k, Xe2[k]
  float XeN1 = 0; // Estado estimado 1 en k+1, Xe1[k+1]
  float XeN2 = 0; // Estado estimado 2 en k+1, Xe2[k+1]

//-- Modelo matemático --//
  // Matriz A discreta
  float AD11 = 0.9058687;
  float AD12 = 0.0464846;
  float AD21 = 0.0464846;
  float AD22 = 0.9523533;

  // Matriz B discreta
  float BD1 = 0.0454312;
  float BD2 = 0.0022418;  

//---------- Escalamientos --------//
  #define mX 0.004882813
  #define bX 0
  #define mU 51
  #define bU 0

//---------- Definicion Pines --------//
  #define pR 2                  
  #define pX1 0 
  #define pX2 1                                    
  #define pU 10                   

//---------- Rutinia de inicio --------//

//--Entradas--//

void setup() {
  Serial.begin(9600);
}

//---------- Rutinia principal --------//

void loop() {
  TIC = millis();
  proc_entradas();                // Procesamiento de Entradas
  control();                      // Control
  proc_salidas();                 // Procesado de Salidas
  coms();                         // Comunicaciones
  espera();
}

//---------- Rutinas --------//

//-- Procesado de entradas --//
void proc_entradas(){
  // No se requiere leer X1 (Vc1) porque se estima con observador
  X2=analogRead(pX2)*mX+bX;
  R=analogRead(pR)*mX+bX;
  Y = X2;
}

//-- Control --//

// ############################################ //
// Introduzca AQUI la ley de control a aplicar  //
void control(){
  U = Kp*R - K1*XeR1 - K2*XeR2;  // Ley de control, donde la Kp es la calculada //
  if(U >= 5.0) U = 5.0;                       
  else if(U < 0) U = 0;
}
// ############################################ //

//-- Procesado de salidas --//
void proc_salidas(){
  Ui=int(U*mU+bU);
  analogWrite(pU, Ui);
}

//-- Comunicación con monitor --//
void coms(){   
  Serial.print(R);    // Referencia
  Serial.print(", 0, 6, ");
  Serial.println(XeR2);  // Salida
}

void espera(){   
  TOC = millis()-TIC;
  TC = TS - TOC;
  if (TOC < TS) delay(TC);        
}
