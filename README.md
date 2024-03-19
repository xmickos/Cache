# Cache


Implemented 2Q caching algorithm using std::multimap & std::list with GoogleTest. First full-C++ project.


### Testing

To run tests, use ctest in 2Q_cache/ and enter test name from ./test_inputs without .txt postfix. To run "zipf_2e5_limited.txt" the input should look like this:
$ ctest
$ zipf_2e5_limited


### 2Q_interactive

This interactive mode was created to make it easier running small handmade tests for certain parts of the code. For automatic tests, there is a 2q_tests binary file and the ctest command.


### 2Q_perfect

The perfect caching algorithm is implemented as follows: For any call to the two_queues.perfect_coaching() method, the algorithm searches in the nearest cache_sz elements for someone who is equal to the current last element of LRU - in order to save it. If there is a suitable one, try to find a place that he can take to stay in RU until his turn comes.
