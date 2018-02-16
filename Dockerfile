# Run Chrome in a container # TBM for the st
#
#  Check: https://blog.jessfraz.com/post/docker-containers-on-the-desktop/
#
# docker run -it \
#	--net host \ # may as well YOLO
#	--cpuset-cpus 0 \ # control the cpu
#	--memory 512mb \ # max memory it can use
#	-v /tmp/.X11-unix:/tmp/.X11-unix \ # mount the X11 socket
#	-e DISPLAY=unix$DISPLAY \
#	-v $HOME/Downloads:/home/chrome/Downloads \
#	-v $HOME/.config/google-chrome/:/data \ # if you want to save state
#	--security-opt seccomp=$HOME/chrome.json \
#	--device /dev/snd \ # so we have sound
#   --device /dev/dri \
#	-v /dev/shm:/dev/shm \
#	--name chrome \
#	jess/chrome
#
# You will want the custom seccomp profile:
# 	wget https://raw.githubusercontent.com/jfrazelle/dotfiles/master/etc/docker/seccomp/chrome.json -O ~/chrome.json

FROM ubuntu:17.10

RUN apt-get update && apt-get install -y \
	autoconf \
	automake \
	make \
	g++ \
	gcc \
	build-essential \ 
	git \
	ubuntu-dev-tools \
	libglu1-mesa-dev \
	freeglut3-dev \
	mesa-common-dev \
	wget \
	libfontconfig1 \
	qt5-default \
	qtbase5-dev \
	libqt5charts5-dev \
	libqt5svg5-dev \
	libarmadillo-dev \
	r-base
#	&& rm -rf /var/lib/apt/lists/*

# Install cmake
RUN wget https://cmake.org/files/v3.7/cmake-3.7.2.tar.gz && \
    tar -xvzf cmake-3.7.2.tar.gz && \
    cd cmake-3.7.2 && \
    ./configure && \
    make -j4 && \
    make install


# Install Qt (Faking display)
# Installed via Apt

# DONWLOAD the QCustomplot
RUN wget http://www.qcustomplot.com/release/1.3.2/QCustomPlot.tar.gz && \
    mkdir /opt/QCustomPlot && tar xf QCustomPlot.tar.gz -C /opt/QCustomPlot


# Downlaod Armadillo debs and install
# Installed via apt

# Install R deps
RUN echo "r <- getOption('repos'); r['CRAN'] <- 'http://cran.us.r-project.org'; options(repos = r);" > ~/.Rprofile
RUN Rscript -e "source('https://bioconductor.org/biocLite.R'); biocLite('DESeq2'); biocLite('scran'); biocLite('Rtsne')"
RUN Rscript -e "install.packages('RInside')"
RUN Rscript -e "install.packages('Rcpp')"
RUN Rscript -e "install.packages('RcppArmadillo')"

WORKDIR /opt/
RUN mkdir /opt/st_viewer
WORKDIR /opt/st_viewer 
ADD . . 
RUN mkdir st_viewer_build
WORKDIR /opt/st_viewer/st_viewer_build


RUN apt-get install -y apt-file
RUN apt-file update

#RUN cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_PREFIX_PATH="/opt/QCustomPlot/qcustomplot" ..


