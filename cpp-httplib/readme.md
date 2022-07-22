# httplib

Library allows to handle HTTP/S server/client in a synchronized (blocking) fashion, see [cpp-httplib](https://github.com/yhirose/cpp-httplib) for more details.


The directory containts

- `client.cpp`/`serv.cpp`: HTTP client/server samples.

- `sclient.cpp`/`sserv.cpp`: HTTPS client/server samples workignwith custom certificate (`server.crt`, `server.key` for server are expected and `myCA.pem` for client).

> TODO: describe how to generate certificate and add sample one ...

- `client_callback.cpp`: HTTP client with callback API sample.


samples.


## TODO

- error handling (in sserv for example there is no any log errro/warning in case certificate files are not present)

- provide sample to handle generic path for server (not only `/hi`)

