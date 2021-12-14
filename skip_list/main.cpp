#include <algorithm>
#include <random>
#include <vector>
#include <iostream>

template<typename T, typename X>
class Skip_list {
public:
    using value_type = std::pair<const int, T>;
    struct Skip_node {
        value_type value;
        std::size_t levels;
        Skip_node *next[1] = std::vector<Skip_node *>(1, nullptr);
    };

private:
    std::vector<Skip_node *> head;
    static bool next_level() noexcept;
    std::size_t generate_level(int size) const;
    static Skip_node* allocate_node(value_type value, std::size_t levels);
    void free_all_nodes(Skip_node *head) noexcept;

public:

    template<typename ValueType>
    class iterator_base {
    public:
        using node_type = Skip_node;
        using value_type = ValueType;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

        iterator_base &operator=(const iterator_base &other) {
            curr = other.curr;
            return *this;
        }

        bool operator==(const iterator_base &b) const { return curr == b.curr; }

        bool operator!=(const iterator_base &b) const { return curr != b.curr; }

        iterator_base &operator++() {
            if (curr)
                curr = curr->next[0];
            return *this;
        }

        iterator_base operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        value_type &operator*() { return curr->value; }

        value_type *operator->() { return &curr->value; }

        iterator_base& operator+=(const int offset)
        {
            if (offset <= 0) return *this;
            for (int i = 0; i < offset; ++i)
                ++*this;
            return *this;
        }

        iterator_base operator+(const int offset)
        {
            iterator_base it = *this;
            it += offset;
            return it;
        }

        bool operator>(const iterator_base& b) {
            if (curr != nullptr && b.curr != nullptr)
                return curr->value.first > b.curr->value.first;
            else if (curr == nullptr && b.curr != nullptr)
                return true;
            else
                return false;
        }

        bool operator<(iterator_base& b)
        {
            return (!(curr < b.curr) && (curr != b.curr));
        }

        bool operator<=(iterator_base& b)
        {
            return ((curr == b.curr) || (b.curr < curr));
        }

        bool operator>=(iterator_base& b)
        {
            return ((curr == b.curr) || (b.curr > curr));
        }

    private:
        node_type *curr;

        iterator_base() : curr(nullptr) {}

        explicit iterator_base(node_type *pos) : curr(pos) {}

        friend class Skip_list;

    };

    using iterator = iterator_base<value_type>;

    Skip_list() = default;// конструктора по умолчанию для пустого списка без элементов

    explicit Skip_list(typename X::iterator it_key,  typename X::iterator end_key, typename X::iterator it_data, typename X::iterator end_data)
    {
        if(std::distance(it_key,end_key)== std::distance(it_data,end_data)){
            while(it_key < end_key) {
                insert(std::make_pair(*it_key,*it_data));
                it_key++;
                it_data++;
            }
        }
    }
//конструктора от диапазона итератора, возможно не отсортированного, на любую другую структуру данных?todo

    ~Skip_list()                   // Деструктор
    {
        if (!head.empty())
            free_all_nodes(head[0]);
    }

    //эффективные конструктора перемещения и перемещающего присваивания
    Skip_list(Skip_list const &other) {
        try {
            head.assign(other.head.size(), nullptr);
            auto tail = std::vector<Skip_node**>{};
            tail.reserve(head.size());
            std::for_each(std::begin(head), std::end(head), [&](auto&& link) { tail.push_back(&link); });
            if(other.head.size() != 0){
                for (auto node = other.head[0]; node != nullptr; node = node->next[0]) {
                    const auto copy_node = allocate_node(node->value, node->levels);

                    for (auto i = 0u; i < copy_node->levels; ++i) {
                        *tail[i] = copy_node;
                        tail[i] = &copy_node->next[i];
                    }
                }}
            std::for_each(std::begin(tail), std::end(tail), [](auto link) { *link = nullptr; });
        }
        catch (...) {
            free_all_nodes(head[0]);
            throw;
        }
    }

    Skip_list &operator=(Skip_list const &other) {
        using std::swap;
        auto temp = other;
        swap(temp, *this);
        return *this;
    }

    friend auto swap(Skip_list &a, Skip_list &b) noexcept {
        std::swap(a.head, b.head);
    }

    Skip_list(Skip_list &&other) noexcept: Skip_list() {
        using std::swap;
        swap(*this, other);
    }

    Skip_list &operator=(Skip_list &&other) noexcept {
        using std::swap;
        swap(*this, other);
        return *this;
    }

    // ------------Итераторы
    iterator begin() noexcept {
        return iterator(head[0]);
    }

    iterator end() noexcept {
        iterator x = this->begin();
        x+=((this->size())-1);
        return iterator(x);
    }

    //____________________________
    T &operator[](const int &key) {
        return find(key)->second;
   }

    bool empty() const noexcept {
        return (head[0] == nullptr);
    }  //метода empty, возвращающего true, если список пуст, иначе - false;


    std::size_t  size() const noexcept {
        auto counter = std::size_t();
        for (auto index = head[0]; index != nullptr; index = index->next[0], ++counter);
        return counter;
    }    //возвращающего текущее количество элементов в списке


    //_____________
    iterator insert(const value_type &value);   //вставки нового значения
    Skip_list<T, X>* insert(typename X::iterator it_key,  typename X::iterator end_key, typename X::iterator it_data, typename X::iterator end_data);
    //перегруженного метода insert для вставки диапазона итератора,
    // возможно не отсортированного, на любую другую структуру данных

    Skip_list<T, X>* erase(const int &key, const T &value);
    Skip_list<T, X>* erase(iterator it);
    Skip_list<T, X>* erase(iterator it1, iterator it2);

    void clear() noexcept               //  удаления всех элементов списка
    {
        free_all_nodes(head[0]);
        head.assign(1, nullptr);
    }

    iterator find(const int &key);  // метода find для поиска положения ключа в списке: еслиключ найден
    // - вернуть итератор на значение, если ключ не найден
    // - вернуть past-the-end итератор, равный вызову end;


    std::size_t count(const int &key){
        int n = 0;
        auto x = find(key);
        if(x.curr!=nullptr){
        while(x.curr->value.first==key) {
            n++;
            x++;
            if(x.curr!=nullptr){
                break;
            }
        }}
        return n;
    }//количество элементов с данным ключом

    std::size_t top_level() const { return head.size(); }     // максимльная высотв башни списка

    iterator lower_bound(const value_type &value);      //возвращающего итератор на первый элемент не меньше заданного;
    iterator upper_bound(const value_type &value);      //возвращающего итератор на первый элемент строго больше заданного;
    std::pair<iterator,iterator> equal_range(const int &key); //метод equal_range, возвращающий промежуток итераторов на равные заданному ключу элементы.

    void free_node(Skip_node *node);

    Skip_list<T, X> *insert(iterator it_key, iterator end_key, iterator it_data, iterator end_data);
};

template<typename T, typename X>
bool Skip_list<T,X>::next_level() noexcept
// шанс выпадания 1
{
    double p = 0.5;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> distrib({1 - p, p});
    return distrib(gen);
}

template<typename T, typename X>
std::size_t Skip_list<T, X>::generate_level(int size) const {
    std::size_t new_node_level = size;
    do {
        ++new_node_level;
    } while (new_node_level <= head.size() && next_level());

    return new_node_level;
}
template< typename T, typename X>
typename Skip_list<T, X>::Skip_node* Skip_list<T, X>::allocate_node(value_type value, std::size_t levels)
{
    const auto node_size = sizeof(Skip_node) + (levels - 1) * sizeof(Skip_node*);
    const auto node = _aligned_malloc(node_size, alignof(Skip_node));
    new(node) Skip_node{ value, levels, nullptr };
    return reinterpret_cast<Skip_node*>(node);
}

template<typename T, typename X>
void Skip_list<T, X>::free_node(Skip_node* node) {
    node->~Skip_node();
    _aligned_free(node);
}

template<typename T, typename X>
void Skip_list<T, X>::free_all_nodes(Skip_node* heads) noexcept {
    for (auto index = heads; index != nullptr;) {
        const auto temp = index;
        index = index->next[0];
        free_node(temp);
    }
}
template<typename T, typename X>
typename Skip_list<T, X>::iterator Skip_list<T, X>::insert(const value_type &value) {
    auto level = head.size();
    const auto insert_level = generate_level(level);
    auto insert_node = allocate_node(value, insert_level);
    if (head.size() < insert_level) {
        head.resize(insert_level, nullptr);
        level = head.size();
    }
    auto next = head.data();
    Skip_list::iterator insert_pos;
    while (level > 0) {
        const auto index = level - 1;
        auto node = next[index];
        if (node == nullptr) {
            if (level <= insert_level) {
                insert_node->next[index] = next[index];
                next[index] = insert_node;
                insert_pos = Skip_list::iterator{next[index]};
            }
            --level;
        }
        else if(node->value.first > value.first) {
            if (level <= insert_level) {
                insert_node->next[index] = next[index];
                next[index] = insert_node;
                insert_pos = Skip_list::iterator{next[index]};
            }
            --level;
        }
        else if (node->value.first == value.first) {
            if (level <= insert_level) {
                if(node->value.second > value.second){
                    insert_node->next[index] = next[index];
                    next[index] = insert_node;
                    insert_pos = Skip_list::iterator{next[index]};}
                else{
                    Skip_node* x(next[index]);
                    if((x->next)[index]!= nullptr){
                        while((x->next)[index] != nullptr ){
                            if ((x->next)[index]->value.first == value.first)
                            {
                                next = x->next;
                                x = next[index];
                            }
                            else
                            {
                                insert_node->next[index] = (x->next)[index];
                                break;
                            }
                        }
                    }
                    x->next[index]=insert_node->next[index];
                    x->next[index] = insert_node;
                    next[index] = x;
                    insert_pos = Skip_list::iterator{next[index]};
                }
            }
            --level;
        }

        else {
            next = node->next;
        }
    }
    return insert_pos;
}

template<typename T, typename X>
Skip_list<T, X>* Skip_list<T, X>::erase(const int &key, const T &value) {
    Skip_list::Skip_node *node = nullptr;
    auto level = head.size();
    auto next = head.data();
    while (level > 0) {
        const auto link_index = level - 1;
        if (!next[link_index] || next[link_index]->value.first > key) {
            --level;
        } else if (next[link_index]->value.first == key && next[link_index]->value.second == value) {
            node = next[link_index];
            next[link_index] = node->next[link_index];
            --level;
        } else {
            next = next[link_index]->next;
        }
    }
    while (head.size() > 1 && head.back() == nullptr)
        head.pop_back();
    if (node) {
        free_node(node);
        return this;
    }
    else {
        return this;
    }
}

template<typename T, typename X>
Skip_list<T, X>* Skip_list<T, X>::erase(Skip_list::iterator it) {
    if(it.curr!= (nullptr)){
        auto x = it.curr->value.first;
        auto y = it.curr->value.second;
        return this->erase(x, y);
    }
}

template<typename T, typename X>
Skip_list<T, X>* Skip_list<T, X>::erase(Skip_list::iterator it1, Skip_list::iterator it2) {
    Skip_list::iterator a = it1; //? Возможно ошибки
    auto b = a;
    do{
        b = a+1;
        *this->erase(a);
        a=b;
    }
    while(a!=it2);
    *this->erase(b);
    return this;
    }

template<typename T, typename X>
typename Skip_list<T, X>::iterator Skip_list<T, X>::find(const int& key)
    {
        auto level = head.size();
        auto next = head.data();
        while (level > 0) {
            const auto index = level - 1;
            if (!next[index] || next[index]->value.first > key) {
                --level;
            }
            else if (next[index]->value.first == key) {
                return iterator{ next[index] };
            }
            else {
                next = next[index]->next;
            }
        }
        auto x=end()+1;
        return x;
    }

template<typename T, typename X>
typename Skip_list<T, X>::iterator Skip_list<T, X>::lower_bound(const value_type &value) {
    auto level = head.size();
    const auto insert_level = generate_level(level);
    if (head.size() < insert_level) {
        head.resize(insert_level, nullptr);
        auto level = head.size();

    }
    auto next = head.data();
    while (level > 0) {
        const auto index = level - 1;
        auto node = next[index];
        if (node->value.second >= value.second) {
            Skip_list::iterator insert_pos = Skip_list::iterator{next[index]};
            return insert_pos;
        }
        else if (node == nullptr) {
            --level;
        }
        else {
            next = node->next;

        }
    }
    return iterator_base();
}

template<typename T, typename X>
typename Skip_list<T, X>::iterator Skip_list<T, X>::upper_bound(const value_type &value) {
    auto level = head.size();
    const auto insert_level = generate_level(level);
    if (head.size() < insert_level) {
        head.resize(insert_level, nullptr);
        auto level = head.size();
    }
    auto next = head.data();
    while (level > 0) {
        const auto index = level - 1;
        auto node = next[index];
        if (node->value.second > value.second) {
            Skip_list::iterator insert_pos = Skip_list::iterator{next[index]};
            return insert_pos;
        }
        else if (node == nullptr) {
            --level;
        }
        else {
            next = node->next;

        }
    }
    return iterator_base();
}


template<typename T, typename X>
std::pair<typename Skip_list<T, X>::iterator, typename Skip_list<T, X>::iterator> Skip_list<T, X>::equal_range(const int &key) {
    auto x = find(key);
    auto y = x;
    if ((y+1).curr != nullptr) {
        while ((y+1).curr->value.first == key) {
            y++;
            if ((y+1).curr == nullptr) {
                break;
            }
        }
        return std::make_pair(x,y);
    }
    else
        return std::make_pair(x, y);
}

template<typename T, typename X>
Skip_list<T, X>* Skip_list<T, X>::insert(typename X::iterator it_key,  typename X::iterator end_key, typename X::iterator it_data, typename X::iterator end_data) {
    if(std::distance(it_key,end_key)== std::distance(it_data,end_data)){
        while(it_key < end_key) {
            this->insert(std::make_pair(*it_key,*it_data));
            it_key++;
            it_data++;
        }
    }
    return  this;
}


int main() {
    std::vector <int> z = {1, 2, 3, 4};
    std::vector <int> e = {5, 6, 7, 8};
    Skip_list<int,std::vector<int>> sk;
    Skip_list<int, std::vector<int>> sk1(sk);
    Skip_list<int, std::vector<int>> sk3(z.begin(), z.end(), e.begin(), e.end());
    sk3.insert(e.begin(), e.end(),z.begin(), z.end() );
    sk.insert(std::make_pair(1, 21));
    sk.insert(std::make_pair(1,34));
    sk.insert(std::make_pair(4, 54));
    sk.insert(std::make_pair(9, 67));
    sk.insert(std::make_pair(7, 16));
    Skip_list<int, std::vector<int>> sk2 (sk);
    auto m = sk.equal_range(1);
    std::cout<<sk.empty()<<'\n';
    std::cout<<sk.count(1)<<'\n';
    sk.erase(1, 30);
    std::cout<<sk.count(1)<<'\n';
    std::cout<<sk.count(4)<<'\n';
    auto a = sk.find(2);
    auto b = sk.find(1);
    sk.erase(a);
    sk.clear();
    sk.erase(sk.begin(),sk.end());

    return 0;
}