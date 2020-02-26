#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* strcasecmp */

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *tmp = q->head, *pre = NULL;
    while (tmp) {
        pre = tmp;
        tmp = tmp->next;
        if (pre->value)
            free(pre->value);
        free(pre);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (!newh)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s));
    *(newh->value + strlen(s)) = '\0';
    newh->next = q->head;
    q->head = newh;
    if (++(q->size) == 1) {
        q->tail = newh;
    }
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (!newh)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s));
    *(newh->value + strlen(s)) = '\0';
    newh->next = NULL;
    if (q->tail)
        q->tail->next = newh;
    q->tail = newh;
    if (++(q->size) == 1) {
        q->head = newh;
    }
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    if (sp && bufsize > 0 && q->head->value) {
        strncpy(sp, q->head->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    if (tmp->value) {
        free(tmp->value);
    }
    free(tmp);
    if (--(q->size) == 0) {
        q->head = q->tail = NULL;
    }
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    list_ele_t *pre = NULL, *cur = q->head, *nex = q->head->next;
    while (nex) {
        cur->next = pre;
        pre = cur;
        cur = nex;
        nex = cur->next;
    }
    cur->next = pre;
    cur = q->head;
    q->head = q->tail;
    q->tail = cur;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    // printf("size: %d\n", q->size);
    // list_ele_t *a = q->head;
    // int sum = 0;
    // puts("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    // for (int qq=0; a && qq<q->size; ++qq) {
    //     // printf("%s,  ", a->value);
    //     if (a->next && strcmp(a->value, a->next->value))    ++sum;
    //     a = a->next;
    // }
    // printf("SUM; %d\n", sum);

    for (int k = 1; k < q->size; k *= 2) {
        list_ele_t **cursor = &(q->head);  // storing point
        list_ele_t *point = q->head;       // processing point
        list_ele_t *left = point, *right = point;
        // printf("K = %d\n", k);
        int t = k;
        while (t && right) {
            --t;
            right = right->next;
        }
        point = right;
        t = k;
        while (t && point) {
            --t;
            point = point->next;
        }
        while (right) {
            int left_size = k;
            // puts("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
            // if (point)
            //     printf("Point: %s\n", point->value);
            // else printf("Point: NULL\n");
            // if (left)
            //     printf("Left : %s\n", left->value);
            // else printf("LEFT : NULL\n");
            // if (right)
            //     printf("Right: %s\n", right->value);
            // else printf("Right: NULL\n");
            while (left_size && right != point) {
                // if (str_natural_cmp(left->value, right->value)*
                // strcasecmp(left->value, right->value) < 0) {
                //     printf("%s vs. \n%s\n", left->value, right->value);
                //     printf("My: %d\n", str_natural_cmp(left->value,
                //     right->value));
                //     printf("judge: %d\n", strcasecmp(left->value,
                //     right->value));
                // }
                if (str_natural_cmp(left->value, right->value) >= 0) {
                    *cursor = right;
                    right = right->next;
                    cursor = &((*cursor)->next);
                } else {
                    *cursor = left;
                    left = left->next;
                    cursor = &((*cursor)->next);
                    --left_size;
                }
            }
            while (left_size) {
                *cursor = left;
                q->tail = left;
                left = left->next;
                cursor = &((*cursor)->next);
                --left_size;
            }
            while (right != point) {
                *cursor = right;
                q->tail = right;
                right = right->next;
                cursor = &((*cursor)->next);
            }
            *cursor = point;
            left = right = point;
            t = k;
            while (t && right) {
                --t;
                right = right->next;
            }
            point = right;
            t = k;
            while (t && point) {
                --t;
                point = point->next;
            }
            // a = q->head;
            // for (int qq=0; qq<q->size && a; ++qq) {
            //     printf("%s,  ", a->value);
            //     a = a->next;
            // }
            // if (q->tail)
            //     printf("tail: %s\n", q->tail->value);
            // else printf("tail: NULL\n");
            // if (point)
            //     printf("Point: %s\n", point->value);
            // else printf("Point: NULL\n");
            // if (left)
            //     printf("Left : %s\n", left->value);
            // else printf("LEFT : NULL\n");
            // if (right)
            //     printf("Right: %s\n", right->value);
            // else printf("Right: NULL\n");
        }
    }
}

/*
 * Compare two character strings according to their natural ordering.
 * Return value is greater than, equal to, or less than zero, accordingly
 * as the string pointed to by sl is greater than. equal to, or less than
 * the string pointed to by s2.
 */
int str_natural_cmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        // if (*s1 != *s2)  return (*s1 > *s2) * (1) + (*s1 < *s2) * (-1);
        ++s1, ++s2;
    }
    return (*s1 > *s2) * (1) + (*s1 < *s2) * (-1);
    // return ((*s1=='\0') * (-1) + (*s2=='\0') * (1));
    // cover (*s1=='\0' && *s2=='\0') which means 2 string are the same.
    // return strcasecmp(s1, s2);
}