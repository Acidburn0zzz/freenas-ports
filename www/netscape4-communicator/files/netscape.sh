#!/bin/sh
export MOZILLA_HOME; MOZILLA_HOME=${MOZILLA_HOME:=@PREFIX@/@NSUBDIR@}
export CLASSPATH ; CLASSPATH=.:$MOZILLA_HOME
export XCMSDB; XCMSDB=/dev/null
ulimit -c 0
exec $MOZILLA_HOME/@BROWSER@-@BROWSER_VER@.bin "$@"
