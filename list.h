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
 *  returns a pointer to the current item in list.
 *
 *  parameters:
 *      list: a pointer to a LIST
 *  return: a pointer to a node if success, NULL if faliure
 */
void *ListCurr(LIST *list);

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
 * delete list. itemFree is a pointer to a routine that frees an item.
 * parameters:
 *      list: a pointer to a list
 *      itemFree: a pointer to a function that free a item(node)
 * return: none
 */
void ListFree(LIST *list, void (*itemFree)(void*));

void *ListPrev(LIST *list);
void *ListNext(LIST *list);
void *ListFirst(LIST *list);
void *ListTrim(LIST *list);
void *ListLast(LIST *list);

int ListCount(LIST *list);

#endif
