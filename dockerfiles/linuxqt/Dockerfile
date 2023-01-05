FROM ubuntu:22.04

ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update && apt-get install -y \
	build-essential \
	git \
	pandoc \
	python-is-python3 \
	python3 \
	mercurial \
	zip \
	qt6-base-dev \
	qmake6 \
	wget

RUN useradd -s /bin/bash -m -u 1000 al
RUN chown -R al /home/al

USER al
WORKDIR /home/al/project/
CMD ["bash", "dockerfiles/linuxqt/build_linux.sh"]
