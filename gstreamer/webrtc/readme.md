# WebRTC samples

## sendrecv

can send video/audio stream to a webbrowser and receive video stream from a webbrowser

```bash
./sendrecv --file=file:///home/adam/hero6.mp4 --server=wss://localhost:8443 --peer-id=7317
```

## send

can send video stream to a webbrowser

```bash
./send --file=file:///home/adam/hero6.mp4 --server=wss://localhost:8443 --peer-id=7317
```
> *bug*: video stream freeze afte couple of seconds (this not happen for `sendrecv` sample)


## unidirectional-h264

can send video stream to a webbrowser without signalling server (working in firefox)

```bash
./unidirectional-h264
```

