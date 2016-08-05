#ifndef _h_EmMemory
#define _h_EmMemory

/*
****************************************

#define EM_DEBUG
to use the functions,please enable EM_DEBUG

usage:
//c style alloc
char *szBuffer = (char*)EM_ALLOC(szBuffer,10);

//c style free
EM_FREE(szBuffer);

//c++ style new
MyClass *pMyClass;
EM_NEW(pMyClass,new MyClass(yourParameter));

//c++ style delete
EM_DELETE(pMyClass);

//c++style new array
char *szBuffer;
EM_NEW(szBuffer,new char[10]);

//c++ style delete array
EM_DELETE_ARR(szBuffer);

****************************************
*/



void* em_alloc(void **ppAddr,int iCount, char* szFile, int iLine);
void em_free(void **ppAddr);

void em_new(void **ppAddr, char* szFile, int iLine);
void em_delete(void **ppAddr);

#ifdef EM_DEBUG

#define EM_ALLOC(pAddr,iCount) em_alloc((void**)&pAddr,iCount,__FILE__,__LINE__)
#define EM_FREE(pAddr) em_free((void**)&pAddr)
#define EM_NEW(pAddr , newConstruction) pAddr = newConstruction; em_new((void**)&pAddr,__FILE__,__LINE__);
#define EM_DELETE(pAddr) if(pAddr != NULL){ delete pAddr; em_delete((void**)&pAddr);}   
#define EM_DELETE_ARR(pAddr) if(pAddr != NULL){ delete[] pAddr; em_delete((void**)&pAddr);} 
#define EM_DELETE_TYPE(typeName,pAddr) if(pAddr != NULL){ delete (typeName*)pAddr; em_delete((void**)&pAddr);}   
 
#else //#ifdef EM_DEBUG

#define EM_ALLOC(pAddr,iCount) malloc(iCount)
#define EM_FREE(pAddr) em_free((void**)&pAddr)
#define EM_NEW(pAddr , newConstruction) pAddr = newConstruction;
#define EM_DELETE(pAddr) if(pAddr != NULL){ delete pAddr; pAddr = NULL;}   
#define EM_DELETE_ARR(pAddr) if(pAddr != NULL){ delete[] pAddr; pAddr = NULL;} 
#define EM_DELETE_TYPE(typeName,pAddr) if(pAddr != NULL){ delete (typeName*)pAddr; pAddr = NULL;} 

#endif 

#endif //#ifndef _EM_MEMORY_H