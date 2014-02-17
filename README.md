grid-mtc
========

Utilerías para transformación de datos de y para la rejilla octagonal
de 1977 puntos usada en el Modelo Termodinámico del Clima (MTC), del
Centro de Ciencias de la Atmósfera de la UNAM.

Se generan ejecutables para convertir datos entre NETCDF y MTC.

nc2mtc 	   Extrae datos de un archivo NetCDF en cualquier tipo de
	   rejilla y por medio de interpolación lineal produce
	   los datos correspondientes en la rejilla del MTC.

mtc2nc	   Ejemplo de uso de esta biblioteca para convertir datos
	   del MTC al formato NetCDF de modo que pueda usarse en
	   aplicaciones que no soportan el formato MTC.

Las utilerías para el MTC son solamente una de las posibles
aplicaciones de esta biblioteca genérica de manejo de datos en rejilla
(grid). Es posible usar la biblioteca para transformaciones e
interpolaciones entre cualquier rejilla cuyos nodos tengan una
posición geográfica definida.

Autor:
M. en C. Alejandro Aguilar Sierra (asierra@unam.mx)
Centro de Ciencias de la Atmósfera, UNAM
