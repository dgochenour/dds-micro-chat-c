# dds-micro-chat-c
A simple "chat app" using the C API provided by Connext Micro 2.4.x 


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