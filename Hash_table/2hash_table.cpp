// chain_hash_teble.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <iostream>

template <class T>
class Element
{
protected:
	Element<T>* next;
	T field;
public:

	Element<T> *  getNext() { return next; }
	void setNext(Element<T> *  str) { next = str; }
	T getValue() { return field; }
	void setValue(T str)
	{
		field = str;
	}
};

template <class T>
class LinkedList
{
protected:
	Element<T>* begin;
	int num;
public:
	int Number() { return num; }
	Element<T> * getBegin() { return begin; }

	LinkedList()
	{
		begin = nullptr;
		num = 0;
	}

	LinkedList(LinkedList& L)
	{
		begin = nullptr;
		num = L.Number();
		Element<T>* temp = L.begin;
		while(temp != nullptr){
			push(temp->getValue());
			temp = temp->getNext();
		}
	}

	Element<T>* push(T a)
	{
		Element<T>* cur = begin;
		if (begin == nullptr)
		{
			num = 1;
			begin = new Element<T>;
			begin->setValue(a);
			begin->setNext(nullptr);
			return begin;
		}
		while (cur->getNext() != nullptr)
		{
			cur = cur->getNext();
		}
		num++;
		cur->setNext(new Element<T>);
		cur->getNext()->setValue(a);
		cur->getNext()->setNext(nullptr);
		return cur;
	}

	~LinkedList<T>()
	{
		num = 0;
		Element<T> * cur = begin;
		if (cur != nullptr)
		{
			while (cur != nullptr)
			{
				Element<T> * prev = cur;
				cur = cur->getNext();
				delete prev;
			}
			begin = nullptr;
		}
	}

	Element<T>* operator[](int i)
	{
		if (i<0 || i>num) 
			return nullptr;
		Element<T> * cur = begin;
		for (int k = 0; k<i; k++)
		{
			cur = cur->getNext();
		}
		return cur;
	}

	void print()
	{
		Element<T> *cur = begin;
		while (cur != nullptr)
		{
			std::cout << cur->getValue() << " ";
			cur = cur->getNext();
		}
		std::cout << "\n";
	}

	
	void remove_i(int n)
	{
		Element<T> * e = (*this)[n];
		remove(e);
	}

	void remove(Element<T> * current)
	{
		if (current != nullptr)
		{
			Element<T> * cur = begin;
			if(cur!=current)
			{
				while (cur->getNext() != current)
				{
					cur = cur->getNext();
				}
			}
			cur->setNext(current->getNext());
			num--;
			delete current;
		}
	}
	
	Element<T> *  find(T a)
	{
		Element<T> * e = begin;
		if(begin==nullptr)
			return nullptr;
		else
		{
			for(e;e!=nullptr;e=e->getNext())
			{
				if (e->getValue()==a)
					return e;
			}
		}
		return nullptr;
	}

	void del_all()
	{
		this->~LinkedList<T>();
	}

	void remove(T a)
	{
		Element<T>*e = find(a);
		if(e!=nullptr)
		{
			Element<T>*next=e->getNext();
			if (e != begin)
				remove(e);
			if (e == begin)
			{
				remove(e);
				begin = next;
			}
		}
		else
			std::cout<< "Not find";
	}
	friend  std::ostream& operator<<(std::ostream& os, LinkedList& F);
};
template <class T>
std::ostream& operator<<(std::ostream& os, LinkedList<T>& F)
{
	Element<T>* cur = F.getBegin();
		while (cur != NULL)
		{
			os << cur->getValue() << "\n";
			cur = cur->getNext();
		}
	return os;
}



template <class T>
class Chainhash
{
private:
	LinkedList<T>* table;
	int len;
	static const int prime = 15;

public:
	int getCount() { return len; }
	void setCount(int str) { len = str; }
	void setTable(int str) { table = str; }
	Chainhash(int size)
	{
		table = new LinkedList<T>[size];
		len = size;
		for(int i = 0; i < len; i++) 
		{
            table->push("");
        }
	}
	~Chainhash()
	{
		Element<T>* elem = table->getBegin();
		Element<T>* next_elem = elem->getNext();
        for(int i = 0; i < len; i++) {
            delete elem;
            Element<T>* elem = next_elem;
            if(i != len - 1)
			{
                next_elem = next_elem->getNext();
			}
        }
        delete table;
    }
	
	int hash_func(int prime, std::string str)
	{
		int hash = 0;
		for (unsigned int i = 0; i < str.size(); i++)
		{
			hash = (prime * hash + str[i]) % len;
		}
		return hash;
	}

	void push(T str)
	{
		int hash = hash_func(prime, str);
		//std::cout<<hash<<"::"<<str<<std::endl;
		table[hash].push(str);
	}

	int find(T str)
	{
		int hash = hash_func(prime, str);
		Element<T>* elem = table[hash].getBegin();
		while ((elem != nullptr) && (elem->getValue() != str))
			elem = elem->getNext();
		if (elem == nullptr)
			return -1;
		else
			return hash+1;
	}

	void remove(T str)
	{
		if (find(str) == -1)
			std::cout << "Not found";
		else
			table[hash_func(prime, str)].remove(str);
	}
	void print()
	{
		for(int i = 0; i < getCount();i++)
		{
			table[i].print();
			std::cout << "----" << std::endl;
		}
	}
};

template <class T>
class DoubleCash
{
private:
	struct Elem
    {
        T str;
        bool state;
        Elem(T val)
        {
	        str=val;
        	state=true;
        }
    };
	Elem** table;
	int len; //размер таблицы
	int size; //количество с удалёнными
	int count; //количество без удалённых
	void Resize()
    {
        int p_len = len;
        len *= 2;
        count = 0;
        size = 0;
        Elem** table2 = new Elem * [len];
        for (int i = 0; i < len; i++)
            table2[i] = nullptr;
        std::swap(table, table2);
        for (int i = 0; i < p_len; i++)
        {
            if (table2[i] && table2[i]->state)
                push(table2[i]->str);
        }
        for (int i = 0; i < p_len; i++)
            if (table2[i])
                delete table2[i];
        delete[] table2;
    
    }

    void Rehash()
    {
        count = 0;
        size = 0;
        Elem** table2 = new Elem * [len];
        for (int i = 0; i < len; ++i)
            table2[i] = nullptr;
        std::swap(table, table2);
        for (int i = 0; i < len; i++)
        {
            if (table2[i] && table2[i]->state)
                push(table2[i]->str);
        }
        for (int i = 0; i < len; i++)
            if (table2[i])
                delete table2[i];
        delete[] table2;
    }
public:

	int getCount() { return len; }
	void setCount(int str) { len = str; }
	void setTable(bool str) { table = str; }
	DoubleCash(int def)
	{
		len = def;
        size = 0;
        count = 0;
        table = new Elem*[len];
        for (int i = 0; i < len;i++)
            table[i] = nullptr;
	}
	~DoubleCash()
	{
		for (int i = 0; i < len; i++)
	       if (table[i])
	           delete table[i];
        delete[] table;
    }
	
	int hash_func1(T str)
	{
		int prime1 = 7;
		int hash = 0;
		for (unsigned int i = 0; i < str.size(); i++)
		{
			hash = (prime1 * hash + str[i]) % len;
		}
		return hash;
	}
	int hash_func2(T str)
	{
		
		int prime2 = 4;
		int alpha = 3;
		int hash = 0;
		for (unsigned int i = 0; i < str.size(); i++)
		{
			hash = (hash * prime2 + alpha) % len;
		}
		return hash;
	}
	bool push(T str)
	{
		if (size + 1 > int(0.75 * len))
            Resize();
        else if (count > 2 * size)
            Rehash();
        int h1 = hash_func1(str);
        int h2 = hash_func2(str);
        int i = 0;
        int first_deleted = -1;
        while (table[h1] != nullptr && i < len)
        {
            if (table[h1]->str == str && table[h1]->state)
                return false;
            if (!table[h1]->state && first_deleted == -1)
                first_deleted = h1;
            h1 = (h1 + h2) % len;
            i++;
        }
        if (first_deleted == -1)
        {
            table[h1] = new Elem(str);
            ++count;
        }
        else
        {
            table[first_deleted]->str = str;
            table[first_deleted]->state = true;
        }
        size++;
        return true;
	}
	
	bool remove(T str)
	{
		int h1 = hash_func1(str);
        int h2 = hash_func2(str);
        int i = 0;
        while (table[h1] != nullptr && i < len)
        {
            if (table[h1]->str == str && table[h1]->state)
            {
                table[h1]->state = false;
                size--;
                return true;
            }
            h1 = (h1 + h2) % len;
            i++;
        }
        return false;
	}
	void print()
	{
		for (int i = 0; i < len; i++) {
            if (table[i]!= nullptr)
                std::cout << i << " --> "
					<< table[i]->str <<  std::endl;
			std::cout << i << " --> " << "NULL" <<  std::endl;
		}}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Chainhash<std::string> test(10);
	test.push( "peller");
	test.push( "sonrai");
	test.push( "merlin");
	test.push( "storax");
	test.push( "zoeic");
	test.push( "dusack");
	test.push( "sandan");
	test.push( "zincke");
	test.push( "fodder");
	test.remove( "zoeic");
	test.print();
	std::cout << std::endl;
	std::cout << std::endl;
	DoubleCash<std::string> test2(10);
	test2.push( "peller");
	test2.push( "sonrai");
	test2.push( "merlin");
	test2.push( "storax");
	test2.push( "zoeic");
	test2.push( "dusack");
	test2.push( "sandan");
	test2.push( "zincke");
	test2.push( "fodder");
	test2.remove( "zoeic");
	test2.print();
	return 0;
}

