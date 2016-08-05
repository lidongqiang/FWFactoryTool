#ifndef _h_EmMacros
#define _h_EmMacros

#define EM_RETURN_IF_NEGATIVE(iVar,xExpression) iVar = xExpression; if(iVar < 0) { return iVar; }

#define EM_DELETE_NULL(pObj) if(pObj != NULL){delete pObj; pObj = NULL;}

#define EM_FREE_NULL(pBuf) if(pBuf != NULL){free(pBuf); pBuf = NULL;}

#endif //#ifndef _EM_MACROS_H