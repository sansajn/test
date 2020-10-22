# X11 GUI app from docker sample
# build with `docker build -t xeyes`
# run with `docker run --network host --env="DISPLAY" --volume "$HOME/.Xauthority:/root/.Xauthority:ro" xeyes`

FROM ubuntu:16.04

RUN apt-get update -y ;\
	apt-get install -y x11-apps

CMD ["/usr/bin/xeyes"]
