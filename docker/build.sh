#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..

DOCKER_IMAGE=${DOCKER_IMAGE:-cryptseocore/cryptseo}
DOCKER_TAG=${DOCKER_TAG:-latest}

BUILD_DIR=${BUILD_DIR:-.}

rm docker/bin/*
mkdir docker/bin
cp $BUILD_DIR/src/cryptseod docker/bin/
cp $BUILD_DIR/src/cryptseo-cli docker/bin/
cp $BUILD_DIR/src/cryptseo-tx docker/bin/
strip docker/bin/cryptseod
strip docker/bin/cryptseo-cli
strip docker/bin/cryptseo-tx

docker build --pull -t $DOCKER_IMAGE:$DOCKER_TAG -f docker/Dockerfile docker
