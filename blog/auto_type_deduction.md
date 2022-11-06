---
layout: page
title: auto, auto&, auto&&
---
>
When I first used C++ in college around 2015, I thought `auto` is the coolest feature of C++. Unlike Java, now I don't have to write long type names anymore (Java was my 1st programming language and `var` type inference was not available at the time). However, I have no idea how `auto` works and thought it was just magic.
>
Now, 8 years later, I finally settle down and read through several books about C++. I start to realize how much time I have wasted on [voodoo programming](https://en.wikipedia.org/wiki/Voodoo_programming) and the fun I have missed from not going deep. 

Let's dive right in. Starting with the simple question, what is the type of `x?` in each line?
```c++
int y = 888;

auto x0 = 888;
const auto x1 = y;
const auto& x2 = y;
auto& x3 = y;
auto&& x4 = y;
auto&& x5 = 888;
```
Answer:
```
x0: int
x1: int const
x2: int const&
x3: int&
x4: int&
x5: int&&
```

The answer shouldn't be surprising except for `x4`. **Didn't I just try to request for an rvalue with auto&&?**
> rvalue is a **prvalue** (pure rvalue) or **xvalue** (expiring value). - As explained [here](https://en.cppreference.com/w/cpp/language/value_category).

> The way I try to differentiate **lvalue** vs **rvalue** is that we **can take address of an lvalue** but we **cannot take address of an rvalue**.

> with `auto&&`, we're trying to get an **universal reference**, which I will explain later.

Before looking at how auto type deduction works, let's see another example with y being const int.
```c++
const int y = 888;
```
The result becomes:
```
x0: int
x1: int const
x2: int const&
x3: int const&
x4: int const&
```
How come `x3` is not `int&` here? 

> "Well, if I can easily get a non-const reference of `y` (which is const) by `auto& x3 = y;`, I seriously doubt the legitimacy of the constness concept - so it has to be this way"

Now that's a post-hoc thought after I know the answer. I should stop taking guesses and try to understand instead.

## How does auto type deduction work then?
Before going to the answer, we need to understand how does **template type deduction** work. - because template type deduction and auto type deduction works exactly the same.

## Template Type Deduction 
> I'll refer to this as **TTD** for short

Template type deduction tries to answer the question: **What is `param`'s type for the following code?**
- *ParamType* is some expression of T. e.g.(`T`, `T&`, `T&&`, ...). 

```c++
template <typename T>
void myFunction(ParamType param);

//call site
myFunction(expr)
```
The answer depends on what *ParamType* is. There are 3 cases:
### 1. *ParamType* is neither a pointer nor reference.

```c++
template <typename T>
void myFunction1(T param); 

template <typename T>
void myFunction2(const T param); 

const MyType t1;
MyType t2;
myFunction1(t1); //example 1, T is MyType, ParamType is MyType
myFunction2(t2); //example 2, T is MyType, ParamType is MyType const
```
This is the easiest case. No matter what `expr` is, the const-ness `const` of `expr` is dropped, then const-ness of *ParamType* is applied base on the expression of *ParamType* (what kind of T is it?). 

- Example 1, `T` is deduced to by `MyType` by dropping `const`-ness of `t1`, then *ParamType* becomes `MyType` as it's expression is simply `T` in `myFunction1`.

- Example 2, `T` is deduced to by `MyType`, then `const` is applied as part of the expression of *ParamType* (`const T`) in `myFunction2`. 

The template functions that fit this case are passing parameters **by-copy**, so it makes sense that the TTD behaves this way. aka, We're already using by-copy, why do we care about const-ness of input.

### 2. *ParamType* is a reference or Pointer, but not universal reference (T&&).

```c++
template <typename T>
void myFunction3(T& param); 

template <typename T>
void myFunction4(const T* param); 

//using t2 from above
const volatile MyType& t3 = t2;
MyType* ptr1;
myFunction3(t3); //example 3, T is MyType const volatile, ParamType is MyType const volatile&
myFunction4(ptr1); //example 4, T is MyType*, ParamType is MyType const*
```
In this case, reference-ness and pointer-ness is ignored, but cv-ness (`const` and `volatile`) is preserved for deducing `T`. Then, cv-ness of *ParamType* is applied based on the expression of *ParamType*. 

- Example 3, `T` is deduced to `MyType const volatile` by dropping `&` (reference-ness of) `t3`, then *ParamType* becomes `MyType const volatile&` as it's expression is `T&` in `myFunction3`.

- Example 4, `T` is deduced to `MyType` by dropping `*` (pointer-ness) of `ptr1`, then *ParamType* becomes `MyType const*` as it's expression is `const T*` in `myFunction3`.

This behavior all makes sense to me as we certainly want to preserve cv-ness of pointers or references, otherwise the cv-ness becomes vulnerable if not pointless.

### 3. ParamType is a **Universal Reference** (`T&&`).
**Universal reference** is a term in the book (*Effective Modern C++ - Scott Meyers*), and we use it to distinguish between (lvalue reference `MyType&`, rvalue `MyType&&`, and universal reference`T&&` or `auto&&`). A universal reference can be an **lvalue reference** or **rvalue** depending on other inputs.

```c++
template <typename T>
void myFunction5(T&& param); //T&& is an universal reference 

MyType t4;
const volatile MyType& t5 = t4;

myFunction5(t4); //example 5, T is MyType&, ParamType is MyType&
myFunction5(t5); //example 6, T is MyType const volatile&, ParamType is MyType const volatile&
myFunction5(move(t4)); //example 7, T is MyType, ParamType is MyType&&
myFunction5(233); //example 8, T is int, ParamType is int&&

myFunction5(move(t5)); //example 9, T is MyType const volatile, ParamType is MyType const volatile&&
```

This looks complicated to me at first, but all we need to figure out is what type is `expr` at call site `myFunction(expr)`.
- If `expr` is an lvalue, 
  - `T` is deduced to be the **lvalue reference type** with the same qualifiers (cv-ness).
  - *ParamType* is deduced to the same type as `T`.
    
    - Example 5, `T` is deduced to t5 `MyType`'s reference type, which is `MyType&`, then *ParamType* is the same.
    - Example 6, `T` is deduced to t5 `MyType const volatile&`'s reference type, which is itself `MyType const volatile&`, then *ParamType* is the same.
    > The result of reference to reference is explained by the behavior of [reference collapsing](https://en.cppreference.com/w/cpp/language/reference). 

- Otherwise - if `expr` is an rvalue, 
  - `T` is deduced to be the rvalue's type dropping the rvalue reference `&&`.
  - *ParamType* is deduced to the rvalue type by **re-adding `&&` to the type T deduced above**.
    
    - Example 7, `move(t4)`'s type is `MyType&&`. `T` is deduced to `MyType` by dropping `&&` then *ParamType* becomes `MyType&&` by re-adding `&&` to T.
    - Example 8, `233`'s type is `int&&`. `T` is deduced to `int` by dropping `&&`, then *ParamType* becomes `int&&` by re-adding `&&` to T.

Obviously, **example 9** is violating these rules, and it has to do with how c++ deal with moving const values specifically. After reading [this article](https://www.sandordargo.com/blog/2021/08/18/const-rvalue-references), I think that topic deserves it's own article, so I'm adding this topic as a `[todo]` to my backlog. 

`[todo]` Explain example 9.

## Conclusion
Now that we know how TTD works, we can figure out **auto type deduction** with the same logic.
Given code like this:

<span style="background-color: #FFFF00">[const/volatile qualifiers]</span>
`auto`<span style="background-color: #00BFFF">[reference-ness (&, &&) or pointer-ness (*)]</span>`t = expr`;

We can figure out `t`'s type by substituting `auto` with `T` and plug it into the template function below as if we are calling `myFunction` and try to figure out `ParamType`. 


`template <typename T>`

`void myFunction(`<span style="background-color: #FFFF00">[const/volatile qualifiers]</span> `T`<span style="background-color: #00BFFF">[reference-ness or pointer-ness]</span> `param);`
`myFunction(expr) //call site`

For example,
```c++
MyType t4;
const volatile MyType& t5 = t4;
auto&& t4_auto = t5;
```
`t4_auto`'s type is equivalent to `param`'s type as in this function below, which is `MyType const volatile &` 
```c++
// same as example 6
template <typename T>
void myFunction5(T&& param); 

myFunction5(t5);
```

## Notes
- This article serves as a study note for myself, and all the information is either a blatant copy or rephrasing of chapters in [*Effective Modern C++ by Scott Meyers*](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/). 
- Any suggestion/correction for this article is welcomed. Please make a PR here.