# PlotterXY
Diseño e implementación Hardware, Software y Firmware de un Plotter XY para arduino Mega2560 + RAMPS 1.4

El dispositivo grabador Plotter XY , basado totalmente en Open Source, ofrece una solución de mínima inversión sin renunciar a prestaciones de calidad gracias a su carácter multifuncional que ofrece soluciones de serigrafía tanto para entornos profesionales como educacionales.
 
## Requisitos HW:
 
 - Arduino Mega2560.
 - RAMPS 1.4.
 - Full Graphic Smart Controller.
 - Pololu A4988.
 - 2 Motores paso a paso.
 - Servomotor.
 - Fuente de alimentacion.
 - 4 conmutadores.
 - Diodo láser.
 
## Requisitos SW:
 
 - Inkscape.
 - Arduino IDE.
 - NetBeans.
 - Algoritmo de transformacion de imagenes.rar (disponible en este repositorio).
 
## Requisitos FW:
 
 - Firmware PlotterXY.ino (disponible en este repositorio).
 
## Puesta en marcha:
 
**1.** Descargar el firmware Plotter XY y el algoritmo de transformación de imágenes.
     
**2.**  Descargar los entornos de desarrollo Arduino Ide, Netbeans e Inkscape.
 
**3.**  Cargar el firmware Plotter XY en la placa Arduino Mega2560 por medio de Arduino Ide
 
**4.** Llevar a cabo el montaje del sistema HW segun los pasos de montaje del HW (disponible en este repositorio)
           -
**5.** Formatear la tarjeta SD a un formato FAT16 o FAT32.
 
**6.** Modelar el diseño de la figura a serigrafiar por medio de Inkscape.
 
**7.** Generar los G-Code de la imagen. Tutorial: https://www.shapeoko.com/wiki/index.php/User:BHSPitMonkey/Inkscape\_and\_Gcodetools\_Tutorial
 
**8.** Aplicar el algoritmo de transformación de imágenes mediante NetBeans importando el Algoritmo de transformación de imagenes.rar
 
**9.** Copiar el archivo de salida “serigrafia.txt” en la tarjeta SD
 
**10.** Conectar la estructura a la red eléctrica.
 
**11.** Insertar la tarjeta SD en el lector de tarjetas de la RAMPS.
