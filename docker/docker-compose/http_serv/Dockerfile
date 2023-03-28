FROM ubuntu:22.04

RUN apt-get update -y && \
	apt-get install -y \
		python3

WORKDIR /root

RUN echo "Hello from the docker container!" > hello.txt 

CMD ["python3", "-m", "http.server"]
