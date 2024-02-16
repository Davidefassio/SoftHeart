/*
* SoftHeart, an ultimate-tic-tac-toe engine.
* Copyright (C) 2023-2024 Davide Fassio
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Tree.hpp"

#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <bit>
#include <vector>

namespace
{
	// two_powers[x] = 2^(63 - x)
	constexpr std::uint64_t two_powers[64] = {
		0x8000000000000000, 0x4000000000000000, 0x2000000000000000, 0x1000000000000000,
		0x0800000000000000, 0x0400000000000000, 0x0200000000000000, 0x0100000000000000,
		0x0080000000000000, 0x0040000000000000, 0x0020000000000000, 0x0010000000000000,
		0x0008000000000000, 0x0004000000000000, 0x0002000000000000, 0x0001000000000000,
		0x0000800000000000, 0x0000400000000000, 0x0000200000000000, 0x0000100000000000,
		0x0000080000000000, 0x0000040000000000, 0x0000020000000000, 0x0000010000000000,
		0x0000008000000000, 0x0000004000000000, 0x0000002000000000, 0x0000001000000000,
		0x0000000800000000, 0x0000000400000000, 0x0000000200000000, 0x0000000100000000,
		0x0000000080000000, 0x0000000040000000, 0x0000000020000000, 0x0000000010000000,
		0x0000000008000000, 0x0000000004000000, 0x0000000002000000, 0x0000000001000000,
		0x0000000000800000, 0x0000000000400000, 0x0000000000200000, 0x0000000000100000,
		0x0000000000080000, 0x0000000000040000, 0x0000000000020000, 0x0000000000010000,
		0x0000000000008000, 0x0000000000004000, 0x0000000000002000, 0x0000000000001000,
		0x0000000000000800, 0x0000000000000400, 0x0000000000000200, 0x0000000000000100,
		0x0000000000000080, 0x0000000000000040, 0x0000000000000020, 0x0000000000000010,
		0x0000000000000008, 0x0000000000000004, 0x0000000000000002, 0x0000000000000001
	};

	constexpr std::uint64_t SIZEOF_NODE = sizeof(Node);
	constexpr std::uint64_t TABLE_VIEW = 64 * SIZEOF_NODE;  // How many bytes a table manages
}

Tree::Tree(const std::uint64_t memory)
{
	// Round down table size 
	m_tableSize = memory / TABLE_VIEW;
	if (m_tableSize <= 0)
		throw std::runtime_error("Need more memory");

	// Allocate all the nodes space
	m_nodesSize = m_tableSize * 64;
	m_nodes = (Node*)malloc(m_nodesSize * SIZEOF_NODE);
	if (m_nodes == nullptr)
		throw std::runtime_error("Failed to allocate the arena");

	// Allocate the index table space
	m_table = (std::uint64_t*)calloc(m_tableSize, 64);
	if (m_table == nullptr)
		throw std::runtime_error("Failed to allocate the index table");
	
	// Setup the root (illegal move)
	m_lastEmptyTable = 0;
	fillFirstEmpty(Node(Vec2(-1, -1)));
	m_root = m_nodes;
}

Tree::~Tree()
{
	free(m_table);
	free(m_nodes);
}

Node* Tree::fillFirstEmpty(const Node& node)
{
	int n;
	for (std::uint64_t i = m_lastEmptyTable; i < m_tableSize; ++i)
	{
		if (m_table[i] != 0xFFFFFFFFFFFFFFFF)
		{
			m_lastEmptyTable = i;

			// If the compiler doesn't recognize std::countl_one replace with:
			// n = __builtin_clzll(~m_table[i]);
			n = std::countl_one(m_table[i]);

			m_table[i] |= two_powers[n];
			m_nodes[(i << 6) + n] = node;
			return &m_nodes[(i << 6) + n];
		}
	}
	m_lastEmptyTable = m_tableSize;
	return nullptr;
}

void Tree::clear()
{
	// Zero the index table (# bytes = 8 * m_tableSize = m_tableSize << 3)
	std::memset(m_table, 0, m_tableSize << 3);
	m_lastEmptyTable = 0;

	// Setup the root (illegal move)
	fillFirstEmpty(Node(Vec2(-1, -1)));
	m_root = m_nodes;
}

void Tree::eraseTree(Node* base)
{
	if (base == nullptr) return;

	eraseSubTree(base->m_child);
	eraseNode(base);

	m_lastEmptyTable = 0;
}

void Tree::eraseNode(Node* ptr)
{
	if (ptr < m_nodes) return;

	auto [iTable, nTable] = std::lldiv(ptr - m_nodes, 64);
	if (static_cast<std::uint64_t>(iTable) >= m_tableSize) return;

	m_table[iTable] &= (~two_powers[nTable]);
}

void Tree::eraseSubTree(Node* base)
{
	if (base == nullptr) return;

	eraseSubTree(base->m_child);
	eraseSubTree(base->m_sibling);
	eraseNode(base);
}