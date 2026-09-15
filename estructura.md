# Estructura inicial de la simulación

La simulación representa de forma simplificada la cola de eventos utilizada por un servidor basado en señales RT.

## Componentes

- `SignalEvent`: representa un evento asociado a una conexión.
- `SignalQueue`: representa la cola de señales pendientes.
- `enqueue()`: agrega nuevos eventos.
- `dequeue_one()`: retira un evento a la vez.
- `dequeue_batch()`: retira varios eventos en una operación.

## que representa

`dequeue_one()` representa conceptualmente el comportamiento de `sigwaitinfo()`, mientras que `dequeue_batch()` representa
la idea introducida por `sigtimedwait4()`.

## Siguiente avance

Agregar  la simulación para simular el comportamiento del servidor cuando aumenta el número de eventos pendientes 
para ver la sobrecarga.