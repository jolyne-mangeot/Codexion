# Codexion

An algorithmic challenge based on the dining philosopher problem to practice C threads, mutex and time management.

## Description

In the Codexion project, `n` coders sit around a quantum computer they use to compile project with. To do so, they require the use of two dongles to insert in the computer. `n` dongles are set around the table, one between each coder. This means, no two neighbouring coders can compile at the same time.

However, each coder must compile from time to time. Otherwise, they would burn out, and the simulation would be a failure. The challenge, then, is to ensure all coders can compile `x` times without ever having a burn-out. No dongle can move between coders, so they will always be picked up by the same two neighbouring coders, and coders cannot communicate.

Two scheduling options were asked to be implemented. First-In First-Out, meaning the dongles are picked by the first coder who requested so, and Earliest Deadline First, so the coder closest to burn-out is prioritized.

This project helps discovering and practicing the following skills:
- Command-Line Argument Parsing
- Resources Management & Starvation Prevention
- Scheduling
- System Design Trade-off Analysis
- Synchronization Primitives
- Multithreading

## Instructions

Compilation, execution and cleaning-up can all be done using the provided Makefile. A Makefile is a script containing rules that can be called in a terminal. Here are the integrated rules and their effect.

| Command called | Effect |
|---|---|
| `make` or `make all` | Compile the project if needed, then calls the newly created executable with default values. These can be modified from within the Makefile. |
| `make clean` | Remove all temporary compiling files, such as .o files, which helps reduce unnecessary compilations. |
| `make fclean` | Calls make clean, but also remove the executable file. |
| `make re` | Calls make fclean then all, effectively reloading the entire project and executing it. |

## Blocking cases handled

The core challenge is avoiding deadlock situations, one where each coder is stuck waiting for their neighbour to be finished. And as all coders sit in a circle, it can happen that, each waits for their two dongles to be free, but is first-in-line for one of them, making the entire circle of coders wait for something to change. To avoid such cases, the order of operations to achieve compilations is definitive.

First, each coder register in both their dongles waiting queue. Then, until they are first-in-line for both, they don't touch any dongles, leaving them set down. But, to avoid each coder being first-in-line in only one of their two dongles, coders with an odd index are given a headstart (sacrificing on burn-out time).

## Thread synchronization mechanisms

This section focuses on the technologies used to avoid deadlock cases and the structural aspect of the project.

Each coder is given an instantiated structure, and runs on its dedicated thread. To ensure data optimization, coders have access to common structures using pointers, as well as, of course, their accessible dongles.

Another thread is created, named monitor. Its mission is to update both coders and dongles to check if any simulation-ending condition has been met, and if a dongle has entered its cooldown before being used again. If any coder enters the burn-out state, or if all coders are done with their compilations, monitor ends, all threads are joined, and structures are freed.

Coders, dongles and the access to the printf function are all protected by mutexes. A mutex can be locked to access variables viable to change during reading, ensuring information isn't duplicated or obsolete by the time of reading. These are data races.

Locking multiple mutexes at once was avoided whenever possible to dodge more deadlock cases. A single case of this remains during the pick-up of dongles, so coders effectively take both dongles at once and can't be "stolen" from.

A single condition variable was used to ensure all threads begin at the same time, rather than from the moment they are initiated one after the other. This condition variable is waited by all of them, coders and monitor, until the main thread triggers it, and the simulation actually starts.

All threads and structures are kept by the main threads to be freed when the simulation ends. It's important then to destroy mutexes before freeing the memory they were initiated in, to avoid memory leaks.

## Resources

> [!NOTE]
> No AI was used in the making of this project

Each file of this project was coded individually to best divide tasks for myself and ease organization. For a concrete project like this one, with not much liberty on the demanded output, I chose to reduce the spread of information, keeping a single header file.

Learning resources:
- [Multi-threading in C](https://www.geeksforgeeks.org/c/multithreading-in-c/)
- [Mutexes with C threads](https://www.geeksforgeeks.org/linux-unix/mutex-lock-for-linux-thread-synchronization/)

Specific resources:
- [Starting all threads at the same time](https://stackoverflow.com/questions/48753142/starting-all-threads-at-the-same-time)
