IMAGE_NAME ?= http_serv
CONTAINER_NAME ?= $(IMAGE_NAME)_container

image:
	docker build -t $(IMAGE_NAME) \
		.

start:
	docker run -it --name $(CONTAINER_NAME) -d \
		--network host \
		$(IMAGE_NAME) \
	|| \
	docker start $(CONTAINER_NAME)

stop:
	docker stop $(CONTAINER_NAME)

join: start
	docker exec -it $(CONTAINER_NAME) /bin/bash

logs:
	docker logs $(CONTAINER_NAME)
	
rm: stop
	-docker rm $(CONTAINER_NAME)

purge: rm
	docker rmi $(IMAGE_NAME)


.PHONY: image start join stop rm purge
