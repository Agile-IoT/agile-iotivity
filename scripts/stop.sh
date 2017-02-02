#!/bin/sh

ARG="$1"
PATTERN="iot.agail.$ARG"

echo "Send kill signal to $PATTERN"
pkill -f $PATTERN
