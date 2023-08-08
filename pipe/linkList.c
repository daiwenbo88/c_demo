#include "linkList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


linkList listCreate()
{
    linkList H;
    H = (linkList)malloc(sizeof(listNode));
    if (NULL == H)
    {
        printf("malloc failed\n");
        return H;
    }
    H->data = 0;
    H->next = NULL;
    printf("malloc success\n");
    return H;
}

int list_tail_insert(linkList H, data_t value)
{
    linkList P;
    linkList q;
    if (NULL == H)
    {
        printf("H is empty\n");
        return -1;
    }

    P = (linkList)listCreate();
    if (NULL == P)
    {
        printf("malloc failed\n");
        return -1;
    }
    P->data = value;
    P->next = NULL;
    q = H;
    while (q->next != NULL)
    {
        q = q->next;
    }
    q->next = P;
    return 0;
}
linkList list_get_before(linkList H, int pos)
{
    if (NULL == H)
    {
        printf("H is empty");
        return NULL;
    }
    if (pos == -1)
    {
        printf("pos is invalid");
        return H;
    }
    linkList P;
    int i = -1;
    P = H;
    while (i < pos)
    {
        P = P->next;
        if (NULL == P)
        {
            printf("pos is invalid\n");
            return NULL;
        }
        i++;
    }

    return P;
}
int listEmpty(linkList H){
    if (NULL == H)
    {
        printf("H is empty");
        return 0;
    }
    if (H->next==NULL){
        return 0;
    }
    return 1;
}

linkList list_poo(linkList H){
    if (NULL == H)
    {
        printf("H is empty");
        return NULL;
    }
    linkList ret, P=H;
    while (P->next!=NULL)
    {
        ret = P;
        P = P->next;

        if (NULL == P->next)
        {
            ret->next=NULL;
            return P;
        }
    }
    return NULL;
}

int list_insert(linkList H, data_t value, int pos)
{
    if (NULL == H)
    {
        printf("H is empty\n");
        return -1;
    }
    linkList P;
    P = list_get_before(H, pos - 1);
    if (NULL == P)
    {
        printf("H before is empty\n");
        return -1;
    }

    linkList q;
    q = listCreate();
    if (NULL == q)
    {
        printf("malloc failed\n");
        return -1;
    }
    q->data = value;
    q->next = P->next;

    P->next = q;
    return 0;
}

int list_show(linkList H)
{
    if (NULL == H)
    {
        printf("H is empty\n");
        return -1;
    }
    linkList P;
    P = H->next;
    while (P->next != NULL)
    {
        printf("%d ", P->data);
        P = P->next;
    }
    printf("%d ", P->data);
    printf("\n");
    return 0;
}

int list_delete(linkList H, int pos)
{
    if (H == NULL)
    {
        printf("H is empty");
        return -1;
    }
    linkList P;
    linkList Q;
    P = list_get_before(H, pos - 1); // 找前面一个节点
    if (NULL == P)
    {
        return -1;
    }
    if (NULL == P->next) // 前面一个节点是最后一个节点情况
    {
        printf("delete pos is invalid\n");
        return -1;
    }

    Q = P->next;
    P->next = Q->next;
    free(Q);
    Q = NULL;
    return 0;
}

linkList list_free(linkList H)
{
    if (NULL == H)
    {
        return NULL;
    }

    linkList P;
    P = H;
    printf("free :");
    while (H != NULL)
    {
        P = H;
        printf("%d ", P->data);
        free(P);
        P = NULL;
        H = H->next;
    }
    puts("");
    return NULL;
}

/**
 * 链表反转
 */
int list_reverse(linkList H)
{
    if (NULL == H)
    {
        printf("H is NULL\n");
        return -1;
    }
    if (H->next == NULL || H->next->next == NULL)
    {
        return 0;
    }
    linkList P;
    linkList Q;
    P = H->next->next;
    H->next->next = NULL;
    while (NULL != P)
    {
        Q = P;
        P = P->next;

        Q->next = H->next;
        H->next = Q;
    }
    return 0;
}
linkList list_add_max(linkList H, data_t *value)
{
    linkList p, q, r;
    data_t sum;
    if (NULL == H)
    {
        printf("H is NULL\n");
        return NULL;
    }
    if (H->next == NULL || H->next->next == NULL || H->next->next->next == NULL)
    {
        return H;
    }
    q = H->next;
    p = H->next->next;
    r = q;
    sum = q->data + p->data;
    while (p->next != NULL)
    {
        p = p->next;
        q = q->next;
        if (sum < q->data + p->data)
        {
            sum = q->data + p->data;
            r = q;
        }
    }
    *value = sum;
    return r;
}

int list_merge(linkList H1, linkList H2)
{
    if (NULL == H1)
    {
        printf("H1 is empty\n");
        return -1;
    }
    if (NULL == H2)
    {
        printf("H2 is empty\n");
        return -1;
    }
    linkList p, q, r;
    p = H1->next;
    q = H2->next;
    r = H1;
    H1->next = NULL;
    H2->next = NULL;
    while (q && p)
    {
        if (p->data <= q->data)
        {
            r->next = p;
            p = p->next;
            r = r->next;
            r->next = NULL;
        }
        else
        {
            r->next = q;
            q = q->next;
            r = r->next;
            r->next = NULL;
        }
    }
    if (NULL == p)
    {
        r->next = q;
    }
    else
    {
        r->next = p;
    }

    return 0;
}