/*
 * =====================================================================================
 *
 *       Filename:  DefaultAllocTemplate.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/09/2016 05:36:22 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  YOUR NAME (wangzhen), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "MallocAllocTemplate.h"

enum { ALIGN = 8 };
enum { MAXBYTES = 128 };
enum { LISTNODES = MAXBYTES / ALIGN };


template<bool threads, int inst>
class DefaultAllocTemplate
{
private:
    union obj
    {
        union obj* _freeListLink;
        char _data[1];
    };
private:
    static union obj* volatile _freeList[LISTNODES];
    static char *_startFree;
    static char *_endFree;
    static size_t _heapSize;
protected:
    static size_t _RoundUp(size_t n)
    {
        return (n + (ALIGN - 1)) & ~(ALIGN - 1);
    }
    static size_t _FreelistIndex(size_t n)
    {
        return (n + (ALIGN -1)) / ALIGN - 1;
    }
    static void* _ChunkAlloc(size_t blockSize, int &nobjs)
    {
        void *res;
        size_t totalBytes = blockSize * nobjs;
        size_t leftBytes = _endFree - _startFree;
        if(leftBytes >= totalBytes)
        {
            res = (void*)_startFree;
            _startFree += totalBytes;
            return res;
        }
        else if(leftBytes >= blockSize)
        {
            nobjs = leftBytes / blockSize;
            res = (void*)_startFree;
            _startFree += nobjs * blockSize;
            return res;
        }
        else
        {
            size_t bytesToGet = totalBytes * 2 + (_RoundUp)(_heapSize >> 4);
            if(leftBytes > 0)
            {
                size_t index = _FreelistIndex(leftBytes);
                ((obj*)_startFree)->_freeListLink = _freeList[index];
                _freeList[index] = (obj*)_startFree;
            }
            _startFree = (char*)malloc(bytesToGet);
            if(_startFree == 0)
            {
                for(size_t size = blockSize; size <= (size_t)MAXBYTES; size += ALIGN)
                {
                    size_t index = _FreelistIndex(size);
                    if(_freeList[index])
                    {
                        obj *tmp = _freeList[index];
                        _freeList[index] = tmp->_freeListLink;
                        _startFree = (char*)tmp;
                        _endFree = _startFree + size;
                        return _ChunkAlloc(blockSize, nobjs);
                    }
                }
                _endFree = 0;
                _startFree = (char*)MallocAllocTemplate<0>::Allocate(bytesToGet);
            }
            _heapSize += bytesToGet;
            _endFree = _startFree + bytesToGet;
            return _ChunkAlloc(blockSize, nobjs);
        }
    }
    static void* _Refill(size_t blockSize)
    {
        int nobjs = 20;
        char *chunk;
        // obj *res;
        size_t index = _FreelistIndex(blockSize);
        obj *cur, *next;
        chunk = (char*)_ChunkAlloc(blockSize, nobjs);
        if(nobjs == 1) {
            return chunk;
        }
        _freeList[index] = next = (obj*)(chunk + blockSize);
        for(size_t i = 1; i < nobjs; i++)
        {
            cur = next;
            if(i < nobjs - 1) 
            {
                next = (obj*)(chunk + blockSize*(i+1));
                cur->_freeListLink = next;
            }
        }
        cur->_freeListLink = 0;
        return chunk;
    }
public:
    static void *Allocate(size_t size)
    {
        void *res;
        if(size > MAXBYTES)
        {
            res = MallocAllocTemplate<0>::Allocate(size);
            return res;
        }
        cout << "DefaultAllocTemplate<false, 0>::Allocate : " << size << endl;
        size_t index = _FreelistIndex(size);
        if(_freeList[index])
        {
            res = _freeList[index];
            _freeList[index] = ((obj*)res)->_freeListLink;
        }
        else
        {
            res = _Refill(_RoundUp(size));
        }
        return res;
    }
    static void Deallocate(void *ptr, size_t blockSize)
    {
       if(blockSize > MAXBYTES)
       {
           MallocAllocTemplate<0>::Deallocate(ptr, blockSize);
           return ;
       }
       //blockSize = _RoundUp(blockSize);
       cout << "DefaultAllocTemplate<false, 0>::Deallocate : " << blockSize << endl;
       size_t index = _FreelistIndex(blockSize);
       ((obj*)ptr)->_freeListLink = _freeList[index];
       _freeList[index] = (obj*)ptr;
    }
};
template<bool threads, int inst>
typename DefaultAllocTemplate<threads, inst>::obj* volatile DefaultAllocTemplate<threads, inst>::_freeList[LISTNODES] = {0};
template<bool threads, int inst>
char *DefaultAllocTemplate<threads, inst>::_startFree = 0;
template<bool threads, int inst>
char *DefaultAllocTemplate<threads, inst>::_endFree = 0;
template<bool threads, int inst>
size_t DefaultAllocTemplate<threads, inst>::_heapSize = 0;
