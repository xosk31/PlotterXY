## Descripción de Módulos HW

Una vez tomada la decisión acerca de los elementos HW que se van a utilizar para llevar a cabo el proyecto se procede a analizar sus especificaciones.

# Estructura

La estructura se adquiere a la empresa Makeblock, encargada de generar kits de construcción asequibles y robustos orientados a un entorno educativo con la intención de sentar unas bases elementales en mecánica, electrónica y programación. La estructura es de aluminio reforzado que aporta la firmeza y estabilidad necesaria, esto unido a su sencillez en el ensamblado mediante raíles roscados con agujeros, hace que los montajes sean muy sencillos y se adapten prácticamente a cualquier proyecto. Estas cualidades lo convierten en la solución idónea para el presente diseño. En la Figura se presenta la estructura armada; el proceso de montaje de la armadura puede consultarse en la siguiente referencia http://www.instructables.com/id/How-to-make-a-XY-plotter-with-Makeblock.

![medidasplotterMakeblock](https://user-images.githubusercontent.com/15126832/27785186-191539fe-5fdd-11e7-85f5-23771155e633.png)

# Arduino Mega2560

La placa de desarrollo Arduino Mega2560, presentada en la Figura, otorga las capacidades de procesamiento gracias a su microcontrolador que permite al prototipo el funcionamiento autónomo conectando su puerto USB a un PC para cargar el Firmware en la misma. 

Sus especificaciones se pueden consultar en la siguiente Tabla:

# RAMPS 1.4

La RAMPS 1.4, presentada en la Figura, se emplea como interfaz de Arduino Mega para adaptar toda la electrónica necesaria y conseguir una RepRap. Es en ella donde se realiza el conexionado con los elementos HW, recogidos en el directorio Montaje paso a paso.

Sus especificaciones se pueden consultar en la siguiente Tabla:

# Fuente de alimentación DC FAC 364C

Como fuente de alimentación para la calibración se emplea una fuente diseñada para entornos industriales como medida de seguridad para no dañar los componentes, la FAC-364C. Esta fuente de alimentación dispone de una salida fija o ajustable de corriente o tensión. De esta manera se limita la corriente o tensión a un determinado nivel para evitar interrupciones en el circuito de prueba. El potenciómetro giratorio que regula tales parámetros está equipado con una doble función: un ajuste fino y otro grueso, aportando una mayor precisión en el ajuste de la corriente y de la tensión. Cabe destacar que tras el proceso de calibración se puede sustituir por una fuente de alimentación estándar  de 12V y 1A. 

Sus especificaciones se recogen en la siguiente Tabla:

# Pololu A4988

El A4988, presentado en la Figura, es un driver de control de motores paso a paso que incorpora un control de corriente ajustable con un potenciómetro para la salida de corriente máxima. De esta forma, es posible aplicar tensiones superiores a la tensión nominal del motor para así lograr mayores tasas de paso y un sistema de protección de sobrecalentamiento térmico: cierre por baja tensión y protección por sobrepico de corriente. Está diseñado para operar con motores paso a paso bipolares en cinco resoluciones diferentes: paso completo, medio, cuarto, octavo y dieciseisavo con una capacidad de salida de hasta 35V y +/-2A. 
 
El driver cuenta con un traductor, clave para la implementación, ya que introduciendo un pulso en la entrada STEP el motor comenzará a moverse automáticamente, sin que haya que iniciar ninguna secuencia para su encendido. Durante la operación escalonada, el control de corte en el A4988 selecciona automáticamente el modo de degradación, lento o mixto. En el modo de degradación mixta, el dispositivo se ajusta inicialmente a un decaimiento rápido para una proporción del tiempo de desconexión fijo, seguidamente, a una decadencia lenta en la que ya permanecerá el resto del tiempo. Gracias a su control de corriente reduce el ruido en el motor, aumenta la precisión en los pasos y la disipación de potencia durante el funcionamiento de PWM debido a su circuito de control de rectificación síncrono.
 
La protección del circuito incluye: apagado térmico con histéresis, bloqueo de subtensión (UVLO) y protección de corriente de cruce. Sus encoders adaptados a los motores permiten una verificación de la posición en cualquier instante de tiempo.

Sus especificaciones se pueden consultar en la siguiente Tabla:

# Motor Nema 17

El motor paso a paso Nema 17, modelo bipolar 42BYGHW609 producido por National Instruments, proporciona un control de movimiento muy preciso, simple y rentable. Su movimiento se realiza en incrementos pequeños y precisos de 1,8º a 200º (pasos/revolución) sin escobillas, sin mantenimiento y sin dispositivos de retroalimentación costosos. 
 
EL motor Nema 17 está basado en el mismo fenómeno que los motores de corriente continua, es decir, las fuerzas ejercidas por un campo electromagnético creadas al hacer circular una corriente eléctrica por los bobinados producen el movimiento del motor en función de la tensión que se aplica. La bobina, denominada estator, se mantiene en una posición mecánica fija, y en su interior, bajo la influencia del campo electromagnético, se coloca otra bobina, llamada rotor, recorrida por una corriente y capaz de girar sobre su eje al excitar el estator, creando así los polos Norte-Sur que provocan la variación del campo magnético generado. La respuesta del rotor consiste en seguir el movimiento de dicho campo, es decir, orientar sus polos Norte-Sur hacia los polos Sur-Norte del estator. En el momento en el cual el rotor alcanza esta posición de equilibrio, el estator cambia la orientación de sus polos y trata de buscar la nueva posición de equilibrio. Manteniendo esta situación de manera continua, se logra un movimiento giratorio y continuo del rotor que se transmite al giro del eje del motor, dando lugar a una transformación de energía eléctrica en mecánica.
 
Los pasos se controlan por el cambio de dirección del flujo de corriente a través de las bobinas, de manera que ofrece la posibilidad de controlar: el desplazamiento del rotor en función de las tensiones que se aplican a las bobinas, el número de pasos por vuelta y la velocidad del motor. El motor está fabricado para trabajar en un rango de frecuencias determinado por el fabricante y sobrepasado dicho rango se pierde la sincronización.

La elección de este tipo de motor reside en las posibilidades de movimiento de forma precisa, el control de la velocidad angular y su torque máximo a bajas velocidades lo que resulta perfecto para este proyecto ya que cumple con los requisitos de baja velocidad y alta precisión. Por otra parte, el principal inconveniente se produce debido a la baja eficiencia que produce un calentamiento en el motor, aspecto que se ha solucionado gracias al driver A4988 explicado previamente.

Sus especificaciones se pueden consultar en la siguiente Tabla:

# Servomotor 9G

Un servomotor de corriente continua consiste en un motor eléctrico que permite ajustar tanto la velocidad como la posición del eje. Para controlar estos movimientos se debe aplicar un pulso eléctrico mediante PWM cuya duración determina el ángulo de giro. Cabe destacar que los servomotores no giran 360º, únicamente 180º hacia la izquierda o hacia la derecha.
 
El servo, como se puede apreciar en la Figura, además del motor eléctrico que genera el movimiento, cuenta con un sistema de regulación mediante engranajes que actúa directamente sobre el motor, un sistema de regulación, un sistema de control encargado de controlar los pulsos y un potenciómetro para conocer el ángulo en el que se encuentra el servo. 
 
La función del servomotor consiste en alzar o descender el elemento de grabado fijo. Para el calibrado de este se realizan una serie de medidas hasta encontrar los grados adecuados en los que debe moverse para producir el grabado sin dañar el elemento. Del análisis de las medidas recogidas se concluye que 90º es el valor óptimo para grabar y 105º para dejar de hacerlo.

Sus especificaciones se pueden consultar en la siguiente Tabla:

# Full Graphic Smart Controller

Consiste en un pequeño controlador para RAMPS que contiene un lector de tarjetas SD, un encoder, un pulsador, un zumbador y una pantalla LCD de matriz de 128x64 píxeles, tal y como se aprecia en la Figura.

Sus especificaciones se pueden consultar en la siguiente Tabla:

# Diodo láser

Los diodos Láser, como se aprecia en la Figura, a diferencia de los diodos LED producen una luz coherente que consigue realizar un guiado de la luz, preferiblemente en una sola dirección, monocromática y monofásica. Los fotones emitidos por un láser poseen longitudes de onda muy cercanas entre sí. Para conseguir la emisión láser, el diodo cuenta con una unión PN de grandes dimensiones que favorece la emisión estimulada generando una potencia óptica de emisión elevada.

Como elemento de simulación de grabado se utiliza un diodo láser de menor potencia, 5mW, ya que para grabar en una superficie se necesitaría uno de 1W pero el conexionado se realiza de igual forma en ambos casos.

Sus especificaciones se pueden consultar en la siguiente Tabla:
