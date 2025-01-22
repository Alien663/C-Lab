# Unix Programing

## Homework 1
web pages for usp and its programs
GNU make document
note of activation record
exercise 1: Compiling C programs
Your have to write four files, a makefile, two c programs and a c header file. (Refer to A.3)
The mylib.c should implement a function named transpose() which will transpose the input string in place, that is, lowercase to uppercase or uppercase to lowercase. Its prototype is put in mylib.h and the prototype is:
void transpose(char *s);
The main() in main.c should call the transpose() on each argument and print them out.
For example:
```bash
ls
Makefile  main.c  mylib.c  mylib.h
make
cc    -c -o main.o main.c
cc    -c -o mylib.o mylib.c
gcc -o trans main.o mylib.o
make
make: 'trans' is up to date.
touch mylib.c
make
cc    -c -o mylib.o mylib.c
gcc -o trans main.o mylib.o
touch main.c
make
cc    -c -o main.o main.c
gcc -o trans main.o mylib.o
make
make: 'trans' is up to date.
ls
Makefile  main.c  main.o  mylib.c  mylib.h  mylib.o  trans*

./trans Klim is 300@moon
kLIM
IS
300@MOON

```
Due: 2019/Feb/27, 00:05
You have to put your files under ~/usp-1072/exer1.

## Homework 2
For your reference, a program which shows the arguments is provided.
exercise 2: probing argv[] and environ[]
Write a program which prints the values of argc, argv[], and environ[]. Also their addresses are printed. Based on the printed information, you can figure out the memory layout of argc, argv[], and environ[].
For example:
```bash
gcc ex2-argv-env.c 
./a.out this is a test
argc is 5
argv[0]: ./a.out
argv[1]: this
argv[2]: is
argv[3]: a
argv[4]: test
environ[0]: CPLUS_INCLUDE_PATH=/usr/lib64/qt/include
environ[1]: NNTPSERVER=news.ncnu.edu.tw
environ[2]: MANPATH=/home/klim/man:/usr/local/info:/usr/local/man:/usr/man:/usr/share/texmf/man
environ[3]: HOSTNAME=lilina.csie.ncnu.edu.tw
environ[4]: TERM=xterm
environ[5]: SHELL=/bin/bash
environ[6]: SSH_CLIENT=10.20.21.206 52766 22
environ[7]: SSH_TTY=/dev/pts/2
......
environ[32]: INFOPATH=/usr/info:/usr/share/texmf/info
environ[33]: G_BROKEN_FILENAMES=1
environ[34]: _=./a.out
environ[35]: OLDPWD=/home/klim/course/usp-1072

argc      is resided at 0x7fffefa44e2c
argv      is resided at 0x7fffefa44e20
environ   is resided at 0x600dc0
argv[]    is resided at 0x7fffefa44f28
environ[] is resided at 0x7fffefa44f58
value of argv[ 0] is 0x7fffefa454ff
value of argv[ 1] is 0x7fffefa45507
value of argv[ 2] is 0x7fffefa4550c
value of argv[ 3] is 0x7fffefa4550f
value of argv[ 4] is 0x7fffefa45511
value of argv[ 5] is (nil)
value of env [ 0] is 0x7fffefa45516
value of env [ 1] is 0x7fffefa4553f
value of env [ 2] is 0x7fffefa4555b
value of env [ 3] is 0x7fffefa455af
value of env [ 4] is 0x7fffefa455d0
value of env [ 5] is 0x7fffefa455db
value of env [ 6] is 0x7fffefa455eb
value of env [ 7] is 0x7fffefa4560c
........
value of env [32] is 0x7fffefa45f86
value of env [33] is 0x7fffefa45faf
value of env [34] is 0x7fffefa45fc4
value of env [35] is 0x7fffefa45fce
```

## Homework 3
deadline: 2019/Mar/06, 00:05
Put your files under ~/usp-1072/exer2
exercise 3: simulating env
Refer to section 2.12.
You have to support at least the following four usages.
```bash
       $ env                  
       $ env cmd arg1 arg2 ....
       $ env -i
       $ env -i e1=v1 e2=v2 .... cmd arg1 arg2 ....
```
Note that there may be more than one environment string and there may be more than one argument for the cmd. To invoke system(), you have to combine `cmd' and all the `arg1 arg2 ....' to form a single string, which is then passed to system().

deadline: 2019/Mar/13
Put your files under ~/usp-1072/exer3

## Homework 4
exercise 4: creating processes with fork()
Write a program to create a process tree.
Each process will print a line to show its pid and ppid.
Use wait() to ensure the lines be printed in post-order.
The examples are shown below. Left side are the required output. Right side are the corresponding processs trees.
```bash
echo $$
1567                             #|  
./a.out                          #|     
I'm 2258, my parent is 1567      #| 
./a.out 1                        #|   2264
I'm 2265, my parent is 2264      #|    |
I'm 2264, my parent is 1567      #|   2265  
./a.out 2                        #|     
I'm 2269, my parent is 2268      #|   2268 
I'm 2270, my parent is 2268      #|   /  \
I'm 2268, my parent is 1567      #| 2269 2270
./a.out 2 1                      #|           2278
I'm 2280, my parent is 2279      #|           /  \
I'm 2279, my parent is 2278      #|         2279 2281
I'm 2281, my parent is 2278      #|          |
I'm 2278, my parent is 1567      #|         2280
./a.out 2 1 3                    #|
I'm 2527, my parent is 2526      #|     2524
I'm 2528, my parent is 2526      #|     /  \
I'm 2529, my parent is 2526      #|   2525 2530
I'm 2526, my parent is 2525      #|     |
I'm 2525, my parent is 2524      #|    2526
I'm 2530, my parent is 2524      #|  /   |   \
I'm 2524, my parent is 1567      #| 2527 2528 2529
```
deadline: 2019/Mar/20, 00:05
Put your files under ~/usp-1072/exer4


## Homework 5
exercise 5: practicing wait()
Refer to the description of exercise 2 (usp-982) for explanation of the code .
Remove the wait(NULL) in the loop. After the line of printing a message to stderr, you should insert code like:

```c
If the process is a leaf,
        while(1) pause();
      
else (the process is an internal node)
        while there are children alive do 
          waiting..
        exit(...);   
```     
The internal node should wait for all children, and for each child, when waited, reports the signal number if it was terminated by a signal, or the value returned if it exited though exit(). In either case, a number is gained from the child. Those numbers from children are summed and the sum is used as the argument of exit(). This is to say the sum is the return value of this process.
Refer to example 3.22 for the usage of wait().
To test your program, you have to open another window through which you can kill a process with the command `kill'.
For example, to kill a process which pid is 12011 with signal 15, you can type 'kill -15 12011'.
For example:
```bash
$ ./a.out ddduuduu abcd                      a
I'm c, my pid=3590, and my ppid=3589        / \
I'm b, my pid=3589, and my ppid=3588       b   d
I'm d, my pid=3591, and my ppid=3588       |
I'm a, my pid=3588, and my ppid=3295       c
Child 3590 is terminated with signal 3.       $ kill -3 3590
Child 3589 exits with value 3.
Child 3591 is terminated with signal 2.       $ kill -2 3591
$ echo $?                            # Check the return value of root.
5                                    # It is the sum of 2 and 3.
$ ./a.out ddduuduu abcd              # Run it agagin.
I'm c, my pid=3594, and my ppid=3593 # This time we try to kill the 
I'm b, my pid=3593, and my ppid=3592 # internal node. The leaf node will
I'm d, my pid=3595, and my ppid=3592 # be an orphan and be adopted by init.
I'm a, my pid=3592, and my ppid=3295 
Child 3593 is terminated with signal 4.       $ kill -4 3593
Child 3595 is terminated with signal 15.      $ kill -15 3595
$ echo $?
19
$ ps -ef | grep a.out                # Find the orphan. Check its ppid.
klim      3594     1  0 13:39 pts/0    00:00:00 ./a.out ddduuduu abcd
klim      3597  3295  0 13:40 pts/0    00:00:00 grep a.out
$ 
```
deadline: 2019/Mar/27, 00:05
Put your files under ~/usp-1072/exer5

## Homework 6
exercise 6: practicing I/O
Write a program which is invoked as
./a.out str-a str-b [f1 f2 ....]
The program will read input from stdin if no files are provided or will read input from the specified files. For each line read, each occurrence of str-a will be replaced by str-b. The processed line is written to stdout. For example,
```bash
$ cat k1
is oak still OK? 
or oak is still oak?
$ gcc ex6-substr.c 
$ ./a.out oak klim < k1             #  no file are provided
is klim still OK? 
or klim is still klim?
$ ./a.out oak klim k1 k1 k1         #  3 files to handle
is klim still OK? 
or klim is still klim?
is klim still OK? 
or klim is still klim?
is klim still OK? 
or klim is still klim?
$ ./a.out oak YG k1                 # test again with another string
is YG still OK? 
or YG is still YG?
$ 
```      
For string manipulation, try 'man string' to find usable functions.
Deadline: 2019/Apr/03/ 00:05am
Put your file(s) under ~/usp-1072/exer6

## Homework 7
Only read(), write() and the functions defined in text are allowed.
exercise 7: practicing pipe()
Refer to the description of exercise 2 (usp-982) for explanation of code .
Use the code shown above to build a process tree, but between a parent and each one of its children you have to create two pipes. One pipe is for parent's sending to child, the other is for child's sending to the parent.
After created, each process will do something as follows.
      loop
        if root then  ch = get a char from stdin
                else  ch = get a char from parent
      
        if ch==label then ch=POST
        for each child  do
          send ch to child
          get ack from child 
        if ch==POST then
           show this process's info

        if not root then send ack to parent
        if ch='q' then break
      forever
      
For example:
```bash
$ ./a.out ddduduudduuu abcdef
I'm a, my pid=1, and my ppid=813
I'm e, my pid=1, and my ppid=1636
I'm b, my pid=1, and my ppid=1636
I'm f, my pid=1, and my ppid=1638
I'm d, my pid=1, and my ppid=1637
I'm c, my pid=1, and my ppid=1637
a
I'm c, my pid=1640, and my ppid=1637
I'm d, my pid=1641, and my ppid=1637
I'm b, my pid=1637, and my ppid=1636
I'm f, my pid=1639, and my ppid=1638
I'm e, my pid=1638, and my ppid=1636
I'm a, my pid=1636, and my ppid=813
b
I'm c, my pid=1640, and my ppid=1637
I'm d, my pid=1641, and my ppid=1637
I'm b, my pid=1637, and my ppid=1636
e
I'm f, my pid=1639, and my ppid=1638
I'm e, my pid=1638, and my ppid=1636
q
$ 
```      
Deadline: 2019 Apr 17 00:05am
Put your file(s) under ~/usp-1072/exer7

## Homework 8
exercise 8: practicing select() or poll()
Generate a bidirectional ring. Refer to program 7.1. The argv[1] specified the number of processes.
After the ring is setup, the root process then does
        loop
            read from stdin 
            if got 'f' then 
              send 0 1 to the clockwise next process
              read two numbers from clockwise previous process
              write them out
            if got 'p' then 
              write a token to the anti-clockwise next process
              read a token from anti-clockwise previous process
              show pid
        end loop
      
Each child does
        loop
           monitor two inputs simultaneously 
           (one clockwise and another anti-clockwise)
           if clockwise input is ready then 
             read two numbers, a and b, from clockwise previous process
             show them
             write b (a+b) to clockwise next process
           if anti-clockwise input is ready then
             read a token from anti-clockwise previous process
             show pid
        end loop
      
You may design a method with which all process can be terminated gracefully.
Due: 2019/May/08, 00:05
For example:
```bash
$ ./a.out 8
p
I'am  598
I'am  596
I'am  594
I'am  593
I'am  591
I'am  590
I'am  589
I'am  588, the root
f
I am  589, I got   0   1
I am  590, I got   1   1
I am  591, I got   1   2
I am  593, I got   2   3
I am  594, I got   3   5
I am  596, I got   5   8
I am  598, I got   8  13
I am root. I got  13  21
q
$ ./a.out 3
p
I'am  607
I'am  606
I'am  600, the root
f
I am  606, I got   0   1
I am  607, I got   1   1
I am root. I got   1   2
q
$ 
```

## Homework 9
exercise 9: practicing binary read() and write()
Write a program to sort an integer array. For example,
 $ ./sort-int array-a array-b 
The two arguments denote files which contains an array. The layout of a file is:
First 4 bytes stores the size. (An integer, denoted as n, stored in binary big-endian)
Next 4*n byte store the integer array. Each integer occupy 4 bytes and is stored in binary, big-endian.
You should invoke qsort() to sort integers.
Only the functions mentioned in textbook are allowed.
Due: 2019/May/15, 00:05
For example:
```bash
$ od -t x1 k1      # 6 numbers: 12 5 6 9 3 191
0000000 00 00 00 06 00 00 00 0c 00 00 00 05 00 00 00 06
0000020 00 00 00 09 00 00 00 03 00 00 00 bf
0000034
$ gcc ex9-sort-int.c 
$ ./a.out k1 k2
$ od -t x1 k2
0000000 00 00 00 06 00 00 00 03 00 00 00 05 00 00 00 06
0000020 00 00 00 09 00 00 00 0c 00 00 00 bf
0000034
$ 
```
For your convenience, here is the test file int-array inside which are 6 12 5 6 9 3 191. The first 6 is the size of array. Others are the contents of the array.


# Homework 10
exercise 10: practicing sigaction
Similar to exercise 8, but the ring is uni-directional. So just refer to program 7.1 for instruction.
The argv[1] specified the number of processes.
After the ring is setup, each process will wait for three signals. One is for sending fibonacci numbers, one is for printing my pid, and the last one is for `quit'. These operations are just the same as those in exercise 8 but there are two differences.
First, these operations are initiated from any process which receives a signal.
Second, there is only ring. So each process may receive messages for 'p' or 'q' or 'f' from the the same neighbour.
Due: 2019/May/22, 00:05
For example:
```bash
$ ./a.out 5
I am 18149
I am 18150
I am 18151
I am 18152                          another window
I am 18153                   +-----------------------------------
I am 18152, I got   0   1    |   $ kill -USR1 18151
I am 18153, I got   1   1    |
I am 18149, I got   1   2    |
I am 18150, I got   2   3    |
I am 18151, I got   3   5    |
I am 18150                   |   $ kill -USR2 18149
I am 18151                   |
I am 18152                   |
I am 18153                   |
I am 18149                   |
I am 18149, I got   0   1    |   $ kill -USR1 18150; kill -USR1 18153
I am 18151, I got   0   1    |
I am 18152, I got   1   1    |
I am 18153, I got   1   2    |
I am 18149, I got   2   3    |
I am 18150, I got   1   1    |
I am 18150, I got   3   5    |
I am 18151, I got   1   2    |
I am 18152, I got   2   3    |
I am 18153, I got   3   5    |
$                            |   $ kill -INT 18152
```      

## Homework 11
exercise 11: practicing pthread
The main thread first set balls[] = {0, 1, 2, 3, 4, 5}.
Then permute the balls[].
Initialize permutations for balls as
perm[0] = {0, 1, 2, .... 48}
......
perm[5] = {0, 1, 2, .... 48}
Then generate 6 threads, denoted as t_0, t_1, ....t_5.
Thread t_i will permute perm[ balls[i] ].
The main thread then join all threads.
Then beginning with balls[], after applied permutations, 6 balls are gotton as the answer. The method is :
for i in 0..5 do balls[i] = perm[0][ balls[i] ];
repeat the previous step for perm[1], .... perm[5].
The following shows an example where the MAX is set to 18. (You should use 49 instead. And the final line is the only required output).
```bash
$ gcc ex11-thread-ball.c -lpthread
$ ./a.out 
The ball's initial setting:   0  1  3  2  4  5

The permuations for balls:
0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
------------------------------------------------------
13  4 15 10  3  7  1 11 12  6  8  5  9  0 16 14  2 17
16  0 15  1  5 11 12  7  4 10  3  6 14 13 17  8  9  2
7  6 15  5 14 13 11 17  8 10  4  1  3  0  9 12 16  2
0  9 14 12 17  7  4 11  8 13 10  2 16  1  5  6 15  3
16  6  8  1 11 14 10  7  2 17  0 15  9  3 12  4  5 13
5 12  8  3 17 16 14  1  2  6  0  7 10  9 13 11 15  4
------------------------------------------------------

After permutations applied, balls are:  15 14  1  8  7 12
So the answer is:                       16 15  2  9  8 13
$ 
```     
You should use thread-safe random functions, such as erand48(), or nrand48().
Each thread should use different seed.
Due: 2019/May/29, 00:05


## Homework 12
exercise 12: pthread synchronizing(mutex and cond)
Refer to Chapter 13.
Similar to previous exercise, but after initialization,
each of t_0, t_1, ...t_5 will wait for notification from main thread,
generate the permutations,
and acknowledge main thread that it has done.
And repeat the above operation forever.
The main thread after creating 6 threads will read from stdin.
When it reads a 'l', it will ask each of the six threads to permute.
When permutations are all done, the main thread figures out the balls.
The main thread continues until it reads a 'q'.
Each thread should use different seeds and you should use thread-safe random functions, such as erand48() or nrand48().
For example,
```bash
$ gcc ex12-thread-mutex.c -lpthread
$ ./a.out   
l
the balls are:   6 28 33 13 29 32
l
the balls are:  24 17 40 49 26 41
l
the balls are:  37  5 23 45 17 10
l
the balls are:  34 42 29 35 19 46
q
$  
```   
Due: 2019/Jun/05, 00:05