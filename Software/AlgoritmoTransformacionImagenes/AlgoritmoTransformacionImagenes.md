# Algoritmo de Transformación de Imágenes

Para facilitar la comprensión y profundizar en la implementación de la conversión de la representación gráfica se ha estructurado 
en tres fases claramente diferenciadas en el esquema general:

## 1. Vectorización del mapa de bits:

Para explicar el algoritmo utilizado se realiza una breve introducción respecto al tratamiento digital de imágenes. Cuando se opera 
con imágenes digitales se tratan como 3 matrices, correspondientes a cada una de las bandas de los colores primarios RGB, típicamente 
de dimensiones de M filas y N columnas, en las que cada posición se trata de un píxel, unidad mínima de una imagen, que quedará 
determinado por p(x, y), de valor p y coordenada (x,y). 
 
A partir de esta imagen dada, el SW Inkscape gracias al motor de vectorizado de mapa de bits, desarrollado por 
Potrace, la transforma a un mapa vectorial donde los elementos que componen la escena quedan representados por formas 
. Hay que tener en cuenta que el propósito de vectorizar no es reproducir un duplicado exacto de la imagen original sino 
que el autotrazado proporciona un set de curvas interpretado a través de mapas de bits según su escala de luminosidad, 
siendo 0.0 el negro y 1.0 el blanco. Los trazos se generan a través de tres filtros distintos de salida como podemos encontrar 
en los ejemplos que facilita Inkscape:

### Escala de grises:

Efectúa la suma de las bandas de colores primarios, RGB, de cada píxel para decidir si se considera blanco o 
negro según un umbral establecido. Un ejemplo de transformación se representa en la Figura:

![sw1](https://user-images.githubusercontent.com/15126832/27788927-b5cd3e06-5fea-11e7-96ba-6574d1f25fb0.PNG)

### Pasos de luminosidad:

Emplea un algoritmo de detección de bordes que efectúa una búsqueda de isoclinas de contrastes similares. Un ejemplo
de transformación se representa en la Figura:

![sw2](https://user-images.githubusercontent.com/15126832/27788929-b5d02e54-5fea-11e7-859c-0e6a79c4fd92.PNG)

### Cuantificación de color:

A diferencia de las anteriores, en lugar de mostrar el contraste, busca los bordes donde los colores cambian decidiendo si 
es blanco o negro según la uniformidad del color. Un ejemplo de transformación se representa en la Figura:

![sw3](https://user-images.githubusercontent.com/15126832/27788928-b5cf20e0-5fea-11e7-8c46-07f69864f4cb.PNG)

Cuanto mayor es el número de trazos mayor es la precisión en la representación vectorial pero en contraposición, se aumenta el 
tiempo de procesamiento por la CPU y genera archivos de salida más pesados. Por ello, cabe resaltar la relevancia de alcanzar 
un compromiso entre la calidad y el volumen de datos mediante la herramienta de simplificación de trazas.

## 2. Coordenadas G-Code:

Una vez se ha vectorizado la imagen el siguiente paso consiste en la transformación a coordenadas. Para ello, se emplea el
lenguaje de programación más utilizado en las máquinas CNC desde su estandarización en el 2009 como ISO 6983, 
los G-Code. Los G-Code homogeneizaron las normas para datos de posicionamiento, movimiento de línea y sistemas de 
control de contorno. Con la utilización de estos se pretende unificar las técnicas de programación, fomentar la uniformidad 
y la interoperabilidad entre los programas de entrada y las CNC. De esta forma se consigue que los comandos simples sean 
capaces de controlar el proceso, la función, el tamaño y la precisión de la maquinaria.

Los G-Codes se obtienen gracias a una herramienta disponible para Inkscape mediante un plugin, Gcodetools, que realiza 
la transformación de los tramos a G-Code. Antes de realizar la conversión resulta fundamental situar los puntos de orientación 
que determinan el origen de coordenadas que la herramienta toma como referencia para obtener el plano con sus medidas en milímetros.
 
Con los trayectos de impresión en formato G-Code se observan dos motivos principales para desarrollar un algoritmo que 
desgrana estos comandos y los adapta a las prestaciones. Por un lado, la extensión .ngc del archivo que se obtiene 
a través del GCodeTools  es incompatible  con la librería SD. Por otro lado, el archivo contiene información innecesaria 
para la impresión del diseño que desemboca en un volumen de archivo un 75\% superior al estrictamente necesario logrado 
con el algoritmo. Por tanto, con la implementación de la adaptación de los G-Code se alcanzan las mismas prestaciones con 
un mejor rendimiento en la SRAM del microprocesador.

## 3. Algoritmo de adaptación de los G-Code:


El acondicionamiento de los comandos, por los motivos explicados en la sección anterior, se lleva a cabo mediante un 
algoritmo diseñado en C++. Para el desarrollo de este se utiliza la plataforma NetBeans para compilar, ensamblar, depurar, 
simular y documentar el código.

A continuación, se presenta el flujograma del algoritmo en la siguiente Figura:

![software 1](https://user-images.githubusercontent.com/15126832/27788926-b5cc8ea2-5fea-11e7-8a6b-811f9e003de7.png)

Siguiendo la figura previa, que representa la implementación, se aprecia que esta comienza con la configuración del archivo 
de entrada con el formato .ngc, una vez que lo ha reconocido se procede a la adaptación de los G-Code. Para ello, 
se analiza la colección de instrucciones
 y se observa que los únicos comandos necesarios para el desarrollo del sistema son los responsables del posicionamiento 
 que empiezan por: G01, G02 y G03, por tanto, se recorre el fichero de configuración para obtener el número de 
 líneas que empiezan por esta estructura que coincidirá con el número de pares de coordenadas de la imagen. Adicionalmente, 
 para la interpretación de los diferentes tramos cerrados, el archivo de configuración introduce un comando de la forma: 
 G00 Z5.000000. Gracias a este, es posible obtener ordenadamente todas las coordenadas de posicionamiento en cada trazo, 
 efectuando la ejecución de una función que devuelve el número de comandos de posicionamiento de X e Y, que deben ser 
 del mismo tamaño por el hecho de ser vectores de pares de coordenadas, así como los indicadores de final de tramo para 
 obtener el número de coordenadas total. Esta cifra es almacenada en la primera línea del archivo de salida .txt dado que es 
 pieza elemental en el Firmware. 
 
Una vez descartadas todas las líneas de comandos que no forman parte del sistema de posicionamiento, se procede al análisis de 
estas que cuentan con el siguiente formato:
 
**G0i 	X xx.xxxxxx 	Y yy.yyyyyy 	Z zz.zzzzzz …..} donde la i es 1, 2 o 3.**
 
Las coordenadas X e Y están claramente diferenciadas por un indicativo con su letra en mayúscula que permite efectuar una 
función en bucle que recorre el número total de coordenadas. A partir de estas, se obtiene el número de tipo float que prosigue 
a dicha letra, guardando cada coordenada en una línea del archivo .txt de forma alterna, es decir, una coordenada de 
X y otra de Y y así sucesivamente hasta que el método detecta el comando que indica el final de tramo que guarda un 5.000000, 
cifra que el Firmware traduce como flag para detectar el final de un trazo cerrado.
 
Aplicando este algoritmo se obtiene el archivo de configuración en un formato .txt que dota al Firmware de toda la información 
necesaria con un mejor rendimiento en la CPU.
