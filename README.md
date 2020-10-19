# Radio system with location dependent errors

In addition to the previous problem (of LTE scheduling), we model the situation when a user has a very bad radio channel for a certain time period.

Consider a radio system consisting of K users (e.g. K=3) which transfer data.

Between the moments t1 and t2, one of the users has a very bad radio link, with so many errors that the scheduler will not allocate radio resources (radio channels) to that user.

Compared to an ideal system (i.e., without errors), the user affected by (location dependent) errors will be lagging, which means that the amount of data transfered by that user is with LAG smaller than the data transfered by its correspondent in the ideal system.

After the moment t2 the user will have again a good radio link, and its weight in the scheduling algorithm has to be increased, with the purpose to reduce the value of LAG to zero, or very close to zero.

LAG is defined as the difference between the amount of data transdered by a user in the ideal system, and the amount of data transfered by the same user, in the real system.

Like in the previous problem, you can start from the fifo model from samples.

We can consider either a discrete scheduling algorithm (e.g. WRR - Weighted Round Robin), or you can consider that each user has a transfer rate equal with 1/K of the capacity C of the network (e.g. C= 1 Mega bits per second).

Try to adjust the weight of the user affected by errors (respectively its transfer rate) on the expense of the other users.

## Minimal requirements

Implement the simulation model using a non-adaptive scheduling algorithm (e.g. Weighted Round Robin).

## For the exam

1. Adapt the scheduling algorithm in order to allow the lagging user to reduce its lag. For example, you can increase its weight in the WRR algorithm.
2. Compare the performance of the scheduling algorithm without adaptaion with the performance of the scheduling algorithm with.
