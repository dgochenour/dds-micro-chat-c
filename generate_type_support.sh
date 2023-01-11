#!/bin/bash

IDL_DIR="./idl"
GEN_DIR="./generated"
GEN_XML_DIR="./xmltypes"

FILES="$IDL_DIR/*.idl"
for f in $FILES
  do
    if [ -f "$f" ]; then
      echo $f
      $RTIMEHOME/rtiddsgen/scripts/rtiddsgen -d "$GEN_DIR" -micro -language C -update typefiles $f
      $RTIMEHOME/rtiddsgen/scripts/rtiddsgen -d "$GEN_XML_DIR" -convertToXml $f
    else
     echo "ERROR: Check \"$f\""
  fi
done