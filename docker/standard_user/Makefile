IMAGE_NAME ?= standard_user
CONTAINER_NAME ?= standard_user_cont

build:
	docker build -t $(IMAGE_NAME) \
		--build-arg UID=`id -u` \
		.

start:
	docker run -it --name ${CONTAINER_NAME} -d \
		--network host \
		${IMAGE_NAME} \
	|| \
	docker start ${CONTAINER_NAME}

stop:
	docker stop $(CONTAINER_NAME)

join: start
	docker exec -it $(CONTAINER_NAME) /bin/bash

rm: stop
	-docker rm $(CONTAINER_NAME)

purge: rm
	docker rmi $(IMAGE_NAME)
	

.PHONY: build start join stop rm purge
