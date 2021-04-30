# OS project 2: process-memory-tracker

11810935 Lan Lu

11811127 Ziyue Zhou

11812202 Weibao Fu

## Project Background and Description

Memory is one of the most import components of computers. It can store the computation data for CPU and transfer data with the external storage. All programs in computers should run in the memory and obviously the degree of memory performance will make a difference to the overall level of computer performance. However, without good management and protection on memory, it will easily lead to some problems or failures such as memory leak.

Memory leak is usually divided into two types, heap leak and resource leak, and will result in a waste of memory resources, slow program running  and even more severe consequences like completely broken up of the system. Heap leak occurs when the dynamically allocated heap memory which will not be used anymore is not released in time or is unable to be released due to some certain reasons. For example, static classes with the same long lifecycle as the program hold references of objects with short lifecycles which also can not be released before the ending of this program or collection classes hold references of objects which will not be used. On the other hand, resource leak refers to the problem that system allocated resources like bitmap, handle or socket are not released by the corresponding functions. Regardless of external factors, during the program development process, references to objects often can not be released in time because of some poor code design. Some specific situations which lead to memory leak in programs based on `C/C++` are as follows. 

1. Memory allocated by `malloc` and `new` in the constructors are not released by `free` and `delete` in their destructors respectively while they are not in use.
2. Nested object pointers are not cleaned correctly.
3.  Square brackets are not used correctly when releasing an array object. On the one hand, without the square bracket to indicate the releasing of an array object, destructor will only be called for the first object in the array. On the other hand, if there is an number larger than the size of this array in the square brackets, memory overflow will happen and leads to the corruption of heap.
4. Pointer array of an array object is not distinguished with the array object itself. The developers are expected to release both the memory allocated for the object and its pointer at the same time.
5. Lack of user-defined copy constructor when copying an object with pointers. In this way, a default copy constructor is used and copies each member attribute one by one. However, passing by value can use the default copy constructor correctly while passing by reference can not,  which will make the two pointers in the previous and new objects pointing to the same address of a dynamically allocated memory space. This may even lead to other errors like double free of the same memory space as well.
6. Lack of overload assignment operator. While copy constructor is used when the programs use one existed object to create another object, assignment means the existed object is used to cover another already existed object. However, problems in this case are similar to the previous one 
7. Reference of a local variable is used as the return value.
8. The destructor of the base class is not defined as a virtual function. In this case, if a pointer in basic type is used to point to a child class object and then the developer wants to delete it, the destructor for the child class can not be called and thus resources are not released properly.

While the price of memory  failure is expensive, the detection of it is hard. Then it seems extremely necessary to have a tool for collecting and memory statistics and detect potential errors and alert problems to users. At the same time, displaying the real-time memory status should be helpful for users to monitor the program states which also has a great significance for maintaining a stable system.

## Expected goals

Through this project, we are expected to realize a memory tracker on Linux operating system which aims to collect and analyze the real-time memory usage statistics of programs written in and then report potential problems, especially  memory leak problem, for better memory management. Besides, the designed memory tracker should fit in most operating systems based on Linux. Since the project as a whole is large, we divide it into the following small tasks.

1.  Collect real-time memory usage statistics from every existed process.
2.  Rank and display memory usage statistics.
3.  Detect and record memory allocation  and release status for specific processes.
4.  Detect and record file handle allocation  and release status for specific processes.
5.  Detect and report potential memory leak according to the tracked allocation and release information.
6. Pack the above features together as our final memory tracker.

Besides what have been discussed above, we have also planed to reach other achievements which focus on monitoring memory performance in other aspects instead of just memory leak if time is sufficient.

1. Detect and alert potential memory overflow or underflow especially caused by read and write.
2. Detect and alert use on uninitialized memory.
3. Detect and alert read and write operations on memory after it has been freed.
4. Detect and alert use on memory beyond the allocated size.
5. Detect and alert invalid access to heap and stack.

Taking into the time limit for this project, we may not allowed to realize all the above. However, we are prepared to try our best to realize a stronger memory tracker.

## Implementation

There are already some well-developed products for automatically detecting memory management and thread bugs, such as `Valgrind `, `memtest`, `mtrace`, `dmalloc` and so on. Our memory can take their well-defined structure as reference. We take `Valgrind` as an example here to briefly illustrate the existing techniques.

`Valgrind ` is a very powerful tool that we can refer to, it actually contains a set of tool, its basic structure is like the picture shown below:

![1619696458(1)](README.assets/1619696458(1).png)

Actually `Memcheck` is the main tool regarding almost all memory problem, it detects the memory leakage and classify the kinds of memory leakage as  four categories:

**1) still reachable:** which means that the pointer still exist and still have a chance to use it or release it, the dynamic memory it points to quits before release.

**2) definitely lost:** it is memory leakage for sure, there is no possible way to get access to that chunk of memory.

**3) indirectly lost:** the pointer to this chunk of memory is located at a part that has memory leakage problem.

**4) possibly lost:** possible memory leakage, there still exist a pointer that can get access to this chunk of memory, but is not pointing to the initial location of this chunk of memory.

`Memcheck` also reveals the implementation structure so that it can do memory tracking:

![1619697481(1)](README.assets/1619697481(1).png)

Its structure implementation creates two global map, `valid-value map` and `valid-address map`  . The `valid-value map`  have corresponding 8 bits for each of the byte in the address space, and for each CPU register, it also has a relevant bit vector. These bits is responsible for recording whether the bit or the value of register is initialized and valid. The `valid-address map`  have 1 bit for each byte in the entire address space of a process, it is used for recording the whether it can be read/write.

Under this structure, the detect principle is just out there, when try to read/write any byte in the memory, it first check the valid-address bit of the byte, if it is invalid, then report read/write error. Whenever some byte is loaded into the CPU, its relevant valid-value bits is loaded into the core (which we can treat it as a virtual CPU environment). Thus, whenever the value in the register is used to create memory address or may affect the output of the program, then `Memcheck` will check the valid-value bits, if it indicates that it is not valid/not initialized it will report error.

`Valgrind` has a mature implementation, we are still digging into the specific way of implement our version of memory tracker, its structure seems very inspiring and worth referencing. We might merge its basic implementation and our own innovative implementation thoughts later to realize a light-weight and more user friendly product.

## Division of labor

A complete procedure for reporting a potential memory problem is divided into three steps including statistics collection and organization, data analysis as well as problem detection.  It is expected that the three members in our group play different roles in one problem reporting procedure and change their task in the next turn for another problem. At the same time, verification and packing of our work should be finished together to guarantee the correctness and consistency of our project.

Last but not the least, the division of labor can be adjusted flexibly according to the real situation and cooperation between group members is greatly appreciated.
