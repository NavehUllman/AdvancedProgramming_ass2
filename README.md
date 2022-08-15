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

- Buffer size: `128 bytes`. We were given an instruction not to transfer information that exceeds the size of the input file, so we decided to send the data in 128 byte chunks. Every chunk is seperated with `<,>`, and the last chunk is marked with `$`.
- Port number: Since we don't know which port number is available, we decided to pass this parameter as an argument in the running command ([Jump to Running Instructions](#running-instructions)).

#
We did not use *Threads*, so a server cannot handle two clients at once. But still, we wrote our program as flexible as possible, so that we can add the functionality of threads in the future without the need for dramatic changes in the code. For example, we wrote a method `Server::communicate(int clientSocket)` that
handles a single client's requests. In the future, each thread will handle different client and use this very method.
#
### Running Instructions
1. Download the project on your local device.
2. You are given a default input file for the client (**clientSrc/Input/unclassified.csv**), and default database for the server (**serverSrc/KNN/Database/classified.csv**). If you would like to change them, you can simply replace those files. The **classified.csv** file cannot be renamed.
3. Open **Terminal**.
4. Navigate to **serverSrc** using the command `cd path/to/your/project` and then `cd serverSrc`.
5. Run the compiling command `g++ *.cpp KNN/*.cpp KNN/DistanceCalcs/*.cpp KNN/Flowers/*.cpp -std=c++11`.
6. A file named *a.out* should appear in the folder. Run the command `./a.out [port] [number_of_connections]` to run the program. Where `[port]` is the port that the server will be binded to, and `[number_of_connections]` is the maximum number of client connections until the server closes itself - You can choose **not** to give that argument, and the server will run forever. Running example: `./a.out 5555 6` or `./a.out 5555`.
8. Now, the program should be running. The server is open and waiting for clients to connect.
7. Open a new **Terminal** window (and do not close the previous Terminal window as the server must remain open).
8. Navigate to **clientSrc** using the command `cd path/to/your/project` and then `cd clientSrc`.
9. Run the compiling command `g++ *.cpp -std=c++11`.
10. A file named *a.out* should appear in the folder. Run the command `./a.out [port] [path_to_unclassified] [path_to_output_file]` to run the program. Where `[port]` is the port number given in the server running command. `[path_to_classified]` is the path to the input file (it must be located anywhere in the **clientSrc** folder. For convenience, there is a dedicated folder for this called *Input*). `[path_to_output_file]` is the path to where you want to save the results (it also must be located in **clientSrc**, recommended, in the *Output* folder). You can choose to classify any amount of files, for example, running: `./a.out 5555 Input/classified1.csv Output/result1.csv Input/classified2.csv Output/result2.csv` will connect to the server on port number `5555`, classify `classified1.csv`, `classified1.csv`, and save the results on `result1.csv`, `result2.csv`
respectively.
12. The result files given in the client running command will be overwritten or created.

### That's it 😎

