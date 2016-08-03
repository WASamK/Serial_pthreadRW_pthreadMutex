# Serial_pthreadRW_pthreadMutex
Perfromance comparison for a linked list implementation as a serial program, parallel program (based on Pthreads) with one mutex for the entire linked list and parallel program (based on Pthreads) with read-write locks for the entire linked list

Linked list as a Serial program
File   :  serial.c
Compile:  gcc -g -Wall -o serial serial.c
Run    :  ./serial <n> <m> <mMember> <mInsert> <mDelete>

Linked list as a Parallel program with one mutex for the entire linked list
File   :  pthreadMutex.c
Compile:  gcc -g -Wall -o pthreadMutex pthreadMutex.c -pthread
Run    :  ./pthreadMutex <threadCount> <n> <m> <mMember> <mInsert> <mDelete>

Linked list as a Parallel program with one mutex for the entire linked list
File   :  pthreadRW.c
Compile:  gcc -g -Wall -o pthreadRW pthreadRW.c -pthread
Run    :  ./pthreadRW <threadCount> <n> <m> <mMember> <mInsert> <mDelete>

Results and conclusion : https://samadhipoornima.wordpress.com/category/projects/serial-pthreadrw-pthreadmutex/
