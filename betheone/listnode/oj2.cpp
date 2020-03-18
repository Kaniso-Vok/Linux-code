//反转一个单链表
//需要用到三个节点：永远指向头结点、永远指向原先头结点、需要进行前置操作的结点
#include <stdio.h>
  struct ListNode {
    int val;
    ListNode *next;
   ListNode(int x) : val(x), next(NULL) {}
 };

class Solution {
public:
  ListNode* reverseList(ListNode* head) {
    ListNode* phead=head;
    if(head==nullptr){
      return head;
    }
    ListNode* cur=head->next;
    ListNode* tmp=head;
    while(cur){
      tmp->next=cur->next;
      cur->next=phead;
      phead=cur;
      cur=tmp->next;
    }
    return phead;
  }
};
