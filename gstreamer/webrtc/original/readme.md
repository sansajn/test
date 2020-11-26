usage instruction for `webrtc-sendrecv.c`

tested with firefox 82 and chrome 86 (both working fine)

- obtain peer id from https://webrtc.nirbheek.in/ page
- run `sendrecv` with

```
/sendrecv --peer-id 1145
```

where 1145 is peer id from the page

done


`h264-sendrecv.c` is modified `webrtc-sendrecv.c` with h264 encoder instead of vp8.

