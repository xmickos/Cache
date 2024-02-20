#include "cache_me_if_you_can.hpp"
#include <gtest/gtest.h>

#define TEST_SIZE 100

TEST(Two_queues, DISABLED_BasicAssertions){

    caches::two_queues<int> my_2q(75, 25);

    int *input = new int[TEST_SIZE];
    bool *results = new bool[TEST_SIZE];

    for(int i = 0; i < TEST_SIZE; i++){
        input[i] = rand() % TEST_SIZE;
    }

    int summ = 0;

    for(int i = 0; i < TEST_SIZE; i++){
        results[i] = my_2q.cache_update(input[i]);
        // std::cout << "i = " << i;
        // if(results[i] == 1) std::cout << " + (" << summ << ")";
        // std::cout << std::endl;
        summ += results[i];
    }

    std::cout << "Summ is " << summ << std::endl;

    delete [] input;
    delete [] results;

    EXPECT_EQ(42, 42);

}


TEST(Two_queues, Fifo_isolated){

    size_t test_size = 75, test_capacity = 25;

    caches::fifo<int> test_fifo(test_capacity);
    std::list<int> &lst = test_fifo.lst_;
    std::unordered_map<int, std::list<int>::iterator> &htable = test_fifo.htable_;

    //basic funcs assertions
    ASSERT_EQ(test_fifo.capacity_, test_capacity);
    ASSERT_FALSE(test_fifo.is_full());
    ASSERT_TRUE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.find_elem(0));


    int *inputs = new int[test_size];

    for(int i = 0; i < test_size; ++i){
        inputs[i] = rand() % test_size;
    }

    test_fifo.insert_elem(inputs[0]);       // added 1 / 25 elem
    ASSERT_FALSE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.is_full());
    ASSERT_EQ(htable.size(), 1);
    ASSERT_EQ(lst.size(), 1);
    ASSERT_EQ(htable.at(inputs[0]), lst.begin());
    ASSERT_EQ(*(lst.begin()), inputs[0]);

    for(int i = 1; i < 9; i++){
        test_fifo.insert_elem(inputs[i]);  // added 9 / 25 elems
    }

    for(int i = 1; i < 9; i++){
        ASSERT_TRUE(test_fifo.find_elem(inputs[i]));
        ASSERT_EQ(*(htable.at(inputs[i])), inputs[i]);
    }

    ASSERT_FALSE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.is_full());

    std::cout << "//////Sizes: " << htable.size() << " " << lst.size() << std::endl;

    ASSERT_EQ(htable.size(), 9);        // <-- fails
    ASSERT_EQ(lst.size(), 9);


    for(int i = 9; i < test_size; i++){
        test_fifo.insert_elem(inputs[i]);  // added 25 / 25 elems
    }

    ASSERT_TRUE(test_fifo.is_full());

    for(int i = 0; i < test_size; i++){
        test_fifo.erase_elem(inputs[i]);
    }

    for(int i = 0; i < test_size; i++){
        ASSERT_FALSE(test_fifo.find_elem(inputs[i]));
    }

    ASSERT_TRUE(test_fifo.is_empty());


    delete [] inputs;
}
