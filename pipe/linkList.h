typedef int data_t;

typedef struct Node
{
    data_t data;
    struct Node *next;
} listNode, *linkList;


linkList listCreate();
int list_tail_insert(linkList H,data_t value);
linkList list_get_before(linkList H,int pos);
int list_insert(linkList H,data_t value,int pos);
int list_show(linkList H);
int list_delete(linkList H,int pos);
linkList list_free(linkList H);

int list_reverse(linkList H);
linkList list_add_max(linkList H,data_t *value);
int list_merge(linkList H1,linkList H2);
linkList list_poo(linkList H);
int listEmpty(linkList H);
