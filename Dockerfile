#
# Build Image
#

FROM ubuntu:18.04 as mapcrafter-builder
MAINTAINER Dinip <dinip@dinip.pt>

ENV DEBIAN_FRONTEND noninteractive

# Get dependency
RUN apt-get update && apt-get install -y libpng-dev libjpeg-turbo8 libboost-iostreams-dev git cmake build-essential libboost-all-dev libjpeg-dev

# Add the git repo and build it
RUN mkdir /git && cd /git && \
    git clone --single-branch --branch world113 -n https://github.com/Dinip/mapcrafter.git && \
    cd mapcrafter/ && git checkout \
    && mkdir build && cd build && \
    cmake .. && \
    make && \
    mkdir /tmp/mapcrafter && \
    make DESTDIR=/tmp/mapcrafter install

#
# Final Image
#

FROM ubuntu:18.04
MAINTAINER Dinip <dinip@dinip.pt>

ENV DEBIAN_FRONTEND noninteractive
ENV HOME /root
ENV threads 4

VOLUME ["/config"]
VOLUME ["/output"]
VOLUME ["/world"]

# Mapcrafter, built in previous stage
COPY --from=mapcrafter-builder /tmp/mapcrafter/ /

# Depedencies needed for running Mapcrafter
RUN apt-get update && apt-get install -y cron \
        libpng16-16 \
        libjpeg-turbo-progs \
        libboost-iostreams1.65.1 \
        libboost-system1.65.1 \
        libboost-filesystem1.65.1 \
        libboost-program-options1.65.1 && \
        apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* && \
        ldconfig

#Add default configs
ADD defaults/ /defaults/
RUN chmod 777 -R /defaults/*

#Add start.sh
ADD start.sh /start.sh
RUN chmod 0777 /start.sh

#Add render.sh 
ADD render.sh /opt/render.sh
RUN chmod 0777 /opt/render.sh

CMD ./start.sh && tail -n 50 -f /config/mapcrafter.log