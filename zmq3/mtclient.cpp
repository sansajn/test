#include <iostream>
#include <unistd.h>
#include "zhelpers.h"

using std::cout;

int main(int argc, char * argv[])
{
    void * ctx = zmq_ctx_new();
    void * req = zmq_socket(ctx, ZMQ_REQ);
    zmq_connect(req, "tcp://localhost:5555");

    while (1)
    {
        s_send(req, "hello");
        sleep(1);
        char * d = s_recv(req);
        cout << "answer: " << d << std::endl;
        free(d);
    }

    zmq_close(req);
    zmq_ctx_destroy(ctx);

    return 0;
}
