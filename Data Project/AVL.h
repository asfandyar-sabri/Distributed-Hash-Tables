#pragma once
#include<iostream>
#include<fstream>
#include<string>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;
template<class T>
struct list_node;
void removeLine(char* sourcefile, int line) {
	ifstream infile;
	char tempPath[100] = "tempfile.txt";
	infile.open(sourcefile, ios::in);
	ofstream outfile;
	outfile.open(tempPath, ios::out);
	char data[100];
	int i = 0;
	while (infile.getline(data, 100)) {
		i++;
		if (i == line)
			outfile << endl;
		else
			outfile << data << endl;
	}
	outfile.close();
	infile.close();
	remove(sourcefile);
	rename(tempPath, sourcefile);
}

struct AVLnode 
{
	long long key;
	int line_number;
	AVLnode* left;
	AVLnode* right;
};

class AVL 
{
private:
	string path;
	AVLnode* root;
public:
	AVL() 
	{
		root = NULL;
	}
	void createFile(string p) {
		path = p;
		ofstream file;
		file.open(p);
		file.close();
	}
	AVLnode*& getroot() 
	{
		return root;
	}
	AVLnode* insert(long long key,string value, AVLnode* temp) 
	{
		if (temp == NULL) 
		{
			int line = 0;
			ifstream file1;
			string temp1;
			file1.open(path, ios::out);
			while (!file1.eof()) {
				getline(file1, temp1);
				line++;
			}
			temp = new AVLnode;
			temp->left = NULL;
			temp->right = NULL;
			temp->key = key;
			temp->line_number = line;
			ofstream file;
			file.open(path, ios::app);
			file << value<<endl;
			file.close();
			return temp;
		}
		else if (key < temp->key)
			temp->left = insert(key, value, temp->left);
		else
			temp->right = insert(key, value, temp->right);

		int balanced = balance(temp);

		if (balanced > 1 && key < temp->left->key) 
		{
			return rightrotate(temp);
		}
		else if (balanced > 1 && key > temp->left->key) 
		{
			temp->left = leftrotate(temp->left);
			return rightrotate(temp);
		}
		else if (balanced < -1 && key > temp->right->key) 
		{
			return leftrotate(temp);
		}
		else if (balanced < -1 && key < temp->right->key) 
		{
			temp->right = rightrotate(temp->right);
			return leftrotate(temp);
		}
		return temp;
	}
	AVLnode* rightrotate(AVLnode* temp) 
	{
		AVLnode* leftchild = temp->left;
		AVLnode* rightofleftchild = leftchild->right;
		leftchild->right = temp;
		temp->left = rightofleftchild;
		return leftchild;
	}
	AVLnode* leftrotate(AVLnode* temp) 
	{
		AVLnode* rightchild = temp->right;
		AVLnode* leftofrightchild = rightchild->left;
		rightchild->left = temp;
		temp->right = leftofrightchild;
		return rightchild;
	}
	int balance(AVLnode* temp) 
	{
		if (temp == NULL)
			return 0;
		return getHeight(temp->left) - getHeight(temp->right);
	}
	int getHeight(AVLnode* temp)
	{
		if (temp == nullptr)
			return -1;
		else
		{
			if (getHeight(temp->left) > getHeight(temp->right))
				return getHeight(temp->left) + 1;
			else
				return getHeight(temp->right) + 1;
		}
	}
	void inorder(AVLnode* temp) 
	{
		if (temp == NULL)
			return;
		inorder(temp->left);
		cout << "\nFilePath is: " << path << endl;
		cout << "\nKey is: "<<temp->key<<endl;
		cout << "Line Number is: " << temp->line_number<<endl;
		ifstream file;
		string value;
		file.open(path, ios::out);
		for (int i = 0; i < temp->line_number; i++)
			getline(file, value);
		file.close();
		cout << "Value is: " << value << endl;
		inorder(temp->right);
	}
	template<class T>
	void check_avl_removal(list_node<T>* dest,AVLnode* temp) {
		if (temp == NULL)
			return;
		ifstream file;
		string value;
		file.open(path, ios::out);
		for (int i = 0; i < temp->line_number; i++)
			getline(file, value);
		file.close();
		dest->tree.update_root(dest->tree.insert(temp->key,value, dest->tree.getroot()));
		check_avl_removal(dest, temp->left);
		check_avl_removal(dest, temp->right);
	}
	template<class T>
	void check_avl_insertion(list_node<T>*& dest,int iS, AVLnode* temp) {
		if (temp == NULL)
			return;
		int previous_id = dest->previous->machine_id;
		int tempvar = previous_id + 1;
		int powered = pow(2, iS);
		int mod_key = temp->key % powered;
		bool flag = false;
		while (tempvar != dest->machine_id) {
			if (tempvar == mod_key)
				flag = true;
			tempvar++;
			if (tempvar == powered)
				tempvar = 0;
		}
		if (tempvar == dest->machine_id && tempvar == mod_key)
			flag = true;
		if (flag) {
			ifstream file;
			string value;
			file.open(path, ios::out);
			for (int i = 0; i < temp->line_number; i++)
				getline(file, value);
			file.close();
			dest->tree.update_root(dest->tree.insert(temp->key, value, dest->tree.getroot()));
			update_root(Delete(temp->key, root));
		}
		check_avl_insertion(dest, iS, temp->left);
		check_avl_insertion(dest, iS, temp->right);
	}
	void update_root(AVLnode * temp) {
		root = temp;
	}
	AVLnode* Delete(int key, AVLnode* temp) 
	{
		if (temp == NULL)
			return temp;
		if (key < temp->key)
			temp->left = Delete(key, temp->left);
		else if (key > temp->key)
			temp->right = Delete(key, temp->right);
		else {
			char cstr[100];
			strcpy(cstr, path.c_str());
			removeLine(cstr, temp->line_number);
			if (temp->right == NULL) {
				temp = temp->left;
			}
			else if (temp->left == NULL) {
				temp = temp->right;
			}
			else {
				AVLnode* temp2 = temp->right;
				while (temp2->left != NULL) {
					temp2 = temp2->left;
				}
				int tempvar = temp->key;
				temp->key = temp2->key;
				temp2->key = tempvar;
				temp->right = Delete(key, temp->right);
			}
		}
		if (temp == NULL)
			return temp;
		int balanced = balance(temp);
		if (balanced > 1 && balance(temp->left) >= 0) 
		{
			return rightrotate(temp);
		}
		else if (balanced > 1 && balance(temp->left) < 0) 
		{
			temp->left = leftrotate(temp->left);
			return rightrotate(temp);
		}
		else if (balanced < -1 && balance(temp->right) >= 0) 
		{
			temp->right = rightrotate(temp->right);
			return leftrotate(temp);
		}
		else if (balanced < -1 && balance(temp->right) <= 0) 
		{
			return leftrotate(temp);
		}
		return temp;
	}
};