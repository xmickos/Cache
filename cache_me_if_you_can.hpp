#include<iostream>
#include<list>
#include<map>
#include<unordered_map>
#include<cstdlib>
#include<gtest/gtest.h>
#include<cstring>

namespace caches {

#define DEFAULT_MAIN_CAP 256
#define DEFAULT_A_IN_CAP 64
#define FLOAT_TOLERANCE 1e-5

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
            if(htable_.find(value) == htable_.end()){
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
            }else{
                #ifdef DEBUG_
                std::cout << "htable - find." << std::endl;
                #endif
                auto it = htable_.at(value);

                #if 0
                std::cout << "{+++++++++}   Yes, we are here!" << std::endl;
                #endif

                T tmp = *it;
                lst_.erase(it);
                lst_.push_front(tmp);
                htable_.erase(tmp);
                htable_.insert({tmp, lst_.begin()});
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

        void perfect_caching(T* input, int wrkbl_sz) {
        /*  Алгоритм идеального кэширования: Если элемент находится в "будущем" (массиве input) от текущей позиции дальше,
        *   чем на capacity_, он не рассматривается. На каждой итерации обновления кэша рассматривается последний элемент
        *   в кэше. Если в будущих элементах попадается рассматриваемый на расстоянии не более чем в capacity_ от начала,
        *   имеет смысл оставить последний элемент в кэше, но переместив ближе к началу, чтобы к моменту наступления своей
        *   очереди он смог быть найден и дал hit. Чтобы переместить его в начало кэша, нужно обменять его местами с некоторым
        *   другим элементом. Для такого обмена элемент, с которым упомянутый обмен производится, сам не должен стать "бесполезным"
        *   в кэше, если вдруг в ближайших capacity_ ячейках массива input он будет запрошен. Таким образом, нужно найти, какой
        *   элемент будет не жалко потерять и запихнуть в конец очереди.
        */
            if(!is_full()){ return; }

            T curr_last = lst_.back();
            int swap_len_request = -1;

            for(size_t i = 0; i < wrkbl_sz; ++i){
                if(fabs(input[i] - curr_last) < FLOAT_TOLERANCE){
                    swap_len_request = i;
                    break;
                }
            }

            if(swap_len_request > 0){
                int swap_len_reply = 0;

                for(auto it = std::next(lst_.begin(), lst_.size() - swap_len_request - 1); it != std::next(lst_.begin(), -1); --it){
                    swap_len_reply = -1;

                    for(int i = 0; i < wrkbl_sz; ++i){
                        if(fabs(input[i] - *it) < FLOAT_TOLERANCE){
                            swap_len_reply = i;
                            break;
                        }
                    }

                    if(swap_len_reply < 0){
                        auto it_last = std::next(lst_.begin(), lst_.size() - 1);

                        htable_.erase(*it);
                        htable_.erase(curr_last);

                        *it_last = *it;
                        *it = curr_last;

                        htable_.insert({curr_last, it});
                        htable_.insert({*it_last, it_last});

                        break;
                    }
                }
            }
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

        void erase_elem(T& value) {
            if(find_elem(value)){
                auto range = htable_.equal_range(value);

                #ifdef DEBUG_
                std::cout << "fifo.erase_elem(" << value << ")" << std::endl;
                std::cout << "{+++++++++}   Yes, we are here!" << std::endl;
                #endif

                for(auto it = range.first; it != range.second; ++it){
                    lst_.erase(it->second);
                }

                htable_.erase(value);
            }
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
                #ifdef DEBUG_
                std::cout << "hop !" << std::endl;
                #endif
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
    size_t lrg_sz_, smll_sz_, lrg_cap_, smll_cap_;
    lru_cache<T> Am;
    fifo<T> Ain1, Ain2;

    public:
        two_queues(int lrg_cap = DEFAULT_MAIN_CAP, int smll_cap = DEFAULT_A_IN_CAP) : lrg_cap_(lrg_cap > 0 ? lrg_cap : 2),
        smll_cap_(smll_cap > 0 ? smll_cap : 1), Am(lrg_cap), Ain1(smll_cap), Ain2(lrg_cap_) {}

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
            std::cout << "lru_cache.drag_into_lru" << std::endl;
            #endif

            Am.insert_elem(value);
            Ain2.erase_last(value);
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

        bool perfect_cache_update(const T& value, T* input, int wrkbl_sz){
            Am.perfect_caching(input, wrkbl_sz);
            return cache_update(value);
        }

};

}
