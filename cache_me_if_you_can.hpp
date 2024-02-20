#include<iostream>
#include<list>
#include<unordered_map>
#include<cstdlib>
#include<gtest/gtest.h>

namespace caches {

#define DEFAULT_MAIN_SZ 256
#define DEFAULT_A_IN_SZ 64

template <typename T>
struct lru_cache {
    int capacity_;
    std::list<T> lst_;
    std::unordered_map<int, typename std::list<T>::iterator> htable_;

    lru_cache(int capacity = DEFAULT_MAIN_SZ) : lst_(), htable_(), capacity_(capacity) {
        std::cout << "Initialized lru_cache with capacity_ == " << capacity_ << std::endl;
    }

    bool find_elem(T value) const{
        return htable_.find(value) != htable_.end();
    }

    void insert_elem(T value){
        // если кэш полон, вытеснить последний элемент <-- ???
        // если в кэше есть указанный элемент, переместить его на первое место

        if(htable_.find(value) == htable_.end()){
            lst_.push_front(value);
            htable_.insert({value, lst_.begin()});
        }else{
            auto it = htable_.at(value);

            T tmp = *it;
            lst_.erase(it);
            lst_.push_front(tmp);
            htable_.insert({tmp, lst_.begin()});
        }
    }

    bool lru_update(T value) {
        bool contained = find_elem(value);
        insert_elem(value);

        return contained;
    }
};


template <typename T>
struct fifo {
    int capacity_;
    std::list<T> lst_;
    std::unordered_map<int,  typename std::list<T>::iterator> htable_;


    fifo(int capacity = DEFAULT_A_IN_SZ) : lst_(), htable_(), capacity_(capacity) { }

    bool is_full() const {
        return htable_.size() == capacity_;
    }

    bool is_empty() const {
        // верификатор

        return(lst_.empty() && htable_.empty());
    }

    bool find_elem(T value) const {
        // верификатор

        return htable_.find(value) != htable_.end();
    }

    void insert_elem(T value) {
        // верификатор
        if(is_full()){
            htable_.erase(lst_.back());
            lst_.pop_back();
        }

        lst_.push_front(value);
        htable_.insert({value, lst_.begin()});
    }

    void erase_elem(T value) {
        if(htable_.find(value) != htable_.end()){
            auto it = htable_.at(value);
            htable_.erase(value);
            lst_.erase(it);
        }
    }

};


template <typename T>
struct two_queues {
    int lrg_sz_, smll_sz_;
    lru_cache<T> Am;
    fifo<T> Ain1, Ain2;

    two_queues(int lrg_sz = DEFAULT_MAIN_SZ, int smll_sz = DEFAULT_A_IN_SZ) : Am(lrg_sz_), Ain1(smll_sz_), Ain2(lrg_sz_),
        lrg_sz_(lrg_sz), smll_sz_(smll_sz) { }

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
        T val = *(Ain2.htable_.at(value));
        Am.insert_elem(val);
        Ain2.erase_elem(val);
    }

    bool cache_update(T value){
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

};

}
