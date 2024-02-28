#include "cache_me_if_you_can.hpp"
#include <gtest/gtest.h>
#include <random>

#define TEST_SIZE 10000

TEST(Two_queues, DISABLED_Main_test_ints){

    caches::two_queues<int> test_qs(75, 25);

    int *input = new int[TEST_SIZE];

    for(int i = 0; i < TEST_SIZE; i++){
        input[i] = rand() % TEST_SIZE;
    }

    int rnd_hits = 0;

    for(int i = 0; i < TEST_SIZE; i++){
        rnd_hits += test_qs.cache_update(input[i]);
    }

    std::cout << "////////////////////\n\n";

    std::cout << "Summ is " << rnd_hits << std::endl;

    delete [] input;

    std::default_random_engine generator;
    std::binomial_distribution<int> distribution(1000,0.5);

    int *bdistr_input = new int[TEST_SIZE];

    for(int i = 0; i < TEST_SIZE; ++i){
        bdistr_input[i] = distribution(generator);
    }

    std::cout << "/////////////////// Input ///////////////////" << std::endl;
    for(int i = 0; i < TEST_SIZE; ++i){
        std::cout << bdistr_input[i] << " ";
        if(i % 25 == 0) { std::cout << std::endl; }
    }
    std::cout << std::endl;

    int b_hits = 0;

    int input1[13] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

    for(int i = 0; i < TEST_SIZE; ++i){
        b_hits += test_qs.cache_update(bdistr_input[i]);
        // std::cout << "\nAfter updating with " << bdistr_input[i] << " we have: " << std::endl <<
        // "Lru:" << std::endl;
        // for(auto it = test_qs.Am.lst_.begin(); it != test_qs.Am.lst_.end(); ++it){
        //     std::cout << *it << " ";
        // }
        // std::cout << std::endl << "Ain1:" << std::endl;
        // for(auto it = test_qs.Ain1.lst_.begin(); it != test_qs.Ain1.lst_.end(); ++it){
        //     std::cout << *it << " ";
        // }
        // std::cout << std::endl << "Ain2:" << std::endl;
        // for(auto it = test_qs.Ain2.lst_.begin(); it != test_qs.Ain2.lst_.end(); ++it){
        //     std::cout << *it << " ";
        // }
        // std::cout << "\n\n\n";
    }

    std::cout << "Binominal distribution: " << b_hits <<  " HITS." << std::endl;
    std::cout << "rand() distribution: " << rnd_hits << " HITS." << std::endl;

    //Perfect strategy:


    b_hits = 0;
    for(int i = 0; i < test_qs.Am.capacity_; ++i){
        b_hits += test_qs.cache_update(bdistr_input[i]);
    }

    for(size_t i = test_qs.Am.capacity_; i < TEST_SIZE; ++i){

        b_hits += test_qs.cache_update(bdistr_input[i]);
    }


    delete [] bdistr_input;

}

TEST(Two_queues, DISABLED_Main_test_doubles){

    caches::two_queues<double> test_qs(75, 25);

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(100, 50);

    double *ndistr_input = new double[TEST_SIZE];

    for(int i = 0; i < TEST_SIZE; ++i){
        ndistr_input[i] = distribution(generator);
    }

    std::cout << "/////////////////// Input ///////////////////" << std::endl;
    for(int i = 0; i < TEST_SIZE; ++i){
        std::cout << ndistr_input[i] << " ";
        if(i % 25 == 0) { std::cout << std::endl; }
    }
    std::cout << std::endl;

    int b_hits = 0;

    for(int i = 0; i < TEST_SIZE; ++i){
        b_hits += test_qs.cache_update(ndistr_input[i]);
        // std::cout << "\nAfter updating with " << ndistr_input[i] << " we have: " << std::endl <<
        // "Lru:" << std::endl;
        // for(auto it = test_qs.Am.lst_.begin(); it != test_qs.Am.lst_.end(); ++it){
        //     std::cout << *it << " ";
        // }
        // std::cout << std::endl << "Ain1:" << std::endl;
        // for(auto it = test_qs.Ain1.lst_.begin(); it != test_qs.Ain1.lst_.end(); ++it){
        //     std::cout << *it << " ";
        // }
        // std::cout << std::endl << "Ain2:" << std::endl;
        // for(auto it = test_qs.Ain2.lst_.begin(); it != test_qs.Ain2.lst_.end(); ++it){
        //     std::cout << *it << " ";
        // }
        // std::cout << "\n\n\n";
    }

    std::cout << "Normal distribution: " << b_hits <<  " HITS." << std::endl;

    delete [] ndistr_input;

}

TEST(Two_queues, DISABLED_Fifo_isolated){

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

    ASSERT_EQ(test_fifo.htable_.size(), lst.size());
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

TEST(Two_queues, DISABLED_LRU_isolated){

    int test_size = 75, test_capacity = 25;
    caches::lru_cache<int> test_lru{test_capacity};

    EXPECT_EQ(test_lru.capacity_, test_capacity);
    EXPECT_EQ(test_lru.htable_.size(), 0);
    EXPECT_EQ(test_lru.lst_.size(), 0);

    for(int i = 0; i < 9; ++i){
        ASSERT_FALSE(test_lru.lru_update(i));
    }

    for(int i = 0; i < 9; ++i){
        ASSERT_TRUE(test_lru.lru_update(i));
    }

    for(auto it = test_lru.lst_.begin(); it != test_lru.lst_.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for(int i = 9; i < test_capacity; ++i){
        test_lru.lru_update(i);
    }

    for(int i = test_capacity; i < test_capacity + 10; ++i){
        test_lru.lru_update(i);
    }

    std::cout << "Current cache state: " << std::endl;
    for(auto it = test_lru.lst_.begin(); it != test_lru.lst_.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for(int i = 0; i < 10; ++i){
        std::cout << "i = " << i << std::endl;
        ASSERT_FALSE(test_lru.lru_update(i));
    }

    std::default_random_engine generator;
    std::binomial_distribution<int> distribution(10000, 0.5);

    int *bdistr_input = new int[TEST_SIZE];

    for(int i = 0; i < TEST_SIZE; ++i){
        bdistr_input[i] = distribution(generator);
    }

    for(int i = 0; i < 250; ++i){
        std::cout << bdistr_input[i] << " ";
        if(i % 35 == 0) { std::cout << std::endl; }
    }
    std::cout << std::endl;

    int b_hits = 0;

    for(int i = 0; i < TEST_SIZE; ++i){
        b_hits += test_lru.lru_update(bdistr_input[i]);
    }

    std::cout << "Binomial distribution: " << b_hits << "." << std::endl;

    delete [] bdistr_input;

}

TEST(Two_queues, DISABLED_next_func){

    std::list<int> lst;

    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.push_back(4);

    // std::swap(std::next(lst.begin(), 1), std::next(lst.begin(), lst.size() - 1));


    for(auto it = lst.begin(); it != lst.end(); ++it){
        std::cout << *it << " ";
    }

    std::cout << std::endl;
}

TEST(Two_queues, Perfect_caching){
    size_t cache_sz = 10, elems_count = 1000;
    int *input = new int[elems_count]{};

    for(int i = 0; i < elems_count; ++i){
        input[i] = rand() % 15;
    }

    caches::two_queues<int> two_q(cache_sz, cache_sz / 3);

    std::cout << "\t\t\t///////\\\\\\\\" << std::endl << "CACHE_SIZE: " << cache_sz << " & " << two_q.Ain1.capacity_
    << std::endl << "INPUT:";

    for(int i = 0; i < elems_count; ++i){
        if(i % 25 == 0) std::cout << std::endl;

        std::cout << input[i] << " ";
    }

    std::cout << std::endl;

    int elem;
    size_t hits = 0;


    for(size_t i = 0; i < elems_count; ++i){
        elem = input[i];
        hits += two_q.cache_update(elem);
    }

    std::cout << two_q.Am.lst_.size() << " - size, " << std::distance(two_q.Am.lst_.begin(), two_q.Am.lst_.end()) << " - distance." << std::endl;


    std::cout << hits << " hits." << std::endl;


    std::cout << "Perfect caching:" << std::endl;
    hits = 0;
    for(int i = 0; i < elems_count; ++i){
        // if(two_q.Am.find_elem(input[i])){ two_q.Am.perfect_caching(input + i); }
        two_q.Am.perfect_caching(input + i, i < elems_count - two_q.Am.lst_.size() ? two_q.Am.lst_.size() : elems_count - i);
        hits += two_q.cache_update(input[i]);

        std::cout << "\nAfter updating with " << input[i] << " we have: " << std::endl <<
        "Lru:" << std::endl;
        for(auto it =  two_q.Am.lst_.begin(); it != two_q.Am.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << std::endl << "Ain1:" << std::endl;
        for(auto it = two_q.Ain1.lst_.begin(); it != two_q.Ain1.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << std::endl << "Ain2:" << std::endl;
        for(auto it = two_q.Ain2.lst_.begin(); it != two_q.Ain2.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << "\n\n\n";
    }
    std::cout << hits << " hits." << std::endl;


    delete [] input;



}
