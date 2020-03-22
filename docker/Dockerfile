#FROM debian:stretch
FROM ubuntu:18.04
LABEL maintainer="The CSEO Developers <sales@mealcryptseos.biz>"
LABEL description="Dockerised CSEO, built from Travis"

RUN apt-get update && apt-get install -y software-properties-common unzip
RUN apt-get install -y wget

RUN wget https://github.com/Cryptseo/Cryptseo/releases/download/v5.2.3/cryptseo-qt-linux-18.zip
RUN unzip cryptseo-qt-linux-18.zip
RUN cp cryptseod /usr/local/bin

VOLUME ["/opt/cryptseo"]

EXPOSE 6480
EXPOSE 6480
EXPOSE 9777
EXPOSE 9777

CMD ["cryptseod", "--conf=/opt/cryptseo/cryptseo.conf", "--printtoconsole"]