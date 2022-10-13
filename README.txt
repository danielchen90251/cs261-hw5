Name: Chiu-Chun, Chen
Email: chenchiu@oregonstate.edu

This program implements a HASH table, which can create, search, insert, and delete keys based on a linked list in O(1) complexity.
It also realizes the dynamic expansion based on the increase of the number of table buckets. 
The main interfaces provided are:
(1) ht_create: to create a HASH table
(2) ht_free: to free all memory of the HASH table
(3) ht_isempty: to check whether the HASH table is released as empty
(4) ht_size: to get the number of elements in the HASH table
(5) ht_insert: to insert elements in the HASH table using KEY
(6) ht_lookup: to look up elements in the HASH table using KEY
(7) ht_remove: to remove elemtents in the HASH table using KEY
The "int convert_int(void* key)" function was also provided, which allows users to implement this function and interpret the value of the key pointer.

This program can be compiled by typing and executing "make" followed by "./test_ht" command.
A few examples of usage are given below:

    int convert_int(void* key){
        int *k = key;
        return *k;
    }

    struct ht *ht;
    ht = ht_create();
    if (ht_isempty(ht) == 1)
    {
	    //OK
    }

    int *e = malloc(4);
    int *v = malloc(4);
    *e = 100;
    *v = 200;

    ht_insert(ht, (void*)e, (void*)v, convert_int);
    void elem_val = ht_lookup(ht, (void*)e, convert_int);
    ht_remove(ht, (void*)e, convert_int);

Note that the KEYs and VALUEs stored in the HASH table are pointers, 
so the users must ensure the validity of the pointer values ​​of the KEYs and VALUEs.

This program may have a slight limitation, that is, if the rand() function used in the test program 
generates the same number, there will be some problems when calling the ht_size() function. 
Further improvements and modifications need to be done.
