#include "queue.h"
#include <stdio.h>
#include <string.h>

/* Static allocation pool */
LIST lists[MAX_LISTS];
LIST *freeLists = &lists[0];

NODE nodes[MAX_NODES];
NODE *freeNodes = &nodes[0];

LIST* ListCreate(){
    static int initialized = 0;
    LIST *list = NULL;

    if (!initialized) {
        /* First time library initialization. */
        int i = 0;
        int j = 0;
        for (i = 0; i < MAX_LISTS - 1; i++) {
            /*
                For each entry, we re-interpret it as a LIST**,
                then store the address of the next free entry in that pointer.
                This gives us the effect of a very simple linked list
                of free entries stored within the entries themselves.
                This means no additional storage is necessary.
            */
            /* Reinterpret cast this LIST* as a LIST** */
            LIST **next = (LIST**)&lists[i];
            /* Dereference and store the next entry in the chain */
            *next = &lists[i+1];
        }
        for (j = 0; j < MAX_NODES - 1; j++) {
            /* Same as above. */
            NODE **next = (NODE**)&nodes[j];
            *next = &nodes[j+1];
        }
        initialized = 1;
    }

    if (freeLists == NULL) {
        /* Out of memory! */
        return NULL;
    }

    /*
     Pull a new list off the stack.
     Then set the next free entry to be the top of the stack.
    */
    list = freeLists;
    freeLists = *(LIST**)freeLists;
    memset(list, 0, sizeof(LIST));
    list->state = NORMAL;

    return list;
}

int ListPrepend(LIST *list, void *item){
    NODE *node = NULL;
    if (list == NULL){
        return -1;
    }else if (item == NULL)
    {
        return -1;
    }
    if (freeNodes == NULL) {
        /* Out of memory! */
        return -1;
    }
    list->state = NORMAL;
    /* Grab a new node */
    node = freeNodes;
    freeNodes = *(NODE**)freeNodes;
    memset(node, 0, sizeof(NODE));

    if (list->listCount == 0) {
        /* Empty list, just add the node simply. */
        list->currNodep = node;
        list->lastNodep = node;
        list->firstNodep = node;
    } else {
        node->next = list->firstNodep;
        list->firstNodep->prev = node;
        list->firstNodep = node;
        list->currNodep = node;
    }

    list->listCount += 1;
    node->item = item;

    return 0;
}

void *ListRemove(LIST *list){
    void *item;
    NODE *removeNode;
    if (list == NULL){
        printf("Error in procedure ListRemove(): Invalid parameter list \n");
        return NULL;
    }
    if (list->listCount == 0) {
        return NULL;
    }
    /* if not in the list (At BEFORE or AFTER) do nothing */
    if (list->state != NORMAL) {
      return NULL;
    }
    item = list->currNodep->item;
    removeNode  = list->currNodep;
    /* only one node */
    if (list->listCount==1) {
      *(NODE**)removeNode = freeNodes;
      freeNodes = removeNode;
      memset(list,0,sizeof(LIST));
    }
    /* when at last node */
    else if (list->currNodep == list->lastNodep) {
        list->currNodep = removeNode->prev;
        list->listCount--;
        list->lastNodep = list->currNodep;
        list->currNodep->next = NULL;
        *(NODE**)removeNode = freeNodes;
        freeNodes = removeNode;
    /* when at first node */
    }else if (list->currNodep == list->firstNodep){
        list->currNodep = removeNode->next;
        list->listCount--;
        list->firstNodep = list->currNodep;
        list->currNodep->prev = NULL;
        *(NODE**)removeNode = freeNodes;
        freeNodes = removeNode;
    /* when in middle */
    }else{
        list->currNodep = removeNode->next;
        list->listCount--;
        removeNode->next->prev = removeNode->prev;
        removeNode->prev->next = removeNode->next;

        *(NODE**)removeNode = freeNodes;
        freeNodes = removeNode;
    }
    return item;
}

void ListFree(LIST *list, void (*itemFree)(void*)){
    NODE *node = NULL;
    if (list == NULL){
        return;
   }else if (itemFree == NULL)
    {
        return;
    }

    /* Free all nodes and item data. */
    node = list->firstNodep;
    while (node) {
        NODE *nextNode = node->next;
        itemFree(node->item);

        /* Free the current node. */
        *(NODE**)node = freeNodes;
        freeNodes = node;

        node = nextNode;
    }

    /* Add the newly freed list entry to the front of the stack */
    *(LIST**)list = freeLists;
    freeLists = list;
}

void *ListFirst(LIST *list){
    if (list == NULL){
        return NULL;
    }
    if (list->listCount == 0) {
        return NULL;
    }
    list->state = NORMAL;
    list->currNodep = list->firstNodep;

    return list->currNodep->item;
}

void *ListNext(LIST *list){
    if (list == NULL){
        return NULL;
    }
    if (list->listCount == 0) {
        return NULL;
    }
    /* if in NULL pointer after the last one */
    if (list->state == AFTER) {
        return NULL;
    }
    /* if in NULL pointer BEFORE the first one
       set current node pointer to first node*/
    if (list->state == BEFORE) {
        list->currNodep = list->firstNodep;
        list->state = NORMAL;
        return list->firstNodep->item;
    }
    /* if at last node, set state to AFTER and return NULL*/
    if (list->currNodep->next == NULL) {
        list->state = AFTER;
        list->currNodep = NULL;
        return NULL;
    }

    list->currNodep = list->currNodep->next;
    return list->currNodep->item;
}

void *ListPrev(LIST *list){
    if (list == NULL){
        return NULL;
    }
    if (list->listCount == 0) {
        return NULL;
    }
    /* if in NULL pointer before the first one */
    if (list->state == BEFORE) {
        return NULL;
    }
    /* if in NULL pointer after the last one
       set current node pointer to last node*/
    if (list->state == AFTER) {
        list->currNodep = list->lastNodep;
        list->state = NORMAL;
        return list->lastNodep->item;
    }
    /* if at first node, set state to BEFORE and return NULL*/
    if (list->currNodep->prev == NULL) {
        list->state = BEFORE;
        list->currNodep = NULL;
        return NULL;
    }

    list->currNodep = list->currNodep->prev;
    return list->currNodep->item;
}
void *ListCurr(LIST *list){
    if (list == NULL){
        return NULL;
    }
    if (list->listCount == 0) {
        return NULL;
    }
    if (list->currNodep == NULL) {
        return NULL;
    }

    return list->currNodep->item;
}
void *ListTrim(LIST *list){
    void *item;
    NODE *removeNode;

    if (list == NULL){
        printf("Error in procedure ListTrim(): Invalid parameter list \n");
        return NULL;
    }
    if (list->listCount == 0) {
        return NULL;
    }

    item = list->lastNodep->item;
    removeNode = list->lastNodep;
    if (list->listCount == 1) {
      *(NODE**)removeNode = freeNodes;
      freeNodes = removeNode;
      memset(list,0,sizeof(LIST));
    }else{
      removeNode->prev->next = NULL;
      list->lastNodep = removeNode->prev;
      list->currNodep = list->lastNodep;
      *(NODE**)removeNode = freeNodes;
      freeNodes = removeNode;
      list->listCount --;
    }


    return item;

}

void *ListLast(LIST *list){
    if (list == NULL){
        return NULL;
    }
    if (list->listCount == 0) {
        return NULL;
    }
    list->state = NORMAL;
    list->currNodep = list->lastNodep;

    return list->currNodep->item;
}

/**
 *  These function just test parameters, no functional implemented
 *  All the comments or designs are in list.h and partC.design.txt
 */
int ListCount(LIST *list){
    if (list == NULL){
        return -1;
    }

    return list->listCount;
}
