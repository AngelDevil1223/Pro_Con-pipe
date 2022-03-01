# Pro_Con-pipe
Purpose
To develop an interprocess communication through the use of both fork() and pipe() process
system calls.
What to Do
You are to modify Project 1 such that the data sharing between producer and consumer is done
by reading and writing through a shared pipe. The data shared by the producer is encoded and
shared with the consumer. The consumer decodes the message, modifies it, and saves it. The
files containing the original data, the encoded version of it, and the modified version of it are to
be stored in files with the extension inpf, binf, and outf respectively. Details regarding such files
are provided by the table below.
File Characteristics
Naming Contents Created by Accessed by
filename.inpf any ASCII character user producer
filename.binf (0 and 1) ASCII characters producer consumer
filename.outf Modified version of .inpf consumer user
1. The Producer
Creates a pipe and uses it to share all the encoded frames with the consumer in addition to all the
other tasks defined in the Project 1 document.
2. The Consumer
In addition to all tasks defined in the Project 1 document, the consumer will read all encoded
frames through the shared pipe, converts all lowercase characters to uppercase and write the
modified version of the received data into a file with extension outf.
3. Details
The following are the tasks to be done for this project:
a. Create a simple consumer/producer application, as discussed in this document, that
uses a shared pipe for communication.
b. The filename.inpf will contain all the original data to be shared between producer and
consumer.c. The filename.binf is the binary (0’s and 1’s) version of the original data. It is created
by the producer, shared with the consumer through the pipe, and processed by the
consumer.
d. The filename.outf is the modified version of filename.inpf where all lower case letters
have been replaced by upper case letters. The consumer is responsible for the creation
of filename.outf.
3. What to Submit
a) Your solution must be uploaded to Blackboard.
b) Copies of the source files for both your producer and your receiver as well as their
executables, and any data you used for testing your solution.
c) You are to place all files that are related to your solution to a .zip file. Your .zip file
must follow the format: CSI 500 Project2 Your Name.
d) The documentation associated with your solution must be typeset in MS Word. Marks
will be deducted if you do not follow this requirement.
Your program should be developed using GNU versions of the C compiler. Your solution must
use Ordinary Pipes (Lecture 03 – Interprocess Communication; slides 31-32). It should be
layered, modularized, and well commented. The following is a tentative marking scheme and
what is expected to be submitted for this assignment:
1. External Documentation (as many pages necessary to fulfill the requirements listed below.)
including the following:
a. Title page
b. A table of contents
c. [20%] System documentation
i. A high-level data flow diagram for the system
ii. A list of routines and their brief descriptions
iii. Implementation details
d. [5%] Test documentation
i. How you tested your program
ii. Test sets must include
- input files .inpf; binary files .binf; and output files .outf.
- You may use the quote below as one of your testing files. You
may name it as winVirus.inpf.
e. [5%] User documentation
i. How to run your program
ii. Describe parameter (if any)
2. Source Code
a. [65%] Correctness
b. [5%] Programming style
i. Layeringii. Readability
iii. Comments
iv. Efficiency
