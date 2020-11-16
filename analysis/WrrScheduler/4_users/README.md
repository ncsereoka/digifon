# Analysis for four users

## Setup

We follow the recommended conditions:

- a scheduling cycle of **1 ms** (an LTE network);
- 30 channels which can be allocated.
  
For this analysis, we chose the following weights for the users: **w = [1, 2, 4, 8]**.

The user that gets disconnected is the user with the weight of 1.

## No tinkering - the weight of the user remains the same after reconnecting

![base](./base.svg)
The other uses don't get affected.

<br/>
<br/>
<br/>

## Some weight change - the reconnected user gets a weight of 8

![adap1](adap1.svg)
The unlucky user gets the same weight as the user with the highest priority - a weight of 8. Users started to get affected.

<br/>
<br/>
<br/>

## Medium weight change - the reconnected user gets a weight of 16

![adap2](adap2.svg)
We see an increase in lag.

<br/>
<br/>
<br/>

## Heavy weight change - the reconnected user gets a weight of 24

![adap3](adap3.svg)
At this point, all the other users gathered up lag.

<br/>
<br/>
<br/>

## Extreme weight change - the reconnected user gets a weight of 32

![adap4](adap4.svg)
The newly allocated weight exceeds the number of channels which can be allocated.

We can see that the network won't allocate channels to any other user until the queue of the unlucky one gets depleted - causing heavy lag to everybody else.
