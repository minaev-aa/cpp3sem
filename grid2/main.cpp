#include <iostream>

template <class T>
class Grid {
private:
    T *memory;
    size_t x_size, y_size;

public:
    Grid();
    Grid(size_t x_size, size_t y_size);
    ~Grid();

    T operator()(size_t x_idx, size_t y_idx) const;
    T& operator()(size_t x_idx, size_t y_idx);

    size_t get_xsize() const;
    size_t get_ysize() const;

    Grid(const Grid&);            // copy constructor
    Grid(Grid&&) noexcept;        // move constructor
    Grid& operator=(Grid const&); // copy assignment
    Grid& operator=(Grid&& d) noexcept ;     // move assignment

    void swap(Grid<T> &  d) noexcept
    {
        std::swap(x_size, d.x_size);
        std::swap(y_size, d.y_size);
        std::swap(memory, d.memory);
    }

    template <typename V>
    friend std::ostream& operator<<(std::ostream&, Grid<V> const&);
    template <typename V>
    friend std::istream& operator>>(std::istream&, Grid<V> &);

};
template<class T>
Grid<T>::~Grid()
{
    if(memory != nullptr)
    {
        delete [] memory;
        memory = nullptr;
    }
}
template<class T>
Grid<T>::Grid():x_size(0), y_size(0) {
    memory = new T[0];
}
template<class T>
Grid<T>::Grid(size_t x, size_t y):x_size(x), y_size(y) {
    memory = new T[x*y];
}

template<class T>
T Grid<T>::operator()(size_t x_idx, size_t y_idx) const {
    T res;
    if ((x_idx <= x_size) && (y_idx <= y_size))
    {
        res = memory[x_idx * this->y_size + y_idx];
    }
    return res;
}

template<class T>
T &Grid<T>::operator()(size_t x_idx, size_t y_idx) {
    T& res = memory[x_idx * this->y_size + y_idx];
    return res;
}

template<class T>
size_t Grid<T>::get_xsize() const {
    return x_size;
}

template<class T>
size_t Grid<T>::get_ysize() const {
    return y_size;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Grid<T> &gr) {
    for (int i = 0; i < (gr.x_size); i ++){
        for (int j = 0; j < (gr.y_size); j++){
            out << gr.memory[i * (gr.y_size) +  j] << " ";
        }
        out << "\n";
    }
    return out;
}
template <typename T>
std::istream &operator>>(std::istream &out, Grid<T> &gr) {
    for (int i = 0; i < gr.x_size * gr.y_size; i++){
        out >> gr.memory[i];
    }
    return out;
}

template<class T>
Grid<T>::Grid(const Grid<T> &gr) : x_size(gr.get_xsize()),  y_size(gr.get_ysize()), memory(new T[gr.get_xsize()*gr.get_ysize()])
{
    std::copy(gr.memory, gr.memory + (gr.x_size*gr.y_size), memory);
}

template<class T>
Grid<T>::Grid(Grid<T> &&gr) noexcept:x_size(std::move(0)), y_size(std::move(0)), memory(std::move(nullptr))
{
    gr.swa(*this);
}

template<class T>
Grid<T> &Grid<T>::operator=(Grid<T> const&d) {
    Grid<T> t(d);
    t.swap(*this);
    return *this;
}

template<class T>
Grid<T> &Grid<T>::operator=(Grid<T> &&d)  noexcept {
    d.swap(*this);
    return *this;
}

int main() {
    Grid<int> A(3,3);
    Grid<int> B(3,3);
    std::cin >> A;
    std::cout << A(0, 1)  << "\n";;
    std::cout << A;
    B = A;
    A.~Grid();
    return 0;
}