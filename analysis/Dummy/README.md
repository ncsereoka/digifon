# **Dummy**

Let's say that we have **N = 4** users, our array of weights looks like this: **w = [1, 2, 3, 4]** and we have **C = 30** channels to distribute.

**sum(wi)** **= 1 + 2 + 3 + 4 = 10**

This means that for a weight of **1**, a user will receive **factor = 1 x (30/10) = 3** channels.

So, **user0** will receive **c0 = w0 x factor = 3**, while the others will receive **c1 = 6**, **c2 = 9**, **c3 = 12**.

The Dummy algorithm will allocate the above mentioned channels each scheduling cycle - even if **userk** did not generate **ck** messages. This can lead to channels which are not used - and users with a low weight might get several messages stuck in their queue.

Let's take a very similar example.
Say we have **N = 4** users, an array of weights **w = [1, 2, 2, 3]** and **C = 30** channels to distribute.

**sum(wi)** **= 1 + 2 + 2 + 3 = 8**

This means that for a weight of **1**, a user will receive **factor = 1 x (30/8) = 3.75** channels.

So, **user0** will receive **c0 = w0 x factor = 3.75**, while the others will receive **c1 = 7.5**, **c2 = 7.5**, **c3 = 11.25**.

These, should be integer values, of course, so we take the integer parts and keep the remaining difference **c = [3, 7, 7, 11]**.

The remaining **30 - (3 + 7 + 7 + 11) = 2** channels are going to be distributed one by one, starting from the first user - we'll obtain **c = [4, 8, 7, 11]**.

These values change only when the unlucky user loses their connection.

Their weight gets set to zero, triggering a change in the channels allocated. Let's go back to our first example.

**C = 30**; **w = [1, 2, 3, 4]**

If **user3** loses their connection, the weights become: **w = [1, 2, 3, 0]**, the allocated channels: **c = [5, 10, 15, 0]**.

Once the unlucky user comes back online with a changed weight e.g. **w3 = 9**

the weights become: **w = [1, 2, 3, 9]**, then the allocated channels: **c = [2, 4, 6, 18]**.

## **Queue Aware**

The Queue Aware algorithm will also taken into account the length of each user's queue.

At first, we do a naive Dummy allocation. We'll probably find a user which produced less messages then they are able to send. We keep count of such users and the remaining channels that they leave behind.

Let's take the previous example: **C = 30**, **w = [1, 2, 3, 4]**, **c = [3, 6, 9, 12]**.

Let's say that these are the queue lengths: **q = [9, 6, 10, 2]**.

We can see that **user0** and **user2** exceed their allocated channels, **user1** hits just enough and **user3** produced much less.

Allocate all those channels that can and should be allocated, that is **c' = [3, 6, 9, 2]**.

We are left with 10 more channels waiting to be allocated (the last user only consumed 2 out of its allocated 12).

We'll do another dummy allocation, but keeping in mind those clients that have been fully served and reduce their weight to zero.

**C' = 10**, **w' = [1, 0, 3, 0]**, and then accordingly **c'' = [2.5, 0, 7.5, 0]** or rather **c''= [3, 0, 7, 0]**.

So now, for the final allocations - **ck = c'k + c''k**:

**c = [6, 6, 16, 2]**.

We can continue this process until we get to an optimal resource allocation. This algorithm will do the optimization only once.

**Observation!** The problem with this algorithm is that it prioritizes the first users. In a scenario in which the channels which can be allocated is much lower than the amount of messages the users generate, the unfortunate users at the end of the user array will rarely get any allocated channels.
