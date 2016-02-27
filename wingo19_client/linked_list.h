/*
 * linked_list.h
 *
 * Created on: 2011/7/13
 * Author: chouhh
 */

#ifndef UCTCLIENT_0_LINKED_LIST_H_
#define UCTCLIENT_0_LINKED_LIST_H_

namespace CJCU
{

class INT_Node
{
public:
	int value;
	INT_Node* next;

	INT_Node();
};

class INT_LinkedList
{
public:
	INT_LinkedList();
	~INT_LinkedList();

	INT_Node* head;
	INT_Node node_array[121]; //�����إߤ@��Node Array�A�H�`��new�snode���ɶ�

	void insert(int v);
	INT_Node* get_head();
	void clear();
};

} //namespace CJCU

#endif /* UCTCLIENT_0_LINKED_LIST_H_ */
