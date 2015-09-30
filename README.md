Transactional Memory Study
============

*Virtues and Limitations of Commodity Hardware Transactional Memory*

Transactional Memory was recently integrated in Intel processors under the name TSX.
In this framework you can find several synchronization backends for a set of common benchmarks, including several based on TSX. They range from software to hardware based, also including hybrid ones.

This framework depends on the libraries 'numactl', 'libnuma-dev', 'python-software-properties', and GCC with support for RTM intrinsics.
After that, you may compile the benchmarks as follows:
 
1) compile each STM using their Makefile(s) (norec, tl2, swissTM, and TinySTM)
2) compile the benchmarks for a given backend
    a) bash build.sh <backend> <lock>
 or
    b) bash build-locks.sh <backend> <lock> <granularity> 


The second command is different as it builds the benchmarks with fine grained locking support. It thus accepts an integer as the last parameter for the number of locks to draw the granularity. A typical value is 10000.

The available backends are:
 - singlelock - for a single global lock
 - norec - for the STM NOrec
 - tl2 - for the STM TL2
 - swisstm - for the STM SwissTM
 - tinystm - for the STM TinySTM
 - tsx - for Intel TSX with a single global lock fallback
 - tsxnorec - for Intel TSX hybrid with NOrec STM
 - tsxtl2 - for Intel TSX hybrid with TL2 STM
 - finelocks - for fine-grained locks
 - tsxfinelocks - for Intel TSX with fine-grained locks fallback

The available locks are those in the SSYNC library. For instance, HTICKET_LOCKS.


After building the binaries, you may find them inside the folder of each benchmark.

*Very important*: to collect energy measurements, the framework needs to run with root privileges. Remember this whenever you run the benchmarks.

You may also need to issue the following command before that: "sudo modprobe msr"

*Note*: if you seek to run with more than 8 threads, you need to modify the thread.c files for the backends where the threads are bound to the available CPUs.


For questions, contact the author:
Nuno Diegues - nmld@tecnico.ulisboa.pt

When using this work, please cite accordingly: 
 Nuno Diegues, Paolo Romano, and Luis Rodrigues, "Virtues and Limitations of Commodity Hardware Transactional Memory", Proceedings of the International Conference on Parallel Architectures and Compiler Techniques, PACT 2014

You may also find the following paper relevant for citation:
 Nuno Diegues and Paolo Romano, "Self-Tuning Intel Transactional Synchronization Extensions", Proceedings of the International Conference on Autonomic Computing, ICAC 2014

This work was supported by national funds through Fundação para a Ciência e Tecnologia (FCT) with reference UID/CEC/50021/2013 and by the GreenTM project (EXPL/EEI-ESS/0361/2013).
