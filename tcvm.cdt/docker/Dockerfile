From ubuntu:18.04

RUN apt-get update && apt-get install -y cmake make g++

COPY . /root

VOLUME ["/root/code"]
VOLUME ["/root/output"]

WORKDIR /root/data

CMD ["build.sh"]
