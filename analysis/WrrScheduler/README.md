# The Weighted Round-Robin Scheduler

The Weighted Round-Robin is a classic scheduling algorithm.

In our problem, this algorithm will look at the length of our users' queues and considering the number of channels which can be allocated, it will allocate each user a number of packets at a given time.

**Round-Robin** because we cycle through our users until the allocable channels are depleted.

**Weighted** because we give user a particular priority.
