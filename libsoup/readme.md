## `wss_echo_server` sample

WebSocket Secure (WSS) echo server sample. Usage

```
wss_echo_server [PORT=8765][PATH="/"]
```

To test echo server run

```bash
./wss_echo_server
```

command, then run client with

```console
$ ./wss_client 
WebSocket connection opened
<<< hello 1!
>>> hello 1!
<<< hello 2!
>>> hello 2!
^CWebSocket connection closed
quit
```

We can optionaly specify server port and path as first and second arguments, this way

```console
$ ./wss_echo_server 40001 /client
listenning on 'wss://127.0.0.1:40001/client' address ...
```


## `wss_client` sample

WebSocket Secure (WSS) client sample. To test it first run echo server with

```bash
./echo_secure.py
```

command, then run client with

```console
$ ./wss_client
WebSocket connection opened
<<< hello!
>>> hello!
WebSocket connection closed
quit
```

the current client implementation sends `"hello!"` and expect `"hello!"` as replay.
