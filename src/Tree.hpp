#pragma once

#include "Vec.hpp"

#include <cstdint>

class Node
{
public:
	Node(Vec2 move, Node* father = nullptr, Node* sibling = nullptr)
	{
		m_wins = 0;
		m_total = 0;
		m_move = move;

		m_child = nullptr;
		m_sibling = sibling;
		m_father = father;
	}

	// Payload
	std::uint64_t m_wins;
	std::uint64_t m_total;
	Vec2 m_move;

	// Tree linkage
	Node* m_child;
	Node* m_sibling;
	Node* m_father;
};

class Tree
{
public:
	Tree(const std::uint64_t);  // Constructor
	~Tree();                    // Destructor

	// Delete copy and move
	Tree(const Tree&) = delete;
	Tree(Tree&&) = delete;
	Tree& operator=(const Tree&) = delete;
	Tree& operator=(Tree&&) = delete;

	Node* fillFirstEmpty(const Node&);
	void eraseNode(Node*);
	void eraseTree(Node*);
	
	Node* m_root;
	Node* m_nodes;

private:
	std::uint64_t* m_table;
	std::uint64_t m_tableSize;
	std::uint64_t m_nodesSize;

	// Private functions
	void eraseSubTree(Node*);
};