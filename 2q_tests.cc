#include "cache_me_if_you_can.hpp"
#include <gtest/gtest.h>

#define TEST_SIZE 100

TEST(Two_queues, BasicAssertions){

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
    ASSERT_EQ(*(lst.begin()), inputs[0]);
    ASSERT_EQ(*(test_fifo.htable_.find(inputs[0])->second), inputs[0]);

    for(int i = 1; i < 9; i++){
        test_fifo.insert_elem(inputs[i]);  // added 9 / 25 elems
        ASSERT_EQ(lst.size(), i + 1);
    }

    for(int i = 1; i < 9; i++){
        ASSERT_TRUE(test_fifo.find_elem(inputs[i]));
        ASSERT_EQ(*(test_fifo.htable_.equal_range(inputs[i]).first->second), inputs[i]);
    }

    ASSERT_FALSE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.is_full());

    std::cout << "//////Sizes: \n" << test_fifo.htable_.size() << " " << lst.size() << std::endl;
    std::cout << std::endl;

    ASSERT_LE(test_fifo.htable_.size(), lst.size());
    ASSERT_EQ(lst.size(), 9);


    for(int i = 9; i < test_size; i++){
        test_fifo.insert_elem(inputs[i]);  // added 25 / 25 elems
    }

    std::cout << "//////Sizes: \n" << test_fifo.htable_.size() << " " << lst.size() << std::endl;


    for(int i = 0; i < test_size; ++i){
        std::cout << "For " << inputs[i] << " -> " << test_fifo.find_elem(inputs[i]);
        if(test_fifo.find_elem(inputs[i])){
            std::cout << " -> " <<  *(test_fifo.htable_.find(inputs[i])->second) << std::endl;
        }else{
            std::cout << std::endl;
        }
    }

    std::cout << "lst size = " << lst.size() << ", htable size = " << test_fifo.htable_.size() << std::endl <<
    "Now htable contains:" << std::endl;

    ASSERT_TRUE(test_fifo.is_full());



    for(int i = 0; i < test_size; i++){
        std::cout << "Erasing elem " << inputs[i] << ", before we had: " << test_fifo.find_elem(inputs[i]) << " ";
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


    for(auto it = lst.begin(), et = lst.end(); it != et; it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    ASSERT_TRUE(test_fifo.is_empty());



    delete [] inputs;
}

TEST(Two_queues, DISABLED_unordered_multimap){

    size_t test_size = 75, test_capacity = 25;

    // caches::fifo<int> test_fifo(test_capacity);
    // std::list<int> &lst = test_fifo.lst_;
    std::unordered_multimap<int, char> htable_;


    htable_.insert({1, 'a'});
    htable_.insert({1, 'b'});
    htable_.insert({1, 'c'});
    htable_.insert({1, 'd'});
    htable_.insert({2, 'e'});
    htable_.insert({2, 'f'});
    htable_.insert({2, 'g'});
    htable_.insert({2, 'h'});


    std::cout << "////////////////////\n\n";


    // std::cout << std::endl;
    // for(auto it = htable_.cbegin(); it != htable_.cend(); it++){
    //     std::cout << it->first << " " << it->second << std::endl;
    // }

    auto rng = htable_.equal_range(1);
    std::cout << "it = " << rng.first->second << ", size = " << htable_.size() << std::endl;

    htable_.erase(std::next(rng.first, htable_.bucket_size(1) - 1));

    std::cout << "Size = " << htable_.size() << std::endl;

    std::cout << std::endl;
    for(auto it = htable_.cbegin(); it != htable_.cend(); it++){
        std::cout << it->first << " " << it->second << std::endl;
    }

    std::cout << "////////////////////\n\n";
}

TEST(Two_queue, DISABLED_multimap){
    std::unordered_multimap<int,char> map = {{1, 'a'},{1, 'b'},{1, 'd'},{2, 'b'}};

    auto range = map.equal_range(1);

    for (auto it = range.first; it != range.second; ++it)
        std::cout << it->first << ' ' << it->second << '\n';
}
