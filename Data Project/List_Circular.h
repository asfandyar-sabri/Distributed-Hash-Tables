#pragma once

#include "AVL.h"
#include <sstream>
#include <string>
#include <stdio.h>

string toString(int id) {
	stringstream ss;
	ss << id;
	string s;
	ss >> s;
	return s;
}

template<class T>
struct list_node;
template<class T>
struct doubly_node;
template<class T>

class DoublyLinkedList
{
private:
	doubly_node<T>* head;
	int size;
public:
	DoublyLinkedList()
	{
		head = NULL;
		size = 0;
	}
	void insert(list_node<T>*&n)
	{
		if (head == NULL)
		{
			head = new doubly_node<T>;
			head->ptr = n;
			head->next = NULL;
			head->previous = NULL;
			size++;
		}
		else
		{
			doubly_node<T>* temp = head;
			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = new doubly_node<T>;
			temp->next->ptr = n;
			temp->next->previous = temp;
			temp = temp->next;
			temp->next = NULL;
			size++;
		}
	}
	list_node<T>*& get(int x) {
		int i = 0;
		doubly_node<T>* temp = head;
		while (i < x && temp != NULL) {
			temp = temp->next;
			i++;
		}
		if (temp != NULL)
			return temp->ptr;
	}
	void print() {
		cout << "Finger Table is: ";
		doubly_node<T>* temp = head;
		while (temp != NULL) {
			cout << temp->ptr->machine_id<<" ";
			temp = temp->next;
		}
		cout << endl;
	}
	void clear() {
		head = NULL;
	}
	~DoublyLinkedList()
	{
		delete head;
	}

};

template<class T>
struct list_node
{
	T machine_id;
	DoublyLinkedList<T> fingerTable;
	list_node<T>* next;
	list_node<T>* previous;
 	AVL tree;
};

template<class T>
struct doubly_node
{
	list_node<T>* ptr;
	doubly_node<T>* next;
	doubly_node<T>* previous;
};

template<class X>
class List
{
private:
	list_node<X>* head;
	int identifior_space;
public:

	List()
	{
		head = NULL;
		identifior_space = 0;
	}
	List(int is) {
		head = NULL;
		identifior_space = is;
	}
	void insert_at_start(X key) {
		if (head == NULL) {
			head = new list_node<X>;
			head->next = head;
			head->previous = head;
			head->machine_id = key;
			head->tree.createFile(toString(key) + ".txt");
		}
		else {
			list_node<X>* temp = new list_node<X>;
			list_node<X>* temp2 = head;
			while (temp2->next != head)
				temp2 = temp2->next;
			temp->machine_id = key;
			temp->next = head;
			head->previous = temp;
			temp->previous = temp2;
			head = temp;
			temp2->next = head;
			temp->tree.createFile(toString(key) + ".txt");
		}
	}
	void insert_at_middle(X key) {
		if (head == NULL)
			insert_at_start(key);
		else if (key < head->machine_id)
			insert_at_start(key);
		else {
			list_node<X>* temp = head;
			while (!(key > temp->machine_id && key < temp->next->machine_id) && temp->next!=head)
				temp = temp->next;
			list_node<X>* temp2 = new list_node<X>;
			temp2->previous = temp;
			temp2->next = temp->next;
			temp2->next->previous = temp2;
			temp->next = temp2;
			temp2->machine_id = key;
			temp2->tree.createFile(toString(key) + ".txt");
		}
	}
	
	void update_tables() {
		list_node<X>* temp = head;
		do {
			temp->fingerTable.clear();
			temp = temp->next;
		} while (temp != head);
		temp = head;
		bool flag = false;
		int successor;
		do{
			for (int i = 1; i <= identifior_space; i++) {
				successor = temp->machine_id + pow(2, i - 1);
				int tempvar;
				if (successor >= pow(2, identifior_space)) {
					tempvar = pow(2, identifior_space);
					successor = successor % tempvar;
				}
				flag = false;
				list_node<X>* temp2 = head;
				do {
					if (temp2->machine_id >= successor) {
						temp->fingerTable.insert(temp2);
						flag = true;
						break;
					}
					temp2 = temp2->next;
				} while (temp2 != head);
				if (!flag)
					temp->fingerTable.insert(head);
			}
			temp = temp->next;
		}while (temp!= head);
	}
	void display()
	{
		list_node<X>* temp = head;
		do
		{
			cout << temp->machine_id << endl;
			temp = temp->next;
		} while (temp!= head);
	}

	void Remove_machine(int id) {
		list_node<X>* temp = head;
		do {
			if (temp->next->machine_id == id) {
				temp->next->tree.check_avl_removal(temp->next->next, temp->next->tree.getroot());
				char cstr[100];
				string path = toString(temp->next->machine_id) + ".txt";
				strcpy(cstr, path.c_str());
				remove(cstr);
				temp->next = temp->next->next;
				update_tables();
				return;
			}
			temp = temp->next;
		} while (temp != head);
		cout << "Machine does not exist" << endl;
	}

	void Add_machine(int id) {
		list_node<X>* temp = head;
		do {
			if (temp->next->machine_id == id) {
				temp->next->next->tree.check_avl_insertion(temp->next, identifior_space, temp->next->next->tree.getroot());
				return;
			}
			temp = temp->next;
		} while (temp != head);
		cout << "Machine does not exist" << endl;
	}


	void print_AVL(int id) {
		list_node<X>* temp = head;
		do {
			if (temp->machine_id == id) {
				cout << "AVL tree for machine id " << temp->machine_id << " is: " << endl;
				temp->tree.inorder(temp->tree.getroot());
				return;
			}
			temp = temp->next;
		} while (temp != head);
		cout << "Machine does not exist" << endl;
	}
	
	void print_routing_table(int id) {
		list_node<X>* temp = head;
		do {
			if (temp->machine_id == id) {
				cout << "Routing Table for machine id " << temp->machine_id << " is: " << endl;
				temp->fingerTable.print();
				return;
			}
			temp = temp->next;
		} while (temp != head);
		cout << "Machine does not exist" << endl;
	}

	void delete_data(long long key) {
		int powered = pow(2, identifior_space);
		int mod_key = key % powered;
		list_node<X>* temp = head;
		do {
			cout << "At Machine: " << temp->machine_id << endl;
			bool flag = false;
			if (temp->machine_id >= mod_key) {
				temp->tree.update_root(temp->tree.Delete(key, temp->tree.getroot()));
				return;
			}
			for (int j = 0; j < identifior_space - 1; j++) {
				if (temp->machine_id < mod_key && mod_key <= temp->fingerTable.get(1)->machine_id) {
					temp = temp->fingerTable.get(1);
					flag = true;
				}
				else if (temp->fingerTable.get(j)->machine_id < mod_key && mod_key <= temp->fingerTable.get(j + 1)->machine_id) {
					temp = temp->fingerTable.get(j);
					flag = true;
				}
			}
			if (!flag)
			temp = temp->fingerTable.get(1);
		} while (temp != head);
		head->tree.update_root(head->tree.Delete(key, head->tree.getroot()));
	}

	void insert_to_AVL(long long k, string value)
	{
		int powered = pow(2, identifior_space);
		int mod_key = k % powered;
		list_node<X>* temp = head;
		do {
			cout << "At Machine: " << temp->machine_id << endl;
			if (temp->machine_id >= mod_key) {
				temp->tree.update_root(temp->tree.insert(k, value, temp->tree.getroot()));
				return;
			}
			bool flag = false;
			for (int j = 0; j < identifior_space-1; j++) {
				if (temp->machine_id < mod_key && mod_key <= temp->fingerTable.get(1)->machine_id) {
					temp = temp->fingerTable.get(1);
					flag = true;
				}
				else if (temp->fingerTable.get(j)->machine_id < mod_key && mod_key <= temp->fingerTable.get(j + 1)->machine_id) {
					temp = temp->fingerTable.get(j);
					flag = true;
				}
			}
			if (!flag)
				temp = temp->fingerTable.get(1);
		} while (temp != head);
		head->tree.update_root(head->tree.insert(k, value, head->tree.getroot()));

	}

	

};

