# Virtualization in XV6

This project is a part of the curse "Operating Systems" (Spring-2019) taught by Prof. Smruti Ranjan Sarangi.
The project requires to implement container related services in the UNIX-v6 based operating system XV6 to implement the following features:
* Container manager
* Virtual scheduler
* System calls
* Resource isolation (Virtual file system)

[Problem Statement](https://github.com/vinayak1998/xv6-public/blob/master/Project%20Details/Problem%20Statement.pdf)

[Project Report](https://github.com/vinayak1998/xv6-public/blob/master/Project%20Details/Project%20Report.pdf)

### High level design of the virtualization in XV6. Color coded arrows represent an operation, with number showing the order.:

![diagram for high level design of virtualization in xv6](https://github.com/vinayak1998/xv6-public/blob/master/High%20level%20design.png)

## Container manager
Container manager is a user program which mimics the functionalities provided by a container service (container manager.c). The basic functionalities provided are:
* Maintenance of the data structures required for the container service. 
* Scheduling of the processes within the container.

> (note: there can be more than one container running at the same time, and each of them are isolated from each other. The data structures maintained by the container manager are local to a specific container.)

The following system calls are implemented:

#### Create Container [uint create_container(void)]
This system call creates a container. 
It chooses thefFirst inactive/unused slot for initialization, and then changes the status to active, returning the cid of the newly created container.
It returns a negative number if the operations fails. (note that the container id is unique across the containers)


#### Join Container[uint join_container(uint container_id)]
The processes will be created using the old fork command. This system call is used by processes to join a container.
* Process is assigned the first empty slot in the container which it requests to join.
* If required, a new container can also be created with 0 initial processes

> note: The XV6 kernel is aware of this process as this virtual system call internally calls the fork system call, however, the scheduling is done by the container service.


#### Leave Container[uint leave_container(void)]
* Find cid of the container to w/c process belongs.
* Number of Process decreased by 1 after previous step.
* If it was the last process, set the state of the container to "READY"

#### Destroy Container [uint destroy_container(uint container_id)]
This system call kills all processes of the container (whose container_id is passed as the parameter) and the Container slot slate is set back to "UNUSED"


## Virtual scheduler
In a container, there can be more than one process can be running at a given time. It is the responsibility of the container to schedule them.
As this project is mimicking a container and not a complete system, the process created within the container is visible to the xv6 kernel and xv6’s scheduler will try to schedule it. To prevent this the state of the process is kept as WAITING and only changed to READY when the container scheduler decides to run this process. Hence, the container maintains the STATE (called V STATE) of the processes within its local data structure and does not rely on the STATE present inside the PCB (process control block).

## System calls
In an actual container implementation, the system calls are trapped by the container and are issued on behalf of the process after verification (whether the process is allowed to make this system call or not). However, in this case if the process makes a system call, it will directly go to the XV6 kernel. Apprpriate changes are made to the kernel so that the container of a particular process is aware of the process activities and the container manager has the capability of blocking certain system calls if the process making that call is not authorized to make it.
It is the responsibility of the kernel to provide the appropriate isolation between two containers. That is processes running in one container is not visible in another container. Also, once a process has joined a particular container it is not be visible to the host machine and any changes made by that process henceforth also are hidden from the host machine.

## Resource isolation 
Containers are isolated from each other by using the mechanisms provided by the kernel. 
For file systems, the containers have a common view of the host file system at the time of their creation. However, any changes made to the file system by a container is local to that container.
### Virtual file system
As with the memory management, a container provides support for file system also. The system calls are trapped and served by the container. The system calls (open, read, write and close) are modified which will be used by the processes running inside the container for their operations. The returned value of these calls will be a cfd ,i.e. a container file descriptor. It is a one to one mapping with fd returned by the XV6 kernel. A table, similar to the page table, is used to maintain this key value pair relationship

# XV6:
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI C.

#### ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also https://pdos.csail.mit.edu/6.828/, which
provides pointers to on-line resources for v6.

xv6 borrows code from the following sources:
    JOS (asm.h, elf.h, mmu.h, bootasm.S, ide.c, console.c, and others)
    Plan 9 (entryother.S, mp.h, mp.c, lapic.c)
    FreeBSD (ioapic.c)
    NetBSD (console.c)

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by Silas
Boyd-Wickizer, Anton Burtsev, Cody Cutler, Mike CAT, Tej Chajed, eyalz800,
Nelson Elhage, Saar Ettinger, Alice Ferrazzi, Nathaniel Filardo, Peter
Froehlich, Yakir Goaron,Shivam Handa, Bryan Henry, Jim Huang, Alexander
Kapshuk, Anders Kaseorg, kehao95, Wolfgang Keller, Eddie Kohler, Austin
Liew, Imbar Marinescu, Yandong Mao, Matan Shabtay, Hitoshi Mitake, Carmi
Merimovich, Mark Morrissey, mtasm, Joel Nider, Greg Price, Ayan Shafqat,
Eldar Sehayek, Yongming Shen, Cam Tenny, tyfkda, Rafael Ubal, Warren
Toomey, Stephen Tu, Pablo Ventura, Xi Wang, Keiichi Watanabe, Nicolas
Wolovick, wxdao, Grant Wu, Jindong Zhang, Icenowy Zheng, and Zou Chang Wei.

The code in the files that constitute xv6 is
Copyright 2006-2018 Frans Kaashoek, Robert Morris, and Russ Cox.

#### ERROR REPORTS

Please send errors and suggestions to Frans Kaashoek and Robert Morris
(kaashoek,rtm@mit.edu). The main purpose of xv6 is as a teaching
operating system for MIT's 6.828, so we are more interested in
simplifications and clarifications than new features.

#### BUILDING AND RUNNING XV6

To build xv6 on an x86 ELF machine (like Linux or FreeBSD), run
"make". On non-x86 or non-ELF machines (like OS X, even on x86), you
will need to install a cross-compiler gcc suite capable of producing
x86 ELF binaries (see https://pdos.csail.mit.edu/6.828/).
Then run "make TOOLPREFIX=i386-jos-elf-". Now install the QEMU PC
simulator and run "make qemu".
