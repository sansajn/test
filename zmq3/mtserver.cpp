#include <pthread.h>
#include <unistd.h>
#include "zhelpers.h"

void * worker_routine(void * context)
{
    void * receiver = zmq_socket(context, ZMQ_REP);
    zmq_connect(receiver, "inproc://workers");

    while (1)
    {
        char * s = s_recv(receiver);
        printf("received request: [%s]\n", s);
        free(s);
        sleep(1);  // do some work
        s_send(receiver, "world");  // send replay back
    }

    zmq_close(receiver);
    return nullptr;
}


int main(int argc, char * argv[])
{
    void * context = zmq_ctx_new();

    // socket to talk to clients
    void * clients = zmq_socket(context, ZMQ_ROUTER);
    zmq_bind(clients, "tcp://*:5555");

    // socket to talk to workers
    void * workers = zmq_socket(context, ZMQ_DEALER);
    zmq_bind(workers, "inproc://workers");

   // launch pool of worker threads
    for (int i = 0; i < 5; ++i)
    {
        pthread_t worker;
        pthread_create(&worker, nullptr, worker_routine, context);
    }

    zmq_proxy(clients, workers, nullptr);

    zmq_close(clients);
    zmq_close(workers);
    zmq_ctx_destroy(context);

    return 0;
}
