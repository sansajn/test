FROM debian:10

RUN apt-get update -y && \
	apt-get install -y \
		locales \
		software-properties-common

RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && locale-gen

ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# add custom repository
RUN echo "deb [trusted=yes] http://192.168.0.134:8000 ./" | tee -a /etc/apt/sources.list > /dev/null

RUN apt-get update -y ;\
	apt-get install hello-deb -y
