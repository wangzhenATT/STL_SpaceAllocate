/*
 * =====================================================================================
 *
 *       Filename:  MallocAllocTemplate.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/09/2016 04:37:09 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  YOUR NAME (wangzhen), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
using namespace std;

template<int inst>
class MallocAllocTemplate
{
private:
    static void* OOMMalloc(size_t size);
    static void (* MallocAllocOOMHandler)();
public:
    static void* Allocate(size_t size)
    {
        cout << "MallocAllocTemplate<0>::Allocate : " << size << endl;
        void *res = malloc(size); 
        if(res == 0) res = OOMMalloc(size);
        return res;
    }
    static void* Deallocate(void* p, size_t /* n */)
    {    
        cout << "MallocAllocTemplate<0>::Deallocate : " << endl;
        free(p);
    }
    static void (* SetMallocHandler(void (* f)()))()
    {
        void (* old)() = MallocAllocOOMHandler;
        MallocAllocOOMHandler = f;
        return old;
    }
    
};
template<int inst>
void (* MallocAllocTemplate<inst>::MallocAllocOOMHandler)() = 0;
template<int inst>
void* MallocAllocTemplate<inst>::OOMMalloc(size_t size)
{
    void (* myMallocHandler)();
    void* res;
    while(1)
    {
        myMallocHandler = MallocAllocOOMHandler;
        if(myMallocHandler == 0) {
            throw "out of memory";
        }
        (* myMallocHandler)();
        res = malloc(size);
        if(res) return res;
    }
}
