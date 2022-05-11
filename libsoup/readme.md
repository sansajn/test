## `wss_echo_server` sample

WebSocket Secure (WSS) echo server sample. To test it run echo server with

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
