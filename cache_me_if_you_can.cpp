#include "cache_me_if_you_can.hpp"

#define DUMP     std::cout << "Lists: " << my_2q.Am.lst_.size() << " " <<  my_2q.Ain1.lst_.size() << " " << my_2q.Ain2.lst_.size() << " " << std::endl; \
    std::cout << "Htables: " <<  my_2q.Am.htable_.size() << " " << my_2q.Ain1.htable_.size() << " " << my_2q.Ain2.htable_.size() << " " << std::endl;   \
    std::cout << "\n";                                                                                                                                  \
    for(auto it = my_2q.Am.lst_.begin(); it != my_2q.Am.lst_.end(); it++){                                                                              \
        std::cout << *it << " ";                                                                                                                        \
    }                                                                                                                                                   \
    std::cout << "| ";                                                                                                                                  \
    for(auto it = my_2q.Ain1.lst_.begin(); it != my_2q.Ain1.lst_.end(); it++){                                                                          \
        std::cout << *it << " ";                                                                                                                        \
    }                                                                                                                                                   \
    std::cout << "| ";                                                                                                                                  \
    for(auto it = my_2q.Ain2.lst_.begin(); it != my_2q.Ain2.lst_.end(); it++){                                                                          \
        std::cout << *it << " ";                                                                                                                        \
    }                                                                                                                                                   \
    std::cout << " //\n";


#define TEST_SIZE 65536

int main(int argc, char **argv){

    caches::two_queues<int> my_2q(75, 25);

    int *input = new int[TEST_SIZE];
    bool *results = new bool[TEST_SIZE];


    for(int i = 0; i < TEST_SIZE; i++){
        input[i] = rand() % TEST_SIZE;
    }

    for(int i = 0; i < 15; i++){
        std::cout << input[i] << std::endl;
    }

    size_t summ = 0;

    for(int i = 0; i < TEST_SIZE; i++){
        results[i] = my_2q.cache_update(input[i]);
        std::cout << "i = " << i;
        if(results[i] == 1) std::cout << " + (" << summ << ")";
        std::cout << std::endl;
        summ += results[i];
    }

    std::cout << "Summ is " << summ << std::endl;

    delete [] input;
    delete [] results;



    return 0;
}

//     std::cout << "And now we have: " << my_2q.lrg_sz_ << " " << my_2q.smll_sz_ << std::endl;
//     printf("(%d, %d)\n", my_2q.lrg_sz_, my_2q.smll_sz_);
//     std::cout << "\n";
//
//
//
//     my_2q.cache_update(1);
//     DUMP
//     my_2q.cache_update(2);
//     DUMP
//     my_2q.cache_update(3);
//     DUMP
//     my_2q.cache_update(4);
//     DUMP
//     my_2q.cache_update(1);
//     DUMP
//     my_2q.cache_update(2);
//     DUMP
//     my_2q.cache_update(5);
//     DUMP
//     my_2q.cache_update(1);
//     DUMP
//     my_2q.cache_update(2);
//     DUMP
//     my_2q.cache_update(3);
//     DUMP
//     my_2q.cache_update(4);
//     DUMP
//     my_2q.cache_update(5);
//     DUMP
