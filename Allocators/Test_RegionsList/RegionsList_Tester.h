#pragma once

#include "Error_Base.h"
#include "Globals.h"
#include "RegionsList.hpp"

#include <memory>


template<class T> class RegionsList;

using Region_P = RegionP<CELL>;
using Region_S = RegionS<CELL>;

struct TestData;

class RegionsList_Tester
{
public:
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
	void GenerateInsertionsComplex( std::vector<std::string>& out_insertions_str );

	void GenerateGrabsComplex( std::vector<TestData>& out_grabbs );
};


class rl_manip
{
public:
	template<class ListType>
	static void SetState( const ListState& state, std::shared_ptr<RegionsList<CELL>> regList, Error_BasePtr& err );

	template<class ListType>
	static void SetContent( const std::vector<ListType>& content, std::shared_ptr<RegionsList<CELL>> regList, Error_BasePtr& err );

	template<class ListType>
	static ListState GetState( std::shared_ptr<RegionsList<CELL>> regList );

	template<class ListType>
	static ListFootprints GetFootprints( std::shared_ptr<RegionsList<CELL>> regList );

	template<class ListType>
	static std::vector<ListType> GetContent( std::shared_ptr<RegionsList<CELL>> regList, Error_BasePtr& err );
};


class rl_check
{
public:
	static ListState Validate_ListState( const ListState& state, Error_BasePtr& err );

	static bool CheckState( const ListState& expected, const ListState& gained, Error_BasePtr& err );

	template<class ListType>
	static bool CheckContent( const std::vector<ListType>& expected, std::vector<ListType>& gained, Error_BasePtr& err );

	template<class ListType>
	static void CheckFootprintsVsState( const ListState& state, const ListFootprints& footpr, Error_BasePtr& err );
};


static TestData ParseInsertionComplex( const std::string& test_case, Error_BasePtr& err );

static ListState ParseListState( const std::string& list_state, Error_BasePtr& err );


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

	std::string to_String() const {
		return
			"P-State initial:  " + utils::to_string( p_listState_initial ) + "\nP-State resulted: " + utils::to_string( p_listState_resulted ) +
			"\nS-State initial:  " + utils::to_string( s_listState_initial ) + "\nS-State resulted: " + utils::to_string( s_listState_resulted ) +
			"\nP-Content initial:  " + utils::to_string( p_listContent_initial ) + "\nP-Content resulted: " + utils::to_string( p_listContent_resulted ) +
			"\nS-Content initial:  " + utils::to_string( s_listContent_initial ) + "\nS-Content resulted: " + utils::to_string( s_listContent_resulted ) +
			"\nIntermediate RegionP: " + utils::to_string( intermediate_reg );
	}
};