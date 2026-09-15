#include <stdio.h>
#include <stdlib.h>

#define MAX_EVENTS 100
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

void init_queue(SignalQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

int enqueue(SignalQueue *queue, SignalEvent event) {
    if (queue->size >= MAX_EVENTS) {
        printf("La cola de señales está llena.\n");
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

int dequeue_batch( SignalQueue *queue, SignalEvent *buffer, int max_signals) {}

int main() {

    SignalQueue queue;
    init_queue(&queue);

    return 0;
}