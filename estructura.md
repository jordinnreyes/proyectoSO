# Estructura de la simulación

La simulación representa de forma simplificada el manejo de eventos de un servidor basado en señales RT y el control de sobrecarga descrito en el paper.

## Componentes

- `SignalEvent`: representa un evento asociado a una conexión.
- `SignalQueue`: representa la cola de eventos pendientes.
- `Statistics`: almacena métricas de la simulación.
- `enqueue()`: agrega nuevos eventos a la cola.
- `dequeue_one()`: retira un evento a la vez.
- `dequeue_batch()`: retira varios eventos en una operación.
- `generate_load()`: genera eventos para simular distintos niveles de carga.
- `update_load_average()`: calcula el promedio móvil utilizado para detectar sobrecarga.
- `run_simulation()`: ejecuta la simulación con o sin control de overload.

## Qué representa

`dequeue_one()` representa conceptualmente el comportamiento de `sigwaitinfo()`, mientras que `dequeue_batch()` representa la idea de `sigtimedwait4()`.

La simulación también utiliza un promedio móvil para detectar overload. Cuando se supera el umbral, las nuevas conexiones se representan como rechazadas y no se agregan a la cola.

## Resultados observados

La simulación permite comparar dos escenarios:

- servidor sin control de overload;
- servidor con control de overload.

Se observan métricas como eventos generados, procesados, rechazados y tamaño máximo de la cola.
