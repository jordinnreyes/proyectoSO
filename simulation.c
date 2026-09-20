#include <stdio.h>
#include <stdlib.h>

#define MAX_EVENTS 100

// Valores para la simulacion
#define MAX_BATCH 23
#define OVERLOAD_THRESHOLD 18.0
#define ALPHA 0.3

typedef struct {
    int id;
    int socket_id;
} SignalEvent;

typedef struct {
    SignalEvent events[MAX_EVENTS];
    int front;
    int rear;
    int size;
} SignalQueue;

typedef struct {
    int generated;
    int accepted;
    int processed;
    int rejected;
    int lost_queue_full;
    int max_queue_size;
} Statistics;


//  COLA 

void init_queue(SignalQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

int enqueue(SignalQueue *queue, SignalEvent event) {

    if (queue->size >= MAX_EVENTS) {
        return 0;
    }

    queue->events[queue->rear] = event;
    queue->rear = (queue->rear + 1) % MAX_EVENTS;
    queue->size++;

    return 1;
}

SignalEvent dequeue_one(SignalQueue *queue) {

    SignalEvent empty = {-1, -1};

    if (queue->size == 0) {
        return empty;
    }
    SignalEvent event = queue->events[queue->front];
    queue->front = (queue->front + 1) % MAX_EVENTS;
    queue->size--;

    return event;
}


// Simula la idea de sigtimedwait4():
// obtener varias señales pendientes de una sola vez.
int dequeue_batch( SignalQueue *queue, SignalEvent *buffer, int max_signals) {
    int count = 0;

    while ( queue->size > 0 && count < max_signals) {
        buffer[count] = dequeue_one(queue);
        count++;
    }
    return count;
}

// CARGA 
// Cantidad de nuevas conexiones/eventos que
// intentan llegar durante una ronda.
void generate_load(SignalQueue *queue, Statistics *stats, int amount, int *next_id, int overload) {
    for (int i = 0; i < amount; i++) {
        stats->generated++;
        //En el paper, cuando se detecta overload, phhttpd empieza a resetear conexiones.
        //En nuestra simulación lo representamos evitando agregar el evento a la cola.
         
        if (overload) {
            stats->rejected++;
            continue;
        }
        SignalEvent event;

        event.id = (*next_id);
        event.socket_id = 100 + (*next_id);
        (*next_id)++;

        if (enqueue(queue, event)) {
            stats->accepted++;
        }
        else {
            stats->lost_queue_full++;
        }
    }

    if (queue->size > stats->max_queue_size) {
        stats->max_queue_size = queue->size;
    }
}


// PROMEDIO MOVIL

double update_load_average(double previous_average, int signals_received) {
    return ALPHA * signals_received + (1.0 - ALPHA) * previous_average;
}

// SIMULACION 

void run_simulation(int use_overload_control) {
    SignalQueue queue;
    Statistics stats = { 0, 0, 0, 0, 0, 0 };

    SignalEvent buffer[MAX_BATCH];
    init_queue(&queue);
    int next_id = 1;
    double load_average = 0.0;
    int overload = 0;
    printf("\n=======================================\n");

    if (use_overload_control) {
        printf("SIMULACION CON CONTROL DE OVERLOAD\n");
    }
    else {
        printf("SIMULACION SIN CONTROL DE OVERLOAD\n");
    }

    printf("=======================================\n\n");


    for (int round = 1; round <= 20; round++) {
        //Carga artificial         
        //Rondas 1-5: carga baja
        //Rondas 6-10: carga media
        //Rondas 11-20: carga alta
        int incoming;
        if (round <= 5) {
            incoming = 8;
        }
        else if (round <= 10) {
            incoming = 18;
        }
        else {
            incoming = 35;
        }

        generate_load(
            &queue,
            &stats,
            incoming,
            &next_id,
            use_overload_control && overload
        );
        //Representa una llamada a sigtimedwait4().
        //Como en el experimento del paper,
        //como máximo se recuperan 23 señales.

        int received =
            dequeue_batch(
                &queue,
                buffer,
                MAX_BATCH
            );

        stats.processed += received;
        //Promedio móvil utilizado para estimar la carga del servidor.
        
        load_average =
            update_load_average(load_average, received);

        //Si el promedio supera 18,consideramos que existe overload.
        if (load_average > OVERLOAD_THRESHOLD) {
            overload = 1;
        }
        else {
            overload = 0;
        }


        printf(
            "Ronda %2d | "
            "Llegan: %2d | "
            "Recogidas: %2d | "
            "Cola: %2d | "
            "Avg: %5.2f | "
            "Overload: %s\n",

            round,
            incoming,
            received,
            queue.size,
            load_average,
            overload ? "SI" : "NO"
        );
    }


    printf("\n---------- RESULTADOS ----------\n");

    printf("Eventos generados:        %d\n", stats.generated);

    printf("Eventos aceptados:        %d\n", stats.accepted);

    printf(
        "Eventos procesados:       %d\n", stats.processed);

    printf(
        "Eventos rechazados:       %d\n", stats.rejected );

    printf(
        "Perdidos por cola llena:  %d\n", stats.lost_queue_full );

    printf(
        "Maximo tamano de cola:    %d\n", stats.max_queue_size );

    printf(
        "Eventos pendientes:       %d\n", queue.size );
}


// MAIN 

int main() {

    //Ejecutamos exactamente la misma carga bajo dos estrategias.

    run_simulation(0);

    run_simulation(1);

    return 0;
}