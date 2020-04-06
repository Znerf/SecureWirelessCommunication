/*   COMMON.H

*/

#ifndef COMMON_H
#define COMMON_H

/* some ASCII codes */
#define ESC	27 
#define BACKSPACE 8
#define TAB 9 


#define START_PAGE 32
#define MAX_FILES (112-START_PAGE)
#define START_PAGE_ADDR (START_PAGE*SPM_PAGESIZE)
#define MAX_FILEID MAX_FILES

typedef enum RESPONSE { NO,YES,CANCEL} RESPONSE;
typedef enum ERRORCODE { SUCCESS, ERR_ABORT, ERR_INVALID, ERR_OVERFLOW, 
						 ERR_FILEID, ERR_EMPTYFILE} ERRORCODE;


#endif