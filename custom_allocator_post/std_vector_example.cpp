#include <iostream>
#include <memory>
#include <vector>

using namespace std;

void * operator new(size_t size)
{
    cout<< "Allocating " << size << "bytes" << endl;
    void * p = malloc(size);
    return p;
}

void operator delete(void * p)
{
    cout<< "Deallocating" << endl;
    free(p);
}

int main()
{
    std::vector<int> v_reserved;
    v_reserved.reserve(6);
    v_reserved.push_back(1);
    v_reserved.push_back(1);
    v_reserved.push_back(1);
    v_reserved.push_back(1);
    v_reserved.push_back(1);
    v_reserved.push_back(1);

    std::vector<int> v;
    cout << "1. capacity " << v.capacity() << endl;
    v.push_back(1);
    cout << "2. capacity " << v.capacity() << endl;
    v.push_back(1);
    cout << "3. capacity " << v.capacity() << endl;
    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    cout << "4. capacity " << v.capacity() << endl;
    v.push_back(1);
    cout << "5. capacity " << v.capacity() << endl;
    return 0;
}
