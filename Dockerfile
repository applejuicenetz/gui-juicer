FROM debian:buster

RUN apt -y update

RUN apt install -y qttools5-dev-tools \
                nsis \
                gcc \
                g++ \
                make

RUN mkdir /work

WORKDIR /work

CMD ["tail", "-f", "/dev/null"]
