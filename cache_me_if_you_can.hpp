#include<iostream>
#include<list>
#include<unordered_map>
#include<cstdlib>
#include<gtest/gtest.h>
#include<cstring>

namespace caches {

#define DEFAULT_MAIN_SZ 256
#define DEFAULT_A_IN_SZ 64
#define FLOAT_TOLERANCE 1e-5

struct cache_szs_{
    int sz1 = 0, sz2 = 0;
};

template <typename U>
struct cache_list_iters{
    typename std::list<U>::const_iterator cbegin, cend;
};

template <typename T>
class lru_cache {

    int capacity_;
    std::list<T> lst_;
    std::unordered_map<T, typename std::list<T>::iterator> htable_;

    public:

        lru_cache(int capacity = DEFAULT_MAIN_SZ) : capacity_(capacity > 0 ? capacity : 2), lst_(), htable_() { }

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
        }

        void insert_elem(const T& value) {
            if(htable_.find(value) == htable_.end()){
                if(is_full()){
                    auto it = lst_.back();
                    htable_.erase(it);
                    lst_.pop_back();
                }

                lst_.push_front(value);
                htable_.insert({value, lst_.begin()});
            }else{
                auto it = htable_.at(value);

                T tmp = *it;
                lst_.erase(it);
                lst_.push_front(tmp);
                htable_.erase(tmp);
                htable_.insert({tmp, lst_.begin()});
            }
        }

        bool lru_update(const T& value) {
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
        *   элемент будет не жалко потерять и запихнуть в конец очереди - этим занимается цикл на строчках 97-102, на строчках
        *   118-127 элементы обмениваются, если подходящий для обмена найден.
        */
            if(!is_full()){ return; }

            T curr_last = lst_.back();
            int swap_len_request = -1;

            for(size_t i = 0; i < wrkbl_sz; ++i){
                // if(input[i] == curr_last){
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
                        // if(input[i] == *it){
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
    int capacity_;
    std::list<T> lst_;
    std::unordered_multimap<int, typename std::list<T>::iterator> htable_;

    public:

        fifo(int capacity = DEFAULT_A_IN_SZ) : capacity_(capacity > 0 ? capacity : 1), lst_(), htable_() { }

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
            if(is_full()){
                auto it_rng = htable_.equal_range(lst_.back());

                htable_.erase(it_rng.first);
                lst_.pop_back();
            }

            lst_.push_front(value);
            htable_.insert({value, lst_.begin()});
        }

        void erase_elem(const T& value) {
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
class two_queues {
    int lrg_sz_, smll_sz_;
    lru_cache<T> Am;
    fifo<T> Ain1, Ain2;

    public:
        two_queues(int lrg_sz = DEFAULT_MAIN_SZ, int smll_sz = DEFAULT_A_IN_SZ) : lrg_sz_(lrg_sz > 0 ? lrg_sz : 2),
        smll_sz_(smll_sz > 0 ? smll_sz : 1), Am(lrg_sz), Ain1(smll_sz), Ain2(lrg_sz_) {}

        cache_szs_ size() const {
            cache_szs_ sizes = { lrg_sz_, smll_sz_ };
            return sizes;
        }

    private:
        bool elem_hit(const T& value) const {
            return Am.find_elem(value) || Ain1.find_elem(value);
        }

        bool find_in_fifos(const T& value) const {
            return Ain1.find_elem(value) || Ain2.find_elem(value);
        }

        void drag_between_fifos() {
            T val = Ain1.lst_.back();
            Ain2.insert_elem(val);
            Ain1.erase_elem(val);
        }

        void update_fifos(const T& value) {

            if(Ain1.is_full()){
                drag_between_fifos();
                Ain1.insert_elem(value);
            }else{
                Ain1.insert_elem(value);
            }

        }

        void drag_into_lru(const T& value) {
            T val = *(Ain2.htable_.equal_range(value).first->second);
            Am.insert_elem(val);
            Ain2.erase_elem(val);
        }


    public:

        bool cache_update(const T& value){
            bool result = elem_hit(value);

            if(Am.find_elem(value)){
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

        bool perfect_cache_update(const T& value, T* input, int wrkbl_sz){
            Am.perfect_caching(input, wrkbl_sz);
            return cache_update(value);
        }

};

}
