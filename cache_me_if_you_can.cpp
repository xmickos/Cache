#include "cache_me_if_you_can.hpp"

int main(int argc, char **argv){

    size_t cache_sz, elems_count;

    if(argc != 2){
        printf("Usage: %s <cache-size> <input elements count>\n", argv[0]);
        abort();
    }

    cache_sz = strtol(argv[1], NULL, 10);
    elems_count = strtol(argv[2], NULL, 10);


    printf("Please, enter elements to be cached:\n
    cache_sz = %zu, elems_count = %ze");

    return 0;
}



// g++ -std=c++17 cache_me_if_you_can.cpp -O2 -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero -o 2Q.o
