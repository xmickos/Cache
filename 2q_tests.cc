#include "cache_me_if_you_can.hpp"
#include <gtest/gtest.h>
#include <random>
#include <ctime>
#include <fstream>

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

    // int *bdistr_input = new int[TEST_SIZE];
    std::vector<int> bdistr_input;
    bdistr_input.reserve(TEST_SIZE);

    for(int i = 0; i < TEST_SIZE; ++i){
        bdistr_input.push_back(distribution(generator));
    }

    std::cout << "/////////////////// Input ///////////////////" << std::endl;
    auto it = bdistr_input.begin();
    for(int i = 0; i < TEST_SIZE && it != bdistr_input.end(); ++i, ++it){
        // std::cout << bdistr_input[i] << " ";
        std::cout << *it << " ";
        if(i % 25 == 0) { std::cout << std::endl; }
    }
    std::cout << std::endl;

    int b_hits = 0;

    int input1[13] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

    it = bdistr_input.begin();
    for(int i = 0; i < TEST_SIZE && it != bdistr_input.end(); ++i, ++it){
        b_hits += test_qs.cache_update(*it);
        // b_hits += test_qs.cache_update(bdistr_input[i]);
#if 0
        // std::cout << "\nAfter updating with " << bdistr_input[i] << " we have: " << std::endl <<
        std::cout << "\nAfter updating with " << *it << " we have: " << std::endl <<
        "Lru:" << std::endl;
        for(auto it = test_qs.Am.lst_.begin(); it != test_qs.Am.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << std::endl << "Ain1:" << std::endl;
        for(auto it = test_qs.Ain1.lst_.begin(); it != test_qs.Ain1.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << std::endl << "Ain2:" << std::endl;
        for(auto it = test_qs.Ain2.lst_.begin(); it != test_qs.Ain2.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << "\n\n\n";
#endif
    }

    std::cout << "Binominal distribution: " << b_hits <<  " HITS." << std::endl;
    std::cout << "rand() distribution: " << rnd_hits << " HITS." << std::endl;

    //Perfect strategy:


    b_hits = 0;
    caches::cache_szs_ caps = test_qs.capacities();
    it = bdistr_input.begin();
    for(int i = 0; i < caps.sz1 && it != bdistr_input.end(); ++i, ++it){
        // b_hits += test_qs.cache_update(bdistr_input[i]);
        b_hits += test_qs.cache_update(*it);
    }
    it = bdistr_input.begin();
    for(size_t i = caps.sz1; i < TEST_SIZE && it != bdistr_input.end(); ++i, ++it){
        // b_hits += test_qs.perfect_cache_update(bdistr_input[i], bdistr_input + i, i < TEST_SIZE - caps.sz1 ? caps.sz1 : TEST_SIZE - i);
        b_hits += test_qs.perfect_cache_update(std::next(it, i - caps.sz1), bdistr_input.end());
    }
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
#if 0
        std::cout << "\nAfter updating with " << ndistr_input[i] << " we have: " << std::endl <<
        "Lru:" << std::endl;
        for(auto it = test_qs.Am.lst_.begin(); it != test_qs.Am.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << std::endl << "Ain1:" << std::endl;
        for(auto it = test_qs.Ain1.lst_.begin(); it != test_qs.Ain1.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << std::endl << "Ain2:" << std::endl;
        for(auto it = test_qs.Ain2.lst_.begin(); it != test_qs.Ain2.lst_.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << "\n\n\n";
#endif
    }

    std::cout << "Normal distribution: " << b_hits <<  " HITS." << std::endl;

    delete [] ndistr_input;

}

TEST(Two_queues, DISABLED_Fifo_isolated){

    size_t test_size = 75, test_capacity = 25;

    caches::fifo<int> test_fifo(test_capacity);

    //basic funcs assertions
    ASSERT_EQ(test_fifo.capacity(), test_capacity);
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
    caches::cache_list_iters iters = test_fifo.get_list_iter();

    test_fifo.insert_elem(inputs[0]);       // added 1 / 25 elem
    caches::cache_szs_ sizes = test_fifo.size();
    ASSERT_FALSE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.is_full());
    ASSERT_EQ(sizes.sz2, 1);
    ASSERT_EQ(sizes.sz1, 1);
    // ASSERT_EQ(*(iters.cbegin), inputs[0]);

    for(int i = 1; i < 9; i++){
        test_fifo.insert_elem(inputs[i]);  // added 9 / 25 elems
    }

    for(int i = 1; i < 9; i++){
        ASSERT_TRUE(test_fifo.find_elem(inputs[i]));
    }

    ASSERT_FALSE(test_fifo.is_empty());
    ASSERT_FALSE(test_fifo.is_full());

    std::cout << "//////Sizes: \n" << sizes.sz2 << " " << sizes.sz1 << std::endl;
    std::cout << std::endl;

    sizes = test_fifo.size();
    ASSERT_EQ(sizes.sz2, sizes.sz1);
    ASSERT_EQ(sizes.sz1, 9);


    for(int i = 9; i < test_size; i++){
        test_fifo.insert_elem(inputs[i]);  // added 25 / 25 elems
    }

    std::cout << "//////Sizes: \n" << sizes.sz2 << " " << sizes.sz1 << std::endl;


    for(int i = 0; i < test_size; ++i){
        std::cout << "For " << inputs[i] << " -> " << test_fifo.find_elem(inputs[i]);
        #if 0
        if(test_fifo.find_elem(inputs[i])){
            std::cout << " -> " <<  *(test_fifo.htable_.find(inputs[i])->second) << std::endl;
        }else{
            std::cout << std::endl;
        }
        #endif
    }

    std::cout << "lst size = " << sizes.sz1 << ", htable size = " << sizes.sz2 << std::endl <<
    "Now htable contains:" << std::endl;

    ASSERT_TRUE(test_fifo.is_full());



    for(int i = 0; i < test_size; i++){
        std::cout << "Erasing elem " << inputs[i] << ", before we had: " << test_fifo.find_elem(inputs[i]) << " ";
        test_fifo.erase_last(inputs[i]);
        std::cout << ", now he got: " << test_fifo.find_elem(inputs[i]) << std::endl;
    }


    std::cout << "lst size = " << sizes.sz1 << ", htable size = " << sizes.sz2 << std::endl;
    iters = test_fifo.get_list_iter();
    for(auto it = iters.cbegin; it != iters.cend; ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for(int i = 0; i < test_size; i++){
        ASSERT_FALSE(test_fifo.find_elem(inputs[i]));
    }

    iters = test_fifo.get_list_iter();
    for(auto it = iters.cbegin; it != iters.cend; ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    ASSERT_TRUE(test_fifo.is_empty());



    delete [] inputs;
}



TEST(Two_queues, DISABLED_LRU_isolated){

    int test_size = 75, test_capacity = 25;
    caches::lru_cache<int> test_lru{test_capacity};
    caches::cache_szs_ sizes;
    caches::cache_list_iters<int> iters;

    EXPECT_EQ(test_lru.capacity(), test_capacity);
    EXPECT_EQ(sizes.sz1, 0);
    EXPECT_EQ(sizes.sz2, 0);

    for(int i = 0; i < 9; ++i){
        ASSERT_FALSE(test_lru.lru_update(i));
    }

    for(int i = 0; i < 9; ++i){
        ASSERT_TRUE(test_lru.lru_update(i));
    }

    iters = test_lru.get_list_iter();
    for(auto it = iters.cbegin; it != iters.cend; ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for(int i = 9; i < test_capacity; ++i){
        test_lru.lru_update(i);
    }

    for(int i = test_capacity; i < test_capacity + 10; ++i){
        test_lru.lru_update(i);
    }


    iters = test_lru.get_list_iter();
    std::cout << "Current cache state: " << std::endl;
    for(auto it = iters.cbegin; it != iters.cend; ++it){
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


TEST(Two_queues, DISABLED_Perfect_caching){

    size_t cache_sz = 40, elems_count = 2*1e5;
    std::vector<int> input;
    input.reserve(elems_count);
    clock_t start, end;

    for(int i = 0; i < elems_count; ++i){
        input.push_back(rand() % 1000);
    }

    caches::two_queues<int> two_q(cache_sz, cache_sz / 3);

    std::cout << "\t\t\t///////\\\\\\\\" << std::endl << "CACHE_SIZE: " << cache_sz << " & " << cache_sz / 3
    << std::endl << "INPUT:";

    std::cout << std::endl;

    int elem;
    size_t hits = 0;
    int percentage = 0;

    start = clock();

    auto it = input.begin();
    for(size_t i = 0; i < elems_count && it != input.end(); ++i, ++it){
        elem = *it;
        std::cout << "i = " << i << " vs " << elems_count << " elems_count " << *it << " " << (it != input.end()) << std::endl;
        hits += two_q.perfect_cache_update(it, input.end());

        if(elems_count * (percentage + 1) > i * 100 && elems_count * percentage <= i * 100){
            std::cout << "Done: " << percentage << "%" << std::endl;
        }else{
            percentage++;
            std::cout << "Done: " << percentage << "%" << std::endl;
        }
    }

    end = clock();

    std::cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;


    std::cout << hits << " hits." << std::endl;


    std::cout << "Perfect caching:" << std::endl;
    hits = 0;
#if 0 //not updated yet
    for(int i = 0; i < elems_count; ++i){
        two_q.Am.perfect_caching(input + i, i < elems_count - two_q.Am.lst_.size() ? two_q.Am.lst_.size() : elems_count - i);
        hits += two_q.cache_update(input[i]);

        if(elems_count * (percentage + 1) > i * 100 && elems_count * percentage <= i * 100){
            std::cout << "Done: " << percentage << "%" << std::endl;
        }else{
            percentage++;
        }

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
#endif


}

TEST(Two_queues, e2e){
    std::string str, str_ = "./test_inputs/", str__ = ".txt";

    std::cin >> str;
    str_.insert(str_.end(), str.begin(), str.end());
    str_.insert(str_.end(), str__.begin(), str__.end());
    std::cout << "Opening " << str_ << std::endl;

    std::ifstream in(str_);
    std::streambuf *cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(in.rdbuf());

    if(!in.is_open()){
        std::cout << "Error while opening the file." << std::endl;
        abort();
    }

    // main() func copy-paste: ...
    size_t cache_sz, elems_count;

    std::cin >> cache_sz;
    std::cin >> elems_count;

    std::cout << "cache_sz = " << cache_sz << ", elems_count = " << elems_count << std::endl;

    int elem = 0;
    size_t hits = 0;
    bool is_perfect = true;
    int hit_ = 0;

    caches::two_queues<int> two_q(cache_sz, 3);

    if(is_perfect){
        std::vector<int> input;
        input.reserve(elems_count);

        #ifdef DEBUG_
        std::cout << "Perfect caching:" << std::endl;
        #endif

        hits = 0;

        for(size_t i = 0; i < elems_count; ++i){
            std::cin >> elem;
            input.push_back(elem);
        }

        for(size_t i = 0; i < elems_count; ++i){

            #ifdef DEBUG_
            std::cout << std::endl << "Lru before perfect cache:" << std::endl;
            for(auto it_ =  two_q.Am.lst_.begin(); it_ != two_q.Am.lst_.end(); ++it_){
                std::cout << *it_ << " ";
            }
            std::cout << std::endl << "input: " << std::endl;

            for(auto it = std::next(input.begin(), i); it < std::next(input.begin(), i + cache_sz) && it != input.end(); ++it){
                std::cout << *it << " ";
            }

            std::cout << std::endl;
            #endif

            hit_ += two_q.perfect_cache_update(std::next(input.begin(), i), input.end());
            hits += hit_;

            #ifdef DEBUG_
            std::cout << "\nAfter updating with " << *std::next(input.begin(), i) << " we have: ";
            hit_ > 0 ? std::cout << "hit" : std::cout << "miss";

            std::cout << std::endl << "Lru:" << std::endl;
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
            #endif

            hit_ = 0;
        }
    }else{
        for(size_t i = 0; i < elems_count; ++i){
            std::cin >> elem;

            #ifdef DEBUG_
            std::cout << "readed " << elem << std::endl;
            #endif

            hit_ += two_q.cache_update(elem);
            hits += hit_;

            #ifdef DEBUG_
            std::cout << "\nAfter updating with " << elem << " we have: ";
            hit_ > 0 ? std::cout << "hit" : std::cout << "miss";
            std::cout << std::endl << std::endl << "Lru:" << std::endl;
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
            #endif

            hit_ = 0;
        }

    }

    std::cout << hits << " hits."  << std::endl;
    std::cin.rdbuf(cinbuf);
    in.close();
}
