#include "../include/cache_me_if_you_can.hpp"

int main(int argc, char **argv){
    char choice;

    #if 0
    bool is_perfect;

    std::cout << "Will your cache be perfect?[y/n]" << std::endl;
    std::cin >> choice;

    switch (choice){
        case 'y':
            is_perfect = true;
            break;
        case 'n':
            is_perfect = false;
            break;
        default:
            std::cout << "Wrong answer" << std::endl;
            abort();
    }
    #endif

    size_t cache_sz, elems_count;


    std::cout << "Cache size?" << std::endl;
    std::cin >> cache_sz;
    std::cout << "Elems count?" << std::endl;
    std::cin >> elems_count;

    int elem;
    size_t hits = 0;

    caches::two_queues<int> two_q(cache_sz, 2);

    #if 0
    if(is_perfect){
        std::vector<int> input;
        input.reserve(elems_count);

        for(int i = 0; i < elems_count; ++i){
            std::cin >> elem;
            input.push_back(elem);
            hits += two_q.perfect_cache_update(std::next(input.begin(), i), input.end());

            std::cout << "Cache state: " << std::endl << "|";
            for(int j = 0; j < 2 * two_q.capacities().sz1; ++j){                    // Am
                std::cout << " ";
                if(j % 2 == 0){
                    if(two_q.size().sz1 <= j / 2){
                        std::cout << " ";
                    } else {
                        std::cout << *(std::next(two_q.Am.lst_.cbegin(), j / 2));
                    }
                }
            }
            std::cout << "|";
            for(int j = 0; j < 2 * two_q.capacities().sz2; ++j){                    // Ain1
                std::cout << " ";
                if(j % 2 == 0){
                    if(two_q.size().sz2 <= j / 2){
                        std::cout << " ";
                    } else {
                        std::cout << *(std::next(two_q.Ain1.lst_.cbegin(), j / 2));
                    }
                }
            }
            std::cout << "|";
            for(int j = 0; j < 2 * two_q.capacities().sz1; ++j){                    // Ain2
                std::cout << " ";
                if(j % 2 == 0){
                    if(two_q.size().sz3 <= j / 2){
                        std::cout << " ";
                    } else {
                        std::cout << *(std::next(two_q.Ain2.lst_.cbegin(), j / 2));
                    }
                }
            }
            std::cout << "|" << std::endl << "Future elems: ";
            for(int j = 0; j < (i < elems_count - cache_sz ? cache_sz : elems_count - i); ++j){
                std::cout << input[j] << " ";
            }
            std::cout << std::endl;

        }
    }else{
    #endif
        while(1){
            std::cin >> elem;
            hits += two_q.cache_update(elem);


            std::cout << "Cache state: " << std::endl << "|";
            for(int j = 0; j < 2 * two_q.capacities().sz1; ++j){                    // Am
                std::cout << " ";
                if(j % 2 == 0){
                    if(two_q.size().sz1 <= j / 2){
                        std::cout << " ";
                    } else {
                        std::cout << *(std::next(two_q.Am.lst_.cbegin(), j / 2));
                    }
                }
            }
            std::cout << "|";
            for(int j = 0; j < 2 * two_q.capacities().sz2; ++j){                    // Ain1
                std::cout << " ";
                if(j % 2 == 0){
                    if(two_q.size().sz2 <= j / 2){
                        std::cout << " ";
                    } else {
                        std::cout << *(std::next(two_q.Ain1.lst_.cbegin(), j / 2));
                    }
                }
            }
            std::cout << "|";
            for(int j = 0; j < 2 * two_q.capacities().sz1; ++j){                    // Ain2
                std::cout << " ";
                if(j % 2 == 0){
                    if(two_q.size().sz3 <= j / 2){
                        std::cout << " ";
                    } else {
                        std::cout << *(std::next(two_q.Ain2.lst_.cbegin(), j / 2));
                    }
                }
            }
            std::cout << "|" << std::endl;
        }
    #if 0
    }
    #endif

    std::cout << hits  << std::endl;

    return 0;
}

// g++ -std=c++17 cache_me_if_you_can.cpp -O2 -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero -o 2Q.o
