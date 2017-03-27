/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/09/2016 06:19:41 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  YOUR NAME (wangzhen), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "DefaultAllocTemplate.h"
#include <vector>

void test()
{
    vector<void* > vec;
    vec.push_back(DefaultAllocTemplate<false, 0>::Allocate(129));
    vec.push_back(DefaultAllocTemplate<false, 0>::Allocate(28));
    vec.push_back(DefaultAllocTemplate<false, 0>::Allocate(16));
    vec.push_back(DefaultAllocTemplate<false, 0>::Allocate(96));
    DefaultAllocTemplate<false, 0>::Deallocate(vec.back(), 96); vec.pop_back();
    DefaultAllocTemplate<false, 0>::Deallocate(vec.back(), 16); vec.pop_back();
    DefaultAllocTemplate<false, 0>::Deallocate(vec.back(), 28); vec.pop_back();
    DefaultAllocTemplate<false, 0>::Deallocate(vec.back(), 129); vec.pop_back();
}
int main()
{
    test();
    return 0;
}

