# dds-micro-chat-c
A simple "chat app" using the C API provided by Connext Micro 2.4.14. Further, the Micro source is build with `-DRTIME_CERT=1` to approximate a Connext Cert release, such as *RTI Connext Cert 2.4.12.1*.

## Building and running this example

### 1) Set the enviroment variables

    $ export RTIMEHOME=/path/to/your/rti_connext_dds_micro-2.4.14
    $ export RTIMEARCH=x64Darwin17clang9.0

### 2) Build the Cert-compatible libraries

    $ cd $RTIMEHOME
    $ resource/scripts/rtime-make --target Darwin --name $RTIMEARCH --build --config Debug -DRTIME_CERT=1

The addition of the `-DRTIME_CERT=1` flag results in libraries being generated with Cert functionality, and those libraries will be in a directory named "`$RTIMEARCH`_cert". For example, `x64Darwin17clang9.0_cert`.

### 3) Generate the type support files

    $ ./generate_type_support.sh


### 4) Build the chat app

    $ $RTIMEHOME/resource/scripts/rtime-make --config Release --build --name ${RTIMEARCH}_cert --target Darwin --source-dir . -G "Unix Makefiles" --delete


3) Run the app
```
./objs/x64Darwin17clang9.0_cert/chat_client
``` 