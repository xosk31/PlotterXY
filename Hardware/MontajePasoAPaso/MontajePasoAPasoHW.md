# Montaje Paso a Paso HW

Atendiendo a la descripción de los diferentes elementos HW así como de sus requisitos eléctricos, se presentan los procedimientos de calibrado 
y conexionado paso a paso acompañados de ilustraciones orientativas.

## 1. Conexión de la fuente de alimentación a la RAMPS:

La fuente de alimentación es configurada para que ofrezca un margen de entre 30V y 2A necesario para prevenir al sistema de corrientes 
elevadas que dañen los componentes. Esto es debido a que alimenta directamente a la RAMPS sobre sus entradas de 5A por lo que su voltaje 
y su corriente debe ser el de operación de esta: 12V y 1A respectivamente.

El esquema de la conexión se presenta en la siguiente Figura

## 2. Conexión de los drivers de control Pololu A4988 y los  motores paso a paso:

El motor paso a paso tiene una corriente y una alimentación máximas de 1.7A y 3.4V respectivamente determinadas por sus bobinas.
Por ello, se emplean los drivers de control de motores paso a paso A4988. Estos controladores requieren una tensión de alimentación 
lógica entre 3,5 y 5V que se conecta a través de los pines VDD y GND, y una tensión de alimentación del motor entre 8 y 35V para ser 
conectada a través de VMOT y GND. Para estas tensiones de alimentación se debe proveer a la placa de condensadores de desacoplo que 
deben ser capaces de entregar las corrientes esperadas.

El A4988 soporta limitación activa de corriente mediante un potenciómetro. Para lograr altas tasas de paso, la alimentación del 
motor debe ser mucho más alta de lo que sería capaz sin limitación de corriente activa. Dado que la fuente de alimentación 
suministra 12V se ajusta, mediante el potenciómetro, a un valor notablemente inferior para evitar cualquier sobrecalentamiento 
que dañe el motor. Para ello, se inserta el Pololu A4988 en la RAMPS, tal y como indica la siguiente Figura, 
y se conecta el voltímetro entre Vref del driver de control y masa para medir el voltaje en el pin y a partir de la limitación 
de corriente establecer el voltaje deseado.

----- Foto 4988 pinout

El datasheet del fabricante establece el valor máximo de limitación de corriente que atraviesa la 
bobina del motor como:

**I = (Vref)/(Rs x  8)**  ,donde Rs, resistencia de medición, tiene un valor de 50mΩ.

Como el controlador tiene una limitación máxima de 2A con disipadores y el motor de 2,5A, se establece un límite de corriente 
conservador para alargar la vida útil de los componentes. Por tanto, se fija este valor a 1A y se obtiene el valor del voltaje 
de referencia:

**Vref =  I x Rs x 8 = 1 x 0,05 x 8 = 0,4V**.

Una vez fijada la corriente que circula por los Pololu A4988, se procede a la inserción de los motores en la placa base. +
Los stepers bipolares tienen una única bobina por fase y requieren circuitos de control más complicados, un puente en H para 
cada fase como el que tiene el A4988. Los motores paso a paso bipolares tienen cuatro conductores, dos para cada bobina,tal y 
como se puede apreciar en la Figura. Para localizar las bobinas se mide la resistencia con un multímetro entre los cables, 
de manera que los que tienen una resistencia distinta de infinito representan una fase.

----Foto motor recortado

Para controlar el motor bipolar con el A4988, se conecta el cable paso a paso A a la salida de la placa 1A (cable azul), 
el cable paso a paso C a la salida de la placa 1B (cable rojo), el cable paso a paso B a la salida de la placa 2A (cable verde) 
y el cable paso a paso D a la salida de la placa 2B (cable negro).

El esquema de la conexión se presenta en la siguiente Figura:

Una vez están conectados y calibrados los motores y los drivers de control se verifica que cada pulso de entrada escalón 
corresponde a un micropaso del motor paso a paso en la dirección seleccionada por el pin DIR. El chip tiene tres 
entradas diferentes para el control de sus múltiples estados de energía: RST, SLP, y EN. Dado que el pin RST es flotante, 
se conecta al pin SLP adyacente en el PCB para llevarlo a nivel alto y activar el driver.

Adicionalmente, se comprueban los pines  MS1, MS2, MS3, encargados de la resolución de los pasos de acuerdo a la Tabla. MS1 y MS3 
tienen una resistencia pull-down interna de 100 kΩ y MS2 tiene una resistencia pull-down interna 50kΩ, de manera que dejar estos tres 
pines al aire corresponde con el modo de paso completo. Gracias al ajuste en la limitación de corriente realizada previamente, 
los niveles intermedios se mantienen correctamente y el motor no saltará micropasos.

El control de la resolución en la RAMPS se realiza mediante la inserción de jumpers, es decir, de forma analógica. Para exprimir 
al máximo el potencial del driver se realiza la conexión de los pines MS1, MS2 y MS3 del Pololu a pines digitales de la RAMPS 
consiguiendo adaptar los pasos del motor de forma controlada desde el Firmware como se muestra en la Tabla:

----Foto tabla pasos

## 3. Conexión de los 4 conmutadores:

Un final de carrera tiene 3 conexiones: alimentación o positiva, tierra o negativa y control, por el que se envía la señal para 
comunicar si se ha pulsado o no.
 
El esquema de la conexión se presenta en la siguiente Figura:

----FOTO PASO 3

La patilla negativa se conecta a tierra, la de alimentación al aire ya que es activo a nivel alto, por tanto, 
el pulso de entrada por el que se detecta el flanco de subida o de bajada es conectado a la tercera conexión, la de la señal, 
que se acopla a las entradas digitales de la RAMPS. Hay que prestar atención y colocar este último en los pines correspondientes 
a D2 y D3 para los límites de X y D18} y D19 para los de Y. Se debe conectar en estos pines ya que corresponden a las entradas 
digitales de las interrupciones HW, asíncronas a la ejecución del procesador, o producidas por dispositivos externos, es decir, se 
pueden producir en cualquier instante independientemente de lo que esté realizando la CPU.

## 4. Conexión del servomotor:

Un servomotor tiene 3 conexiones: alimentación o positiva representado por un cable rojo, tierra o negativa representado 
por un cable negro y control, por el que se envía la señal para comunicar el ángulo en el que se debe posicionar según el pulso. 

 El esquema de la conexión se presenta en la siguiente Figura:
 
 --fOTO PASO 4
 
 El cable negativo se conecta a tierra, la alimentación a 5V, regido por las especificaciones puesto que su voltaje debe estar 
 comprendido entre 4.8 y 6V con una corriente de 40 mA y, la señal de control a una entrada digital.
 
 ## 5. Conexión del diodo láser:
 
Un diodo láser tiene 3 conexiones: alimentación o positiva, tierra o negativa y control, por el que se envía la señal 
para comunicar si se debe encender o no.

El esquema de la conexión se presenta en la siguiente Figura:

La patilla negativa se conecta a tierra, la de alimentación al aire ya que si estuviese conectada siempre emitiría, 
por ser activo a nivel alto y por tanto, el pulso de entrada por el que se detecta el flanco de subida o de bajada es 
conectado a la tercera conexión, la de señal, que se acopla a las entradas digitales de la RAMPS.

## 6. Conexión del Full Graphic Smart Controller:

El conexionado de este módulo controlador se lleva a cabo a través de un adaptador que se conecta directamente a la RAMPS. Este 
acoplador concentra todas las salidas de los pines en dos buses de datos EXP1 para la pantalla y EXP2 para la SD, el encoder, 
el pulsador y el zumbador. La alimentación de ambos buses de datos es de 5V a 40 mA provenientes de los pines de salida Vcc y 
GND del Arduino Mega2560.

El esquema de la conexión se presenta en la siguiente Figura:

-- fOTO PASO 5




