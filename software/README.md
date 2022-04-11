# Compilación

Ejecutar `make`.

# Ejecución

Ejecutar `bin/main_exe` pasando como argumentos `[tipo algoritmo] [semillas] [ficheros]`.

Ejemplo: `bin/main_exe LocalSearch 10,20,30,40 data/GKD-b_20_n50_m15.txt,data/GKD-b_32_n125_m12.txt`.

# Resultados 

`bin/main_exe` devuelve por la salida estándar el nombre del fichero seguido por cada semilla introducida la dispersión de la solución obtenida por el algoritmo seguido todo esto por el tiempo medio de ejecución del algoritmo de todas las semillas.

El script `launch.sh` ejecuta cada uno de los charts bajo la carpeta data y para cada uno de las semillas indicadas en el script. Escribe el resultado de las operaciones a los ficheros `resultados_{greedy,localsearch}.dat`