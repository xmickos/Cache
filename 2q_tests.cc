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
    // std::unordered_map<int, std::list<int>::iterator> &htable = test_fifo.htable_;

    //basic funcs assertions
    ASSERT_EQ(test_fifo.capacity_, test_capacity);
    ASSERT_FALSE(test_fifo.is_full());
    ASSERT_TRUE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.find_elem(0));


    int *inputs = new int[test_size];

    for(int i = 0; i < test_size; ++i){
        inputs[i] = rand() % test_size;
    }

    std::cout << "//////Input:" << std::endl;

    for(int i = 0; i < test_size; i++){
        if(i % test_capacity == 0) std::cout << std::endl;
        std::cout << inputs[i] << " ";
    }

    std::cout << std::endl;

    test_fifo.insert_elem(inputs[0]);       // added 1 / 25 elem
    ASSERT_FALSE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.is_full());
    ASSERT_EQ(test_fifo.htable_.size(), 1);
    ASSERT_EQ(lst.size(), 1);
    ASSERT_EQ(*((test_fifo.htable_.at(inputs[0]))->begin()), lst.begin());
    ASSERT_EQ(*(lst.begin()), inputs[0]);

    for(int i = 1; i < 9; i++){
        test_fifo.insert_elem(inputs[i]);  // added 9 / 25 elems
        ASSERT_EQ(lst.size(), i + 1);
    }

    for(int i = 1; i < 9; i++){
        ASSERT_TRUE(test_fifo.find_elem(inputs[i]));
        ASSERT_EQ(*(*(test_fifo.htable_.at(inputs[i])->begin())), inputs[i]);
    }

    ASSERT_FALSE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.is_full());

    std::cout << "//////Sizes: \n" << test_fifo.htable_.size() << " " << lst.size() << std::endl << "//////Contents: htable vs lst\n";



    for(auto it_htable = test_fifo.htable_.begin(); it_htable != test_fifo.htable_.end(); it_htable++){
        auto it_lst = it_htable->second;
        for(auto it_lst_lst = it_lst->begin(); it_lst_lst != it_lst->end(); ++it_lst_lst){
            std::cout << it_htable->first << " " << **it_lst_lst << std::endl;
            ASSERT_EQ(it_htable->first, **it_lst_lst);
        }
    }

    std::cout << std::endl;

    ASSERT_LT(test_fifo.htable_.size(), lst.size());
    ASSERT_EQ(lst.size(), 9);


    for(int i = 9; i < test_size; i++){
        test_fifo.insert_elem(inputs[i]);  // added 25 / 25 elems
    }

    for(int i = 0; i < test_size; ++i){
        std::cout << "For " << inputs[i] << " -> " << test_fifo.find_elem(inputs[i]) << std::endl;
    }

    std::cout << "lst size = " << lst.size() << ", htable size = " << test_fifo.htable_.size() << std::endl <<
    "Now htable contains:" << std::endl;

    for(auto it_htable = test_fifo.htable_.begin(); it_htable != test_fifo.htable_.end(); it_htable++){
        auto it_lst = it_htable->second;
        for(auto it_lst_lst = it_lst->begin(); it_lst_lst != it_lst->end(); ++it_lst_lst){
            std::cout << it_htable->first << " " << **it_lst_lst << " in the amout of " << it_lst->size() <<std::endl;
            ASSERT_EQ(it_htable->first, **it_lst_lst);
        }
    }


    ASSERT_TRUE(test_fifo.is_full());
//
//     test_fifo.erase_elem(inputs[0]);
//     test_fifo.erase_elem(inputs[1]);
//     test_fifo.erase_elem(inputs[2]);
//     test_fifo.erase_elem(inputs[3]);
//     test_fifo.erase_elem(inputs[4]);
//     test_fifo.erase_elem(inputs[5]);
//     test_fifo.erase_elem(inputs[6]);
//     test_fifo.erase_elem(inputs[7]);
//     test_fifo.erase_elem(inputs[8]);
//     test_fifo.erase_elem(inputs[9]);
//     test_fifo.erase_elem(inputs[10]);
//     test_fifo.erase_elem(inputs[11]);
//     test_fifo.erase_elem(inputs[12]);
//     test_fifo.erase_elem(inputs[13]);
//     test_fifo.erase_elem(inputs[14]);
//     test_fifo.erase_elem(inputs[15]);
//     test_fifo.erase_elem(inputs[16]);
//     test_fifo.erase_elem(inputs[17]);
//     test_fifo.erase_elem(inputs[18]);
//     test_fifo.erase_elem(inputs[19]);
//     test_fifo.erase_elem(inputs[20]);
//     test_fifo.erase_elem(inputs[21]);
//     test_fifo.erase_elem(inputs[22]);
//     test_fifo.erase_elem(inputs[23]);
//     test_fifo.erase_elem(inputs[24]);
//     test_fifo.erase_elem(inputs[25]);

    for(int i = 0; i < test_size; i++){
        std::cout << "Erasing elem " << inputs[i] << ", before we had: " << test_fifo.find_elem(inputs[i]);
        test_fifo.erase_elem(inputs[i]);
        std::cout << ", now he got: " << test_fifo.find_elem(inputs[i]) << std::endl;
    }


    std::cout << "lst size = " << lst.size() << ", htable size = " << test_fifo.htable_.size() << std::endl;
    for(auto it = lst.begin(); it != lst.end(); ++it){
        std::cout << *it << " ";
    }

    std::cout << std::endl;

    for(int i = 0; i < test_size; i++){
        ASSERT_FALSE(test_fifo.find_elem(inputs[i]));
    }


//     for(auto it = lst.begin(), et = lst.end(); it != et; it++){
//         std::cout << *it << " ";
//     }
//     std::cout << std::endl;

//     ASSERT_TRUE(test_fifo.is_empty());
//
//
//
//     delete [] inputs;

//     caches::fifo<int> test_fifa(25);
//
//     for(int i = 0; i < 8; i++){
//         test_fifa.insert_elem(1);
//     }
//
//     std::cout << "lst size = " << test_fifa.lst_.size() << ", htable size = " << test_fifa.htable_.size() << std::endl;
}
