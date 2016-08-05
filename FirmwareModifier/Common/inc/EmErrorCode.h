#ifndef _h_EmErrorCode
#define _h_EmErrorCode

#define EM_ERR_OK 0//no error
#define EM_ERR_UNKNOWN -1//unknown error
#define EM_ERR_FILE -100 //for file
#define EM_ERR_NET -200//for network
#define EM_ERR_SYS -300//for system
#define EM_ERR_COM -400//for COM
#define EM_ERR_OBJ -500//for object
#define EM_ERR_STR -600//for string
#define EM_ERR_MEM -700//for memory
#define EM_ERR_RES -800//for resource
#define EM_ERR_GDIP -900//for gdip
#define EM_ERR_XML -1000//for xml
#define EM_ERR_WEB -1100//for web browser

#define EM_ERR_OBJ_NOT_READY          (EM_ERR_OBJ - 1)//not ready

#define EM_ERR_STR_EMPTY			(EM_ERR_STR - 1)//invalid format 
#define EM_ERR_STR_FORMAT          (EM_ERR_STR - 2)//invalid format 

#define EM_ERR_FILE_LOST          (EM_ERR_FILE - 1)//not found
#define EM_ERR_FILE_LOST_FILE     (EM_ERR_FILE - 2)//file not found
#define EM_ERR_FILE_LOST_DIR      (EM_ERR_FILE - 3)//dir not found
#define EM_ERR_FILE_EXISTED       (EM_ERR_FILE - 4)//existed
#define EM_ERR_FILE_EXISTED_FILE  (EM_ERR_FILE - 5)//existed directory
#define EM_ERR_FILE_EXISTED_DIR   (EM_ERR_FILE - 6)//existed file
#define EM_ERR_FILE_OPEN     (EM_ERR_FILE - 7)//open fail,can not create handle
#define EM_ERR_FILE_OPEN_FILE     (EM_ERR_FILE - 8)//open fail,can not create handle
#define EM_ERR_FILE_OPEN_DIR     (EM_ERR_FILE - 9)//open fail,can not create handle
#define EM_ERR_FILE_CREATE     (EM_ERR_FILE - 10)//create fail,
#define EM_ERR_FILE_CREATE_FILE     (EM_ERR_FILE - 11)//create file fail,
#define EM_ERR_FILE_CREATE_DIR     (EM_ERR_FILE - 12)//delete dir fail,
#define EM_ERR_FILE_DELETE     (EM_ERR_FILE - 13)//delete fail,
#define EM_ERR_FILE_DELETE_FILE     (EM_ERR_FILE - 14)//delete file fail,
#define EM_ERR_FILE_DELETE_DIR     (EM_ERR_FILE - 15)//delete directory fail,
#define EM_ERR_FILE_NAME     (EM_ERR_FILE - 16)//invalid name,
#define EM_ERR_FILE_MOVE     (EM_ERR_FILE - 17)//move fail,
#define EM_ERR_FILE_COPY     (EM_ERR_FILE - 18)//copy fail,
#define EM_ERR_FILE_LOCK     (EM_ERR_FILE - 19)//lock fail,
#define EM_ERR_FILE_RENAME     (EM_ERR_FILE - 20)//rename fail,
#define EM_ERR_FILE_FIND     (EM_ERR_FILE - 21)//find fail, for FindFirstFile() or FindNextFile()
#define EM_ERR_FILE_POS     (EM_ERR_FILE - 22)//invalid position in file
#define EM_ERR_FILE_EMPTY     (EM_ERR_FILE - 23)//file empty or lost


//#define EM_ERR_PTR_NULL          (EM_ERR_PTR - 1)//NULL POINTER

#define EM_ERR_COM_INIT          (EM_ERR_COM - 1)//init COM fail
#define EM_ERR_COM_CREATE          (EM_ERR_COM - 2)//create COM object fail

#define EM_ERR_NET_STARTUP          (EM_ERR_NET - 1)//startup winsock fail
#define EM_ERR_NET_CLEANUP          (EM_ERR_NET - 2)//cleanup winsock fail
#define EM_ERR_NET_CREATE          (EM_ERR_NET - 3)//create socket fail
#define EM_ERR_NET_CLOSE          (EM_ERR_NET - 4)//close socket fail
#define EM_ERR_NET_SHUTDOWN          (EM_ERR_NET - 5)//shutdown socket fail
#define EM_ERR_NET_NOT_STARTUP          (EM_ERR_NET - 6)//winsock not startup
#define EM_ERR_NET_INVALID_HOSTNAME          (EM_ERR_NET - 7)//invalid domain
#define EM_ERR_NET_BIND          (EM_ERR_NET - 8)//bind fail
#define EM_ERR_NET_LISTEN          (EM_ERR_NET - 9)//listen fail
#define EM_ERR_NET_ACCEPT          (EM_ERR_NET - 10)//accept fail
#define EM_ERR_NET_SEND          (EM_ERR_NET - 11)//send fail
#define EM_ERR_NET_RECV          (EM_ERR_NET - 12)//recv fail
#define EM_ERR_NET_SETOPT          (EM_ERR_NET - 13)//setopt fail
#define EM_ERR_NET_IOCTL          (EM_ERR_NET - 14)//ioctlsocket fail
#define EM_ERR_NET_TIMEOUT          (EM_ERR_NET - 15)//timeout 
#define EM_ERR_NET_CONNECT          (EM_ERR_NET - 16)//CONNECT 
#define EM_ERR_NET_INVALID_URL          (EM_ERR_NET - 17)//invalid url 
#define EM_ERR_NET_NO_RESPONSE          (EM_ERR_NET - 18)//no response
#define EM_ERR_NET_INVALID_RESPONSE          (EM_ERR_NET - 19)//invalid response
#define EM_ERR_NET_TIMEOUT_SEND          (EM_ERR_NET - 20)//timeout 
#define EM_ERR_NET_TIMEOUT_RECV          (EM_ERR_NET - 21)//timeout 
#define EM_ERR_NET_RECV_OVERFLOW          (EM_ERR_NET - 22)//recv overflow 

#define EM_ERR_SYS_CREATE_PIPE          (EM_ERR_SYS - 1)//create pipe fail 
#define EM_ERR_SYS_CREATE_PROCESS          (EM_ERR_SYS - 2)//create process fail 

#define EM_ERR_RES_FIND          (EM_ERR_RES - 1)//resource not found
#define EM_ERR_RES_LOAD          (EM_ERR_RES - 2)//resource not found

#define EM_ERR_MEM_ALLOC          (EM_ERR_MEM - 1)//alloc memory
#define EM_ERR_MEM_NULL          (EM_ERR_MEM - 2)//null pointer

#define EM_ERR_GDIP_LOAD_FILE          (EM_ERR_GDIP - 1)//load file fail
#define EM_ERR_GDIP_LOAD_HBITMAP          (EM_ERR_GDIP - 2)//load hbitmap fail
#define EM_ERR_GDIP_LOAD_RES          (EM_ERR_GDIP - 3)//load hbitmap fail


#define EM_ERR_XML_LOST_ROOT         (EM_ERR_XML - 1)//no root element
#define EM_ERR_XML_LOST_ELEMENT         (EM_ERR_XML - 2)//no  element
#define EM_ERR_XML_PARSE         (EM_ERR_XML - 3)//error format
#define EM_ERR_XML_EMPTY         (EM_ERR_XML - 4)//error format


#define EM_ERR_WEB_NO_DOC         (EM_ERR_WEB - 1)//no  html doc
#define EM_ERR_WEB_SCRIPT_ENGINE         (EM_ERR_WEB - 2)//no script
#define EM_ERR_WEB_SCRIPT_ID         (EM_ERR_WEB - 3)//no script
#define EM_ERR_WEB_SCRIPT_INVOKE       (EM_ERR_WEB - 4)//no script
#endif