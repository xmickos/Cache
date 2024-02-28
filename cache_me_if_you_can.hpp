#include<iostream>
#include<list>
#include<unordered_map>
#include<cstdlib>
#include<gtest/gtest.h>
#include<cstring>

namespace caches {

#define DEFAULT_MAIN_SZ 256
#define DEFAULT_A_IN_SZ 64


template <typename T>
struct lru_cache {

    int capacity_;
    std::list<T> lst_;
    std::unordered_map<T, typename std::list<T>::iterator> htable_;

    lru_cache(int capacity = DEFAULT_MAIN_SZ) : capacity_(capacity > 0 ? capacity : 2), lst_(), htable_() { }

    bool find_elem(T value) const {
        return htable_.find(value) != htable_.end();
    }

    bool is_full() const {
        return lst_.size() == capacity_;
    }

    void insert_elem(T value) {
        // если кэш полон, вытеснить последний элемент
        // если в кэше есть указанный элемент, переместить его на первое место

        if(htable_.find(value) == htable_.end()){
            if(is_full()){
                auto it = lst_.back();
                htable_.erase(it);
                lst_.pop_back();
            }
            lst_.push_front(value);
            // htable_.insert({value, lst_.begin()});
            htable_.insert(std::make_pair<T, typename std::list<T>::iterator>(value + 0, lst_.begin()));
        }else{
            auto it = htable_.at(value);
            std::cout << "In LRU finded elem " << value << std::endl << "All htable elems: ";
            for(auto it = htable_.begin(); it != htable_.end(); ++it){
                std::cout << it->first << "(" << *(it->second) << ") ";
            }
            std::cout << std::endl;

            T tmp = *it;
            lst_.erase(it);
            lst_.push_front(tmp);
            htable_.erase(tmp);
            // htable_.insert({tmp, lst_.begin()});
            htable_.insert(std::make_pair<T, typename std::list<T>::iterator>(tmp + 0, lst_.begin()));
        }
    }

    bool lru_update(T value) {
        bool contained = find_elem(value);
        insert_elem(value);

        return contained;
    }

    void perfect_caching(T* input, int wrkbl_sz) {
        if(!is_full()){ return; }

        T curr_last = lst_.back();
        int swap_len_request = -1;
        for(size_t i = 0; i < wrkbl_sz; ++i){
            if(input[i] == curr_last){
                swap_len_request = i;
                break;
            }
        }

        std::cout << "Input: ";
        for(int i = 0; i < wrkbl_sz; ++i){std::cout << input[i] << " "; }
        std::cout << std::endl << "LRU: ";
        for(auto it = lst_.begin(); it != lst_.end(); ++it){ std::cout << *it << " "; }
        std::cout << std::endl;


        if(swap_len_request > 0){ /* если в input в первых cache_sz - 1 элементах есть тот что сейчас закэширован последним - есть запрос на замену*/

            std::cout << "We finded elem " << curr_last << " in input at pos " << swap_len_request << ".\nInput: ";
            for(int i = 0; i < wrkbl_sz; ++i){std::cout << input[i] << " ";}
            std::cout << std::endl;

            int swap_len_reply = 0;
//             /* делаем request на замену на позицию не менее чем len */
//             /* вычисление статуса swap_reply */
//             for(auto it = lst_.begin(); it != lst_.end(); ++it){
//                 for(size_t i = 0; i < std::distance(lst_.end(), it); ++i){
//                     if(*it == input[i]){
//                         if(swap_len_reply < std::distance(it, lst_.end())){ swap_len_reply = std::distance(it, lst_.end()); }
//                         // Таким образом находится максимально близкая к голове кэша позиция, на которую может стать элемент,
//                         // сформировавший запрос. Если эта позиция недостаточно близка для него, запрос отклоняется.
//                     }
//                 }
//             }

            for(auto it = std::next(lst_.begin(), lst_.size() - swap_len_request - 1); it != std::next(lst_.begin(), -1); --it){
                swap_len_reply = -1;
                for(int i = 0; i < wrkbl_sz; ++i){
                    if(input[i] == *it){
                        swap_len_reply = i;
                        break;
                    }
                }
                if(swap_len_reply < 0){
                    // swap_len_reply = std::distance(lst_.begin(), it);

                    std::cout << "Swapping " << curr_last << " and " << *it << std::endl;

                    auto it_last = std::next(lst_.begin(), lst_.size() - 1);

                    htable_.erase(*it);
                    htable_.erase(curr_last);

                    *it_last = *it;
                    *it = curr_last;

                    htable_.insert({curr_last, it});
                    htable_.insert({*it_last, it_last});

                    std::cout << "LRU after swap:" << std::endl;
                    for(auto it = lst_.begin(); it != lst_.end(); ++it){
                        std::cout << *it << " ";
                    }
                    std::cout << "\n\n";

                    break;
                }else{
                    swap_len_reply = capacity_ + 1;
                }
            }

            if(swap_len_reply == capacity_ + 1){
                std::cout << "Elem for swap not finded :(" << std::endl;
            }


//             if(swap_len_reply > swap_len_request && swap_len_reply != lst_.size() - 1){     // Запрос удовлетворён, притом
//                 /* swap с подходящим элементом */                                           // обмен происходит не с самим собой.
//                 std::cout << "Swapped elems " <<  *(std::next(lst_.begin(), swap_len_reply)) << " and " << lst_.back() << " -- ";
//                 for(int i = 0; i < capacity_; ++i){std::cout << input[i] << " ";}
//                 std::cout << "\nBefore swap we had: "; for(auto it = lst_.begin(); it != lst_.end(); ++it) std::cout << *it << " ";
//
//                 std::cout << "\nSWAPPING " << *(std::next(lst_.begin(), swap_len_reply)) << " AND " << *(std::next(lst_.begin(), lst_.size() - 1)) << std::endl;
//                 std::cout << "swap_len_reply = " << swap_len_reply << std::endl;
//
//                 T tmp = *(std::next(lst_.begin(), swap_len_reply));
//                 *(std::next(lst_.begin(), swap_len_reply)) = lst_.back();
//                 *(std::next(lst_.begin(), lst_.size() - 1)) = tmp;
//
//                 std::cout << std::endl << "After swap we have: ";
//                 for(auto it = lst_.begin(); it != lst_.end(); ++it){ std::cout << *it << " "; }
//                 std::cout << std::endl;
//             }
        }else{
            std::cout << "No swap request :(" << std::endl;
        }
    }
};


template <typename T>
struct fifo {
    int capacity_;
    std::list<T> lst_;
    std::unordered_multimap<int, typename std::list<T>::iterator> htable_;

    fifo(int capacity = DEFAULT_A_IN_SZ) : capacity_(capacity > 0 ? capacity : 1), lst_(), htable_() { }

    bool is_full() const {
        return lst_.size() == capacity_;
    }

    bool is_empty() const {
        return(lst_.empty() && htable_.empty());
    }

    bool find_elem(T value) const {
        return htable_.count(value) != 0;
    }

    void insert_elem(T value) {
        if(is_full()){
            auto it_rng = htable_.equal_range(lst_.back());

            htable_.erase(it_rng.first);
            lst_.pop_back();
        }

        lst_.push_front(value);
        htable_.insert({value, lst_.begin()});
    }

    void erase_elem(T value) {
        if(find_elem(value)){
            auto range = htable_.equal_range(value);
            for(auto it = range.first; it != range.second; ++it){
                lst_.erase(it->second);
            }
            htable_.erase(value);
        }
    }
};


template <typename T>
struct two_queues {
    int lrg_sz_, smll_sz_;
    lru_cache<T> Am;
    fifo<T> Ain1, Ain2;

    two_queues(int lrg_sz = DEFAULT_MAIN_SZ, int smll_sz = DEFAULT_A_IN_SZ) : lrg_sz_(lrg_sz > 0 ? lrg_sz : 2),
    smll_sz_(smll_sz > 0 ? smll_sz : 1), Am(lrg_sz), Ain1(smll_sz), Ain2(lrg_sz_) {}

    bool elem_hit(T value) const {
        return Am.find_elem(value) || Ain1.find_elem(value);
    }

    bool find_in_fifos(T value) const {
        return Ain1.find_elem(value) || Ain2.find_elem(value);
    }

    void drag_between_fifos() {
        T val = Ain1.lst_.back();
        Ain2.insert_elem(val);
        Ain1.erase_elem(val);
    }

    void update_fifos(T value) {

        if(Ain1.is_full()){
            drag_between_fifos();
            Ain1.insert_elem(value);
        }else{
            Ain1.insert_elem(value);
        }

    }

    void drag_into_lru(T value) {
        T val = *(Ain2.htable_.equal_range(value).first->second);
        Am.insert_elem(val);
        Ain2.erase_elem(val);
    }

    bool cache_update(T value){
        bool result = elem_hit(value);

        if(Am.find_elem(value)){
            std::cout << "Trying to update LRU with " << value << std::endl;
            Am.lru_update(value);
        }else{
            if(Ain2.find_elem(value)){
                drag_into_lru(value);
            }else{
                update_fifos(value);
            }
        }

        return result;
    }

};

}
