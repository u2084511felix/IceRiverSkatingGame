#!/usr/bin/env bash
set -e

set -a
source .env

rm -f ${NAME}.zip

(
    cd dist
    zip -r ../${NAME}.zip .
)

unzip -l $NAME.zip