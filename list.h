#ifndef _LIST_H
#define _LIST_H
#define _INLINE_ static inline

#define LIST_HEAD_INIT(name) {&(name), &(name)} 
#define LIST_HEAD(name)  struct list_head name = LIST_HEAD_INIT(name)
#define INIT_LIST_HEAD(ptr) do { (ptr)->next = (ptr);(ptr)->prev = (ptr);} while (0)
#define list_entry(ptr, type, member) ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
#define list_for_each(pos, head)  for (pos = (head)->next; pos != (head); pos = pos->next)
#define list_for_each_safe(pos, pnext, head)  for (pos = (head)->next, pnext = pos->next; pos != (head); pos = pnext, pnext = pos->next)

struct list_head {
    struct list_head *next, *prev;
};

_INLINE_ void __list_add(struct list_head *add,
        struct list_head *prev,
        struct list_head *next){
    next->prev = add;
    add->next = next;
    add->prev = prev;
    prev->next = add;
}

_INLINE_ void list_add(struct list_head *add, struct list_head *head){//头插法 
    __list_add(add, head, head->next);
}

_INLINE_ void list_add_tail(struct list_head *add, struct list_head *head){//在表尾添加节点
    __list_add(add, head->prev, head);
}

_INLINE_ int list_empty(struct list_head *head){//判断链表是否为空
    return head->next == head;
}
#undef _INLINE_
#endif
