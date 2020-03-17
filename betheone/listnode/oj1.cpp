//删除链表中等于给定值 val 的所有节点

#include <stdio.h>
struct ListNode {
   int val;
   ListNode *next;
   ListNode(int x) : val(x), next(NULL) {}
  };
        
class Solution {
  public:
    ListNode* removeElements(ListNode* head, int val) {
     ListNode* cur = new ListNode(0);
     cur->next=head;                
     ListNode* cur1=cur;                 
     while(cur1->next!=nullptr){
      if(cur1->next->val==val){                               
        ListNode* pur=cur1->next;                                                 
        cur1->next=pur->next;                                                     
        delete pur;
      }
      else
        cur1=cur1->next;
     }
     return cur->next;
    }
};
