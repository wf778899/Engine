#pragma once

#include "Error_RegionsList.h"
#include "LogInfo.h"
#include "Types.h"

#include <stdint.h>
#include <vector>

#include "Utilites.hpp"

static const uint8_t g_minimum_capacity = 3; // �� �������

class RegionsList_Tester;

template<class T>
class RegionsList
{
public:
	friend class RegionsList_Tester;

	RegionsList( size_t capacity );

    ~RegionsList();

	Error_BasePtr ReleaseRegion( const RegionP<T> &region );

	Error_BasePtr GrabRegion( size_t size, T **out );

private:
	size_t m_p_list_size;
	size_t m_p_list_capacity;
	size_t m_p_list_spaceLeft;
	size_t m_p_list_spaceRight;
	RegionP<T> *m_p_list;
	RegionP<T> *m_p_list_begin;
	RegionP<T> *m_p_list_end;
	size_t m_s_list_size;
	size_t m_s_list_capacity;
	size_t m_s_list_spaceLeft;
	size_t m_s_list_spaceRight;
	RegionS<T> *m_s_list;
	RegionS<T> *m_s_list_begin;
	RegionS<T> *m_s_list_end;

	template<class ListType>
	void ExpandList();

	template<class ListType>
	void DelFrom_List( size_t index );								// ������� �� ������� ������� �� S- ��� P-List, � ������� ������ ������� �������� = 0

	void InserTo_S_List( const RegionS<T>& ins, size_t index );		// ��������� �� ������� ������� � S-List, � ������� ������ ������� �������� = 0
};




template<class T>
RegionsList<T>::RegionsList( size_t capacity )
	: m_p_list_size( 0 )
	, m_p_list_capacity( capacity )
	, m_p_list_spaceLeft( 0 )
	, m_p_list_spaceRight( 0 )
	, m_p_list( nullptr )
	, m_p_list_begin( nullptr )
	, m_p_list_end( nullptr )
	, m_s_list_size( 0 )
	, m_s_list_capacity( 0 )
	, m_s_list_spaceLeft( 0 )
	, m_s_list_spaceRight( 0 )
	, m_s_list( nullptr )
	, m_s_list_begin( nullptr )
	, m_s_list_end( nullptr )
{
	// �������� ������ ��� ������� ����������. ������� - ��� ������. ��������������
	if (m_p_list_capacity < g_minimum_capacity) {
		m_p_list_capacity = g_minimum_capacity;
	}
	m_s_list_capacity = m_p_list_capacity;

	m_p_list = reinterpret_cast<RegionP<T>*>(calloc( m_p_list_capacity, sizeof( RegionP<T> ) ));
	m_s_list = reinterpret_cast<RegionS<T>*>(calloc( m_s_list_capacity, sizeof( RegionS<T> ) ));
	memset( (void*)m_p_list, 0, m_p_list_capacity * sizeof( RegionP<T> ) );
	memset( (void*)m_s_list, 0, m_s_list_capacity * sizeof( RegionS<T> ) );

	// �������������� ������ ������/������� ���� � �������
	m_p_list_spaceLeft = m_p_list_capacity / 2;
	m_p_list_spaceRight = m_p_list_capacity - m_p_list_spaceLeft;
	m_s_list_spaceLeft = m_p_list_spaceLeft;
	m_s_list_spaceRight = m_p_list_spaceRight;

	// ������ ������� - � ��������� ���������� �������� (��� ���������� ��������� � ������ � ����� ��� �����������������)
	m_p_list_begin = m_p_list + m_p_list_spaceLeft;
	m_p_list_end = m_p_list_begin;
	m_s_list_begin = m_s_list + m_s_list_spaceLeft;
	m_s_list_end = m_s_list_begin;
}


template<class T>
RegionsList<T>::~RegionsList()
{
	if (m_p_list) {
		free( m_p_list );
	}
	if (m_s_list) {
		free( m_s_list );
	}
}


template<class T>
Error_BasePtr RegionsList<T>::ReleaseRegion( const RegionP<T>& region )
{
	// ���� ������ ����� - ������ ��������� � ��� ��������. ��� ���� ����������� ������������ ������.
	if (!m_p_list_size) {
		*m_p_list_begin = region;
		*m_s_list_begin = { region.start, region.size, 1 };
		++m_s_list_size;
		++m_p_list_size;
		--m_p_list_spaceRight;
		--m_s_list_spaceRight;
		++m_p_list_end;
		++m_s_list_end;
		return nullptr;
	}

	// ���� � ������� ������ ���� ��������
	if (m_p_list_size == 1)
	{											// �������� � Pointers-�������
		// ���� ����� ������ ��� ���� � ������ - ���������� ������
		if (region.start == m_p_list_begin->start) {
			return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_EXISTING_REG_INSERTION, (size_t)region.start, region.size, PLACE(), "Log.txt" );
		}
		// ���� �������� ������ (� �����)
		if (region.start > m_p_list_begin->start)
		{
			// ���� ������ ������������� � ����� - ������
			if (m_p_list_begin->start + m_p_list_begin->size > region.start) {
				return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION, (size_t)region.start, region.size, PLACE(), "Log.txt" );
			}
			// ������ �� ����� �����? ���� �� - �������� �� �����������, � �������������� ��������� (� ����� �������).
			if ((m_p_list_begin->start + m_p_list_begin->size) == region.start) {
				m_p_list_begin->size += region.size;
				m_s_list_begin->size = m_p_list_begin->size;
				return nullptr;
			}
			// ��������� ����� ������ ������. ����� ������ �����������, ����� ������ ���������� ������.
			*m_p_list_end++ = region;
			++m_p_list_size;
			--m_p_list_spaceRight;
		}
		// ���� �������� ����� (� ������)
		else
		{
			// ���� ������ ������������� � ������ - ������
			if (region.start + region.size > m_p_list_begin->start) {
				return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION, (size_t)region.start, region.size, PLACE(), "Log.txt" );
			}
			// ������ �� ������ �����? ���� �� - �������� �� �����������, � �������������� ��������� (� ����� �������).
			if (m_p_list_begin->start == region.start + region.size) {
				m_p_list_begin->start = region.start;
				m_p_list_begin->size += region.size;
				*m_s_list_begin = { m_p_list_begin->start, m_p_list_begin->size, 1 };
				return nullptr;
			}
			// ��������� ����� ������ �����. ����� ����� �����������, ������ ������ ���������� �����.
			*(--m_p_list_begin) = region;
			++m_p_list_size;
			--m_p_list_spaceLeft;
		}
												// �������� � Size-�������
		// ���� �������� ������
		if (region.size > m_s_list_begin->size) {
			*m_s_list_end++ = { region.start, region.size, 1 };
			--m_s_list_spaceRight;
		}
		// ���� �������� �����
		else if (region.size < m_s_list_begin->size) {
			*(--m_s_list_begin) = { region.start, region.size, 1 };
			--m_s_list_spaceLeft;
		}
		// ���� ������ ������������ ��������� ��������� � ������� ��� ���������� - ��������� �� �� ����������.
		else {
			if (region.start > m_s_list_begin->start) {
				m_s_list_begin->count = 2;
				*m_s_list_end++ = { region.start, region.size, 0 };
				--m_s_list_spaceRight;
			}
			else {
				m_s_list_begin->count = 0;
				*(--m_s_list_begin) = { region.start, region.size, 2 };
				--m_s_list_spaceLeft;
			}
		}
		++m_s_list_size;
	}
	// ���� ���������� �����
	else {
		RegionS<T> regToInsert = { 0 };
		RegionS<T> regsToDelete[2];
		uint8_t delCount = 0;
																/* �������� � Pointers-������� */
		// ���������� ������ �������
		bool founded;
		size_t index = utils::lower_bound( m_p_list_begin, m_p_list_size, region.start, founded );

		// ���� ����� ������ ��� ���� � ������ - ���������� ������
		if (founded) {
			return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_EXISTING_REG_INSERTION, (size_t)region.start, region.size, PLACE(), "Log.txt" );
		}

		// ���� ������� � ������
		if (index == 0)
		{
			// ���� ������ ������������� � ������ - ������
			if (region.start + region.size > m_p_list_begin->start) {
				return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION, (size_t)region.start, region.size, PLACE(), "Log.txt" );
			}
			// ������ �� ������ �����?
			if (region.start + region.size == m_p_list_begin->start) {
				delCount = 1;
				regsToDelete[0] = { m_p_list_begin->start, m_p_list_begin->size, 0 };
				m_p_list_begin->start = region.start;
				m_p_list_begin->size += region.size;
				regToInsert = { m_p_list_begin->start, m_p_list_begin->size, 0 };
			}
			else {
				// ����� ����� �� �������?
				if (!m_p_list_spaceLeft) {
					ExpandList<RegionP<T>>();
				}
				*(--m_p_list_begin) = region;
				--m_p_list_spaceLeft;
				++m_p_list_size;
				regToInsert = { region.start, region.size, 0 };
			}
		}
		// ���� ������� � �����
		else if (index == m_p_list_size)
		{
			RegionP<T>* lastReg = (m_p_list_end - 1);

			// ���� ������ ������������� � ����� - ������
			if (lastReg->start + lastReg->size > region.start) {
				return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION, (size_t)region.start, region.size, PLACE(), "Log.txt" );
			}
			// ������ �� ����� �����?
			if ((lastReg->start + lastReg->size) == region.start)
			{
				delCount = 1;
				regsToDelete[0] = { lastReg->start, lastReg->size, 0 };
				lastReg->size += region.size;
				regToInsert = { lastReg->start, lastReg->size, 0 };
			}
			else
			{
				// ����� ������ �� ������� ��� ������� 1 ��������?
				if (!m_p_list_spaceRight) {
					ExpandList<RegionP<T>>();
				}
				*m_p_list_end++ = region;
				--m_p_list_spaceRight;
				++m_p_list_size;
				regToInsert = { region.start, region.size, 0 };
			}
		}
		// ���� ������� � ��������
		else {
			RegionP<T>* right = m_p_list_begin + index;
			RegionP<T>* left = right - 1;

			// ���� ������ ������������� � ����� ��� ������ - ������
			if (((left->start + left->size) > region.start) || ((region.start + region.size) > right->start)) {
				return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION, (size_t)region.start, region.size, PLACE(), "Log.txt" );
			}

			// ���� ����� � ������ ������ - �������
			if (((left->start + left->size) == region.start) && ((region.start + region.size) == right->start))
			{
				delCount = 2;
				regsToDelete[0] = { left->start, left->size, 0 };
				regsToDelete[1] = { right->start, right->size, 0 };

				// ������ ������� ����� � ����� ������?
				if (index >= m_p_list_size / 2) {
					left->size += region.size + right->size;		// ������������ ������ ������ �� size
					regToInsert = { left->start, left->size, 0 };

					memmove( right, right + 1, (m_p_list_size - index - 1) * sizeof( RegionP<T> ) );	// �������� ��-��, ������� � ������+1, �� 1 ������� ����� (����������� ������� ������)
					++m_p_list_spaceRight;
					*(--m_p_list_end) = { 0 };
				}
				else {
					right->start = left->start;						// ������������ ������� ������ �� start � size
					right->size += left->size + region.size;
					regToInsert = { right->start, right->size, 0 };

					memmove( m_p_list_begin + 1, m_p_list_begin, (index - 1) * sizeof( RegionP<T> ) );	// �������� ��-��, ������� � Begin, �� 1 ���. ������ (����������� ������ ������)
					++m_p_list_spaceLeft;
					*m_p_list_begin++ = { 0 };
				}
				--m_p_list_size;
			}
			// ���� ������ ������ ����� - �������
			else if ((region.start + region.size) == right->start)
			{
				delCount = 1;
				regsToDelete[0] = { right->start, right->size, 0 };
				right->start = region.start;			// ������������ ������� ������ �� start � size
				right->size += region.size;
				regToInsert = { right->start, right->size, 0 };
			}
			// ���� ������ ����� ����� - �������
			else if ((left->start + left->size) == region.start)
			{
				delCount = 1;
				regsToDelete[0] = { left->start, left->size, 0 };
				left->size += region.size;				// ������������ ������ ������ �� size
				regToInsert = { left->start, left->size, 0 };
			}
			// ���� ��� ������� �������
			else
			{
				// ������ ������� ����� � ����� ������?
				if (index >= m_p_list_size / 2)
				{
					// ����� ������ �� ������� ��� ������� 1 ��������?
					if (m_p_list_spaceRight == 0) {
						ExpandList<RegionP<T>>();
						right = m_p_list_begin + index;		// right ����� ����������� ������ ���������. �������������� ���.
					}
					memmove( right + 1, right, (m_p_list_size - index) * sizeof( RegionP<T> ) );	// �������� ��-��, ������� � ������+1 �� 1 ���. ������ (����������� ����� ��� �������)

					*right = region;
					--m_p_list_spaceRight;
					++m_p_list_end;
					regToInsert = { region.start, region.size, 0 };
				}
				else {
					// ����� ����� �� ������� ��� ������� 1 ��������?
					if (m_p_list_spaceLeft == 0) {
						ExpandList<RegionP<T>>();
						right = m_p_list_begin + index;		// right � left ����� ����������� ������ ���������. �������������� ��.
						left = right - 1;
					}
					memmove( m_p_list_begin - 1, m_p_list_begin, (index + 1) * sizeof( RegionP<T> ) );	// �������� ��-��, ������� � Begin, �� 1 ���. ����� (����������� ����� ��� �������)

					*left = region;
					--m_p_list_spaceLeft;
					--m_p_list_begin;
					regToInsert = { region.start, region.size, 0 };
				}
				++m_p_list_size;
			}
		}
																	/* �������� � Size-������� */
		/*   ��������  ���������   */
		for (uint8_t i = 0; i < delCount; ++i)
		{
			// ���������� ������ ���������� ��������
			index = utils::lower_bound( m_s_list_begin, m_s_list_size, regsToDelete[i].size );

			// ������� � ����� size �� ������������?
			if ((m_s_list_begin + index)->count != 1)
			{
				size_t subIndex = utils::lower_bound( m_s_list_begin + index, (m_s_list_begin + index)->count, regsToDelete[i].start );

				// �������� ������?
				if (subIndex) {
					(m_s_list_begin + index)->count--;
					index += subIndex;
				}
				else {
					(m_s_list_begin + index + 1)->count = (m_s_list_begin + index)->count - 1;
				}
			}
			DelFrom_List<RegionS<T>>( index );
		}
		/*   ���������� ��������   */
		index = utils::lower_bound( m_s_list_begin, m_s_list_size, regToInsert.size );		// ���������� ������ �������

		// ������� � ����� size ��� ���� � ������?
		if (index != m_s_list_size && (m_s_list_begin + index)->size == regToInsert.size)
		{
			// ������� � ����� size ���� ������������ � ������?
			if ((m_s_list_begin + index)->count == 1)
			{
				// ������� ������?
				if (regToInsert.start > (m_s_list_begin + index)->start) {
					(m_s_list_begin + index)->count = 2;
					index += 1;
				}
				else {
					(m_s_list_begin + index)->count = 0;
					regToInsert.count = 2;
				}
			}
			else {
				size_t subindex = utils::lower_bound( m_s_list_begin + index, (m_s_list_begin + index)->count, regToInsert.start );

				// ������� ������?
				if (subindex) {
					(m_s_list_begin + index)->count++;
					index += subindex;
				}
				else {
					regToInsert.count = (m_s_list_begin + index)->count + 1;
					(m_s_list_begin + index)->count = 0;
				}
			}
		}
		else {
			regToInsert.count = 1;
		}
		InserTo_S_List( regToInsert, index );
	}
	return nullptr;
}


template<class T>
Error_BasePtr RegionsList<T>::GrabRegion( size_t size, T **out )
{
	// ���� S-List ������ - ������
	if (!m_s_list_size) {
		return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_GRAB_FROM_EMPTY_LIST, PLACE(), "Log.txt" );
	}
	// ���� � ������� 1 ������
	else if (m_s_list_size == 1)
	{
		// ���� ������ �������������� ������� ������ ���, ��� ��� - ������
		if (m_s_list_begin->size < size) {
			return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_CONSISTENT_REG_NOTFOUND, size, PLACE(), "Log.txt" );
		}
		// ���� ������ �������������� ������� ������ ���, ��� ���� - ������������ ������� � P- � S-List
		if (m_s_list_begin->size > size) {
			*out = m_s_list_begin->start;
			m_s_list_begin->start += size;
			m_s_list_begin->size -= size;
			m_p_list_begin->start = m_s_list_begin->start;
			m_p_list_begin->size = m_s_list_begin->size;
		}
		// ���� ������ �������������� ������� ����� ���, ��� ���� - ������� ������� �� P- � S-List
		else {
			*out = m_s_list_begin->start;
			*m_s_list_begin = { 0 };
			m_s_list_end--;
			m_s_list_size--;
			m_s_list_spaceRight++;
			*m_p_list_begin = { 0 };
			m_p_list_end--;
			m_p_list_size--;
			m_p_list_spaceRight++;
		}
		return nullptr;
	}
	// ���� � ������� ����� ��������
	else {
		T* toDel_or_Modify = nullptr;	// RegionP.start ��� �������� ��� ����������� � P-List
		T* newStart = nullptr;			// ����� .start
		size_t newSize;					// ����� .size
		bool mode = false;				// true = Modify, false = Delete (�� ���������)

		size_t index = utils::lower_bound( m_s_list_begin, m_s_list_size, size );

		// ���� ���������� ������ �� ������ - ������
		if (index == m_s_list_size)	{
			return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_CONSISTENT_REG_NOTFOUND, PLACE(), "Log.txt" );
		}
														/* �������� � S-List */
		RegionS<T> foundedRegion = *(m_s_list_begin + index);
		*out = foundedRegion.start;
			
		// �� ��������� ������ �� P-List ����� ����� (�� �������� .start ���������� � S-List �������)
		toDel_or_Modify = foundedRegion.start;

		// ���� ��������� ������ �� ������ ������ �������������� - ������ � P-List ����� ������������� (�� �����)
		if (foundedRegion.size > size) {
			mode = true;
			newStart = toDel_or_Modify + size;
			newSize = foundedRegion.size - size;
		}
		/*   ������� ��������� ������ �� S-List   */
		if (foundedRegion.count != 1) {
			(m_s_list_begin + index + 1)->count = foundedRegion.count - 1;
		}
		DelFrom_List<RegionS<T>>( index );
			
		if (mode) {
			/*   �������� � S-List ����� ������, � ���������� .start � .size   */
			RegionS<T> toInsert_in_SList = { newStart, newSize, 0 };
			index = utils::lower_bound( m_s_list_begin, m_s_list_size, toInsert_in_SList.size );

			// ������� � ����� size ��� ���� � ������?
			if ((m_s_list_begin + index)->size == toInsert_in_SList.size)
			{
				// ������� � ����� size ���� ������������ � ������?
				if ((m_s_list_begin + index)->count == 1)
				{
					// ������� ������?
					if (toInsert_in_SList.start > (m_s_list_begin + index)->start) {
						(m_s_list_begin + index)->count = 2;
						index += 1;
					}
					else {
						(m_s_list_begin + index)->count = 0;
						toInsert_in_SList.count = 2;
					}
				} else {
					size_t subindex = utils::lower_bound( m_s_list_begin + index, (m_s_list_begin + index)->count, toInsert_in_SList.start );
					
					// ������� ������?
					if (subindex) {
						(m_s_list_begin + index)->count++;
						index += subindex;
					}
					else {
						toInsert_in_SList.count = (m_s_list_begin + index)->count + 1;
						(m_s_list_begin + index)->count = 0;
					}
				}
			} else {
				toInsert_in_SList.count = 1;
			}
			InserTo_S_List( toInsert_in_SList, index );
		}
														/* �������� � P-List */
		bool founded;
		index = utils::lower_bound( m_p_list_begin, m_p_list_size, toDel_or_Modify, founded );

		// ���� ������� � ����� .start � P-List �� ������ - ������
		if (!founded) {
			return std::make_shared<Error_RegionsList>( RegList_ErrType::ERR_REG_WITH_SUCH_START_NOTFOUND, (size_t)toDel_or_Modify, 0, PLACE(), "Log.txt" );
		}
		// ��������� ������ � P-List ��������������?
		if (mode) {
			(m_p_list_begin + index)->start = newStart;
			(m_p_list_begin + index)->size = newSize;
			return nullptr;
		}
		DelFrom_List<RegionP<T>>( index );
		return nullptr;
	}
}


template<class T>
template<class ListType>
void RegionsList<T>::ExpandList()
{
	if constexpr (std::is_same_v<ListType, RegionP<T>>)
	{
		size_t prev_capacity = m_p_list_capacity;
		size_t prev_byStart_left = m_p_list_spaceLeft;
		m_p_list_capacity *= 2;
		size_t N = m_p_list_capacity - prev_capacity;

		m_p_list_spaceLeft = (m_p_list_spaceLeft + m_p_list_spaceRight + N) / 2;
		m_p_list_spaceRight = m_p_list_capacity - m_p_list_spaceLeft - m_p_list_size;

		m_p_list = reinterpret_cast<RegionP<T>*>(realloc( m_p_list, m_p_list_capacity * sizeof( RegionP<T> ) ));
		memmove( m_p_list + m_p_list_spaceLeft - prev_byStart_left, m_p_list, prev_capacity * sizeof( RegionP<T> ) );

		m_p_list_begin = m_p_list + m_p_list_spaceLeft;
		m_p_list_end = m_p_list_begin + m_p_list_size;
	}
	else if constexpr (std::is_same_v<ListType, RegionS<T>>)
	{
		size_t prev_capacity = m_s_list_capacity;
		size_t prev_bySize_left = m_s_list_spaceLeft;
		m_s_list_capacity *= 2;
		size_t N = m_s_list_capacity - prev_capacity;

		m_s_list_spaceLeft = (m_s_list_spaceLeft + m_s_list_spaceRight + N) / 2;
		m_s_list_spaceRight = m_s_list_capacity - m_s_list_spaceLeft - m_s_list_size;

		m_s_list = reinterpret_cast<RegionS<T>*>(realloc( m_s_list, m_s_list_capacity * sizeof( RegionS<T> ) ));
		memmove( m_s_list + m_s_list_spaceLeft - prev_bySize_left, m_s_list, prev_capacity * sizeof( RegionS<T> ) );

		m_s_list_begin = m_s_list + m_s_list_spaceLeft;
		m_s_list_end = m_s_list_begin + m_s_list_size;
	}
	else {
		return;
	}
}


template<class T>
template<class ListType>
void RegionsList<T>::DelFrom_List( size_t index )
{
	if constexpr (std::is_same_v<ListType, RegionS<T>>)
	{
		// �������� �� ������?
		if (index == 0) {
			*m_s_list_begin++ = { 0 };
			++m_s_list_spaceLeft;
		}
		// �������� �� �����?
		else if (index == m_s_list_size - 1) {
			*(--m_s_list_end) = { 0 };
			++m_s_list_spaceRight;
		}
		// �������� ����� � �����?
		else if (index >= m_s_list_size / 2) {
			memmove( m_s_list_begin + index, m_s_list_begin + index + 1, (m_s_list_size - index - 1) * sizeof( RegionS<T> ) );
			*(--m_s_list_end) = { 0 };
			++m_s_list_spaceRight;
		}
		// �������� ����� � ������?
		else {
			memmove( m_s_list_begin + 1, m_s_list_begin, index * sizeof( RegionS<T> ) );
			*m_s_list_begin++ = { 0 };
			++m_s_list_spaceLeft;
		}
		--m_s_list_size;
	}
	else if constexpr (std::is_same_v<ListType, RegionP<T>>)
	{
		// �������� �� ������?
		if (index == 0) {
			*m_p_list_begin++ = { 0 };
			++m_p_list_spaceLeft;
		}
		// �������� �� �����?
		else if (index == m_p_list_size - 1) {
			*(--m_p_list_end) = { 0 };
			++m_p_list_spaceRight;
		}
		// �������� ����� � �����?
		else if (index >= m_p_list_size / 2) {
			memmove( m_p_list_begin + index, m_p_list_begin + index + 1, (m_p_list_size - index - 1) * sizeof( RegionP<T> ) );
			*(--m_p_list_end) = { 0 };
			++m_p_list_spaceRight;
		}
		// �������� ����� � ������?
		else {
			memmove( m_p_list_begin + 1, m_p_list_begin, index * sizeof( RegionP<T> ) );
			*m_p_list_begin++ = { 0 };
			++m_p_list_spaceLeft;
		}
		--m_p_list_size;
	}
	else {
		return;
	}
}


template<class T>
void RegionsList<T>::InserTo_S_List( const RegionS<T>& ins, size_t index )
{
	// ������� � ������?
	if (index == 0) {
		if (!m_s_list_spaceLeft) {
			ExpandList<RegionS<T>>();
		}
		*(--m_s_list_begin) = ins;
		--m_s_list_spaceLeft;
	}
	// ������� � �����?
	else if (index == m_s_list_size) {
		if (!m_s_list_spaceRight) {
			ExpandList<RegionS<T>>();
		}
		*m_s_list_end++ = ins;
		--m_s_list_spaceRight;
	}
	// ������� ����� � �����?
	else if (index >= m_s_list_size / 2) {
		if (!m_s_list_spaceRight) {
			ExpandList<RegionS<T>>();
		}
		memmove( m_s_list_begin + index + 1, m_s_list_begin + index, (m_s_list_size - index) * sizeof( RegionS<T> ) );
		*(m_s_list_begin + index) = ins;
		++m_s_list_end;
		--m_s_list_spaceRight;
	}
	// ������� ����� � ������?
	else {
		if (!m_s_list_spaceLeft) {
			ExpandList<RegionS<T>>();
		}
		memmove( m_s_list_begin - 1, m_s_list_begin, index * sizeof( RegionS<T> ) );
		--m_s_list_begin;
		*(m_s_list_begin + index) = ins;
		--m_s_list_spaceLeft;
	}
	++m_s_list_size;
}