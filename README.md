# programacion_sobre_redes_tp_2
Segundo Trabajo Práctico de Programación sobre Redes

En este trabajo el objetivo es implementar, utilizando C++, el uso de _threads_ para el procesamiento de imágenes. El programa final deberá contar con variedad de filtros para imágenes que se ingresarán en formato ```ppm``` (Portable Pixmap), y la interfaz de usuario será por línea de comandos.

## Filtros implementados

### Plain

Todos los píxeles de la imagen se traslada a un único tono de gris.

### Black & White

Por cada píxel, se toma la suma de los valores de rojo, verde y azul y se saca un promedio entre los tres. Ese valor pasa a ser el que reemplaza a los tres anteriores, logrando así dibujar sólo valores monocromáticos entre 0 y 255.