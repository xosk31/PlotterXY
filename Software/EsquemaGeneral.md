## Esquema General SW

Una vez calibrado y conectado el HW se procede a detallar las implementaciones SW llevadas a cabo. En primer lugar, 
se presenta un esquema general de los requisitos específicos del SW y seguidamente, 
se ofrecerá información detallada del algoritmo desarrollado para el tratamiento de imágenes.

El SW de este proyecto es una pieza clave ya que dota al sistema de los datos y las configuraciones necesarias para que el Firmware, 
PlotterXY, sea capaz de interpretarlos y coordinarlos con el HW. La implementación consiste en la elaboración de un 
algoritmo de transformación de imágenes, partiendo de cualquier formato, que convierte la información de la ilustración en valores 
de coordenadas X e Y que se almacenan en un fichero de datos con extensión \textit{.txt}. Para la visualización global de la 
implementación se elabora la siguiente Figura:
