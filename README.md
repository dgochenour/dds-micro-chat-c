# dds-micro-chat-c
A simple "chat app" using the C API provided by Connext Micro 2.4.14. Further, the Micro source is build with `-DRTIME_CERT=1` to approximate a Connext Cert release, sure as *RTI Connext Cert 2.4.12.1*.


Assuming RTIMEHOME is set...

1) Generate the type support files

```
./generate_type_support.sh
```

2) Build the chat app
```
$RTIMEHOME/resource/scripts/rtime-make --config Release --build --name x64Darwin17clang9.0 --target Darwin --source-dir . -G "Unix Makefiles" --delete
```

3) Run the app
```
./objs/x64Darwin17clang9.0/chat_client
``` 