#pragma once
#include <iostream>

template <typename T>
struct Node {
	T data;
	Node* next;
};

template <typename T>
class LinkedList {
private:
	Node<T>* head;
	int listsize = 0;
public:
	LinkedList() {
		head = nullptr;
	}
	int size() { return listsize; }

	// 在链表末尾插入元素
	void insert(T data) {
		listsize++;
		Node<T>* newNode = new Node<T>;
		newNode->data = data;
		newNode->next = nullptr;

		if (head == nullptr) {
			head = newNode;
		}
		else {
			Node<T>* current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = newNode;
		}
	}

	// 从大到小排序
	void sortDesc() {
		if (head == nullptr || head->next == nullptr) {
			return;
		}

		Node<T>* current = head;
		Node<T>* temp = nullptr;
		bool isSorted = false;

		while (!isSorted) {
			isSorted = true;
			current = head;

			while (current->next != nullptr) {
				if (current->data < current->next->data) {
					// 交换节点数据
					T tempData = current->data;
					current->data = current->next->data;
					current->next->data = tempData;
					isSorted = false;
				}
				current = current->next;
			}
		}
	}

	// 从小到大排序
	void sortAsc() {
		if (head == nullptr || head->next == nullptr) {
			return;
		}

		Node<T>* current = head;
		Node<T>* temp = nullptr;
		bool isSorted = false;

		while (!isSorted) {
			isSorted = true;
			current = head;

			while (current->next != nullptr) {
				if (current->data > current->next->data) {
					// 交换节点数据
					T tempData = current->data;
					current->data = current->next->data;
					current->next->data = tempData;
					isSorted = false;
				}
				current = current->next;
			}
		}
	}

	// 迭代器类
	class Iterator {
	private:
		Node<T>* current;

	public:
		Iterator(Node<T>* node) {
			current = node;
		}

		T& operator*() {
			return current->data;
		}

		Iterator& operator++() {
			current = current->next;
			return *this;
		}

		bool operator!=(const Iterator& other) {
			return current != other.current;
		}
	};

	// 迭代器的开始和结束方法
	Iterator begin() const {
		return Iterator(head);
	}

	Iterator end() const {
		return Iterator(nullptr);
	}


};