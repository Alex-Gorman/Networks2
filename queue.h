#ifndef __LIST_H__
#define __LIST_H__

#define MAX_LISTS 10
#define MAX_NODES 1000

/* BEFORE means the current pointer is before the first node
 * NORMAL means the current pointer is in the list (NOT NULL)
 * BEFORE means the current pointer is after the last node
 */
enum LIST_STATE {BEFORE=1, NORMAL=0, AFTER=2};

/**
 * NODE structure that saves item, pointer to next and pointer to prev
 * nodeSize is used to check memory usage
 */
typedef struct NODE NODE;
struct NODE {
		void *item;
		NODE *next;
    NODE *prev;
};

/**
 * LIST structure that saves pointer to current node, pointer to the first node
 * and pointer to last node
 * listSize is used to check memory usage
 * listCount is tracking the number of nodes in LIST
 */
typedef struct LIST LIST;
struct LIST
{
    NODE *currNodep;
    NODE *firstNodep;
    NODE *lastNodep;
    int listCount;
		enum LIST_STATE state;
};

/*
 * Create an empty list
 *
 * return: a pointer of the list
 */
LIST *ListCreate();

/*
 * get the number of nodes in the list

 * * parameters:
 *      list: a pointer to a LIST
 * return: The number of nodes in the list if success.
 */
int ListCount(LIST *list);

/*
 * get the first node in the list, and make first node the current one
 *
 * parameters:
 *      list: a pointer to a LIST
 * return: a pointer to a node.
 */
void *ListFirst(LIST *list);

/*
 * get the last node in the list, and make last node the current one
 *
 * parameters:
 *      list: a pointer to a LIST
 * return: a pointer to a node.
 */
void *ListLast(LIST *list);

/*
 * advances the list's current node by one,
 * and returns a pointer to the new current item.
 * If this operation attempts to advances the current
 * item beyond the end of the list, a NULL pointer is returned.
 *
 * parameters:
 *      list: a pointer to a LIST
 * return: a pointer to a node if success, NULL if faliure
 */
void *ListNext(LIST *list);

/*
 * backs up the list's current node by one,
 * and returns a pointer to the new current item.
 * If this operation attempts to back up the current
 * item beyond the start of the list, a NULL pointer is returned.
 *
 * parameters:
 *      list: a pointer to a LIST
 * return: a pointer to a node if success, NULL if faliure
 */
void *ListPrev(LIST *list);

/*
 *  returns a pointer to the current item in list.
 *
 *  parameters:
 *      list: a pointer to a LIST
 *  return: a pointer to a node if success, NULL if faliure
 */
void *ListCurr(LIST *list);

/*
 *  adds the new item to list directly after the current item,
 *  and makes the new item the current item.
 *  If the current pointer is at the end of the list,
 *  the item is added at the end.
 *
 *  parameters:
 *      list: a pointer to a LIST
 *      item: a pointer to any type of data will be stored in node.
 *  Returns: 0 on success, -1 on failure.
 */
int ListAdd(LIST *list,void *item);

/**
 *
 * adds item to list directly before the current item,
 * and makes the new item the current one.
 * If the current pointer is at the start of the list,
 * the item is added at the start.
 *
 * parameters:
 *      list: a pointer to a LIST
 *      item: a pointer to any type of data will be stored in node.
 * Returns: 0 on success, -1 on failure.
 */
int ListInsert(LIST *list, void *item);

/*
 *
 * adds item to the end of list, and makes the new item the current one.
 *
 * parameters:
 *      list: a pointer to a LIST
 *      item: a pointer to any type of data will be stored in node.
 * Returns: 0 on success, -1 on failure.
 */
int ListAppend(LIST *list, void *item);

/*
 * adds item to the front of list, and makes the new item the current one.
 * Returns: 0 on success, -1 on failure.
 *
 * parameters:
 *      list: a pointer to a LIST
 *      item: a pointer to any type of data will be stored in node.
 * Returns: 0 on success, -1 on failure.
 */
int ListPrepend(LIST *list, void *item);

/*
 * Return current item and take it out of list.
 * Make the next item the current one.
 *
 * parameters:
 *      list: a pointer to a LIST
 * Return: a pointer to a node or NULL
 */
void *ListRemove(LIST *list);

/*
 * adds list2 to the end of list1. The current pointer is set to
 * the current pointer of list1.
 *
 * parameters:
 *      list1: a pointer to a LIST
 *      list2: a pointer to a LIST will be merged
 * Return: none
 */
void ListConcat(LIST *list1, LIST *list2);

/*
 * delete list. itemFree is a pointer to a routine that frees an item.
 * parameters:
 *      list: a pointer to a list
 *      itemFree: a pointer to a function that free a item(node)
 * return: none
 */
void ListFree(LIST *list, void (*itemFree)(void*));

/*
 * Return last item and take it out of list.
 * The current pointer shall be the new last item in the list.
 *
 * parameters:
 *      list: a pointer to a list
 * return: none
 */
void *ListTrim(LIST *list);

/**
 * searches list starting at the current item until
 * the end is reached or a match is found.
 *
 * parameters:
 *      list: a pointer to a list
 *      comparator: a pointer to a function
 *      comparisionArg: a pointer to a item will be compared
 * Return: pointer of matched item is success, NULL on failure
 */
void *ListSearch(LIST *list, int (*comparator)(void *, void *)
, void * comparisonArg);

#endif
