
# Entrega Final Proyecto IA

**Nombre:** Cristian Soriano \
**Rol:** 201954014-9

## Instrucciones de compilación y limpieza

Para compilar el programa, debe usar el siguiente comando en su terminal UNIX:

```{console}
make
```

Para limpiar los archivos generados por este, puede usar el siguiente comando:

```{console}
make clean
```

## Instrucciones de ejecución

Para correr el programa, puede usar el siguiente comando:

```{console}
./SGP
```

Sin embargo esto no hará nada e imprimira los argumentos con sus respectivas flags y modo de uso:

```{console}
Usage:
	-i <instance path>
	-t <# de iteraciones (default 20)>
	-m <modo de HC: mejor-mejora (default)|alguna-mejora>
	-g <modo de greedy: random (default)|libertad|bruto>
	-r <# de restarts (default 0)>
	-nr <# de iteraciones antes de hacer un restart (default 0) 
	-pswap <probabilidad de hacer un swap con jugadores de la misma semana (p in [0,1], default 0.3)
	-pins <probabilidad de insertar un jugador aleatorio o el que corresponde cuando usamos libertad como modo de greedy (p in [0,1], default 0.8)>
```

El flag `-i` nos permite ingresar el path de donde se encuentra la instancia que queremos ejecutar en específico, el resto, aunque bastante descriptivos por el CLI, nos deja definir una cantidad de iteraciones (`-r`), la heurística de Hill Climbing (`-m`), el tipo de algoritmo greedy que queremos usar (`-g`) y la cantidad de restarts que deseamos usar (`-r`), la cantidad de iteraciones antes de hacer un restart (`-nr`), la probabilidad de hacer un swap con jugadores de la misma semana (`-pswap`) y la probabilidad de insertar un jugador aleatorio cuando estamos usando el greedy que usa los grados de libertad (`-pins`). Cabe mencionar que esta lista de parametros, a excepción del path de la instancia, son opcionales ya que cada uno tiene un valor por defecto.

Un caso de uso podría ser el siguiente:

```{console}
./SGP -i Instancias/i-4-4-5.txt -t 30 -m alguna-mejora -g libertad -r 2
```

Lo primero que hace el código es ver que tipo de algoritmo greedy se va a usar, luego calcula la evaluación de la solución construida y pasa a realizar Hill Climbing según se le indique. Una vez realiza HC calcula el porcentaje de mejora según la formula $(eval_{Greedy} - eval_{HC}) / eval_{Greedy}$, por lo que un 100% de mejora es ideal.

En `script.py` se encuentra el código de python que genera una cierta cantidad de parámetros de manera aleatoria y busca aquella configuración que de mejores resultados. Para ejecutar este script debería tener instalado python 3.x y para correrlo lo puede hacer mediante el shell script de la siguiente manera:

```{console}
./tunning.sh <instance_path> <n_parametros>
```

Lo primero que hace este script es compilar el programa y después correr el código de python para ejecutar la instancia especificada con multithreading con los `n_parametros` especificados.

El caso de uso que se ve en el informe se realizo mediante la ejecución de este comando con todas las instancias disponibles y con 20 parametros generados, esto se debería ver algo así:

```{console}
./tunning.sh Instancias/i-8-4-10.txt 20
```

Dentro de la carpeta `src/` se encuentras los archivos de implementación en conjunto con el programa principal (`main.cc`), por lo que es en estos archivos donde se van a encontrar más comentarios con respecto a qué hace cada función.
