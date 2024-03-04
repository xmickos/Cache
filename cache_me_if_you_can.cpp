#include "cache_me_if_you_can.hpp"

int main(int argc, char **argv){

    size_t cache_sz, elems_count;

    scanf("%zu", &cache_sz);
    scanf("%zu", &elems_count);

    int elem;
    size_t hits = 0;

    // int *input = new int[elems_count]{};

    caches::two_queues<int> two_q(cache_sz, cache_sz / 3);

    for(size_t i = 0; i < elems_count; ++i){
        scanf("%d", &elem);
        hits += two_q.cache_update(elem);
        // input[i] = elem;
    }


    std::cout << hits /* << " hits." */ << std::endl;

//     caches::two_queues<int> two_qs(cache_sz, cache_sz / 3);
//
//     std::cout << "Perfect caching:" << std::endl;
//     hits = 0;
//     for(int i = 0; i < elems_count; ++i){
//             hits += two_qs.perfect_cache_update(input[i], input + i, i < elems_count - cache_sz ? cache_sz : elems_count - i);
//     }
//     std::cout << hits << " hits." << std::endl;
//
//
//     delete [] input;



    return 0;
}



// g++ -std=c++17 cache_me_if_you_can.cpp -O2 -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero -o 2Q.o
