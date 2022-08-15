# KNN using client-server
At the previous assignment, we were asked to implement the KNN-Algorithm, which was designed to classify an object to a certain membership (see: https://github.com/arielverbin/AdvancedProgramming).
### Additions
Now, we would like to extend that idea and divide the tasks of the algorithm through *Server* and *Client* mechanism. The idea is intuitive, the server will own a *Database* file which holds classified objects, so it will have the ability to classify unclassified objects.
The client, in contrast,  will connect to the server in order to use its service: it will send the server a list of unclassified objects, and the server will classify those objects and send them back.
### Our Implementation
Firstly, it should be noted that the client and the server are two separate programs. Therefore, each of them
requires a different compiling and running command. To avoid confusion, we separated their code files into two directories, **clientSrc** and **serverSrc**.

* **The Server (serverSrc)**

    As we said, the server will hold the role of classifying objects, so it must access the KNN algorithm implementation. Therefore, we added a directory under **serverSrc**, called **KNN**, with the implementation from the previous assignment.
The *Database* file is *KNN/Database/classified.csv* and can be replaced. 
In order to communicate with our clients, we created a class called *Server* which is responsible to *open sockets*, *connect*, *send*, and *receive* messages from clients.
The clients sends the unclassified objects to a *Server* instance.


* **The Client (clientSrc)**

    In **clientSrc** we created a class called *Client*, which also deals the Send and Receive methods, but obviously from the other size.
This class is also responsible for copying the output into the designated file.




### Constants
We were given the chance to choose constant parameters such as *Port number* and *Buffer size*.

- Buffer size: `512 bytes`
- Port number: Since we don't know which port number is available, we decided to pass this parameter as an argument in the running command ([Jump to Running Instructions](#running-instructions)).

#
We did not use *Threads*, so a server cannot handle two clients at once. But still, we wrote our program as flexible as possible, so that we can add the functionality of threads in the future without the need for dramatic changes in the code. For example, we wrote a method `Server::communicate(int clientSocket)` that
handles a single client's requests. In the future, each thread will handle different client and use this very method.
#
### Running Instructions
