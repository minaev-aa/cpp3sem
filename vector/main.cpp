#include <iostream>

using namespace std;

template <class T>
class Vector
{
private:
    int dim;
    T *ptr;
public:
    Vector<T>& operator = (const Vector& A)
    {
        //оператор для копирования, a = b
        if(dim!=A.dim)
        {
            ptr = new T[A.dim];
            dim = A.dim;
        }
        int i=0;
        for(i=0;i<dim;i++)
        {
            //ptr[i] = A.GetComponent(i);
            ptr[i] = A.ptr[i];
        }

        return *this;
    }

    Vector<T>(const Vector& V)
    {
        //конструктор копий: создание копий и операция Vector A = B
        ptr = new T [V.dim];
        dim = V.dim;
        for(int i=0;i<V.dim;i++)
        {
            ptr[i] = V.ptr[i];
        }
    }

    Vector<T>(int Dimension)
    {
        dim = Dimension;
        ptr = new T [dim];
    }

    Vector<T>()
    {
        //конструктор Vector A;
        cout<<"\nConstructor 0 is working\n";
        dim = 3;
        ptr = new T [dim];
    }

    ~Vector<T>()
    {
        //деструктор
        cout<<"\nDestructor is working\n";
        delete[] ptr;
    }

    int Dimension()
    {
        return dim;
    }

    T GetComponent(int index)
    {
        if(index<0 || index>=dim)
            return 0;
        else
            return ptr[index];
    }

    void SetComponent(int index, T number)
    {
        if(index<0 || index>=dim)
            return;
        ptr[index] = number;
    }
    void push_back(T number)
    {
        T * ptr2 = new T [dim+1];
        for(int i=0; i<dim; i++)
        {
            ptr2[i]=ptr[i];
        }
        delete [] ptr;
        ptr = ptr2;
        ptr2[dim+1] = number;
    }
    void pop()
    {
        T * ptr2 = new T [dim-1];
        for(int i=0; i<dim-1; i++)
        {
            ptr2[i]=ptr[i];
        }
        delete [] ptr;
        ptr = ptr2;
    }
    void print()
    {
        int i=0;
        std::cout<<"\n";
        for(i=0;i<dim;i++)
        {
            std:: cout << ptr[i];
        }
        std::cout<<"\n";
    }


    T &operator[] (int i)
    {
        if(i>=0 && i<dim)
            return ptr[i];
    }
};

int main()
{
    if(1)
    {
        Vector<int> V(3);
        Vector<int> W(2);
        V[0] = 1; V[1] = 2; V[2] = 3;
        W[0] = -1; W[1] = 1;
        V.print();
    }
    system("pause");
    return 0;
}
