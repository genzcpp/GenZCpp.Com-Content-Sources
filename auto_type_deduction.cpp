#include <iostream>
#include <boost/type_index.hpp>

using namespace std;

class MyType {};

template <typename T>
void myFunction1(T param) {
    cout << "T: "<<boost::typeindex::type_id_with_cvr<T>().pretty_name() << endl;
    cout << "paramType: "<<boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << endl << endl;
} 

template <typename T>
void myFunction2(const T param) {
    cout << "T: "<<boost::typeindex::type_id_with_cvr<T>().pretty_name() << endl;
    cout << "paramType: "<<boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << endl << endl;
}

template <typename T>
void myFunction3(T& param) {
    cout << "T: "<<boost::typeindex::type_id_with_cvr<T>().pretty_name() << endl;
    cout << "paramType: "<<boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << endl << endl;
}

template <typename T>
void myFunction4(const T* param) {
    cout << "T: "<<boost::typeindex::type_id_with_cvr<T>().pretty_name() << endl;
    cout << "paramType: "<<boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << endl << endl;
}

template <typename T>
void myFunction5(T&& param) {
    cout << "T: "<<boost::typeindex::type_id_with_cvr<T>().pretty_name() << endl;
    cout << "paramType: "<<boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << endl << endl;
}

int main () {
    int y = 888;
    auto x0 = 888;
    cout << "x0: "<<boost::typeindex::type_id_with_cvr<decltype(x0)>().pretty_name() << endl;
    const auto x1 = y;
    cout << "x1: "<< boost::typeindex::type_id_with_cvr<decltype(x1)>().pretty_name() << endl;
    const auto& x2 = y;
    cout << "x2: "<< boost::typeindex::type_id_with_cvr<decltype(x2)>().pretty_name() << endl;
    auto& x3 = y;
    cout << "x3: "<< boost::typeindex::type_id_with_cvr<decltype(x3)>().pretty_name() << endl;
    auto&& x4 = y;
    cout << "x4: "<< boost::typeindex::type_id_with_cvr<decltype(x4)>().pretty_name() << endl;
    auto&& x5 = 888;
    cout << "x5: "<< boost::typeindex::type_id_with_cvr<decltype(x5)>().pretty_name() << endl << endl;


    const MyType t1;
    MyType t2;
    cout << "t1: " << endl;
    myFunction1(t1); //example 1, T is MyType, ParamType is MyType
    cout << "t2: " << endl;
    myFunction2(t2); //example 2, T is MyType, ParamType is MyType const


    const volatile MyType& t3 = t2;
    MyType* ptr1;
    cout << "t3: " << endl;
    myFunction3(t3); //example 3, T is MyType const volatile, ParamType is MyType const volatile&
    cout << "ptr1: " << endl;
    myFunction4(ptr1); //example 4, T is MyType*, ParamType is MyType const*


    MyType t4{};
    const volatile MyType& t5 = t4;

    cout << "t4-lvalue: " << endl;
    myFunction5(t4); //example 5, T is MyType&, ParamType is MyType&
    cout << "t5: " << endl;
    myFunction5(t5); //example 6, T is MyType const volatile&, ParamType is MyType const volatile&
    cout << "t4-rvalue: " << endl;
    myFunction5(move(t4)); //example 7, T is MyType, ParamType is MyType&&
    cout << "233: " << endl;
    myFunction5(233); //example 8, T is int, ParamType is int&&

    cout << "t5 - rvalue: " << endl;
    myFunction5(move(t5)); //example 9, T is MyType const volatile, ParamType is MyType const volatile&&


    auto&& t4_auto = t5;
    cout << "t4_auto: "<<boost::typeindex::type_id_with_cvr<decltype(t4_auto)>().pretty_name() << endl << endl;
}