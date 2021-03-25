IMAGE_NAME ?= repo_image
CONTAINER_NAME ?= repo

build:
	docker build -t $(IMAGE_NAME) .

start:
	docker run -it --name ${CONTAINER_NAME} -d \
		--network host \
		${IMAGE_NAME}

stop:
	docker stop $(CONTAINER_NAME)

stoprm: stop
	-docker rm $(CONTAINER_NAME)

join:
	docker exec -it $(CONTAINER_NAME) /bin/bash
	

.PHONY: build start join stop stoprm
