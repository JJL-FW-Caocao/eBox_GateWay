#ifndef __LISTC_H
#define __LISTC_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "ebox_type.h"


typedef struct DdcNode
{
    unsigned char *p;
    struct DdcNode *next;
}DdcNode_t;  

// create a new node with a value
DdcNode_t* list_creat_node(unsigned char *p);
void list_insert(DdcNode_t *head, unsigned char *p);
void list_delete_by_val(DdcNode_t *head, unsigned int id);
void list_free(DdcNode_t *head);


#ifdef __cplusplus
}
#endif

#endif 
