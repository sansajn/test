This directory contains following

- `http_responder.cpp`: HTTP server sample responding to HTTP GET requests by JSON object string, based on http_server_async.cpp Beast sample.

## HTTPS

- `http_server_async_ssl_d10.cpp`: HTTPS server sample for debian10 (taken from `http_server_async_ssl.cpp` in `beast/example` directory).

```console
Usage: http_server_async_ssl_d10 <address> <port> <doc_root> <threads>
Example:
    http_server_async_ssl_d10 127.0.0.1 8123 . 1
```

> **note**: The server expects `index.html` file present in a directory.


- `http_client_async_ssl_d10.cpp`: HTTPS client sample for debian10 (taken from `http_cliant_async_ssl.cpp` in `beast/example` directory).

```console
Usage: http_client_async_ssl_d10 <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]
Example:
    http_client_async_ssl_d10 127.0.0.1 8123 /
```

samples.
