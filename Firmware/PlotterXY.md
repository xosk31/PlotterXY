# Firmware Plotter XY

El Firmware de este proyecto, Plotter XY, es la pieza que se comporta como enlace en la integración del sistema 
ya que es el encargado de proveer a los distintos elementos HW el conjunto de datos e instrucciones para su correcto 
funcionamiento. Complementariamente, dota al sistema de una interfaz que permite al usuario introducir configuraciones necesarias 
para convertir el fichero de datos .txt en instrucciones de control y coordinación para realizar la impresión.
 
Plotter XY, actúa como SW de solo lectura en el microcontrolador Atmel ATmega2560 , datasheet, que reside en la flash 
 ROM de la placa de desarrollo Arduino Mega 2560, lo que permite su actualización mediante el borrado y reescritura. Los recursos 
 de memoria utilizados se pueden consultar en la Tabla:
 
 
 
 Tal y como se observa en la tabla tabla:recursosmemoria, el Sketch utiliza 42116B del espacio de almacenamiento de programa, 
 las variables globales emplean 1748B y quedan libres 6444B para las variables locales. La EEPROM no es necesaria ya que no se precisa 
 de configuraciones no volátiles previas. Adicionalmente, el tiempo de lectura y escritura es muy superior al de la SRAM lo que 
 conlleva una ralentización innecesaria en la agilidad del Plotter XY.
 
Cabe destacar el alto rendimiento del Firmware dado que en ningún caso se supera el cuarto de su capacidad evitando así posibles 
fallos en el procesamiento de los datos. Gracias a este tratamiento de datos se consigue una fluidez en el sistema que implica un 
tiempo de ejecución de punto a punto de la impresión en una media de 250ms por coordenada.

## Flujograma Ploter XY

A continuación, se muestra el flujograma del Plotter XY, que ha sido diseñado específicamente para este proyecto a partir de cuatro 
librerías, tres de ellas OpenSource para entornos Arduino IDE: \textit{Servo,  ug8lib y SD} y una adicional diseñada durante este 
proyecto para el control de los motores. Todas ellas son detalladas en la descripción del programa y adicionalmente, en la 
documentación alojada en GitHub se especifican los pines de conexión a la RAMPS para que el Firmware trabaje adecuadamente 
tanto en las lecturas como en las escrituras analógicas o digitales de datos, comunicaciones SPI e I$^{2}$C y las entradas 
digitales de las interrupciones.



## Descripción de módulos:

El Firmware, como se observa en el flujograma, es estructurado modularmente donde cada módulo es responsable de una parte del proceso 
de impresión. A continuación, se detallan sus funciones:

### SetUp:
Realiza las configuraciones previas a la ejecución del Firmware de los diferentes elementos HW: el controlador de tarjeta de 
memoria SD, los drivers de control de los motores paso a paso, las interrupciones producidas por los conmutadores, el encoder, 
el servo, el zumbador, el láser y la pantalla LCD. En él se establecen las declaraciones de los pines como entradas o salidas, 
así como de apertura del puerto serie de comunicación con el ordenador empleado para testear y depurar.

### Interrupciones:
Se encarga de asociar una ISR (Interruption Service Rutine) a la que se llama cuando se presiona alguno de los cuatro conmutadores 
que están dispuestos en los extremos de los ejes X e Y. De este modo, si en el movimiento del motor se pulsa cualquiera de ellos, 
el sistema entrará en parada de emergencia para evitar dañar los componentes. Las interrupciones son asíncronas a la ejecución del 
procesador, es decir, se pueden producir en cualquier momento independientemente de lo que esté haciendo el controlador. La 
función “parada de emergencia” deshabilita los motores, el láser, el servo, activa el zumbador durante un segundo para alertar 
al usuario, cierra la comunicación con la tarjeta SD y activa el flag stop para evitar que se lea el fichero nuevamente.

### ¿Tarjeta SD detectada?:
Es el módulo de decisión que detecta si esta insertada la tarjeta SD, activo a nivel bajo. En caso de no detectarse la presencia 
de la SD, desemboca en una parada de emergencia que supone la finalización de la impresión del diseño.

### Activar motores, servo y láser:
Se encarga de permitir el paso de corriente a los motores, activos a nivel bajo, al servo y al láser.

### Posición Inicial:
Encargado de situar los motores en el origen de coordenadas establecido para el sistema de forma que presionan los conmutadores 
mínimos del eje X e Y. Se decide optar por este punto como referencia para permitir el máximo plano de impresión de 31x39cm.

### Abrir el fichero:
Establece la comunicación serie con la tarjeta SD. Cabe destacar que se modifica la librería para cerrar automáticamente la 
conexión, desmontar la tarjeta y cerrar los ficheros que haya abiertos para impedir la pérdida de datos cuando se extrae la SD.

### Guardar el número de coordenadas:
Se produce la lectura de la primera línea del archivo de configuración que indica el número de coordenadas de X e Y. Este valor 
resulta esencial ya que permite realizar una lectura mediante un número de iteraciones igual a este cerciorándose de que todas 
las coordenadas han sido procesadas correctamente.

### Ajustar la escala:
Es un menú interactivo que se muestra en la pantalla LCD y se maneja mediante el encoder que permite desplazarse por él y 
seleccionar la escala deseada haciendo uso del pulsador que confirma la operación. Esta función del sistema permite al 
usuario ajustar el tamaño de su imagen, tal y como desee, en un factor de 4, 2, 1, 1/2, 1/4


Los siguientes módulos se sitúan en un bucle while que itera las coordenadas hasta que se procesan la totalidad 
de valores que conforman la imagen. Para la comprensión de este bloque resulta esencial explicar la cantidad de pasos que 
deben realizar los motores para alcanzar el plano deseado.

Este algoritmo se basa en la resta de dos vectores, de forma que si el motor lee la coordenada (4, 8) y se encuentra en la
(2,12), estos deben moverse 2 unidades en el sentido positivo del eje X y 4 unidades en el sentido negativo del eje Y. Para el 
sistema de trazado no es suficiente con moverse de una coordenada a otra, sino que tiene que trazar la línea recta que una 
ambos puntos. Para ello se establece una proporción de movimiento entre los motores. Siguiendo con el ejemplo, como se desplaza 
2 unidades el eje X y 4 el eje Y, la relación será que el motor del eje Y tiene que realizar el doble de pasos que los que debe 
realizar el eje X y para ello, hacer la relación entre ambos ejes. 

Partiendo de esta introducción se procede a la explicación de los módulos que hacen posible estos mecanismos y determinan 
la cantidad de pasos que deben desplazarse los motores.

### Guardar coordenada:
Realiza el salvado de la coordenada anteriormente leída que actúa como sustraendo en la resta de vectores, esto significa, 
la posición en la que se encuentran los motores antes de realizar el siguiente movimiento. Cuando se produce la primera 
iteración este valor almacena el (0,0) dado que se encuentra en el origen del sistema de referencia del mecanismo.

### Leer valores:
Efectúa la lectura de la siguiente coordenada del fichero de la imagen, es decir, el minuendo de la resta de vectores 
para determinar la posición que se quiere alcanzar en la iteración.

### Actualizar LCD:
En la pantalla LCD se muestran las características de la posición actual: las coordenadas en milímetros que se van a alcanzar 
durante la iteración, la resolución en micropasos que se va a emplear en ambos motores, el número de coordenadas y el tiempo 
restantes. Resulta intuitivo y dota al sistema de independencia para indicar en qué estado se encuentra el grabado.


El siguiente paso a realizar, consiste en calcular la diferencia, entre los valores aportados como minuendo o leído y sustraendo
o guardado, que corresponde a los milímetros que distan de una coordenada a otra, es decir, el trayecto que debe realizar 
el elemento de grabado. Este valor es transformado en el bloque “proporción de pasos X e Y” en micro pasos que los motores deben
realizar para alcanzar, con la mayor precisión posible, el trazado deseado. A continuación, se presentan los casos particulares 
que el sistema puede encontrar: 

  * **Primera iteración:**
      en este caso el valor leído es la diferencia directamente. Por tanto, se le añade un margen de seguridad que desplaza 
      la primera coordenada sobre el eje de referencia: escala x 10mm. Este traslado se efectúa para prevenir al sistema de 
      un posible fallo en la impresión cuando se sitúan los puntos de orientación que determinan el origen de coordenadas.
      
  * **Fin de trazo:**
     tal y como se ha explicado previamente, el archivo .txt contiene entre las coordenadas un indicativo que determina el final 
     de un tramo cerrado, por tanto, cuando el programa lo detecta se activa un flag que paraliza los motores dando valor 0 a movimiento.

  * **Coordenadas intermedias:**
     sucede repetidamente en los pasos intermedios de un tramo cerrado. En ellos se realiza la diferencia entre el valor actual 
     o leído y el anterior o guardado.
     
### Control de Servo y Láser:
Se actúa directamente sobre el control de los elementos de grabado para indicarles cuándo deben grabar dependiendo de si en el 
paso previo se ha detectado el flag que indica el final de un trazado. En el servo se establecen los grados que debe desplazarse 
siendo 90º para bajar y 105º para subir y en el láser se activa un encendido lógico HIGH, o un apagado LOW.

### Sentido de giro del motor:
A partir de la diferencia calculada previamente, el sistema se encuentra con dos posibilidades para ir de la coordenada origen 
a la coordenada destino. Dependiendo del resultado se establece el sentido de giro de los motores: en el sentido horario el 
movimiento se realiza en el sentido positivo y en el antihorario en el negativo.

### Resolución de micropasos:
Tal y como se ha expuesto previamente en la sección de HW, el driver de control de motores paso 
a paso A4988 permite la disposición de los micropasos. Para decidir la resolución de los micropasos se efectúa el siguiente proceso:

* **Primero:** 
se mide el avance del motor en cada paso según su resolución, es decir, un step; teniendo en cuenta que en una vuelta 
completa con cualquier resolución recorre 36,78mm. Las medidas obtenidas se recogen en la siguiente Tabla:
    
* **Segundo:** utilizando los valores previos, se realiza la división entre el valor de la diferencia o desplazamiento que deben 
abarcar los motores y la distancia que avanza el motor con cada resolución. A partir de esta relación se obtiene el resto 
que conforma el valor más significativo para decidir qué resolución es más precisa. Cuanto más próximo a cero sea, más 
riguroso será el movimiento. Finalmente, se ejecuta un bucle que realiza la siguiente Ecuación con cada micropaso y 
se obtiene la resolución óptima.
    
**desplazamiento / distancia del paso ---> resolución óptima**
    
* **Tercero:** la última fase consiste en enviar la resolución óptima al driver A4988 de acuerdo a la codificación explicada previamente
    
### Proporción de pasos X e Y: 
Como se cita anteriormente, el sistema traza una línea recta que une la coordenada anterior dibujada con la siguiente. Para ello, 
se establece una proporción entre los pasos del motor X e Y, que permite conocer el movimiento de uno en relación al otro. Como esta 
debe ser positiva para que el Firmware la interprete correctamente, primero se determina qué motor de los dos tiene un mayor número 
de pasos y posteriormente la relación.

### Movimiento de los motores: 
Configurados ya los elementos y parámetros necesarios, se procede al trazado de las líneas rectas por medio del movimiento de 
los motores a una velocidad de 200RPM ya que resulta la velocidad óptima para el trazado que puede ser consultada en el 
apartado VelocidadOptimaMotores dentro de la carpeta Firmware. El movimiento se efectúa partiendo de la razón entre pasos 
de los motores, el número de pasos mayor, X o Y, y un flag que activa un motor u otro dependiendo de cuál de los dos tiene 
más pasos. El número de steps se establece a partir de la siguiente Ecuación:

 **N = desplazamiento en mm / avance por paso en mm**
 
 Cabe destacar que la operación se realiza por acumulación y redondeo, es decir, en cada iteración se suma el valor de la 
 proporción y se descarta la parte decimal. Si el valor entero difiere respecto al anterior se mueven los dos motores, en 
 caso contrario sólo se moverá uno. Con esta implementación se produce un error debido a esta aproximación, ya que si, por 
 ejemplo, se tiene una relación de 0.1 periódico y 9 pasos a realizar por el motor con mayor número de pasos, el motor con 
 menor número de pasos no ejecutará ninguno; por tanto, como la resolución mayor es en la cual el motor gira más grados, 
 1,8º, se obtiene un error máximo de 183,9 μm.
 
 ### Finalizado: 
 Este bloque de decisión verifica si se han leído todas las muestras del fichero .txt. En caso negativo prosigue con el 
 bucle WHILE y en caso afirmativo finaliza la ejecución, acciona el apagado del HW del sistema y vuelve al bloque 
 "Tarjeta SD detectada".
 
    
    










