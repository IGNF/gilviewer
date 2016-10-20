FROM ubuntu:14.04

ENV BUILD_PACKAGES="libwxgtk2.8-dev cmake git g++ libjpeg-dev libpng-dev libboost1.55-dev libboost-filesystem1.55-dev libboost-system1.55-dev libboost-thread1.55-dev libboost-serialization1.55-dev libtiff-dev"
ENV RUNTIME_PACKAGES="libwxgtk2.8 libcanberra-gtk-module libjpeg8 libpng12-0 libtiff5 libboost1.55 libboost-filesystem1.55 libboost-system1.55 libboost-thread1.55 libboost-serialization1.55"

RUN apt-get -y -qq update && apt-get -y --fix-missing install $BUILD_PACKAGES \
&& git clone https://github.com/IGNF/gilviewer.git \
&& cd gilviewer \
&& mkdir build \
&& cd build \
&& cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-Wno-unused-local-typedefs .. \
&& make install \
&& cp GilViewerApp /usr/local/bin \
&& rm -rf gilviewer \
&& AUTO_ADDED_PACKAGES=`apt-mark showauto`
&& apt-get remove --purge -y $BUILD_PACKAGES $AUTO_ADDED_PACKAGES \
&& apt-get -y --fix-missing install $RUNTIME_PACKAGES \
&& apt-get -y clean \
&& rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* /etc/apt/apt.conf

RUN export uid=1000 gid=1000 && \
    mkdir -p /home/developer && \
    echo "developer:x:${uid}:${gid}:Developer,,,:/home/developer:/bin/bash" >> /etc/passwd && \
    echo "developer:x:${uid}:" >> /etc/group && \
    echo "developer ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/developer && \
    chmod 0440 /etc/sudoers.d/developer && \
    chown ${uid}:${gid} -R /home/developer

USER developer
ENV HOME /home/developer
CMD /usr/local/bin/GilViewerApp
