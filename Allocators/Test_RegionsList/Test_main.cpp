#include "RegionsList_Tester.h"
#include "LogDebug.h"
#include "LogError.h"
#include "LogWarning.h"
#include "LogInfo.h"
#include "LogTest.h"

#include <memory>
#include <mutex>
#include <thread>
#include <iostream>
#include <set>

int main(int argc, const char** argv)
{
	std::set<std::string> args;
	for (int i = 1; i < argc; ++i) {
		args.insert( argv[i] );
	}

	if (args.find( "--help" ) != args.end()) {
		std::cout << "Usage: Test_RegionsList [ testcase name ]. Possible names are:\n" \
			"--DoubleInserion\n--OverlappedInsertion\n--GrabbingFromEmptyList\n--GrabbingTooBigRegion\n--ListsExpanding\n--InitialReleaseRegion\n--SecondRelease_LeftRightAdj\n" \
			"--SecondRelease_Left_Less_Size\n--SecondRelease_Left_Great_Size\n--SecondRelease_Left_Equal_Size\n--SecondRelease_Right_Less_Size\n--SecondRelease_Right_Great_Size\n" \
			"--SecondRelease_Right_Equal_Size\n--GrabbingFromSingleSizedList\n--GrabingssComplex\n--InsertionsComplex\nIf no names specified - all the testcases will be executed.";
		return 0;
	}
	bool DoubleInserion = args.find( "--DoubleInserion" ) != args.end();
	bool OverlappedInsertion = args.find( "--OverlappedInsertion" ) != args.end();
	bool GrabbingFromEmptyList = args.find( "--GrabbingFromEmptyList" ) != args.end();
	bool GrabbingTooBigRegion = args.find( "--GrabbingTooBigRegion" ) != args.end();
	bool ListsExpanding = args.find( "--ListsExpanding" ) != args.end();
	bool InitialReleaseRegion = args.find( "--InitialReleaseRegion" ) != args.end();
	bool SecondRelease_LeftRightAdj = args.find( "--SecondRelease_LeftRightAdj" ) != args.end();
	bool SecondRelease_Left_Less_Size = args.find( "--SecondRelease_Left_Less_Size" ) != args.end();
	bool SecondRelease_Left_Great_Size = args.find( "--SecondRelease_Left_Great_Size" ) != args.end();
	bool SecondRelease_Left_Equal_Size = args.find( "--SecondRelease_Left_Equal_Size" ) != args.end();
	bool SecondRelease_Right_Less_Size = args.find( "--SecondRelease_Right_Less_Size" ) != args.end();
	bool SecondRelease_Right_Great_Size = args.find( "--SecondRelease_Right_Great_Size" ) != args.end();
	bool SecondRelease_Right_Equal_Size = args.find( "--SecondRelease_Right_Equal_Size" ) != args.end();
	bool GrabbingFromSingleSizedList = args.find( "--GrabbingFromSingleSizedList" ) != args.end();
	bool GrabingssComplex = args.find( "--GrabingssComplex" ) != args.end();	
	bool InsertionsComplex = args.find( "--InsertionsComplex" ) != args.end();

	std::unique_ptr<RegionsList_Tester> tester = std::make_unique<RegionsList_Tester>();

	if (DoubleInserion || argc == 1) {
		tester->Test_DoubleInserion();
	}
	if (OverlappedInsertion || argc == 1) {
		tester->Test_OverlappedInsertion();
	}
	if (GrabbingFromEmptyList || argc == 1) {
		tester->Test_GrabbingFromEmptyList();
	}
	if (GrabbingTooBigRegion || argc == 1) {
		tester->Test_GrabbingTooBigRegion();
	}
	if (ListsExpanding || argc == 1) {
		tester->Test_ListsExpanding();
	}
	if (InitialReleaseRegion || argc == 1) {
		tester->Test_InitialReleaseRegion();
	}
	if (SecondRelease_LeftRightAdj || argc == 1) {
		tester->Test_SecondRelease_LeftRightAdj();
	}
	if (SecondRelease_Left_Less_Size || argc == 1) {
		tester->Test_SecondRelease_Left_Less_Size();
	}
	if (SecondRelease_Left_Great_Size || argc == 1) {
		tester->Test_SecondRelease_Left_Great_Size();
	}
	if (SecondRelease_Left_Equal_Size || argc == 1) {
		tester->Test_SecondRelease_Left_Equal_Size();
	}
	if (SecondRelease_Right_Less_Size || argc == 1) {
		tester->Test_SecondRelease_Right_Less_Size();
	}
	if (SecondRelease_Right_Great_Size || argc == 1) {
		tester->Test_SecondRelease_Right_Great_Size();
	}
	if (SecondRelease_Right_Equal_Size || argc == 1) {
		tester->Test_SecondRelease_Right_Equal_Size();
	}
	if (GrabbingFromSingleSizedList || argc == 1) {
		tester->Test_GrabbingFromSingleSizedList();
	}
	if (GrabingssComplex || argc == 1) {
		tester->Test_GrabbingsComplex();
	}
	if (InsertionsComplex || argc == 1) {
		tester->Test_InsertionsComplex();
	}

	system( "Pause" );
	return 0;
}







