#include<iostream>
#include<list>
#include<unordered_map>
#include<cstdlib>

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

            // for(auto it = lst_.begin(); it != lst_.end(); it++) { std::cout << *it << " "; }
            // std::cout << std::endl;

            // std::cout << "Erased.\n";
            // std::cout << lst_.back() << std::endl;

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
    std::unordered_map<int,  typename std::list<T>::iterator> htable_; // <int, std::list::iterator>


    fifo(int capacity = DEFAULT_A_IN_SZ) : lst_(), htable_(), capacity_(capacity) {

        std::cout << "Initialized fifo with capacity_ == " << capacity_ << std::endl;

        // в конструкторе это можно и не подвязывать, тк всё равно пока у конкретного элемента хэш-таблицы нет привязки к
        // своему листу, в этом элементе хэш-таблицы должен стоять ноль (а точнее nullptr).

    }

    bool is_full() const {
        // std::cout << "In is_full capacity_ is " << capacity_ << ", htable_.size() is " << htable_.size() << std::endl;
        return htable_.size() == capacity_;
    }

    bool is_empty() const {
        // верификатор
        assert(lst_.empty() && htable_.empty());
        assert(!htable_.empty() && !lst_.empty());

        return(lst_.empty() && htable_.empty());
    }

    bool find_elem(T value) const {
        // верификатор

        return htable_.find(value) != htable_.end();

        // если значения нет, нам вернут lst_.end(), иначе - итератор на элемент списка.
        // а нужна ли эта функция вовсе ?.. пусть останется пока что.

    }

    void insert_elem(T value) {
        // верификатор
        //
        // Но "очередь полна" æ - это сколько ? Нужно
        // понятие capacity. Но если переменная sz_ всегда задаётся lst_.size() == htable_.size() (при соблюдении
        // валидности структуры), нужна ли отдельная переменная sz_, если можно оставить только capacity_ и убрать
        // возможно resize'a вовсе. На самом деле, её быть и не может, тк кэш в любом случае должен быть конечен - иначе
        // зачем он нужен.
        // std::cout << "yes, we are here." << std::endl;

        // std::cout << htable_.size() << " " << lst_.size() <<std::endl;

        // если очередь полна, удали последний элемент и вставь новый в начало.
        if(is_full()){
            htable_.erase(lst_.back());
            lst_.pop_back();
        }

        lst_.push_front(value);
        htable_.insert({value, lst_.begin()});

        // std::cout << "After insert_elem htable_.size() is " << htable_.size() << std::endl;
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
        lrg_sz_(lrg_sz), smll_sz_(smll_sz) {
        std::cout << "Initialized 2Q with lrg_sz == " << lrg_sz_ << ", smll_sz_ == " << smll_sz_ << std::endl;
    }

    bool elem_hit(T value) const {
        return Am.find_elem(value) || Ain1.find_elem(value);
    }

    bool find_in_fifos(T value) const {
        return Ain1.find_elem(value) || Ain2.find_elem(value);
    }

    void drag_between_fifos() {
        // std::cout << "[drag_between_fifos]\n";

        T val = Ain1.lst_.back();
        Ain2.insert_elem(val);
        Ain1.erase_elem(val);
    }

    void update_fifos(T value) {

        if(Ain1.is_full()){
            // std::cout << "Ain1 is full\n";
            drag_between_fifos();
            Ain1.insert_elem(value);
        }else{
            // std::cout << "Ain1 is not full!\n";
            Ain1.insert_elem(value);
        }

    }

    void drag_into_lru(T value) {
        T val = *(Ain2.htable_.at(value));
        // std::cout << "Inserting in Am.\n";
        Am.insert_elem(val);
        // std::cout << "Erasing from Ain2.\n";
        Ain2.erase_elem(val);
    }

    bool cache_update(T value){
        bool result = elem_hit(value);

        if(Am.find_elem(value)){
            // std::cout << "So, we are here! Am!" << std::endl;
            Am.lru_update(value);
        }else{
            if(Ain2.find_elem(value)){
                // std::cout << "So, we are here! Lru!" << std::endl;
                drag_into_lru(value);
            }else{
                // std::cout << "So, we are here! Fifos!" << std::endl;
                update_fifos(value);
            }
        }

        // std::cout << std::endl;

        return result;
    }

};

}
