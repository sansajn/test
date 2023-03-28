FROM ubuntu:22.04

RUN apt-get update -y && \
	apt-get install -y \
		curl

CMD ["curl", "http://server:8000/hello.txt"]
