#include "RegionsList_Tester.h"
#include "Error_Custom.h"
#include "Error_RegionsList.h"
#include "Globals.h"
#include "LogInfo.h"
#include "LogError.h"
#include "LogDebug.h"
#include "LogTest.h"

#include <array>
#include <iostream>
#include <fstream>
#include <ppl.h>
#include <ppltasks.h>
#include <string>
#include <time.h>
#include <typeinfo>


using Ins_complex =    Globals::Ins_complex;
using InsertionPlace = Globals::Ins_complex::InsertionPlace;
using FieldState =     Globals::Ins_complex::FieldState;

using namespace std::filesystem;


// ��������� ������� ��� ������������� ������� � RegionsList
void RegionsList_Tester::Test_DoubleInserion()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Existing Region Insertions", 1 };

	CELL* memoryPitch = new CELL[4000];
	auto regList = std::make_shared<RegionsList<CELL>>( 0 );

	// �������� �������
	Region_P initial_region = { memoryPitch + 100, 100 };
	Region_P second_region  = { memoryPitch + 300, 100 };

	do {
		// ������ ���������������� �������
		Error_BasePtr err = regList->ReleaseRegion( initial_region );                                                                                                           TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained during RegionsList initial insertion" );

		// �������� ����������� ������������ ������, ������ �� �������
		std::shared_ptr<Error_RegionsList> errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( initial_region ));

		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_EXISTING_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (existing region insertion after initial insertion)", PLACE(), "Log.txt" );               TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		// ������ ������ �������
		err = regList->ReleaseRegion( second_region );                                                                                                                          TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained during RegionsList second insertion" );

		// �������� ����������� ������������ ������, ������ �� �������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( second_region ));

		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_EXISTING_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (existing region insertion after second insertion)", PLACE(), "Log.txt" );                TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}

		delete[] memoryPitch;
		Log::test( "Log.txt" ) << LogTest::Finished{ true };
		return;
	} while (0);

	delete[] memoryPitch;
	Log::test( "Log.txt" ) << LogTest::Finished{ false };
	system( "Pause" );
}


// ��������� ������� �������, ������� ������������� � ������������(�) ��������(���) � PList
void RegionsList_Tester::Test_OverlappedInsertion()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Overlapping Region Insertions", 1 };

	CELL* memoryPitch = new CELL[4000];
	auto regList = std::make_shared<RegionsList<CELL>>( 0 );

	// �������� �������
	Region_P initial_region              = { memoryPitch + 100, 50 };
	Region_P left_overlapped             = { memoryPitch + 50,  51 };
	Region_P right_overlapped            = { memoryPitch + 149, 50 };
	Region_P medium_left_overlapped      = { memoryPitch + 149, 50 };
	Region_P medium_left_overlapped_adj  = { memoryPitch + 149, 51 };
	Region_P medium_right_overlapped     = { memoryPitch + 151, 50 };
	Region_P medium_right_overlapped_adj = { memoryPitch + 150, 51 };
	Region_P medium_left_right           = { memoryPitch + 149, 52 };

	do {
		// ������ ���������������� �������
		Error_BasePtr err = regList->ReleaseRegion( initial_region );                                                                                                           TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained during RegionsList initial insertion" );

		// �������� ����������� ��������������� ����� ������, ������ �� �������
		std::shared_ptr<Error_RegionsList> errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( left_overlapped ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (left overlapped region insertion after initial insertion)", PLACE(), "Log.txt" );        TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}

		// �������� ����������� ��������������� ������ ������, ������ �� �������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( right_overlapped ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (right overlapped region insertion after initial insertion)", PLACE(), "Log.txt" );       TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}

		// ������ ������ �������
		Region_P second_region = { memoryPitch + 200, 50 };
		err = regList->ReleaseRegion( second_region );                                                                                                                          TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained during RegionsList second insertion" );
		right_overlapped = { memoryPitch + 249, 50 };

		// �������� ����������� ���������� ��������������� ����� ������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( medium_left_overlapped ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (medium left overlapped region insertion after second insertion)", PLACE(), "Log.txt" );  TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		// �������� ����������� ���������� ��������������� ����� � ������� ������ ������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( medium_left_overlapped_adj ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (medium left overlapped right-adjacent region insertion after second insertion)", PLACE(), "Log.txt" );   TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		// �������� ����������� ���������� ��������������� ������ ������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( medium_right_overlapped ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (medium right overlapped region insertion after second insertion)", PLACE(), "Log.txt" ); TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		// �������� ����������� ���������� ��������������� ������ � ������� ����� ������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( medium_right_overlapped_adj ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (medium right overlapped left-adjacent region insertion after second insertion)", PLACE(), "Log.txt" );   TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		// �������� ����������� ���������� ��������������� ����� � ������ ������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( medium_left_right ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (medium right and left overlapped region insertion after second insertion)", PLACE(), "Log.txt" );        TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		// �������� ����������� ������� ����� ��������������� ������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( left_overlapped ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (left overlapped region insertion after second insertion)", PLACE(), "Log.txt" );         TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		// �������� ����������� ������� ������ ��������������� ������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->ReleaseRegion( right_overlapped ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_OVERLAPPED_REG_INSERTION) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (right overlapped region insertion after second insertion)", PLACE(), "Log.txt" );        TRACE_PRINT_CONTINUE( err, "ReleaseRegion() Wrong behavior" );
		}
		delete[] memoryPitch;
		Log::test( "Log.txt" ) << LogTest::Finished{ true };
		return;
	} while (0);

	delete[] memoryPitch;
	Log::test( "Log.txt" ) << LogTest::Finished{ false };
	system( "Pause" );
}


// ��������� ������, ����� RegionnsList ����
void RegionsList_Tester::Test_GrabbingFromEmptyList()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Grabbing From Empty List", 1 };

	Error_BasePtr err = nullptr;
	CELL* marker = new CELL;
	auto regList = std::make_shared<RegionsList<CELL>>( 0 );

	do {
		// �������� ��������� � ��������� ������� ��, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList creation" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList creation" );
		rl_check::CheckState( { 3, 0, 1, 2, 1, 1 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList creation" );
		rl_check::CheckState( { 3, 0, 1, 2, 1, 1 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList creation" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after RegionsList creation" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after RegionsList creation" );

		// �������� ��������� ������ �� ������� ������, ������ �� �������
		CELL* grabbedReg = marker;
		std::shared_ptr<Error_RegionsList> errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->GrabRegion( 1, &grabbedReg ));

		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_GRAB_FROM_EMPTY_LIST) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (Trying to grab the region from the empty S-List)", PLACE(), "Log.txt" );                 TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior" );
		}
		if (grabbedReg != marker) {
			err = std::make_shared<Error_Custom>( "Output pointer has been changed during the trying to grab the region from the empty S-List", PLACE(), "Log.txt" );           TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior" );
		}

		// �������� ��������� � ��������� ������� �����, ��������� �� ������������
		PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList grabbing" );
		SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList grabbing" );
		rl_check::CheckState( { 3, 0, 1, 2, 1, 1 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList grabbing" );
		rl_check::CheckState( { 3, 0, 1, 2, 1, 1 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList grabbing" );

		PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after RegionsList grabbing" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after RegionsList grabbing" );

		delete marker;
		Log::test( "Log.txt" ) << LogTest::Finished{ true };
		return;

	} while (0);

	delete marker;
	Log::test( "Log.txt" ) << LogTest::Finished{ false };
	system( "Pause" );
}


// ��������� ������ �������, ��������, ��� ���� � RegionsList
void RegionsList_Tester::Test_GrabbingTooBigRegion()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Grabbing Too Big Region", 1 };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[20000];
	CELL* marker = new CELL;
	auto regList = std::make_shared<RegionsList<CELL>>( 0 );

	// �������� �������
	Region_P initial_insertion = { memoryPitch,      10 };
	Region_P second_insertion  = { memoryPitch + 20, 11 };

	do {
		// ������ ���������������� �������
		regList->ReleaseRegion( initial_insertion );

		// ��������� ���������, ��������� � ������� ����� ���������������� �������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		rl_check::CheckState( { 3, 1, 1, 1, 1, 2 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( { 3, 1, 1, 1, 1, 2 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after RegionsList initial insertion" );

		std::vector<Region_P> PList_Content = rl_manip::GetContent<Region_P>( regList, err );                                       TRACE_PRINT_CONTINUE( err, "Can't get P-List content after initial insertion" );
		std::vector<Region_S> SList_Content = rl_manip::GetContent<Region_S>( regList, err );                                       TRACE_PRINT_CONTINUE( err, "Can't get S-List content after initial insertion" );
		rl_check::CheckContent<Region_P>( { { memoryPitch, 10 } }, PList_Content, err );                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-List content gained after initial insertion" );
		rl_check::CheckContent<Region_S>( { { memoryPitch, 10, 1 } }, SList_Content, err );                                         TRACE_PRINT_CONTINUE( err, "Incorrect S-List content gained after initial insertion" );

		// �������� ��������� ������� ������� ������, ������ �� �������
		CELL* grabbedReg = marker;
		std::shared_ptr<Error_RegionsList> errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->GrabRegion( 11, &grabbedReg ));

		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_CONSISTENT_REG_NOTFOUND) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (Trying to grab the region with size is greater than the S-List has)", PLACE(), "Log.txt" );      TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior after initial insertion" );
		}
		if (grabbedReg != marker) {
			err = std::make_shared<Error_Custom>( "Output pointer has been changed during the trying to grab the region with too big size", PLACE(), "Log.txt" );                       TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior after initial insertion" );
		}
		// ������ ������ �������
		regList->ReleaseRegion( second_insertion );

		// ��������� ���������, ��������� � ������� ����� ������ �������
		PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList second insertion" );
		SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList second insertion" );
		rl_check::CheckState( { 3, 2, 1, 0, 1, 3 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList second insertion" );
		rl_check::CheckState( { 3, 2, 1, 0, 1, 3 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList second insertion" );

		PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after RegionsList second insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after RegionsList second insertion" );

		PList_Content = rl_manip::GetContent<Region_P>( regList, err );                                                             TRACE_PRINT_CONTINUE( err, "Can't get P-List content after second insertion" );
		SList_Content = rl_manip::GetContent<Region_S>( regList, err );                                                             TRACE_PRINT_CONTINUE( err, "Can't get S-List content after second insertion" );
		rl_check::CheckContent<Region_P>( { {memoryPitch, 10}, {memoryPitch + 20, 11} }, PList_Content, err );                      TRACE_PRINT_CONTINUE( err, "Incorrect P-List content gained after second insertion" );
		rl_check::CheckContent<Region_S>( { {memoryPitch, 10, 1}, {memoryPitch + 20, 11, 1} }, SList_Content, err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-List content gained after second insertion" );

		// �������� ��������� ������� ������� ������, ������ �� �������
		errRegs = std::dynamic_pointer_cast<Error_RegionsList, Error_Base>(regList->GrabRegion( 12, &grabbedReg ));
		if (!errRegs || errRegs->Type() != RegList_ErrType::ERR_CONSISTENT_REG_NOTFOUND) {
			err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (Trying to grab the region with size is greater than the S-List has)", PLACE(), "Log.txt" );      TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior after second insertion" );
		}
		if (grabbedReg != marker) {
			err = std::make_shared<Error_Custom>( "Output pointer has been changed during the trying to grab the region with too big size", PLACE(), "Log.txt" );                       TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior after second insertion" );
		}
		// ��������� ���������, ��������� � ������� ����� ������� �������
		PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList second insertion" );
		SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList second insertion" );
		rl_check::CheckState( { 3, 2, 1, 0, 1, 3 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList second insertion" );
		rl_check::CheckState( { 3, 2, 1, 0, 1, 3 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList second insertion" );

		PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after RegionsList second insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after RegionsList second insertion" );

		PList_Content = rl_manip::GetContent<Region_P>( regList, err );                                                             TRACE_PRINT_CONTINUE( err, "Can't get P-List content after second insertion" );
		SList_Content = rl_manip::GetContent<Region_S>( regList, err );                                                             TRACE_PRINT_CONTINUE( err, "Can't get S-List content after second insertion" );
		rl_check::CheckContent<Region_P>( { {memoryPitch, 10}, {memoryPitch + 20, 11} }, PList_Content, err );                      TRACE_PRINT_CONTINUE( err, "Incorrect P-List content gained after second insertion" );
		rl_check::CheckContent<Region_S>( { {memoryPitch, 10, 1}, {memoryPitch + 20, 11, 1} }, SList_Content, err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-List content gained after second insertion" );

		delete marker;
		delete[] memoryPitch;
		Log::test( "Log.txt" ) << LogTest::Finished{ true };
		return;

	} while (0);

	delete marker;
	delete[] memoryPitch;
	Log::test( "Log.txt" ) << LogTest::Finished{ false };
	system( "Pause" );
}


// ��������� ������ RegionsList ��� ���������� ������� (ExpandList)
void RegionsList_Tester::Test_ListsExpanding()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Lists Expanding", 1 };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[4000];
	auto regList = std::make_shared<RegionsList<CELL>>( 0 );

	do {
		// �������� ��������� � ��������� ������� � ��������� ��������� (�� �����-���� �������), ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList creation" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList creation" );
		ListState expected_PState = { 3, 0, 1, 2, 1, 1 };
		ListState expected_SState = { 3, 0, 1, 2, 1, 1 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList creation" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList creation" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList creation" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList creation" );

		// ��������� ����� �������
		std::vector<Region_P> regs( 200 );
		for (size_t i = 0; i < 200; ++i) {
			regs[i] = Region_P{ memoryPitch + i * 10, 9 };
		}

		// ���������� ������� � ����� ���������� P- � S-�������
		std::string errMess = "ReleaseRegion() error gained after RegionsList expanding";
		err = regList->ReleaseRegion( regs[100] );      TRACE_PRINT_CONTINUE( err, errMess );	// [---] [#B#] [-E-]
		err = regList->ReleaseRegion( regs[101] );      TRACE_PRINT_CONTINUE( err, errMess );	// [---] [#B#] [###] E
		err = regList->ReleaseRegion( regs[102] );      TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [#B#] [###] [###] [-E-]
		err = regList->ReleaseRegion( regs[103] );      TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [#B#] [###] [###] [###] E
		err = regList->ReleaseRegion( regs[104] );      TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [---] [---] [#B#] [###] [###] [###] [###] [---] [---] [-E-]

		for (uint8_t i = 105; i < 108; ++i) {
			err = regList->ReleaseRegion( regs[i] );    TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [---] [---] [#B#] [###] [###] [###] [###] [###] [###] [###] E
		}
		err = regList->ReleaseRegion( regs[108] );      TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [---] [---] [---] [---] [---] [---] [#B#] [###] [###] [###] [###] [###] [###] [###] [#�#] [-E-] [---] [---] [---] [---] [---] [---]

		for (uint8_t i = 109; i < 116; ++i) {
			err = regList->ReleaseRegion( regs[i] );    TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [---] [---] [---] [---] [---] [---] [#B#] [###] [###] [###] [###] [###] [###] [###] [#�#] [###] [###] [###] [###] [###] [###] [###] E
		}
		for (uint8_t i = 92; i < 100; ++i) {
			err = regList->ReleaseRegion( regs[i] );    TRACE_PRINT_CONTINUE( err, errMess );	// [#B#] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [#�#] [###] [###] [###] [###] [###] [###] [###] E
		}
		err = regList->ReleaseRegion( regs[91] );       TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [---] [---] [---] [---] [---] [---] [---] [---] [---] [#B#] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [-E-] [---] [---] [---] [---] [---] [---] [---] [---] [---] [---] [---]

		for (uint8_t i = 116; i < 128; ++i) {
			err = regList->ReleaseRegion( regs[i] );    TRACE_PRINT_CONTINUE( err, errMess );	// [---] [---] [---] [---] [---] [---] [---] [---] [---] [---] [---] [#B#] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] E
		}
		for (uint8_t i = 80; i < 91; ++i) {
			err = regList->ReleaseRegion( regs[i] );    TRACE_PRINT_CONTINUE( err, errMess );	// [#B#] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] [###] E
		}

		// �������� ��������� � ��������� ������� ����� ����������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList expanding" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList expanding" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList expanding" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList expanding" );
		expected_PState = { 48, 48, 0, 0, 0, 48 };
		expected_SState = { 48, 48, 0, 0, 0, 48 };
		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList expanding" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList expanding" );

		// �������� ������� �������, ���������
		std::vector<Region_P> p_content = rl_manip::GetContent<Region_P>( regList, err );                                           TRACE_PRINT_CONTINUE( err, "Can't get P-List content after RegionsList expanding" );
		std::vector<Region_S> s_content = rl_manip::GetContent<Region_S>( regList, err );                                           TRACE_PRINT_CONTINUE( err, "Can't get S-List content after RegionsList expanding" );

		std::vector<Region_P> p_content_expected = std::vector<Region_P>( regs.begin() + 80, regs.begin() + 128 );
		std::vector<Region_S> s_content_expected;
		utils::SListFromPList<CELL>( p_content_expected, s_content_expected );

		rl_check::CheckContent<Region_P>( p_content_expected, p_content, err );                                                     TRACE_PRINT_CONTINUE( err, "Incorrect P-List content gained after RegionsList expanding " );
		rl_check::CheckContent<Region_S>( s_content_expected, s_content, err );                                                     TRACE_PRINT_CONTINUE( err, "Incorrect S-List content gained after RegionsList expanding " );

	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ���������������� ������� � RegionsList (����� �� ������ ������ � ������ �� ��������)
void RegionsList_Tester::Test_InitialReleaseRegion()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Insertions Initial" };

	Error_BasePtr err = nullptr;
	size_t pitch_size = 20000;
	CELL* memoryPitch = new CELL[pitch_size];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P initial_insertion = { memoryPitch, pitch_size };

	do {
		// �������� ��������� � ��������� ������� ��, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList creation" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList creation" );
		rl_check::CheckState( { 5, 0, 2, 3, 2, 2 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList creation" );
		rl_check::CheckState( { 5, 0, 2, 3, 2, 2 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList creation" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after RegionsList creation" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after RegionsList creation" );

		// ������ ���������������� �������
		err = regList->ReleaseRegion( initial_insertion );                                                                          TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �����, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ���������� ������ ��������� �� � ����� (�������� ���������� - ���������)
		PList_State_before.end_pos += 1;
		PList_State_before.size += 1;
		PList_State_before.spaceRight -= 1;

		SList_State_before.end_pos += 1;
		SList_State_before.size += 1;
		SList_State_before.spaceRight -= 1;

		rl_check::CheckState( PList_State_before, PList_State_after, err );                                                         TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( SList_State_before, SList_State_after, err );                                                         TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������� � RegionsList, ������� ������, ����� �����, ����� �� �������� 1 ������
void RegionsList_Tester::Test_SecondRelease_LeftRightAdj()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Second Left/Right Adjacent Insertions" };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[1000];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P first_insertion     = { memoryPitch + 500, 50 };
	Region_P left_insertion_adj  = { memoryPitch + 450, 50 };
	Region_P right_insertion_adj = { memoryPitch + 550, 50 };

	do {
		// ������ ���������������� �������
		Error_BasePtr err = regList->ReleaseRegion( first_insertion );                                                              TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �� �������, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		ListState expected_PState = { 5, 1, 2, 2, 2, 3 };
		ListState expected_SState = { 5, 1, 2, 2, 2, 3 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ������ ������� ������� �����
		err = regList->ReleaseRegion( left_insertion_adj );                                                                         TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList left_adj insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_adj insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_adj insertion" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList left_adj insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList left_adj insertion" );

		// ���������� ��������� �� � ����� (��� ������� ������� ��������� ���� �� ������)
		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_adj insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_adj insertion" );

		// ������ ������� ������� ������
		err = regList->ReleaseRegion( right_insertion_adj );                                                                        TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList right_adj insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_adj insertion" );
		SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_adj insertion" );
		PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList right_adj insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList right_adj insertion" );

		// ���������� ��������� �� � ����� (��� ������� ������� ��������� ���� �� ������)
		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_adj insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_adj insertion" );
	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������� ������� ������ ����� � RegionsList, ����� �� �������� 1 ������
void RegionsList_Tester::Test_SecondRelease_Left_Less_Size()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Second Left Insertion With Less Size" };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[1000];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P first_insertion          = { memoryPitch + 500, 50 };
	Region_P left_insertion_size_less = { memoryPitch + 450, 49 };

	do {
		// ������ ���������������� �������
		err = regList->ReleaseRegion( first_insertion );                                                                            TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �� �������, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		ListState expected_PState = { 5, 1, 2, 2, 2, 3 };
		ListState expected_SState = { 5, 1, 2, 2, 2, 3 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ������ ������� ����� ������� ������
		err = regList->ReleaseRegion( left_insertion_size_less );                                                                   TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList left_size_less insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_size_less insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_size_less insertion" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList left_size_less insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList left_size_less insertion" );

		// ���������� ��������� �� � �����
		expected_PState.begin_pos -= 1;
		expected_PState.size += 1;
		expected_PState.spaceLeft -= 1;
		expected_SState.begin_pos -= 1;
		expected_SState.size += 1;
		expected_SState.spaceLeft -= 1;

		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_size_less insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_size_less insertion" );
	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������� ������� ������ ����� � RegionsList, ����� �� �������� 1 ������
void RegionsList_Tester::Test_SecondRelease_Left_Great_Size()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Second Left Insertion With Great Size" };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[1000];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P first_insertion           = { memoryPitch + 500, 50 };
	Region_P left_insertion_size_great = { memoryPitch + 448, 51 };

	do {
		// ������ ���������������� �������
		err = regList->ReleaseRegion( first_insertion );                                                                            TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �� �������, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		ListState expected_PState = { 5, 1, 2, 2, 2, 3 };
		ListState expected_SState = { 5, 1, 2, 2, 2, 3 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ������ ������� ����� ������� ������
		err = regList->ReleaseRegion( left_insertion_size_great );                                                                  TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList left_size_great insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_size_great insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_size_great insertion" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList left_size_great insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList left_size_great insertion" );

		// ���������� ��������� �� � �����
		expected_PState.begin_pos -= 1;
		expected_PState.size += 1;
		expected_PState.spaceLeft -= 1;
		expected_SState.end_pos += 1;
		expected_SState.size += 1;
		expected_SState.spaceRight -= 1;

		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_size_great insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_size_great insertion" );
	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������� ������ ������ ����� � RegionsList, ����� �� �������� 1 ������
void RegionsList_Tester::Test_SecondRelease_Left_Equal_Size()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Second Left Insertion With Equal Size" };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[1000];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P first_insertion =           { memoryPitch + 500, 50 };
	Region_P left_insertion_size_equal = { memoryPitch + 449, 50 };

	do {
		// ������ ���������������� �������	
		err = regList->ReleaseRegion( first_insertion );                                                                            TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �� �������, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		ListState expected_PState = { 5, 1, 2, 2, 2, 3 };
		ListState expected_SState = { 5, 1, 2, 2, 2, 3 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ������ ������� ����� ������ ������
		err = regList->ReleaseRegion( left_insertion_size_equal );                                                                  TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList left_size_equal insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_size_equal insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_size_equal insertion" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList left_size_equal insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList left_size_equal insertion" );

		// ���������� ��������� �� � �����
		expected_PState.begin_pos -= 1;
		expected_PState.size += 1;
		expected_PState.spaceLeft -= 1;
		expected_SState.begin_pos -= 1;
		expected_SState.size += 1;
		expected_SState.spaceLeft -= 1;

		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList left_size_equal insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList left_size_equal insertion" );
	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������� ������� ������ ������ � RegionsList, ����� �� �������� 1 ������
void RegionsList_Tester::Test_SecondRelease_Right_Less_Size()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Second Right Insertion With Less Size" };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[1000];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P first_insertion =           { memoryPitch + 500, 50 };
	Region_P right_insertion_size_less = { memoryPitch + 551, 49 };

	do {
		// ������ ���������������� �������
		err = regList->ReleaseRegion( first_insertion );                                                                            TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �� �������, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		ListState expected_PState = { 5, 1, 2, 2, 2, 3 };
		ListState expected_SState = { 5, 1, 2, 2, 2, 3 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ������ ������� ������ ������� ������
		err = regList->ReleaseRegion( right_insertion_size_less );                                                                  TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList right_size_less insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_size_less insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_size_less insertion" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList right_size_less insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList right_size_less insertion" );

		// ���������� ��������� �� � �����
		expected_PState.end_pos += 1;
		expected_PState.size += 1;
		expected_PState.spaceRight -= 1;
		expected_SState.begin_pos -= 1;
		expected_SState.size += 1;
		expected_SState.spaceLeft -= 1;

		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_size_less insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_size_less insertion" );
	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������� ������� ������ ������ � RegionsList, ����� �� �������� 1 ������
void RegionsList_Tester::Test_SecondRelease_Right_Great_Size()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Second Right Insertion With Great Size" };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[1000];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P first_insertion =            { memoryPitch + 500, 50 };
	Region_P right_insertion_size_great = { memoryPitch + 551, 51 };

	do {
		// ������ ���������������� �������
		err = regList->ReleaseRegion( first_insertion );                                                                            TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �� �������, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		ListState expected_PState = { 5, 1, 2, 2, 2, 3 };
		ListState expected_SState = { 5, 1, 2, 2, 2, 3 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ������ ������� ������ ������� ������
		err = regList->ReleaseRegion( right_insertion_size_great );                                                                 TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList right_size_great insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_size_great insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_size_great insertion" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList right_size_great insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList right_size_great insertion" );

		// ���������� ��������� �� � �����
		expected_PState.end_pos += 1;
		expected_PState.size += 1;
		expected_PState.spaceRight -= 1;
		expected_SState.end_pos += 1;
		expected_SState.size += 1;
		expected_SState.spaceRight -= 1;

		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_size_great insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_size_great insertion" );
	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������� ������ ������ ������ � RegionsList, ����� �� �������� 1 ������
void RegionsList_Tester::Test_SecondRelease_Right_Equal_Size()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Second Right Insertion With Equal Size" };

	Error_BasePtr err = nullptr;
	CELL* memoryPitch = new CELL[1000];
	auto regList = std::make_shared<RegionsList<CELL>>( 5 );

	// �������� �������
	Region_P first_insertion =            { memoryPitch + 500, 50 };
	Region_P right_insertion_size_equal = { memoryPitch + 551, 50 };

	do {
		// ������ ���������������� �������
		err = regList->ReleaseRegion( first_insertion );                                                                            TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList initial insertion" );

		// �������� ��������� � ��������� ������� �� �������, ��������� �� ������������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );
		ListState expected_PState = { 5, 1, 2, 2, 2, 3 };
		ListState expected_SState = { 5, 1, 2, 2, 2, 3 };
		rl_check::CheckState( expected_PState, PList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList initial insertion" );
		rl_check::CheckState( expected_SState, SList_State_before, err );                                                           TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList initial insertion" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList initial insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList initial insertion" );

		// ������ ������� ������ ������� ������
		err = regList->ReleaseRegion( right_insertion_size_equal );                                                                 TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained after RegionsList right_size_equal insertion" );

		// �������� ��������� � ��������� ������� ����� �������, ��������� �� ������������
		ListState PList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_size_equal insertion" );
		ListState SList_State_after = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                 TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_size_equal insertion" );
		ListFootprints PList_Footprints_after = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_after = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_after, PList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect P-State/Footprints combo after RegionsList right_size_equal insertion" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_after, SList_Footprints_after, err );                               TRACE_PRINT_CONTINUE( err, "Incorrect S-State/Footprints combo after RegionsList right_size_equal insertion" );

		// ���������� ��������� �� � �����
		expected_PState.end_pos += 1;
		expected_PState.size += 1;
		expected_PState.spaceRight -= 1;
		expected_SState.end_pos += 1;
		expected_SState.size += 1;
		expected_SState.spaceRight -= 1;

		rl_check::CheckState( expected_PState, PList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after RegionsList right_size_equal insertion" );
		rl_check::CheckState( expected_SState, SList_State_after, err );                                                            TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after RegionsList right_size_equal insertion" );
	} while (0);

	delete[] memoryPitch;

	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������ �������, ����� � RegionsList ���� �������
void RegionsList_Tester::Test_GrabbingFromSingleSizedList()
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Test Grabbing From Single Sized List", 1 };

	Error_BasePtr err = nullptr;
	uint32_t pitch_size = 100;
	CELL *memoryPitch = new CELL[pitch_size];
	CELL *grabbedReg = nullptr;
	auto regList = std::make_shared<RegionsList<CELL>>( 0 );

	// �������� �������
	Region_P initial_region{ memoryPitch, pitch_size };

	do {
		// ������ ���������������� �������
		err = regList->ReleaseRegion( initial_region );                                                                             TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained during RegionsList initial insertion" );

		// ������ ������ ������� ������������ ������, ��������� RegionsList � ��������� ���������
		err = regList->GrabRegion( 100, &grabbedReg );                                                                              TRACE_PRINT_CONTINUE( err, "Error gained during grabbing the region with max size" );

		// ��������� ���������, ��������� � �������
		ListState PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after grabbing the region with max size" );
		ListState SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after grabbing the region with max size" );
		rl_check::CheckState( { 3, 0, 1, 2, 1, 1 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after grabbing the region with max size" );
		rl_check::CheckState( { 3, 0, 1, 2, 1, 1 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after grabbing the region with max size" );

		ListFootprints PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		ListFootprints SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );	                            TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after grabbing the region with max size" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );	                            TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after grabbing the region with max size" );

		std::vector<Region_P> PList_Content = rl_manip::GetContent<Region_P>( regList, err );                                       TRACE_PRINT_CONTINUE( err, "Can't get P-List content after grabbing the region with max size" );
		std::vector<Region_S> SList_Content = rl_manip::GetContent<Region_S>( regList, err );                                       TRACE_PRINT_CONTINUE( err, "Can't get S-List content after grabbing the region with max size" );
		rl_check::CheckContent<Region_P>({}, PList_Content, err );                                                                  TRACE_PRINT_CONTINUE( err, "Incorrect P-List content gained after grabbing the region with max size" );
		rl_check::CheckContent<Region_S>({}, SList_Content, err );                                                                  TRACE_PRINT_CONTINUE( err, "Incorrect S-List content gained after grabbing the region with max size" );

		// ��������� ��������� �� ����������� ������
		if (grabbedReg != memoryPitch) {
			err = std::make_shared<Error_Custom>( "Output pointer is incorrect", PLACE(), "Log.txt" );                              TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior after grabbing the region with max size" );
		}
		grabbedReg = nullptr;

		// ������ ���������������� �������
		err = regList->ReleaseRegion( initial_region );                                                                             TRACE_PRINT_CONTINUE( err, "ReleaseRegion() error gained during RegionsList initial insertion" );

		// ������ ������ ������� �������������� ������
		err = regList->GrabRegion( 99, &grabbedReg );                                                                               TRACE_PRINT_CONTINUE( err, "Error gained during grabbing the region with non-max size" );

		// ��������� ���������, ��������� � �������
		PList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after grabbing the region with non-max size" );
		SList_State_before = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                          TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after grabbing the region with non-max size" );
		rl_check::CheckState( { 3, 1, 1, 1, 1, 2 }, PList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect P-State gained after grabbing the region with non-max size" );
		rl_check::CheckState( { 3, 1, 1, 1, 1, 2 }, SList_State_before, err );                                                      TRACE_PRINT_CONTINUE( err, "Incorrect S-State gained after grabbing the region with non-max size" );

		PList_Footprints_before = rl_manip::GetFootprints<Region_P>( regList );
		SList_Footprints_before = rl_manip::GetFootprints<Region_S>( regList );
		rl_check::CheckFootprintsVsState<Region_P>( PList_State_before, PList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect P-List state/footprints combo after grabbing the region with non-max size" );
		rl_check::CheckFootprintsVsState<Region_S>( SList_State_before, SList_Footprints_before, err );                             TRACE_PRINT_CONTINUE( err, "Incorrect S-List state/footprints combo after grabbing the region with non-max size" );

		PList_Content = rl_manip::GetContent<Region_P>( regList, err );                                                             TRACE_PRINT_CONTINUE( err, "Can't get P-List content after grabbing the region with non-max size" );
		SList_Content = rl_manip::GetContent<Region_S>( regList, err );                                                             TRACE_PRINT_CONTINUE( err, "Can't get S-List content after grabbing the region with non-max size" );
		rl_check::CheckContent<Region_P>( { {grabbedReg + 99, 1} }, PList_Content, err );                                           TRACE_PRINT_CONTINUE( err, "Incorrect P-List content gained after grabbing the region with non-max size" );
		rl_check::CheckContent<Region_S>( { {grabbedReg + 99, 1, 1} }, SList_Content, err );                                        TRACE_PRINT_CONTINUE( err, "Incorrect S-List content gained after grabbing the region with non-max size" );

		// ��������� ��������� �� ����������� ������
		if (grabbedReg != memoryPitch) {
			err = std::make_shared<Error_Custom>( "Output pointer is incorrect", PLACE(), "Log.txt" );                              TRACE_PRINT_CONTINUE( err, "GrabRegion() Wrong behavior after grabbing the region with non-max size" );
		}

	} while (0);

	delete[] memoryPitch;
	if (err) {
		Log::test( "Log.txt" ) << LogTest::Finished{ false };
		system( "Pause" );
		return;
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


// ��������� ������ �� RegionsList �������� ��������� ������ ��� ��������� �������� ���������� RegionsList
void RegionsList_Tester::Test_GrabbingsComplex()
{
	std::vector<TestData> grabbs_complex;
	GenerateGrabsComplex( grabbs_complex );

	Log::test( "Log.txt" ) << LogTest::Start{ "Test Grabbings Complex", grabbs_complex.size() };

	double sec_grab = 0.0;
	std::list<Error_BasePtr> errors;
	std::mutex mutex_error, mutex_timing;

	concurrency::cancellation_token_source cts;
	concurrency::run_with_cancellation_token(
		[&cts, &grabbs_complex, &errors, &sec_grab, &mutex_timing, &mutex_error, this]()
		{
			concurrency::parallel_for_each( grabbs_complex.begin(), grabbs_complex.end(),
				[&cts, &errors, &sec_grab, &mutex_timing, &mutex_error, this]( TestData test_step )
				{
					Log::test() << LogTest::Progress{};
					Error_BasePtr err = nullptr;
					CELL* marker = new CELL;
					std::shared_ptr<RegionsList<CELL>> tested_regList = std::make_unique<RegionsList<CELL>>( 0 );
					
					do {
						rl_manip::SetState<Region_P>( test_step.p_listState_initial, tested_regList, err );                             TRACE_CONTINUE( err, "Can't set P-List initial state in step:\n" + test_step.to_String() );
						rl_manip::SetState<Region_S>( test_step.s_listState_initial, tested_regList, err );                             TRACE_CONTINUE( err, "Can't set S-List initial state in step:\n" + test_step.to_String() );
						rl_manip::SetContent<Region_P>( test_step.p_listContent_initial, tested_regList, err );                         TRACE_CONTINUE( err, "Can't set P-List initial content in step:\n" + test_step.to_String() );
						rl_manip::SetContent<Region_S>( test_step.s_listContent_initial, tested_regList, err );                         TRACE_CONTINUE( err, "Can't set S-List initial content in step:\n" + test_step.to_String() );

						ListState PList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( tested_regList ), err );    TRACE_CONTINUE( err, "Incorrect P-State gained after initialisation in step:\n" + test_step.to_String() );
						ListState SList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( tested_regList ), err );    TRACE_CONTINUE( err, "Incorrect S-State gained after initialisation in step:\n" + test_step.to_String() );
						rl_check::CheckState( test_step.p_listState_initial, PList_State, err );                                        TRACE_CONTINUE( err, "Incorrect P-State gained after initialisation in step:\n" + test_step.to_String() );
						rl_check::CheckState( test_step.s_listState_initial, SList_State, err );                                        TRACE_CONTINUE( err, "Incorrect S-State gained after initialisation in step:\n" + test_step.to_String() );
						
						ListFootprints PList_Footprints = rl_manip::GetFootprints<Region_P>( tested_regList );
						ListFootprints SList_Footprints = rl_manip::GetFootprints<Region_S>( tested_regList );
						rl_check::CheckFootprintsVsState<Region_P>( PList_State, PList_Footprints, err );                               TRACE_CONTINUE( err, "Incorrect P-State/Footprints combo after initialisation in step:\n" + test_step.to_String() );
						rl_check::CheckFootprintsVsState<Region_S>( SList_State, SList_Footprints, err );                               TRACE_CONTINUE( err, "Incorrect S-State/Footprints combo after initialisation in step:\n" + test_step.to_String() );

						std::vector<Region_P> PList_Content = rl_manip::GetContent<Region_P>( tested_regList, err );                    TRACE_CONTINUE( err, "Can't get P-List content after initialisation in step:\n" + test_step.to_String() );
						std::vector<Region_S> SList_Content = rl_manip::GetContent<Region_S>( tested_regList, err );                    TRACE_CONTINUE( err, "Can't get S-List content after initialisation in step:\n" + test_step.to_String() );
						rl_check::CheckContent<Region_P>( test_step.p_listContent_initial, PList_Content, err );                        TRACE_CONTINUE( err, "Incorrect P-List content gained after initialisation in step:\n" + test_step.to_String() );
						rl_check::CheckContent<Region_S>( test_step.s_listContent_initial, SList_Content, err );                        TRACE_CONTINUE( err, "Incorrect S-List content gained after initialisation in step:\n" + test_step.to_String() );

						// ���������� ������
						CELL* grabbedReg = marker;
						clock_t start = clock();
						err = tested_regList->GrabRegion( test_step.intermediate_reg.size, &grabbedReg );
						clock_t end = clock();
						{
							std::lock_guard<std::mutex> mut( mutex_timing );
							sec_grab += (double)((size_t)end - (size_t)start) / CLOCKS_PER_SEC;
						}
						// ���� ��������� ����� ������������ ������� - null, ������ ������������� ������ ������� �����. GrabRegion() ������ ������� ������, � �������� ��������� - �������� �����������
						if (test_step.intermediate_reg.start == nullptr) {
							if (!err) {
								err = std::make_shared<Error_Custom>( "Incorrect condition didn't handled (Trying to grab the region with size greater than S-List has)", PLACE(), "Log.txt" );			TRACE_CONTINUE( err, "GrabRegion() Wrong behavior" );
							}
							if (grabbedReg != marker) {
								err = std::make_shared<Error_Custom>( "Output pointer has been changed during the trying to grab the region with size greater than S-List has", PLACE(), "Log.txt" );	TRACE_CONTINUE( err, "GrabRegion() Wrong behavior" );
							}
							err.reset();
						}
						// ��������� ��������� � ������� P- � S-������� ������
						PList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( tested_regList ), err );              TRACE_CONTINUE( err, "Incorrect P-State gained after grabbing in step:\n" + test_step.to_String() );
						SList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( tested_regList ), err );              TRACE_CONTINUE( err, "Incorrect S-State gained after grabbing in step:\n" + test_step.to_String() );
						rl_check::CheckState( test_step.p_listState_resulted, PList_State, err );                                       TRACE_CONTINUE( err, "Incorrect P-State gained after grabbing in step:\n" + test_step.to_String() );
						rl_check::CheckState( test_step.s_listState_resulted, SList_State, err );                                       TRACE_CONTINUE( err, "Incorrect S-State gained after grabbing in step:\n" + test_step.to_String() );

						PList_Footprints = rl_manip::GetFootprints<Region_P>( tested_regList );
						SList_Footprints = rl_manip::GetFootprints<Region_S>( tested_regList );
						rl_check::CheckFootprintsVsState<Region_P>( PList_State, PList_Footprints, err );                               TRACE_CONTINUE( err, "Incorrect P-State/Footprints combo after grabbing in step:\n" + test_step.to_String() );
						rl_check::CheckFootprintsVsState<Region_S>( SList_State, SList_Footprints, err );                               TRACE_CONTINUE( err, "Incorrect S-State/Footprints combo after grabbing in step:\n" + test_step.to_String() );

						PList_Content = rl_manip::GetContent<Region_P>( tested_regList, err );                                          TRACE_CONTINUE( err, "Can't get P-List content after grabbing in step:\n" + test_step.to_String() );
						SList_Content = rl_manip::GetContent<Region_S>( tested_regList, err );                                          TRACE_CONTINUE( err, "Can't get S-List content after grabbing in step:\n" + test_step.to_String() );
						rl_check::CheckContent<Region_P>( test_step.p_listContent_resulted, PList_Content, err );                       TRACE_CONTINUE( err, "Incorrect P-List content gained after grabbing in step:\n" + test_step.to_String() );
						rl_check::CheckContent<Region_S>( test_step.s_listContent_resulted, SList_Content, err );                       TRACE_CONTINUE( err, "Incorrect S-List content gained after grabbing in step:\n" + test_step.to_String() );
					
						delete marker;
						return;

					} while (0);
					{
						std::lock_guard<std::mutex> mut( mutex_error );
						errors.push_back( err );
						delete marker;
						cts.cancel();
					}
				});
		},
		cts.get_token());

	for (auto err : errors) {
		err->Print();
	}
	Log::info( "Log.txt" ) << "Sumary time spent for grabbing: " << sec_grab << " sec" << Log::endlog{};
	Log::test( "Log.txt" ) << LogTest::Finished{ errors.empty() };
	if (!errors.empty()) {
		system( "Pause" );
	}
	return;
}


// ��������� ������� � RegionsList (������ ������, ������� � ��� - �� ��� ������������). ���������� RegionsList ������ 5 ������������� �������� ������ ������
void RegionsList_Tester::Test_InsertionsComplex()
{
	std::vector<std::string> insertions_complex;
	GenerateInsertionsComplex( insertions_complex );

	Log::test( "Log.txt" ) << LogTest::Start{ "Test Insertions Complex", insertions_complex.size() };

	std::list<Error_BasePtr> errors;
	std::mutex error_write_mutex, time_write_mutex;
	double sec_ins = 0.0;

	concurrency::cancellation_token_source cts;
	concurrency::run_with_cancellation_token( 
		[&insertions_complex, &sec_ins, &errors, &error_write_mutex, &time_write_mutex, &cts, this]()
		{
			concurrency::parallel_for_each( insertions_complex.begin(), insertions_complex.end(),
				[&sec_ins, &errors, &error_write_mutex, &time_write_mutex, &cts]( std::string step )
				{
					Log::test() << LogTest::Progress{};
					auto regList = std::make_shared<RegionsList<CELL>>( 0 );
					Error_BasePtr err = nullptr;

					do {
						TestData t_data = ParseInsertionComplex( step, err );																	    TRACE_CONTINUE( err, "Can't parse test-case in step: " + step );
						// ������������� P- � S-������ � �������� ���������. ����� � ��� �������� ����� ���������.
						rl_manip::SetState<Region_P>( t_data.p_listState_initial, regList, err );                                                   TRACE_CONTINUE( err, "Can't set P-List initial state in step: " + step );
						rl_manip::SetState<Region_S>( t_data.s_listState_initial, regList, err );                                                   TRACE_CONTINUE( err, "Can't set S-List initial state in step: " + step );
						rl_manip::SetContent<Region_P>( t_data.p_listContent_initial, regList, err );                                               TRACE_CONTINUE( err, "Can't set P-List initial content in step: " + step );
						rl_manip::SetContent<Region_S>( t_data.s_listContent_initial, regList, err );                                               TRACE_CONTINUE( err, "Can't set S-List initial content in step: " + step );

						// ��������� ������������� ��������� � ������� P- � S-�������
						ListState PList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                       TRACE_CONTINUE( err, "Incorrect P-State gained after initialisation in step: " + step );
						ListState SList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                       TRACE_CONTINUE( err, "Incorrect S-State gained after initialisation in step: " + step );
						rl_check::CheckState( t_data.p_listState_initial, PList_State, err );                                                       TRACE_CONTINUE( err, "Incorrect P-State gained after initialisation in step: " + step );
						rl_check::CheckState( t_data.s_listState_initial, SList_State, err );                                                       TRACE_CONTINUE( err, "Incorrect S-State gained after initialisation in step: " + step );

						ListFootprints PList_Footprints = rl_manip::GetFootprints<Region_P>( regList );
						ListFootprints SList_Footprints = rl_manip::GetFootprints<Region_S>( regList );
						rl_check::CheckFootprintsVsState<Region_P>( PList_State, PList_Footprints, err );                                           TRACE_CONTINUE( err, "Incorrect P-State/Footprints combo after initialisation in step: " + step );
						rl_check::CheckFootprintsVsState<Region_S>( SList_State, SList_Footprints, err );                                           TRACE_CONTINUE( err, "Incorrect S-State/Footprints combo after initialisation in step: " + step );

						std::vector<Region_P> PList_Content = rl_manip::GetContent<Region_P>( regList, err );                                       TRACE_CONTINUE( err, "Can't get P-List content after initialisation in step: " + step );
						std::vector<Region_S> SList_Content = rl_manip::GetContent<Region_S>( regList, err );                                       TRACE_CONTINUE( err, "Can't get S-List content after initialisation in step: " + step );
						rl_check::CheckContent<Region_P>( t_data.p_listContent_initial, PList_Content, err );                                       TRACE_CONTINUE( err, "Incorrect P-List content gained after initialisation in step: " + step );
						rl_check::CheckContent<Region_S>( t_data.s_listContent_initial, SList_Content, err );                                       TRACE_CONTINUE( err, "Incorrect S-List content gained after initialisation in step: " + step );

						// ���������� �������
						clock_t start = clock();
						err = regList->ReleaseRegion( t_data.intermediate_reg );                                                                    TRACE_CONTINUE( err, "ReleaseRegion() error gained after insertion in step: " + step );
						clock_t end = clock();
						{
							std::lock_guard l( time_write_mutex );
							sec_ins += (double)((size_t)end - (size_t)start) / CLOCKS_PER_SEC;
						}
						// ��������� ��������� � ������� P- � S-������� ������
						PList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_P>( regList ), err );                                 TRACE_CONTINUE( err, "Incorrect P-State gained after insertion in step: " + step );
						SList_State = rl_check::Validate_ListState( rl_manip::GetState<Region_S>( regList ), err );                                 TRACE_CONTINUE( err, "Incorrect S-State gained after insertion in step: " + step );
						rl_check::CheckState( t_data.p_listState_resulted, PList_State, err );                                                      TRACE_CONTINUE( err, "Incorrect P-State gained after insertion in step: " + step );
						rl_check::CheckState( t_data.s_listState_resulted, SList_State, err );                                                      TRACE_CONTINUE( err, "Incorrect S-State gained after insertion in step: " + step );

						PList_Footprints = rl_manip::GetFootprints<Region_P>( regList );
						SList_Footprints = rl_manip::GetFootprints<Region_S>( regList );
						rl_check::CheckFootprintsVsState<Region_P>( PList_State, PList_Footprints, err );                                           TRACE_CONTINUE( err, "Incorrect P-State/Footprints combo after insertion in step: " + step );
						rl_check::CheckFootprintsVsState<Region_S>( SList_State, SList_Footprints, err );                                           TRACE_CONTINUE( err, "Incorrect S-State/Footprints combo after insertion in step: " + step );

						PList_Content = rl_manip::GetContent<Region_P>( regList, err );                                                             TRACE_CONTINUE( err, "Can't get P-List content after insertion in step: " + step );
						SList_Content = rl_manip::GetContent<Region_S>( regList, err );                                                             TRACE_CONTINUE( err, "Can't get S-List content after insertion in step: " + step );
						rl_check::CheckContent<Region_P>( t_data.p_listContent_resulted, PList_Content, err );                                      TRACE_CONTINUE( err, "Incorrect P-List content gained after insertion in step: " + step );
						rl_check::CheckContent<Region_S>( t_data.s_listContent_resulted, SList_Content, err );                                      TRACE_CONTINUE( err, "Incorrect S-List content gained after insertion in step: " + step );
						return;

					} while (0);
					{
						std::lock_guard l( error_write_mutex );
						errors.push_back( err );
					}
					cts.cancel();
				}
			);
		}, cts.get_token() );

	for (auto err : errors) {
		err->Print();
	}
	Log::info( "Log.txt" ) << "Sumary time spent for insertions: " << sec_ins << " sec" << Log::endlog{};
	Log::test( "Log.txt" ) << LogTest::Finished{ errors.empty() };
	if (!errors.empty()) {
		system( "Pause" );
	}
}


std::vector<std::array<uint8_t, InsertionPlace::P5_Finish>> CreateInitialSizesSet()
{
	std::vector<std::array<uint8_t, InsertionPlace::P5_Finish>> initial_sizes;
	for (size_t i = 11111; i <= 55555; ++i)
	{
		uint8_t p1, p2, p3, p4, p5;
		p1 = static_cast<uint8_t>((i - i % 10000) / 10000);
		p2 = static_cast<uint8_t>(((i - p1 * 10000ULL) - (i - p1 * 10000ULL) % 1000ULL) / 1000ULL);
		p3 = static_cast<uint8_t>(((i - p1 * 10000ULL - p2 * 1000ULL) - (i - p1 * 10000ULL - p2 * 1000ULL) % 100ULL) / 100ULL);
		p4 = static_cast<uint8_t>(((i - p1 * 10000ULL - p2 * 1000ULL - p3 * 100ULL) - (i - p1 * 10000ULL - p2 * 1000ULL - p3 * 100ULL) % 10ULL) / 10ULL);
		p5 = static_cast<uint8_t>(((i - p1 * 10000ULL - p2 * 1000ULL - p3 * 100ULL - p4 * 10ULL)));

		if (p1 > 5 || p2 > 5 || p3 > 5 || p4 > 5 || p5 > 5 || !p1 || !p2 || !p3 || !p4 || !p5)
			continue;
			
		initial_sizes.push_back( { p1, p2, p3, p4, p5 } );
	}
	return initial_sizes;
}


void RegionsList_Tester::GenerateGrabsComplex( std::vector<TestData> &out_grabbs )
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Generate Insertions For Test Grabbings Complex", 1 };

	// ��������� ��� ��������� ���������� �������� Size ��� 5 �������� ������. Max size = 5, Min size = 1.
	std::vector<std::array<uint8_t, InsertionPlace::P5_Finish>> initial_sizes = CreateInitialSizesSet();

	std::vector<Region_P> p_list_initial( InsertionPlace::P5_Finish );	// �������� P-������
	std::vector<Region_S> s_list_initial( 0 );	// �������� S-������

	TestData td;
	for (size_t n = 0; n < initial_sizes.size(); ++n)
	{
		// ��������� �������� P-������
		for (int i = 0; i < InsertionPlace::P5_Finish; ++i)
		{
			Region_P reg = { Ins_complex::regPtrs[i], initial_sizes[n][i] };
			p_list_initial[i] = reg;
		}
		// ��������� �������� S-������ �� ������ P-������ (��������� ��-�� ������� �� Size, ����� �� Pointer)
		utils::SListFromPList( p_list_initial, s_list_initial );

		// ������� ��������� LR-����� ������ (L0R0, L0R1, L1R0, L1R1)
		for (const auto& f : Ins_complex::FieldStates)
		{
			// ������� �������� ������� 1-5
			for (size_t i = 1; i < 6; ++i)
			{
				std::vector<Region_P> p_list_resulted( p_list_initial );
				std::vector<Region_S> s_list_resulted( s_list_initial );
				switch (f)                   /*   P-State init   ||  S-State  init   ||  P-State  rslt   ||  S-State  rslt   || P-Cont init  ||  S-Cont init  */
				{                            /* C  S  L  R  B  E || C  S  L  R  B  E || C  S  L  R  B  E || C  S  L  R  B  E ||              ||               */
				case FieldState::L0_R0: td = { {5, 5, 0, 0, 0, 5}, {5, 5, 0, 0, 0, 5}, {5, 5, 0, 0, 0, 5}, {5, 5, 0, 0, 0, 5}, p_list_initial, s_list_initial, {}, {}, {nullptr, i} }; break;
				case FieldState::L0_R1: td = { {6, 5, 0, 1, 0, 5}, {6, 5, 0, 1, 0, 5}, {6, 5, 0, 1, 0, 5}, {6, 5, 0, 1, 0, 5}, p_list_initial, s_list_initial, {}, {}, {nullptr, i} }; break;
				case FieldState::L1_R0: td = { {6, 5, 1, 0, 1, 6}, {6, 5, 1, 0, 1, 6}, {6, 5, 1, 0, 1, 6}, {6, 5, 1, 0, 1, 6}, p_list_initial, s_list_initial, {}, {}, {nullptr, i} }; break;
				case FieldState::L1_R1: td = { {7, 5, 1, 1, 1, 6}, {7, 5, 1, 1, 1, 6}, {7, 5, 1, 1, 1, 6}, {7, 5, 1, 1, 1, 6}, p_list_initial, s_list_initial, {}, {}, {nullptr, i} }; break;
				}
				size_t index = utils::lower_bound<CELL>( s_list_resulted.data(), s_list_resulted.size(), i );
				
				// ���������� ������ �� ������?
				if (index == s_list_resulted.size()) {
					td.p_listContent_resulted = p_list_initial;
					td.s_listContent_resulted = s_list_initial;
					out_grabbs.push_back( td );
					continue;
				}
				td.intermediate_reg.start = s_list_resulted[index].start;
				
				// ��������� ������ � ����� size �� ������������?
				if (s_list_resulted[index].count != 1) {
					s_list_resulted[index + 1].count = s_list_resulted[index].count - 1;
				}
				// ��������� ������ �� ������ ����� ��������������?
				if (s_list_resulted[index].size == i)
				{
					// ������� ������� �� P-List
					Side delSide_p = utils::DeleteRegion<RegionP, CELL>( p_list_resulted, { s_list_resulted[index].start, s_list_resulted[index].size } );
					if (delSide_p == Side::Side_LEFT) {
						td.p_listState_resulted.spaceLeft++;
						td.p_listState_resulted.begin_pos++;
					}
					else if (delSide_p == Side::Side_RIGHT) {
						td.p_listState_resulted.spaceRight++;
						td.p_listState_resulted.end_pos--;
					}
					else {
						std::cout << "Wrong state" << std::endl;
					}
					td.p_listState_resulted.size--;

					// ������� ������� �� S-List
					Side delSide_s = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, s_list_resulted[index] );
					if (delSide_s == Side::Side_LEFT) {
						td.s_listState_resulted.spaceLeft++;
						td.s_listState_resulted.begin_pos++;
					}
					else if (delSide_s == Side::Side_RIGHT) {
						td.s_listState_resulted.spaceRight++;
						td.s_listState_resulted.end_pos--;
					}
					else {
						std::cout << "Wrong state" << std::endl;
					}
					td.s_listState_resulted.size--;
				}
				else {
					// ������������ ������� � P-List
					size_t p_modify_index = utils::FindRegion<RegionP, CELL>( p_list_resulted, { s_list_resulted[index].start, s_list_resulted[index].size } );
					if (p_modify_index == std::numeric_limits<size_t>::max()) {
						std::cout << "Wrong state" << std::endl;
					}
					p_list_resulted[p_modify_index].start += i;
					p_list_resulted[p_modify_index].size -= i;

					// ���������������� ������� � S-List
					RegionS<CELL> toInsert = { s_list_resulted[index].start + i, s_list_resulted[index].size - i, 0 };

					// ���������� ������� ��������/������� � S-List
					Side delSide_s = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, s_list_resulted[index] );
					Side insSide_s = utils::Find_SList_InsertionSide<CELL>( s_list_resulted, toInsert );

					// �������� S-List �� P-List
					utils::SListFromPList<CELL>( p_list_resulted, s_list_resulted );

					// � ����������� �� ���������� ������ ��������/������� �������� �������������� ���������
					switch (f) {
					case FieldState::L0_R0: {
						if (delSide_s == Side::Side_LEFT && insSide_s == Side::Side_RIGHT) { td.s_listState_resulted = { 10, 5, 3, 2, 3, 8 }; }
						if (delSide_s == Side::Side_RIGHT && insSide_s == Side::Side_LEFT) { td.s_listState_resulted = { 10, 5, 2, 3, 2, 7 }; }
						break;
					}
					case FieldState::L0_R1: {
						if (delSide_s == Side::Side_LEFT && insSide_s == Side::Side_RIGHT) { td.s_listState_resulted = { 6, 5, 1, 0, 1, 6 }; }
						if (delSide_s == Side::Side_RIGHT && insSide_s == Side::Side_LEFT) { td.s_listState_resulted = { 12, 5, 3, 4, 3, 8 }; }
						break;
					}
					case FieldState::L1_R0: {
						if (delSide_s == Side::Side_LEFT && insSide_s == Side::Side_RIGHT) { td.s_listState_resulted = { 12, 5, 4, 3, 4, 9 }; }
						if (delSide_s == Side::Side_RIGHT && insSide_s == Side::Side_LEFT) { td.s_listState_resulted = { 6, 5, 0, 1, 0, 5 }; }
						break;
					}
					case FieldState::L1_R1: {
						if (delSide_s == Side::Side_LEFT && insSide_s == Side::Side_RIGHT) { td.s_listState_resulted = { 7, 5, 2, 0, 2, 7 }; }
						if (delSide_s == Side::Side_RIGHT && insSide_s == Side::Side_LEFT) { td.s_listState_resulted = { 7, 5, 0, 2, 0, 5 }; }
						break;
					}
					}
				}
				td.p_listContent_resulted = p_list_resulted;
				td.s_listContent_resulted = s_list_resulted;
				out_grabbs.push_back( td );
			}
		}
	}
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


/*__________________________________________________________________________________________________
 *		��������� �������� ����� ������� ��� ��������� �������� ��������� Pointers-������.	������� |
 *	������, � �.�. �������. ����������� ��������������� ��������  ��������� Size-������ � ��������� |
 *	�������������� ��������� Size- � Pointer-�������.											    |
 *		�������� ������������ ������ "ComplexInsertions".											|
 *_________________________________________________________________________________________________*/
void RegionsList_Tester::GenerateInsertionsComplex( std::vector<std::string>& out_insertions_str )
{
	Log::test( "Log.txt" ) << LogTest::Start{ "Generate Insertions For Test Insertions Complex", 1 };

	// ��������� ��� ��������� ���������� �������� Size ��� 5 �������� ������. Max size = 5, Min size = 1.
	std::vector<std::array<uint8_t, InsertionPlace::P5_Finish>> initial_sizes = CreateInitialSizesSet();

	std::mutex write_ins_mutex;

	concurrency::parallel_for_each( initial_sizes.begin(), initial_sizes.end(), [&out_insertions_str, &write_ins_mutex]( std::array<uint8_t, InsertionPlace::P5_Finish> sizes )
		{
			std::vector<Region_P> p_list_initial( InsertionPlace::P5_Finish );  // �������� P-������
			std::vector<Region_S> s_list_initial( 0 );                          // �������� S-������

			// ��������� �������� P-������
			for (int i = 0; i < InsertionPlace::P5_Finish; ++i)
			{
				Region_P reg = { Ins_complex::regPtrs[i], sizes[i] };
				p_list_initial[i] = reg;
			}
			// ��������� �������� S-������ �� ������ P-������ (��������� ��-�� ������� �� Size, ����� �� Pointer)
			utils::SListFromPList( p_list_initial, s_list_initial );

			std::string empty_RegP_str = std::string( utils::to_string( p_list_initial[0] ).size(), ' ' );
			std::string empty_RegS_str = std::string( "[" ) + std::string( utils::to_string( s_list_initial[0] ).size() - 2, '-' ) + std::string( "]" );

			// ��������� �������. �������� ��� �������: "regNum + 1" - � ������, � ����� � ����� ��������� ������.
			// ������ � ����� ���������� ������� ��� �������
			CELL* start = Ins_complex::mem;
			CELL* stop = Ins_complex::regPtrs[0];

			// ������� ��������
			for (auto const& i : Ins_complex::InsertionPlaces)
			{
				std::vector<Region_P> insertions;		// ������� �� ������ �������
				size_t insertPosNum = stop - start;		// ����� ������� ��� ������� �� �������

				for (size_t p = 0; p < insertPosNum; ++p) {	// ������� ���� ������� �������
					for (size_t s = 1; s <= insertPosNum - p; ++s) {	// ������� ��������� size �������
						insertions.push_back( { start + p, s } );	// ������� �������
					}
				}
				// ������� ��������� LR-����� ������ (L0R0, L0R1, L1R0, L1R1)
				for (const auto& f : Ins_complex::FieldStates)
				{
					// ������� ����������� �������, ����������� ��������� ��������� P-������
					for (const auto& reg : insertions)
					{
						std::vector<Region_P> p_list_resulted = p_list_initial;
						std::vector<Region_S> s_list_resulted = s_list_initial;
						std::string p_state, s_state, p_content, s_content;

						Region_S to_del_1, to_del_2, to_ins;
						Region_P* left, * right;
						left = right = nullptr;

						if (i < InsertionPlace::P5_Finish) {
							right = &p_list_resulted[i];
						}
						if (i) {
							left = &p_list_resulted[i - 1ULL];
						}
						switch (i) {
							// ���� ������ ������� � ������ (����� �� ������� ������� � ������)
						case InsertionPlace::Start_P1: {
							// | #[0]   [1]   [2]   [3]   [4]  |
							if (utils::CheckAdj( reg, *right ))
							{
								to_del_1 = utils::RegionPtoS( *right );
								right->start = reg.start;
								right->size += reg.size;
								to_ins = utils::RegionPtoS( *right );

								Side del_1_side = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, to_del_1 );
								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, del_1_side, Side_NONE );

								p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::Start_P1_Adj_R];
								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted ) + empty_RegP_str;
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + utils::to_string( to_del_1 ) + " =>" + empty_RegS_str + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted );
							}
							// |# [0]   [1]   [2]   [3]   [4]  |
							else
							{
								to_ins = { reg.start, reg.size, 0 };
								p_list_resulted.insert( p_list_resulted.begin(), reg );

								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, Side_NONE, Side_NONE );

								p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::Start_P1_Adj_None];
								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted );
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + empty_RegS_str + " =>" + empty_RegS_str + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted );
							}
							break;
						}
													 // ���� ������ ������� � ����� (������ �� ���������� ������� � ������)
						case InsertionPlace::P5_Finish:
						{
							// |  [0]   [1]   [2]   [3]   [4]# |
							if (utils::CheckAdj( *left, reg ))
							{
								to_del_1 = utils::RegionPtoS( *left );
								left->size += reg.size;
								to_ins = utils::RegionPtoS( *left );

								Side del_1_side = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, to_del_1 );
								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, del_1_side, Side_NONE );

								p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P5_Finish_Adj_L];
								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted ) + empty_RegP_str;
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + utils::to_string( to_del_1 ) + " =>" + empty_RegS_str + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted );
							}
							// |  [0]   [1]   [2]   [3]   [4] #|
							else
							{
								to_ins = { reg.start, reg.size, 0 };
								p_list_resulted.insert( p_list_resulted.end(), reg );

								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, Side_NONE, Side_NONE );

								p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P5_Finish_Adj_None];
								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted );
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + empty_RegS_str + " =>" + empty_RegS_str + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted );
							}
							break;
						}
						// ���� ������ ������� ����� ���������
						default:
						{
							if (utils::CheckAdj( *left, reg, *right ) == Adjacency::Adj_Both)
							{
								switch (i) {
								case InsertionPlace::P1_P2: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P1_P2_Adj_RL]; break; } // |  [0]###[1]   [2]   [3]   [4]  |
								case InsertionPlace::P2_P3: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P2_P3_Adj_RL]; break; } // |  [0]   [1]###[2]   [3]   [4]  |
								case InsertionPlace::P3_P4: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P3_P4_Adj_RL]; break; } // |  [0]   [1]   [2]###[3]   [4]  |
								case InsertionPlace::P4_P5: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P4_P5_Adj_RL]; break; } // |  [0]   [1]   [2]   [3]###[4]  |
								}

								to_del_1 = utils::RegionPtoS( *left );
								to_del_2 = utils::RegionPtoS( *right );
								left->size += reg.size + right->size;
								to_ins = utils::RegionPtoS( *left );
								p_list_resulted.erase( p_list_resulted.begin() + i );

								Side del_1_side = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, to_del_1 );
								Side del_2_side = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, to_del_2 );
								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, del_1_side, del_2_side );

								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted ) + empty_RegP_str + empty_RegP_str;
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + utils::to_string( to_del_1 ) + " =>" + utils::to_string( to_del_2 ) + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted ) + "         ";
							}
							else if (utils::CheckAdj( *left, reg, *right ) == Adjacency::Adj_Left)
							{
								switch (i) {
								case InsertionPlace::P1_P2: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P1_P2_Adj_L]; break; } // |  [0]#  [1]   [2]   [3]   [4]  |
								case InsertionPlace::P2_P3: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P2_P3_Adj_L]; break; } // |  [0]   [1]#  [2]   [3]   [4]  |
								case InsertionPlace::P3_P4: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P3_P4_Adj_L]; break; } // |  [0]   [1]   [2]#  [3]   [4]  |
								case InsertionPlace::P4_P5: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P4_P5_Adj_L]; break; } // |  [0]   [1]   [2]   [3]#  [4]  |
								}

								to_del_1 = utils::RegionPtoS( *left );
								left->size += reg.size;
								to_ins = utils::RegionPtoS( *left );

								Side del_1_side = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, to_del_1 );
								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, del_1_side, Side_NONE );

								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted ) + empty_RegP_str;
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + utils::to_string( to_del_1 ) + " =>" + empty_RegS_str + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted ) + "         ";
							}
							else if (utils::CheckAdj( *left, reg, *right ) == Adjacency::Adj_NONE)
							{
								switch (i) {
								case InsertionPlace::P1_P2: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P1_P2_Adj_None]; break; } // |  [0] # [1]   [2]   [3]   [4]  |
								case InsertionPlace::P2_P3: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P2_P3_Adj_None]; break; } // |  [0]   [1] # [2]   [3]   [4]  |
								case InsertionPlace::P3_P4: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P3_P4_Adj_None]; break; } // |  [0]   [1]   [2] # [3]   [4]  |
								case InsertionPlace::P4_P5: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P4_P5_Adj_None]; break; } // |  [0]   [1]   [2]   [3] # [4]  |
								}

								to_ins = utils::RegionPtoS( reg );
								p_list_resulted.insert( p_list_resulted.begin() + i, reg );

								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, Side_NONE, Side_NONE );

								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted );
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + empty_RegS_str + " =>" + empty_RegS_str + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted );
							}
							else
							{
								switch (i) {
								case InsertionPlace::P1_P2: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P1_P2_Adj_R]; break; } // |  [0]  #[1]   [2]   [3]   [4]  |
								case InsertionPlace::P2_P3: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P2_P3_Adj_R]; break; } // |  [0]   [1]  #[2]   [3]   [4]  |
								case InsertionPlace::P3_P4: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P3_P4_Adj_R]; break; } // |  [0]   [1]   [2]  #[3]   [4]  |
								case InsertionPlace::P4_P5: { p_state = Ins_complex::P_StatesFlowDesc[f][Ins_complex::P4_P5_Adj_R]; break; } // |  [0]   [1]   [2]   [3]  #[4]  |
								}
								to_del_1 = utils::RegionPtoS( *right );
								right->start = reg.start;
								right->size += reg.size;
								to_ins = utils::RegionPtoS( *right );

								Side del_1_side = utils::DeleteRegion<RegionP, CELL>( s_list_resulted, to_del_1 );
								Side ins_side = utils::Find_SList_InsertionSide( s_list_resulted, to_ins );
								Ins_complex::S_ActionType combo = Ins_complex::SListActionDetermination( ins_side, del_1_side, Side_NONE );

								s_state = Ins_complex::S_StatesFlowDesc[f][combo];

								utils::SListFromPList( p_list_resulted, s_list_resulted );
								p_content = "P-Content: " + utils::to_string( p_list_initial ) + " <=" + utils::to_string( reg ) + " = " + utils::to_string( p_list_resulted ) + empty_RegP_str;
								s_content = " S-Content: " + utils::to_string( s_list_initial ) + " =>" + utils::to_string( to_del_1 ) + " =>" + empty_RegS_str + " <=" + utils::to_string( to_ins ) +
									" = " + utils::to_string( s_list_resulted );
							}
							break;
						}
						}
						{
							std::lock_guard l( write_ins_mutex );
							out_insertions_str.push_back( p_state + s_state + p_content + s_content );
						}
					}
				}
				// ��������� � ���������� �������
				if (i == InsertionPlace::P5_Finish)
					break;
				start = stop + p_list_initial[i].size;
				stop = Ins_complex::regPtrs[i + 1];
			}
			s_list_initial.clear();
		}
	);
	Log::test( "Log.txt" ) << LogTest::Finished{ true };
}


/*___________________________________________________________________________________________________
 *       ������ ������ ���������, �������� ����� ������ ��� ���������� ����� "ComplexInsertions".    |
 *__________________________________________________________________________________________________*/
TestData ParseInsertionComplex( const std::string &test_case, Error_BasePtr &err)
{
	std::string err_message;

    Regex p_state_reg("P-State.+S-State");
    Regex s_state_reg("S-State.+P-Content");
    Regex p_content_reg("P-Content.+S-Content");
    Regex s_content_reg("S-Content.+$");
    Regex p_state_initial_reg("([CSLRBE]\\d{2}.)+");
    Regex p_state_resulted_reg("->.+([CSLRBE]\\d{2}.)+");
    Regex s_state_initial_reg("([CSLRBE]\\d{2}.)+");
    Regex s_state_resulted_reg("->\\s+([CSLRBE]\\d{2}.)+");
    Regex p_content_initial_reg("(\\[\\d+;\\d+\\])+\\s+<=");
    Regex p_content_resulted_reg("\\s=\\s(\\[\\d+;\\d+\\])+");
    Regex s_content_initial_reg(":\\s+(\\[\\d+;\\d+;\\d+\\])+");
    Regex s_content_resulted_reg("[^><]=[^><](\\[\\d+;\\d+;\\d+\\])+");
    Regex p_insertion_reg("<=\\[\\d+;\\d+\\]");

    bool dummy;

    // �������� ��������� ����� (����� ��������� ���������). ������� ������.
	err_message = "Can't proceed to parse a test-case string:\t\"" + test_case + "\"";
	std::string p_state		= utils::FindStrRegular( test_case, p_state_reg, dummy, err, "" );		TRACE_RETURN( err, err_message, TestData() );
	std::string s_state		= utils::FindStrRegular(test_case, s_state_reg, dummy, err, "");		TRACE_RETURN( err, err_message, TestData() );
	std::string p_content	= utils::FindStrRegular(test_case, p_content_reg, dummy, err, "");		TRACE_RETURN( err, err_message, TestData() );
	std::string s_content	= utils::FindStrRegular(test_case, s_content_reg, dummy, err, "");		TRACE_RETURN( err, err_message, TestData() );

    //���� ���-�� �� ������� - ������� ������.
    if (p_state.empty() || s_state.empty() || p_content.empty() || s_content.empty())
	{
        err = std::make_shared<Error_Custom>(
            "Can't find description for:\t\"P-State\" or \"S-State\" or \"P-Content\" or \"S-Content\"\nIn a string:\t\"" + test_case + "\"\nBy regex:\t\"" + 
            p_state_reg.str + "\" or \"" + s_state_reg.str + "\" or \"" + p_content_reg.str + "\" or \"" + s_content_reg.str + "\"", PLACE(), "Log.txt");
        return TestData();
    }

    // �������� ������, ������� ������, ���� �������.
	std::string p_state_initial    = utils::FindStrRegular( p_state, p_state_initial_reg, dummy, err, "" );			TRACE_RETURN( err, err_message, TestData() );
	std::string p_state_resulted   = utils::FindStrRegular( p_state, p_state_resulted_reg, dummy, err, "" );		TRACE_RETURN( err, err_message, TestData() );
	std::string s_state_initial    = utils::FindStrRegular( s_state, s_state_initial_reg, dummy, err, "" );			TRACE_RETURN( err, err_message, TestData() );
	std::string s_state_resulted   = utils::FindStrRegular( s_state, s_state_resulted_reg, dummy, err, "" );		TRACE_RETURN( err, err_message, TestData() );
	std::string p_content_initial  = utils::FindStrRegular( p_content, p_content_initial_reg, dummy, err, "" );		TRACE_RETURN( err, err_message, TestData() );
	std::string p_content_resulted = utils::FindStrRegular( p_content, p_content_resulted_reg, dummy, err, "" );	TRACE_RETURN( err, err_message, TestData() );
	std::string s_content_initial  = utils::FindStrRegular( s_content, s_content_initial_reg, dummy, err, "" );		TRACE_RETURN( err, err_message, TestData() );
	std::string s_content_resulted = utils::FindStrRegular( s_content, s_content_resulted_reg, dummy, err, "" );	TRACE_RETURN( err, err_message, TestData() );
	std::string intermediate_reg   = utils::FindStrRegular( p_content, p_insertion_reg, dummy, err, "" );			TRACE_RETURN( err, err_message, TestData() );

    //  ���� ���-�� �� ������� - ������� ������.
    if (p_state_initial.empty() || p_state_resulted.empty() || s_state_initial.empty() || s_state_resulted.empty() || p_content_initial.empty() ||
        p_content_resulted.empty() || s_content_initial.empty() || s_content_resulted.empty() || intermediate_reg.empty())
    {
        err = std::make_shared<Error_Custom>(
            "Can't find description for:\t\"P-State initial\" or \"P-State resulted\" or \"S-State initial\" or \"S-State resulted\" or \"P-Content initial\""
            " or \"P-Content resulted\" or \"S-Content initial\" or \"S-Content resulted\" or \"P-Insertion\"\nIn a string:\t\"" + test_case + "\"\nBy regex:\t\"" +
            p_state_initial_reg.str + "\" or \"" + p_state_resulted_reg.str + "\" or \"" + s_state_initial_reg.str + "\" or \"" + 
            s_state_resulted_reg.str + "\" or \"" + p_content_initial_reg.str + "\" or \"" + p_content_resulted_reg.str + "\" or \"" + 
            s_content_initial_reg.str + "\" or \"" + s_content_resulted_reg.str + "\" or \"" + p_insertion_reg.str + "\"", PLACE(), "Log.txt" );
        return TestData();
    }

    TestData test_data;

    // TestData: ��������� �������� � �������������� ��������� P- � S-List
	err_message = "Can't get appropriate ListStates for P-List or S-List from string:\n\"" + test_case + "\"";
    test_data.p_listState_initial = ParseListState( p_state_initial, err );											TRACE_RETURN( err, err_message, TestData() );
    test_data.p_listState_resulted = ParseListState( p_state_resulted, err );										TRACE_RETURN( err, err_message, TestData() );
    test_data.s_listState_initial = ParseListState( s_state_initial, err );											TRACE_RETURN( err, err_message, TestData() );
    test_data.s_listState_resulted = ParseListState( s_state_resulted, err );										TRACE_RETURN( err, err_message, TestData() );

    // TestData: ��������� �������� � �������������� ������� P- � S-List
	err_message = "Can't get appropriate ListContents for P-List or S-List from string:\n\"" + test_case + "\"";
    test_data.p_listContent_initial = utils::RegionVector_from_String<RegionP, CELL>( p_content_initial, err );		TRACE_RETURN( err, err_message, TestData() );
    test_data.p_listContent_resulted = utils::RegionVector_from_String<RegionP, CELL>( p_content_resulted, err );	TRACE_RETURN( err, err_message, TestData() );
    test_data.s_listContent_initial = utils::RegionVector_from_String<RegionS, CELL>( s_content_initial, err );		TRACE_RETURN( err, err_message, TestData() );
    test_data.s_listContent_resulted = utils::RegionVector_from_String<RegionS, CELL>( s_content_resulted, err );	TRACE_RETURN( err, err_message, TestData() );

    // TestData: ��������� ������� � P-List � ���������� ������� � �������� � S-List
	err_message = "Can't get P-Insertion from string:\n\"" + test_case + "\"";
	test_data.intermediate_reg = utils::Region_from_String<RegionP, CELL>( intermediate_reg, err );					TRACE_RETURN( err, err_message, TestData() );

    return test_data;
}


/*___________________________________________________________________________________________________
 *       ������ ������ � ��������� ��������� ������, ����� ������� ListState.  ������ ������ �����: |
 *   C06,S05,L01,R00,B01,E06 (��������). �������� ����� �������� �������, ��������� ������ �����, �� |
 *   ������� ��������� CSLRBE �����������.                                                           |
 *__________________________________________________________________________________________________*/
ListState ParseListState( const std::string &list_state, Error_BasePtr &err )
{
    Regex C_reg( "C\\d+" );
    Regex S_reg( "S\\d+" );
    Regex L_reg( "L\\d+" );
    Regex R_reg( "R\\d+" );
    Regex B_reg( "B\\d+" );
    Regex E_reg( "E\\d+" );

    bool dummy;
	std::string err_message = "Can't proceed to parse a List-state string:\t\"" + list_state + "\"";
    std::string C = utils::FindStrRegular( list_state, C_reg, dummy, err, "" );				TRACE_RETURN( err, err_message, ListState() );
    std::string S = utils::FindStrRegular( list_state, S_reg, dummy, err, "" );				TRACE_RETURN( err, err_message, ListState() );
    std::string L = utils::FindStrRegular( list_state, L_reg, dummy, err, "" );				TRACE_RETURN( err, err_message, ListState() );
    std::string R = utils::FindStrRegular( list_state, R_reg, dummy, err, "" );				TRACE_RETURN( err, err_message, ListState() );
    std::string B = utils::FindStrRegular( list_state, B_reg, dummy, err, "" );				TRACE_RETURN( err, err_message, ListState() );
    std::string E = utils::FindStrRegular( list_state, E_reg, dummy, err, "" );				TRACE_RETURN( err, err_message, ListState() );

    // ���� ���-�� �� ������� - ������� ������.
    if ( C.empty() || S.empty() || L.empty() || R.empty() || B.empty() || E.empty() ) {
        err = std::make_shared<Error_Custom>(
            "Can't find the description for List-state parameter:\t\"C\" or \"S\" or \"L\" or \"R\" or \"B\" or \"E\"\nIn a string:\t\"" + list_state + "\"\nBy regex:\t\"" +
            C_reg.str + "\" or \"" + S_reg.str + "\" or \"" + L_reg.str + "\" or \"" + R_reg.str + "\" or \"" + B_reg.str + "\" or \"" + E_reg.str + "\"", PLACE(), "Log.txt" );
        return ListState();
    }

    return {
        (uint32_t)std::stoull(C.substr(1, C.size() - 1)), (uint32_t)std::stoull(S.substr(1, S.size() - 1)), (uint32_t)std::stoull(L.substr(1, L.size() - 1)),
        (uint32_t)std::stoull(R.substr(1, R.size() - 1)), (uint32_t)std::stoull(B.substr(1, B.size() - 1)), (uint32_t)std::stoull(E.substr(1, E.size() - 1))
    };
}


template<class ListType>
inline void rl_manip::SetState( const ListState& state, std::shared_ptr<RegionsList<CELL>> regList, Error_BasePtr& err )
{
	// ���������, ��� ��������������� ��������� - ���������
	rl_check::Validate_ListState( state, err );							TRACE_RETURN_VOID( err, "Can't pass ListState validation" );

	if constexpr (std::is_same_v<ListType, Region_P>)
	{
		// ����� ��������, �������, ��������� ����� ������ � ����� � P-List
		regList->m_p_list_size = state.size;
		regList->m_p_list_capacity = state.capacity;
		regList->m_p_list_spaceLeft = state.spaceLeft;
		regList->m_p_list_spaceRight = state.spaceRight;

		// �������� ������ ��� P-List ������, ��������������
		if (regList->m_p_list != nullptr) {
			free( regList->m_p_list );
		}
		regList->m_p_list = reinterpret_cast<Region_P*>(calloc( state.capacity, sizeof( Region_P ) ));
		memset( regList->m_p_list, 0, state.capacity * sizeof( Region_P ) );

		// ������ � ����� ��������� � P-List
		regList->m_p_list_begin = regList->m_p_list + state.spaceLeft;
		regList->m_p_list_end = regList->m_p_list_begin + state.size;
	}
	else if constexpr (std::is_same_v<ListType, Region_S>)
	{
		// ����� ��������, �������, ��������� ����� ������ � ����� � S-List
		regList->m_s_list_size = state.size;
		regList->m_s_list_capacity = state.capacity;
		regList->m_s_list_spaceLeft = state.spaceLeft;
		regList->m_s_list_spaceRight = state.spaceRight;

		// �������� ������ ��� S-List ������, ��������������
		if (regList->m_s_list != nullptr) {
			free( regList->m_s_list );
		}
		regList->m_s_list = reinterpret_cast<Region_S*>(calloc( state.capacity, sizeof( Region_S ) ));
		memset( regList->m_s_list, 0, state.capacity * sizeof( Region_S ) );

		// ������ � ����� ��������� � S-List
		regList->m_s_list_begin = regList->m_s_list + state.spaceLeft;
		regList->m_s_list_end = regList->m_s_list_begin + state.size;
	}
	else {
		err = std::make_shared<Error_Custom>( "Undefined ListType received: " + std::string( typeid(ListType).name() ), PLACE(), "Log.txt" );
	}
}


template<class ListType>
inline void rl_manip::SetContent( const std::vector<ListType>& content, std::shared_ptr<RegionsList<CELL>> regList, Error_BasePtr& err )
{
	ListType* it;
	ListType* end;

	if constexpr (std::is_same_v<ListType, Region_P>) {
		it = regList->m_p_list_begin;
		end = regList->m_p_list_end;
	}
	else if constexpr (std::is_same_v<ListType, Region_S>) {
		it = regList->m_s_list_begin;
		end = regList->m_s_list_end;
	}
	else {
		err = std::make_shared<Error_Custom>( "Undefined ListType received: " + std::string( typeid(ListType).name() ), PLACE(), "Log.txt" );
		return;
	}
	if (it > end) {
		err = std::make_shared<Error_Custom>( "Incorrect state of " + std::string( typeid(ListType).name() ) + "-List iterators (begin > end)", PLACE(), "Log.txt" );
		return;
	}
	if (content.size() != end - it) {
		err = std::make_shared<Error_Custom>(
			"Size of Content's vector to be set must match exactly with " + std::string( typeid(ListType).name() ) + "-List size [begin ... end].\nContent size is: " +
			std::to_string( content.size() ) + "\n" + std::string( typeid(ListType).name() ) + "-List size is: " + std::to_string( end - it ), PLACE(), "Log.txt" );
		return;
	}
	size_t pos = 0;
	for (; it != end; ++it, ++pos) {
		*it = content[pos];
	}
}


template<class ListType>
inline ListState rl_manip::GetState( std::shared_ptr<RegionsList<CELL>> regList )
{
	if constexpr (std::is_same_v<ListType, Region_P>) {
		return
		{
			regList->m_p_list_capacity,
			regList->m_p_list_size,
			regList->m_p_list_spaceLeft,
			regList->m_p_list_spaceRight,
			regList->m_p_list_spaceLeft,								// begin pos (not address!)
			regList->m_p_list_spaceLeft + regList->m_p_list_size		// end pos (not address!)
		};
	}
	else if constexpr (std::is_same_v<ListType, Region_S>) {
		return
		{
			regList->m_s_list_capacity,
			regList->m_s_list_size,
			regList->m_s_list_spaceLeft,
			regList->m_s_list_spaceRight,
			regList->m_s_list_spaceLeft,								// begin pos (not address!)
			regList->m_s_list_spaceLeft + regList->m_s_list_size		// end pos (not address!)
		};
	} else {
		return ListState();
	}
}


template<class ListType>
inline ListFootprints rl_manip::GetFootprints( std::shared_ptr<RegionsList<CELL>> regList )
{
	if constexpr (std::is_same_v<ListType, Region_P>) {
		return
		{
			reinterpret_cast<size_t>(regList->m_p_list),
			reinterpret_cast<size_t>(regList->m_p_list) + (regList->m_p_list_capacity * sizeof( Region_P )),
			reinterpret_cast<size_t>(regList->m_p_list_begin),
			reinterpret_cast<size_t>(regList->m_p_list_end)
		};
	}
	else if constexpr (std::is_same_v<ListType, Region_S>) {
		return
		{
			reinterpret_cast<size_t>(regList->m_s_list),
			reinterpret_cast<size_t>(regList->m_s_list) + (regList->m_s_list_capacity * sizeof( Region_S )),
			reinterpret_cast<size_t>(regList->m_s_list_begin),
			reinterpret_cast<size_t>(regList->m_s_list_end)
		};
	} else {
		return ListFootprints();
	}
}


template<class ListType>
inline std::vector<ListType> rl_manip::GetContent( std::shared_ptr<RegionsList<CELL>> regList, Error_BasePtr& err )
{
	std::vector<ListType> out;
	ListType* it;
	ListType* end;

	if constexpr (std::is_same_v<ListType, Region_P>) {
		it = regList->m_p_list_begin;
		end = regList->m_p_list_end;
	}
	else if constexpr (std::is_same_v<ListType, Region_S>) {
		it = regList->m_s_list_begin;
		end = regList->m_s_list_end;
	}
	else {
		err = std::make_shared<Error_Custom>( "Undefined ListType received: " + std::string( typeid(ListType).name() ), PLACE(), "Log.txt" );
		return;
	}
	if (it > end) {
		err = std::make_shared<Error_Custom>( "Incorrect state of " + std::string( typeid(ListType).name() ) + "-List iterators (begin > end)", PLACE(), "Log.txt" );
		return out;
	}
	for (; it != end; ++it) {
		out.push_back( *it );
	}
	return out;
}



ListState rl_check::Validate_ListState( const ListState& state, Error_BasePtr& err )
{
	std::string errMess( "Invalid ListState field(s):" );
	bool ok = true;
	if (state.capacity < state.size) {
		errMess += "\nListState.capacity < ListState.size";
		ok = false;
	}
	if (state.spaceLeft + state.spaceRight + state.size != state.capacity) {
		errMess += "\nListState.spaceLeft + ListState.spaceRight + ListState.size != ListState.capacity";
		ok = false;
	}
	if (state.begin_pos != state.spaceLeft) {
		errMess += "\nListState.begin_pos != ListState.spaceLeft";
		ok = false;
	}
	if (state.end_pos != state.spaceLeft + state.size) {
		errMess += "\nListState.end_pos != ListState.spaceLeft + ListState.size";
		ok = false;
	}
	if (!ok) {
		errMess += "\n" + utils::to_string( state );
		err = std::make_shared<Error_Custom>( errMess, PLACE(), "Log.txt" );
	}
	return state;
}


bool rl_check::CheckState( const ListState& expected, const ListState& gained, Error_BasePtr& err )
{
	if (expected != gained) {
		err = std::make_shared<Error_Custom>( "Incorrect ListState:\nExpected:\n" + utils::to_string( expected ) + "\nGained:\n" + utils::to_string( gained ), PLACE(), "Log.txt" );
		return false;
	}
	return true;
}


template<class ListType>
bool rl_check::CheckContent( const std::vector<ListType>& expected, std::vector<ListType>& gained, Error_BasePtr& err )
{
	if (expected.size() != gained.size()) {
		err = std::make_shared<Error_Custom>( "Incorrect content size:\nExpected: " + std::to_string( expected.size() ) + "\nGained: " + std::to_string( gained.size() ), PLACE(), "Log.txt" );
		return false;
	}
	if constexpr (std::is_same_v<ListType, Region_P> || std::is_same_v<ListType, Region_S>)
	{
		bool ok = true;
		for (size_t i = 0; i < expected.size(); ++i) {
			ok &= expected[i] == gained[i];
		}
		if (!ok) {
			err = std::make_shared<Error_Custom>( "Incorrect content :\nExpected: " + utils::to_string( expected ) + "\nGained:   " + utils::to_string( gained ), PLACE(), "Log.txt" );
		}
		return ok;
	}
	else {
		err = std::make_shared<Error_Custom>( "Undefined ListType received: " + std::string( typeid(ListType).name() ), PLACE(), "Log.txt" );
		return false;
	}
}


template<class ListType>
void rl_check::CheckFootprintsVsState( const ListState& state, const ListFootprints& footpr, Error_BasePtr& err )
{
	std::string errMess( "Incorrect ListFootprints VS ListState combination:" );
	bool ok = true;

	if (footpr.addr_begin != footpr.addr_start + (state.spaceLeft * sizeof( ListType ))) {
		errMess += "\nListFootprints.addr_begin != ListFootprints.addr_start + (ListState.spaceLeft * sizeof( CELL ))";
		ok = false;
	}
	if (footpr.addr_end != footpr.addr_start + (state.spaceLeft * sizeof( ListType )) + (state.size * sizeof( ListType ))) {
		errMess += "\nListFootprints.addr_end != ListFootprints.addr_start + (ListState.spaceLeft * sizeof( CELL )) + (ListState.size * sizeof( CELL ))";
		ok = false;
	}
	if (footpr.addr_stop != footpr.addr_start + (state.spaceLeft * sizeof( ListType )) + (state.size * sizeof( ListType )) + (state.spaceRight * sizeof( ListType ))) {
		errMess += "\nListFootprints.addr_stop != ListFootprints.addr_start + (ListState.spaceLeft * sizeof( CELL )) + (ListState.size * sizeof( CELL )) + (ListState.spaceRight * sizeof( CELL ))";
		ok = false;
	}
	if ((footpr.addr_stop - footpr.addr_start) / sizeof( ListType ) != state.capacity) {
		errMess += "\n(ListFootprints.addr_stop - ListFootprints.addr_start) / sizeof( CELL ) != ListState.capacity";
		ok = false;
	}

	if (!ok)
	{
		errMess += "\n" + utils::to_string( state ) + "\n" + utils::to_string( footpr );
		err = std::make_shared<Error_Custom>( errMess, PLACE(), "Log.txt" );
	}
}