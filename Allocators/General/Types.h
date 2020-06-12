#pragma once

#include <stdint.h>
#include <regex>

/* ������� P-List */
template<class T> 
struct RegionP {
	T *start = nullptr;     // ������ �������
	size_t size = 0;		// ������ �������, ����
};


/* ������� S-List */
template<class T>
struct RegionS {
	T *start = nullptr;     // ������ �������
	size_t size = 0;		// ������ �������, ����
	size_t count = 0;		// ���������� �������� � ����� "size" � Size-������
};


/* ����������� ����� (������/�����). ������������ ��� �������� ������� ������� ��� �������� */
enum Side
{
	Side_NONE,
	Side_LEFT,
	Side_RIGHT
};


/* ���� ��������� */
enum class Adjacency {
	Adj_NONE,
	Adj_Left,
	Adj_Right,
	Adj_Both
};


/* �������� ��������� ������������ ������ */
struct ListFootprints
{
	size_t addr_start;		// ������ ������
	size_t addr_stop;		// ����� ������
	size_t addr_begin;		// ������ ������������������ ��������� � ������
	size_t addr_end;		// ����� ������������������ ��������� � ������
};


/* ��������� ������ */
struct ListState
{
	size_t capacity;		// ������� ������
	size_t size;			// ����� ��������� � ������
	size_t spaceLeft;		// ������ ������� ���� (���������)
	size_t spaceRight;		// ������ ������ ���� (���������)
	size_t begin_pos;		// ������� Begin() ������ (���������)
	size_t end_pos;			// ������� End() ������ (���������)

	bool operator==( const ListState& r ) const {
		return (this->capacity == r.capacity && this->size == r.size && this->spaceLeft == r.spaceLeft && this->spaceRight == r.spaceRight && this->begin_pos == r.begin_pos && this->end_pos == r.end_pos);
	}
	bool operator!=( const ListState& r ) const {
		return !(*this == r);
	}
};


/* ��������� ������ ��� std::regex */
struct Regex
{
    Regex(std::string expr) 
        : expr(expr)
        , str(expr)
    {}

    std::regex expr;
    std::string str;
};