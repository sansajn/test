FROM debian:10

USER root

ARG UID
ARG USER=me

# debian 10 basics
RUN apt-get update -y && \
	apt-get install -y \
		systemd \
      ca-certificates \
		locales \
		software-properties-common \
		sudo

# set locales
RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && locale-gen
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# create $USER user (and change password to $USER)
RUN useradd -d /home/${USER} -l -U -G sudo -m -s /bin/bash -u ${UID} ${USER}
RUN echo "root:${USER}" | chpasswd
RUN echo "${USER}:${USER}" | chpasswd

USER ${USER}
WORKDIR /home/${USER}
