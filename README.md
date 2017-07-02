# PlotterXY
Diseño e implementación Hardware, Software y Firmware de un Plotter XY para arduino Mega2560 + RAMPS 1.4

El dispositivo grabador Plotter XY, basado totalmente en Open Source, ofrece una solución de mínima inversión sin renunciar a prestaciones de calidad gracias a su carácter multifuncional que ofrece soluciones de impresión tanto para entornos profesionales como educacionales.

* **Manual de usuario** (disponible en este repositorio).

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
 - Inkscape.
 - Arduino IDE.
 - NetBeans.
 - Algoritmo de transformacion de imagenes.rar (disponible en este repositorio).
 
## Firmware

* **Código**

### Requisitos FW:
 
 - Firmware PlotterXY.ino (disponible en este repositorio).
