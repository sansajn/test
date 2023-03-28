Run

```bash
docker-compose up --build
```

command to build and run images. There are two images build `test/http_serv` and `test/http_client` there, see

```console
$ docker-compose images
       Container             Repository       Tag       Image Id       Size  
-----------------------------------------------------------------------------
docker-compose_client_1   test/http_client   latest   782a7a0928c9   127.5 MB
docker-compose_server_1   test/http_serv     latest   056a39d66ec7   150.5 MB
```

Note `up` command will end with

```console
client_1  | curl: (7) Failed to connect to server port 8000 after 1 ms: Connection refused
docker-compose_client_1 exited with code 7
```

complain, just run

```console
docker-compose start client
```

with a following

```console
server_1  | 172.24.0.3 - - [28/Mar/2023 12:20:05] "GET /hello.txt HTTP/1.1" 200 -
client_1  | Hello from the docker container!
docker-compose_client_1 exited with code 0
```

prove that setup is working.

Run

```bash
docker-compore down
```

command to destroy composition.
