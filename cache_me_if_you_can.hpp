#include<iostream>
#include<list>
#include<map>
#include<unordered_map>
#include<cstdlib>
#include<gtest/gtest.h>
#include<cstring>
#include<cassert>
#include<algorithm>

namespace caches {

#define DEFAULT_MAIN_CAP 256
#define DEFAULT_A_IN_CAP 64
#define FLOAT_TOLERANCE 1e-5
#define DEFAULT_CACHE_RATIO 3

struct cache_szs_{
    size_t sz1 = 0, sz2 = 0, sz3 = 0;
};

template <typename U>
struct cache_list_iters{
    typename std::list<U>::const_iterator cbegin, cend;
};

template <typename T>
struct lru_cache {

    size_t capacity_;
    std::list<T> lst_;
    std::unordered_map<T, typename std::list<T>::iterator> htable_;

    public:

        lru_cache(int capacity = DEFAULT_MAIN_CAP) : capacity_(capacity > 0 ? capacity : 2), lst_(), htable_() { }

        bool find_elem(const T& value) const {
            return htable_.find(value) != htable_.end();
        }

        bool is_full() const {
            return lst_.size() == capacity_;
        }

        int capacity() const { return capacity_; }

        cache_list_iters<T> get_list_iter() const {
            cache_list_iters<T> iters = { lst_.cbegin(), lst_.cend() };
            return iters;
        }

        cache_szs_ size() const {
            cache_szs_ sizes = { lst_.size(), htable_.size() };
            return sizes;
        }

        void insert_elem(const T& value) {
            if(find_elem(value)){
                #ifdef DEBUG_
                std::cout << "htable - find." << std::endl;
                #endif

                auto it = htable_.at(value);

                lst_.erase(it);
                lst_.push_front(value);
                htable_.erase(value);
                htable_.insert({value, lst_.begin()});
            }else{
                #ifdef DEBUG_
                std::cout << "htable - end." << std::endl;
                #endif

                if(is_full()){
                    auto it = lst_.back();
                    htable_.erase(it);
                    lst_.pop_back();
                }

                lst_.push_front(value);
                htable_.insert({value, lst_.begin()});
            }
        }

        bool lru_update(const T& value) {
            #ifdef DEBUG_
            std::cout << "lru_cache.lru_update()" << std::endl;
            #endif
            bool contained = find_elem(value);
            insert_elem(value);

            return contained;
        }
};


template <typename T>
struct fifo {
    size_t capacity_;
    std::list<T> lst_;
    std::multimap<int, typename std::list<T>::iterator> htable_;

    public:

        fifo(int capacity = DEFAULT_A_IN_CAP) : capacity_(capacity > 0 ? capacity : 1), lst_(), htable_() { }

        bool is_full() const {
            return lst_.size() == capacity_;
        }

        bool is_empty() const {
            return(lst_.empty() && htable_.empty());
        }

        bool find_elem(const T& value) const {
            return htable_.count(value) != 0;
        }

        void insert_elem(const T& value) {
            #ifdef DEBUG_
            std::cout << "fifo.insert_elem()" << std::endl;
            #endif
            if(is_full()){
                auto it_rng = htable_.equal_range(lst_.back());

                htable_.erase(it_rng.first);
                lst_.pop_back();
            }

            lst_.push_front(value);
            htable_.insert({value, lst_.begin()});
        }

        void erase_last(){
            T val = lst_.back();
            #ifdef DEBUG_
            std::cout << "fifo.erase_last()" << std::endl;
            #endif

            auto iter = htable_.equal_range(val);
            auto iter_last = iter.second;

            if(iter.first != iter.second) {
                iter_last = std::next(iter_last, -1);
            }

            htable_.erase(iter_last);
            lst_.pop_back();
        }

        void erase_last(const T& val){
            #ifdef DEBUG_
            std::cout << "fifo.erase_last(" << val << ")" << std::endl;
            #endif

            auto iter = htable_.equal_range(val);
            auto iter_last = iter.second;

            if(iter.first != iter.second) { iter_last = std::next(iter_last, -1); }

            lst_.erase(iter_last->second);
            htable_.erase(iter_last);
        }

        cache_list_iters<T> get_list_iter() const {
            cache_list_iters<T> iters = { lst_.cbegin(), lst_.cend() };
            return iters;
        }

        cache_szs_ size() const {
            cache_szs_ sizes = { lst_.size(), htable_.size() };
            return sizes;
        }

        int capacity() const { return capacity_; }

};


template <typename T>
class two_queues {
    #ifdef DEBUG_
    public:
    #endif
    size_t cache_ratio, lrg_cap_, smll_cap_;
    lru_cache<T> Am;
    fifo<T> Ain1, Ain2;

    public:
        two_queues(int lrg_cap = DEFAULT_MAIN_CAP, int cache_ratio = DEFAULT_CACHE_RATIO) : lrg_cap_(lrg_cap > 0 ? lrg_cap : 2),
        smll_cap_(lrg_cap / cache_ratio > 0 ? lrg_cap / cache_ratio : 1), Am(lrg_cap), Ain1(smll_cap_), Ain2(lrg_cap_) {}

        cache_szs_ size() const {
            cache_szs_ sizes = { Am.lst_.size(), Ain1.lst_.size(), Ain2.lst_.size() };
            return sizes;
        }

        cache_szs_ capacities() const {
            cache_szs_ caps = { lrg_cap_, smll_cap_ };
            return caps;
        }

    #ifdef DEBUG_
    public:
    #else
    private:
    #endif

        bool elem_hit(const T& value) const {
            return Am.find_elem(value) || Ain2.find_elem(value);
        }

        bool find_in_fifos(const T& value) const {
            return Ain1.find_elem(value) || Ain2.find_elem(value);
        }

        void drag_between_fifos() {
            #ifdef DEBUG_
            std::cout << "2Q.drag_between_fifos()" << std::endl;
            #endif
            T val = Ain1.lst_.back();
            Ain2.insert_elem(val);
            Ain1.erase_last();
        }

        void update_fifos(const T& value) {
            #ifdef DEBUG_
            std::cout << "2Q.update_fifos()" << std::endl;
            #endif

            if(Ain1.is_full()){
                drag_between_fifos();
            }

            Ain1.insert_elem(value);
        }

        void drag_into_lru(const T& value) {
            #ifdef DEBUG_
            std::cout << "2Q.drag_into_lru" << std::endl;
            #endif

            Am.insert_elem(value);
            Ain2.erase_last(value);
        }

        bool is_valid(typename std::vector<T>::iterator it, typename std::vector<T>::iterator et) const {

            // Нет смысла кэшировать элемент, пара для которого встретится слишком рано или слишком поздно.
            auto first_last = std::find_if(std::next(it, 1), et, [&it](auto&& elt){ return elt == *it; });
            if(first_last != et){

                #ifdef DEBUG_
                std::cout << "finded : " << *first_last << ", first cond: " << (std::distance(it, first_last) < lrg_cap_ + smll_cap_) <<
                 ", second cond: " << (std::distance(it, first_last) > smll_cap_) << ", dist = " << std::distance(it, first_last) << std::endl;
                #endif

                if(std::any_of(Am.lst_.begin(), Am.lst_.end(), [&first_last](auto&& elt){ return elt == *first_last;})){
                    return true;
                }

                if(std::any_of(Ain2.lst_.begin(), Ain2.lst_.end(), [&first_last](auto&& elt){ return elt == *first_last;})){
                    return true;
                }

                if((std::distance(it, first_last) < 10 * lrg_cap_ + smll_cap_ ) && std::distance(it, first_last) > smll_cap_){
                    return true;
                }

                #ifdef DEBUG_
                std::cout << "Rejected." << std::endl;
                #endif
            }

            return false;
        }

        void perfect_caching(typename std::vector<T>::iterator it, typename std::vector<T>::iterator et){
        /* Алгоритм идеального кэширования: Задача алгоритма - определять, когда элемент, который на следующем шаге должен
        * вылететь из LRU кэша, может в ближайшем будущем быть востребован, и "спасать" его, меняя местами с таким элементом,
        * который в ближайшем будущем востребован не будет. Считается, что элемент скоро будет полезен, если он есть в ближайших
        * lru_size элементах в будущем. Если это выполнено, в LRU ищется элемент, отстоящий достаточно далеко от конца, чтобы обмен
        * имел смысл, и при этом не имеющий возможности дать hit на ближайших cache_sz элементах в будущем. При нахождении подходящего
        * кандидата на обмен, последний элемент меняется местами с найденым, и в будущем даёт hit.
        */
            if(!Am.is_full()){ return; }
            std::cout << "Am.perfect_caching()" << std::endl;

            T curr_last = Am.lst_.back();

            auto iter = std::find_if(it, et, [&curr_last, &it, &et](auto&& elt) {
                                     return (std::abs(elt - curr_last) < FLOAT_TOLERANCE); });

            if(iter != et){
                size_t distance = std::distance(it, iter);

                #if 0
                auto it_ = std::find_if( lst_.begin(), iter,
                                        std::none_of(it, et,
                                                    [](auto&& elt, auto&& alt){ return (std::abs(elt - alt) < FLOAT_TOLERANCE); }));
                #endif

                // Найти элемент, который не встречается в ближайшем input'е и при этом стоит в списке достаточно рано
                auto it_ = Am.lst_.begin(),  et_ = Am.lst_.end();
                for(; std::distance(it_, et_) > distance; ++it_){
                    if(std::none_of(it, et, [&it_](auto&& elt){ return (std::abs(elt - *it_) < FLOAT_TOLERANCE); })){
                        break;
                    }
                }


                if(it_ != et_){
                    auto it_last = std::next(Am.lst_.end(), -1);

                    Am.htable_.erase(*it_);
                    Am.htable_.erase(curr_last);

                    #ifdef DEBUG_
                    std::cout << "Swapped " << *it_last << " and " << *it_ << std::endl;
                    #endif

                    std::iter_swap(it_last, it_);

                    Am.htable_.insert({curr_last, it_});
                    Am.htable_.insert({*it_last, it_last});
                }
            }
        }


    public:

        bool cache_update(const T& value){
            bool result = elem_hit(value);

            if(Am.find_elem(value)){
                #ifdef DEBUG_
                std::cout << "First subtree" << std::endl;
                #endif
                Am.lru_update(value);
            } else if(Ain2.find_elem(value)){
                #ifdef DEBUG_
                std::cout << "Second subtree" << std::endl;
                #endif
                drag_into_lru(value);
            } else {
                #ifdef DEBUG_
                std::cout << "Third subtree" << std::endl;
                #endif
                update_fifos(value);
            }

            return result;
        }

        bool perfect_cache_update(typename std::vector<T>::iterator it, typename std::vector<T>::iterator et){

            // Нет смысла кэшировать уникальный элемент
            if(std::none_of(std::next(it, 1), et, [&it](auto&& elt){ return elt == *it; })){

                #ifdef DEBUG_
                std::cout << "Unique: " << *it << std::endl;
                #endif

                return false;
            }

            if(!is_valid(it, et)){
                return false;
            }

            return cache_update(*it);
        }

};

}
