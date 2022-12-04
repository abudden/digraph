# More complex version; see below

FROM ubuntu:22.04

ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update && apt-get install -y \
		software-properties-common \
		gnupg \
		mercurial \
		zip \
		\
		autoconf \
		automake \
		autopoint \
		bash \
		bison \
		bzip2 \
		flex \
		g++ \
		g++-multilib \
		gettext \
		git \
		gperf \
		gtk-doc-tools \
		intltool \
		libc6-dev-i386 \
		libgdk-pixbuf2.0-dev \
		libgl-dev \
		libltdl-dev \
		libopengl-dev \
		libssl-dev \
		libtool-bin \
		libxml-parser-perl \
		lzip \
		make \
		openssl \
		p7zip-full \
		patch \
		perl \
		python2.7 \
		python-is-python3 \
		python3 \
		python3-mako \
		ruby \
		sed \
		unzip \
		wget \
		xz-utils


WORKDIR /opt
RUN git clone https://github.com/mxe/mxe.git

WORKDIR /opt/mxe

RUN make MXE_TARGETS="x86_64-w64-mingw32.shared x86_64-w64-mingw32.static" cc qt6

# This simpler version should (hopefully) work as soon as
# MXE bother to update the repositories (reference:
# https://github.com/mxe/mxe/issues/2844 )

#FROM debian:buster
#
#RUN apt-get update && apt-get install -y \
#		software-properties-common \
#		gnupg \
#		mercurial \
#		zip \
#		lsb-release
#
## Add and refresh MXE repository:
#
#RUN apt-key adv \
#		--keyserver keyserver.ubuntu.com \
#		--recv-keys 86B72ED9 && \
#		add-apt-repository \
#		"deb [arch=amd64] https://pkg.mxe.cc/repos/apt `lsb_release -sc` main"
#
#RUN apt update && apt install -y \
#		mxe-i686-w64-mingw32.shared-cc \
#		mxe-i686-w64-mingw32.shared-qt5 \
#		mxe-i686-w64-mingw32.shared-libzip \
#		mxe-i686-w64-mingw32.shared-mpfr

RUN useradd -s /bin/bash -m -u 1000 al
RUN chown -R al /home/al
USER al
WORKDIR /home/al/project/

CMD ["bash", "dockerfiles/winqt/build_dockerwin.sh"]
