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

	// ѕровер€ет вставку уже существующего региона в RegionsList
	void Test_DoubleInserion();

	// ѕровер€ет вставку региона, который перекрываетс€ с существующим(и) регионом(ами) в PList
	void Test_OverlappedInsertion();

	// ѕровер€ет захват, когда RegionnsList пуст
	void Test_GrabbingFromEmptyList();

	// ѕровер€ет захват региона, большего, чем есть в RegionsList
	void Test_GrabbingTooBigRegion();

	// ѕровер€ет работу RegionsList при расширении списков (ExpandList)
	void Test_ListsExpanding();

	// ѕровер€ет инициализирующую вставку в RegionsList (когда он только создан и ничего не содержит)
	void Test_InitialReleaseRegion();

	// ѕровер€ет вставку в RegionsList, смежную справа, потом слева, когда он содержит 1 регион
	void Test_SecondRelease_LeftRightAdj();

	// ѕровер€ет вставку меньшей ширины слева в RegionsList, когда он содержит 1 регион
	void Test_SecondRelease_Left_Less_Size();

	// ѕровер€ет вставку большей ширины слева в RegionsList, когда он содержит 1 регион
	void Test_SecondRelease_Left_Great_Size();

	// ѕровер€ет вставку равной ширины слева в RegionsList, когда он содержит 1 регион
	void Test_SecondRelease_Left_Equal_Size();

	// ѕровер€ет вставку меньшей ширины справа в RegionsList, когда он содержит 1 регион
	void Test_SecondRelease_Right_Less_Size();

	// ѕровер€ет вставку большей ширины справа в RegionsList, когда он содержит 1 регион
	void Test_SecondRelease_Right_Great_Size();

	// ѕровер€ет вставку равной ширины справа в RegionsList, когда он содержит 1 регион
	void Test_SecondRelease_Right_Equal_Size();

	// ѕровер€ет захват региона, когда в RegionsList один элемент
	void Test_GrabbingFromSingleSizedList();

	// ѕровер€ет захват из RegionsList регионов различной ширины при различных исходных состо€ни€х RegionsList
	void Test_GrabbingsComplex();

	// ѕровер€ет вставки в RegionsList (разной ширины, смежные и нет - во всЄм многообразии). »значально RegionsList хранит 5 равноудалЄнных регионов разной ширины
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


/* «аполн€етс€ парсером при обработке текста с набором тестов из "insertions_complex.txt" */
struct TestData
{
	/* »сходные и результирующие состо€ни€ P- и S-List */
	ListState p_listState_initial;
	ListState s_listState_initial;
	ListState p_listState_resulted;
	ListState s_listState_resulted;

	/* »сходный и результирующий контент P- и S-List */
	std::vector<Region_P> p_listContent_initial;
	std::vector<Region_S> s_listContent_initial;
	std::vector<Region_P> p_listContent_resulted;
	std::vector<Region_S> s_listContent_resulted;

	Region_P intermediate_reg;		// »спользуетс€ при тесте и вставок и удалений

	std::string to_String() const;
};