#include "cache_me_if_you_can.hpp"

#define TEST_SIZE 250

int main(int argc, char **argv){

    caches::two_queues<int> my_2q(75, 25);

    int *input = new int[TEST_SIZE];
    bool *results = new bool[TEST_SIZE];

    for(int i = 0; i < TEST_SIZE; i++){
        input[i] = rand() % TEST_SIZE;
    }

    for(int i = 0; i < 15; i++){
        // std::cout << input[i] << std::endl;
    }

    size_t summ = 0;

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



    return 0;
}



// g++ -std=c++17 cache_me_if_you_can.cpp -O2 -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero -o 2Q.o
