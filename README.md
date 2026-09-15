# proyectoSO Sistemas Operativos

Este proyecto está basado en el paper **“Analyzing the Overload Behavior of a Simple Web Server”** de N. Provos, C. Lever y S. Tweedie.

El artículo estudia cómo un servidor web basado en señales POSIX Real-Time maneja eventos de entrada/salida y qué ocurre cuando la carga aumenta. En particular, compara el uso de `sigwaitinfo()`, que obtiene una señal por llamada, con la propuesta `sigtimedwait4()`, que permite recuperar varias señales pendientes en una sola llamada.

## Simulación

La simulación en C representa de forma simplificada una cola de eventos generados por distintas conexiones.