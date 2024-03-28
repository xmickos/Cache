#undef DEBUG_
#include "cache_me_if_you_can.hpp"

int main(int argc, char **argv){

    size_t cache_sz, elems_count;

    std::cin >> cache_sz;
    std::cin >> elems_count;

    int elem;
    size_t hits = 0;

    caches::two_queues<int> two_q(cache_sz, cache_sz / 3);

    if(IS_PERFECT_){
        std::vector<int> input;
        input.reserve(elems_count);

        for(int i = 0; i < elems_count; ++i){
            std::cin >> elem;
            input.push_back(elem);
        }

        for(int i = 0; i < elems_count; ++i){
            hits += two_q.perfect_cache_update(std::next(input.begin(), i), input.end());
        }

    }else{
        for(size_t i = 0; i < elems_count; ++i){
            std::cin >> elem;
            hits += two_q.cache_update(elem);
        }
    }

    std::cout << hits << std::endl;

    return 0;
}
