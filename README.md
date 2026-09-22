# proyectoSO - Sistemas Operativos

Este proyecto está basado en el paper **“Analyzing the Overload Behavior of a Simple Web Server”** de N. Provos, C. Lever y S. Tweedie.

El artículo estudia cómo un servidor web basado en señales POSIX Real-Time maneja eventos de entrada/salida y qué ocurre cuando la carga aumenta. En particular, compara `sigwaitinfo()`, que obtiene una señal por llamada, con `sigtimedwait4()`, que permite recuperar varias señales pendientes en una sola llamada.
La simulación se enfoca en el nuevo uso de `sigtimedwait4()` propuesto en el paper: utilizar la cantidad de señales recuperadas para detectar situaciones de sobrecarga y controlar la aceptación de nuevas conexiones.

## Simulación

La simulación en C representa de forma simplificada una cola de eventos asociados a conexiones.

Se simulan dos comportamientos:

- extracción individual de eventos;
- extracción por lotes.

Además, se genera carga creciente y se utiliza un promedio móvil para detectar situaciones de overload.

Cuando se detecta sobrecarga, la simulación representa el rechazo de nuevas conexiones evitando agregar sus eventos a la cola.

Finalmente, se comparan dos escenarios:

- servidor sin control de overload;
- servidor con control de overload.



## Ejecución

Compilar:

```bash
gcc simulation.c -o simulation
```
ejecutar:

```bash
./simulation
```
