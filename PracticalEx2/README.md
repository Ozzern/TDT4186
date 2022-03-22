## How to compile
1. Navigate to the ```/PracticalEx2``` folder
2. Run ```gcc mtwwd.c bbuffer.c sem.c -lpthread -o mtwwd```
3. Run the executable
    1. Run ```./mtwwd [directory] [port] [#threads] [#bufslots]```
    2. If you want the server to look in the ```/doc``` folder, and run on port 8000 with 10 threads and 5 bufferslots you would run the command ```./mtwwd doc 8000 10 5```
