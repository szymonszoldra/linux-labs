## PROJECT 4

The project consists of two programs that run independently: the server and the client.
The client process sends a string to the server process. The server receives this string and processes it
in it changing all letters to uppercase, and then outputs the string z
back to the client. The client receives the processed string and prints it to the screen. By using
the message queue mechanism, the above task of the client type should be implemented
server with the ability to handle multiple clients simultaneously. Use one queue in the solution
messages. Use appropriate message labeling to distinguish between the queue
data for the server and data for individual clients.
The server creates a message queue and waits for messages from clients. The server removes the queue after
receiving a user-defined signal (eg SIGINT).
The client is a multi-threaded application (see project no.2). Thread 1 is responsible for sending messages,
for receiving thread2. The threads work asynchronously, i.e. thread 1 can send several messages
before thread 2 receives any feedback from the server. You must handle the error
queue overflow; and message overflow.
Program error handling based on the perror() function and the errno variable for system functions.
