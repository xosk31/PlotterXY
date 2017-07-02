# PlotterXY
Diseño e implementación Hardware, Software y Firmware de un Plotter XY para Arduino Mega2560 + RAMPS 1.4.

El dispositivo grabador Plotter XY, completamente basado en Open Source, ofrece una solución de mínima inversión sin renunciar a prestaciones de calidad gracias a su carácter multifuncional que ofrece soluciones de impresión tanto para entornos profesionales como educacionales.

# Descripción del contenido:

* **Manual de usuario**.

## Hardware:

* **Esquema general:** esquema del módulo HW con todos los elementos que lo componen.
* **Descripción de elementos HW requeridos** (con sus respectivas referencias a datasheets).
* **Montaje paso a paso:** calibrado y conexionado acompañado de figuras ilustrativas.

 ### Requisitos Hardware
 - Arduino Mega2560.
 - RAMPS 1.4.
 - Full Graphic Smart Controller.
 - Pololu A4988.
 - 2 Motores paso a paso.
 - Servomotor.
 - Fuente de alimentacion.
 - 4 conmutadores.
 - Diodo láser.
 
## Software:
* **Esquema general:** esquema del módulo SW con todos los elementos que lo componen.
* **Algoritmo de transformación de imágenes:** 
    + Descripción de sus módulos: vectorización de mapa de bits, transformación de coordenadas a G-Code y algoritmo de adaptación de los G-Code.
    + Código y ejemplos de calibración.
    
 ### Requisitos SW:
 - Inkscape: https://inkscape.org/es/descargas/
 - Arduino IDE: https://www.arduino.cc/en/main/software
 - NetBeans: https://netbeans.org/downloads/
 - Algoritmo de transformacion de imagenes.rar.
 
## Firmware

* **Código**.

### Requisitos FW:
 
 - Firmware PlotterXY.ino.
