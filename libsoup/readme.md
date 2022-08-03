## `ws_client` sample

To test `ws_client` against `ws_echo_server` run client with

```bash
./ws_client ws://localhost:40001/ws
```

command.

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

or open `wss_client.html` page in a browser window (tested with Firefox 99 and Chrome 80) after you imported `localhost.crt` certificate.

> for *Firefox 99* go to *Settings > Privacy & Security > Certificates > View Certificates...*, then *Autorities* where click to *Import* button. Point to `localhost.crt` and click *OK* button.

We can optionaly specify server port and path as first and second arguments, this way

```console
$ ./wss_echo_server 40001 /client
listenning on 'wss://127.0.0.1:40001/client' address ...
```

### self-sign localhost certificate

To test stuff on localhost or private network it is fine to use self-signed certificate. See `localhost.crt`, `localhost.key` files for example. I've used 

```bash
openssl genrsa -des3 -out localhost.secure.key 2048
```

to generate private (encripted) key as `localhost.secure.key`, then

```bash
openssl rsa -in localhost.secure.key -out localhost.key
```

to generate (unencripted) private key as `localhost.key` so `openssl` will not ask for password anymore, then


```bash
openssl req -new -key localhost.key -out localhost.csr
```

to generate CSR (Certificate Signing Request) as `localhost.csr` and provide following information during CSR creation

```
Country Name (2 letter code) [AU]:US
State or Province Name (full name) [Some-State]:New York
Locality Name (eg, city) []:New York City
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Bouncy Castles, Inc.
Organizational Unit Name (eg, section) []:Ministry of Water Slides
Common Name (e.g. server FQDN or YOUR name) []:localhost
Email Address []:admin@localhost.com
```

the most important field is *Common Name*. And finally following

```bash
openssl x509 -req -days 365 -in localhost.csr -signkey localhost.key -out localhost.crt
```

command to generate the certificate as `localhost.crt` file.


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
