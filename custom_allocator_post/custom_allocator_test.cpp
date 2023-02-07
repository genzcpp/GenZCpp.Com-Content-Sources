#include <vector>
#include <iostream>
#include <typeinfo>

#include "../utils/ScopedTimer.hpp"
#include "ShortAlloc.hpp"
#include "MyStackResource.hpp"

#ifndef EACH_VECTOR_SIZE
#define EACH_VECTOR_SIZE 5
#endif
#pragma message "EACH_VECTOR_SIZE is: " STR(EACH_VECTOR_SIZE)

using namespace std;

using DataType = int;
constexpr size_t StackSize = MAX_STACK_SIZE;
constexpr int VectorSize = EACH_VECTOR_SIZE;
constexpr int TestDataSize = StackSize / sizeof(std::max_align_t) / VectorSize;
using ll = long long;

using Vector = std::vector<DataType>;

using ShortAllocVector = vector<DataType, ShortAlloc<DataType, StackSize>>;
using MyArenaType = ShortAllocVector::allocator_type::arena_type;

using MyPMRAllocator = std::pmr::polymorphic_allocator<DataType>;
using MyPMRVector = std::pmr::vector<DataType>;
using MyPMRVectorsAllocator = std::pmr::polymorphic_allocator<MyPMRVector>;

vector<ShortAllocVector> createManyShortAllocVectors(MyArenaType& allocator) {
    return vector<ShortAllocVector>(TestDataSize, ShortAllocVector(VectorSize, DataType{}, allocator));
}

std::pmr::vector<MyPMRVector> createManyPmrVectors(MyPMRVectorsAllocator& vecsAllocator, MyPMRAllocator& allocator) {
    return std::pmr::vector<MyPMRVector>(TestDataSize, MyPMRVector(VectorSize, DataType{}, allocator), vecsAllocator);
}

void createManyStdVectorsWithSource(vector<Vector>& vecs) {
    std::fill(vecs.begin(), vecs.end(), Vector(VectorSize, DataType{-1}));
}

auto doSomethingWithVectors(auto& vecs) {
    int i = 0;
    for (auto& v : vecs) {
        for (auto& e : v) {
            e = i++;
        }
    }
} 

constexpr int TestLoopCount = 10;
int main() {
    printf("vector count:%d, vector size:%d\n", TestDataSize, VectorSize);

    {
        ll sumInit = 0, sumIterate = 0, sumDestruct = 0;
        cout << "ShortAllocVector" << endl;
        for (int i = 0; i < TestLoopCount; i++) {
            ScopedTimer timer{false};
            {
                auto stack_arena = ShortAllocVector::allocator_type::arena_type{};
                auto vecs = createManyShortAllocVectors(stack_arena);
                sumInit += timer.elapsedMiliseconds();
                doSomethingWithVectors(vecs);
                sumIterate += timer.elapsedMiliseconds();
                //out-of-scope vecs destruction
            }
            sumDestruct += timer.elapsedMiliseconds();
            
        }
        printf("  average of %d loops:\n\tInit:{%lld}ms\n", TestLoopCount, sumInit / TestLoopCount);
        printf("  average of %d loops:\n\tIterate:{%lld}ms\n", TestLoopCount, sumIterate / TestLoopCount);
        printf("  average of %d loops:\n\tDestruct:{%lld}ms\n", TestLoopCount, sumDestruct / TestLoopCount);
    }

    {
        ll sumInit = 0, sumIterate = 0, sumDestruct = 0;
        cout << "MyPMRAllocator" << endl;
        for (int i = 0; i < TestLoopCount; i++) {

            ScopedTimer timer{false};
            {
                MyStackResource myStackResource{};
                MyPMRAllocator pmrStackAllocator{&myStackResource};
                MyPMRVectorsAllocator pmrVecsStackAllocator{&myStackResource};

                auto vecs = createManyPmrVectors(pmrVecsStackAllocator, pmrStackAllocator);
                sumInit += timer.elapsedMiliseconds();
                doSomethingWithVectors(vecs);
                sumIterate += timer.elapsedMiliseconds();
                //out-of-scope vecs destruction
            }
            sumDestruct += timer.elapsedMiliseconds();
            
        }
        printf("  average of %d loops:\n\tInit:{%lld}ms\n", TestLoopCount, sumInit / TestLoopCount);
        printf("  average of %d loops:\n\tIterate:{%lld}ms\n", TestLoopCount, sumIterate / TestLoopCount);
        printf("  average of %d loops:\n\tDestruct:{%lld}ms\n", TestLoopCount, sumDestruct / TestLoopCount);
    }


    {
        ll sumInit = 0, sumIterate = 0, sumDestruct = 0;
        cout << "std::vector" << endl;
        
        for (int i = 0; i < TestLoopCount; i++) {
            ScopedTimer timer{false};
            {
                vector<Vector> stdVecs; 
                stdVecs.resize(TestDataSize);
                createManyStdVectorsWithSource(stdVecs);
                sumInit += timer.elapsedMiliseconds();
                doSomethingWithVectors(stdVecs);
                sumIterate += timer.elapsedMiliseconds();
                //out-of-scope vecs destruction
            }
            sumDestruct += timer.elapsedMiliseconds();
        }
        printf("  average of %d loops:\n\tInit:{%lld}ms\n", TestLoopCount, sumInit / TestLoopCount);
        printf("  average of %d loops:\n\tIterate:{%lld}ms\n", TestLoopCount, sumIterate / TestLoopCount);
        printf("  average of %d loops:\n\tDestruct:{%lld}ms\n", TestLoopCount, sumDestruct / TestLoopCount);
    }
}