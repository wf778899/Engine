#pragma once

#include "RegionsList.hpp"
#include "Globals.h"
#include "Error_Base.h"

#include <memory>
template<class T> class RegionsList;

using Region_P = RegionP<CELL>;
using Region_S = RegionS<CELL>;

struct TestData;

class RegionsList_Tester
{
public:
	RegionsList_Tester();

	// ��������� ������� ��� ������������� ������� � RegionsList
	void Test_DoubleInserion();

	// ��������� ������� �������, ������� ������������� � ������������(�) ��������(���) � PList
	void Test_OverlappedInsertion();

	// ��������� ������, ����� RegionnsList ����
	void Test_GrabbingFromEmptyList();

	// ��������� ������ �������, ��������, ��� ���� � RegionsList
	void Test_GrabbingTooBigRegion();

	// ��������� ������ RegionsList ��� ���������� ������� (ExpandList)
	void Test_ListsExpanding();

	// ��������� ���������������� ������� � RegionsList (����� �� ������ ������ � ������ �� ��������)
	void Test_InitialReleaseRegion();

	// ��������� ������� � RegionsList, ������� ������, ����� �����, ����� �� �������� 1 ������
	void Test_SecondRelease_LeftRightAdj();

	// ��������� ������� ������� ������ ����� � RegionsList, ����� �� �������� 1 ������
	void Test_SecondRelease_Left_Less_Size();

	// ��������� ������� ������� ������ ����� � RegionsList, ����� �� �������� 1 ������
	void Test_SecondRelease_Left_Great_Size();

	// ��������� ������� ������ ������ ����� � RegionsList, ����� �� �������� 1 ������
	void Test_SecondRelease_Left_Equal_Size();

	// ��������� ������� ������� ������ ������ � RegionsList, ����� �� �������� 1 ������
	void Test_SecondRelease_Right_Less_Size();

	// ��������� ������� ������� ������ ������ � RegionsList, ����� �� �������� 1 ������
	void Test_SecondRelease_Right_Great_Size();

	// ��������� ������� ������ ������ ������ � RegionsList, ����� �� �������� 1 ������
	void Test_SecondRelease_Right_Equal_Size();

	// ��������� ������ �������, ����� � RegionsList ���� �������
	void Test_GrabbingFromSingleSizedList();

	// ��������� ������ �� RegionsList �������� ��������� ������ ��� ��������� �������� ���������� RegionsList
	void Test_GrabbingsComplex();

	// ��������� ������� � RegionsList (������ ������, ������� � ��� - �� ��� ������������). ���������� RegionsList ������ 5 ������������� �������� ������ ������
	void Test_InsertionsComplex();

private:
	ListState Validate_ListState( const ListState& state, Error_BasePtr& err ) const;

    bool CheckState( const ListState &expected, const ListState &gained, Error_BasePtr &err ) const;

	template<class ListType>
	bool CheckContent( const std::vector<ListType>& expected, std::vector<ListType>& gained, Error_BasePtr& err ) const;

	template<class ListType>
	void CheckFootprintsVsState( const ListState& state, const ListFootprints& footpr, Error_BasePtr& err ) const;

	template<class ListType>
	void SetState( const ListState& state, Error_BasePtr& err );

	template<class ListType>
	void SetContent( const std::vector<ListType>& s_vec, Error_BasePtr& err );

	template<class ListType>
	ListState GetState() const;

	template<class ListType>
	ListFootprints GetFootprints() const;

	template<class ListType>
	std::vector<ListType> GetContent( Error_BasePtr& err );

	void GenerateInsertionsComplex();

	void GenerateGrabsComplex();

    TestData ParseInsertionComplex(const std::string &test_case, Error_BasePtr &err) const;

    ListState ParseListState( const std::string &list_state, Error_BasePtr &err) const;

	std::unique_ptr<RegionsList<CELL>> m_tested_regList;

	std::vector<std::string> m_insertions_complex;
	std::vector<TestData> m_grabbs_complex;
	size_t m_ins_complex_testCasesCount;
};


/* ����������� �������� ��� ��������� ������ � ������� ������ �� "insertions_complex.txt" */
struct TestData
{
	/* �������� � �������������� ��������� P- � S-List */
	ListState p_listState_initial;
	ListState s_listState_initial;
	ListState p_listState_resulted;
	ListState s_listState_resulted;

	/* �������� � �������������� ������� P- � S-List */
	std::vector<Region_P> p_listContent_initial;
	std::vector<Region_S> s_listContent_initial;
	std::vector<Region_P> p_listContent_resulted;
	std::vector<Region_S> s_listContent_resulted;

	Region_P intermediate_reg;		// ������������ ��� ����� � ������� � ��������

	std::string to_String() const;
};