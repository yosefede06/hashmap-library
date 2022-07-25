# hashmap-library
A library that containing a generic hash table type data structure (hashmap)

• Capacity of the table
Represents the maximum amount of members that can be kept in the data structure. In this exercise, the capacity of the table will be a power of 2, when the initial capacity of the table will be 16 (defined as constant). Note - capacity ≥ 1 necessarily holds. We will change the capacity according to the amount of values required to be stored.

• Load Factor
Apart from the actual input size, the performance of the hash map is affected by two parameters: an upper load factor and Lower load factor (load factor lower & load factor upper.) The load factor is defined as follows:

`LoadFactor = M / capacity, capacity > 0 ∧ M ≥ 0`

where M represents the amount of members that the data structure contains at a given moment (actually), and the capacity (we described in section 1.3). The load factor the top and bottom define how empty or full we will agree the data structure to be.
That is, if the load factor crosses a certain threshold, we will increase or decrease the capacity of the data structure accordingly, so that on the one hand we will not require a lot of unnecessary memory and on the other hand we can contain the amount of members we want.

• The Hash Function
As mentioned above, we must choose a "good" hash function to achieve an efficient data structure. We will use a hash function very simple:

`h(x) = x mod capacity , capacity ∈ ℕ s.t capacity ≥ 1`

Where the capacity is the same as how we defined it earlier. x is a numerical representation of the value we want to store in the table. to convert strings, numbers, and so on for an integer. Note that the modulo operator (%) in C does not work like the mathematical one.
For example, in C we will get 3 = −7%3,(− but the mathematical modulo will give the value 4. For this and other reasons, we calculate the modulo as follows:

`𝐯 𝐦𝐨𝐝 𝐜𝐚𝐩𝐚𝐜𝐢𝐭𝐲 = 𝐯 & (𝐜𝐚𝐩𝐚𝐜𝐢𝐭𝐲 − 𝟏)`

Note that this is not a logical and operator, but a bitwise and! This solution allows us to do the modulo operation quickly and efficiently.

• Dealing with collisions
As mentioned, from the way it is defined, the hashing function is not a hash function, so sometimes we come across two values ​​y, x that are different from each other that maintain (y(h) = x(h) – that is, they will be mapped to the same cell in the hash map. There are two common methods for resolving conflicts:

1. Open hashing (Hashing Open) - a method that allows more than one value to be saved in each cell in the hash map. The cells, also called "baskets" (buckets) (will be built from an additional data structure (for example a dynamic array or a linked list). Thus, in the event of a collision, the conflicting member is added to the the linked list or array representing the cell to which it is mapped.

2. Closed hashing (Hashing Closed -) a method according to which each cell can contain only one member. In these cases, we have to find another way to deal with collisions and value maps.

_The conflicts in this library where resolved with open hashing_
