#pragma once
#include "Types.h"

#include <stdint.h>
#include <string>
#include <map>
#include <vector>


struct TestCell
{
	uint32_t	field_1;
	uint32_t	field_2;
	float		field_3;
	uint16_t	field_4;
	char		field_5;
	char		field_6;
};

struct DescriptorHandler
{
	uint64_t desc_handle;
	uint64_t index_in_heap;
};

/*����������������� ������ ���*/

//using CELL = TestCell;
//using CELL = DescriptorHandler;
using CELL = uint8_t;


class Globals
{
public:
   /*_________________________________________________________________________________________________
	*		����  �  ����������  ��� ��������� �������� ����� "ComplexInsertions" � �� ����������. �� |
	*	�������� ������� �����, RegionList ���������� �������� 5 �������������  ��������.  �� ������ |
	*	�����������  ��  ����  ���������  �����������,  �����������  ������������  �������� ��������� |
	*	RegionList. ��������� ���������  �����  RegionList  �����  �������� - ��. FieldState. ������� |
	*	������������ � ��������� ����� ��/�����/����� ��������, ������ � ����� ������� ����� �������� |
	*	�� ���� ��������� ���������.																  |
	*		�.�. ���� ���������� ��� ��������� ���������� ����� ���������:							  |
	*			- �������� ��������� ������ (��� ���������� �������� �� ������, ��������� �����)	  |
	*			- ������ ������� (����������� ������ ��������� ������ ����� ���������)				  |
	*			- ������ ������� (��� ��������, ������������ ������� ������� � �������� ������)		  |
	*		����� ���������� ����� - 750 000.														  |
	*________________________________________________________________________________________________*/
	class Ins_complex
	{
	public:
		/* ����� ������� */
		enum InsertionPlace
		{
			Start_P1,	// �� ������� �������
			P1_P2,		// ����� ���������
			P2_P3,		// ...
			P3_P4,		// ...
			P4_P5,		// ...
			P5_Finish,	// ����� ���������� �������
			Regions_Count,
		};
		static const InsertionPlace InsertionPlaces[Regions_Count];


		/* ��������� ����� (���������� ��� P-List � S-List. ������ ���� - 1 RegionP ��� RegionS) */
		enum FieldState
		{
			L0_R0,		// ��� �����
			L0_R1,		// ���� ���� ������
			L1_R0,		// ���� ���� �����
			L1_R1,		// ���� ��� ����
			FieldStates_Count
		};
		static const FieldState FieldStates[FieldStates_Count];


		/* ��� ������� � P-List */
		enum P_InsertionType
		{
			Start_P1_Adj_None,	// � ������, ���������
			Start_P1_Adj_R,		// � ������, �������
			P1_P2_Adj_None,		// ����� 1 � 2 ���������, ���������
			P1_P2_Adj_L,		// ����� 1 � 2 ���������, ������� � ����� (1)
			P1_P2_Adj_R,		// ����� 1 � 2 ���������, ������� � ������ (2)
			P1_P2_Adj_RL,		// ����� 1 � 2 ���������, ������� � ����� � ������ (1 � 2)
			P2_P3_Adj_None,		// ����� 2 � 3 ���������, ���������
			P2_P3_Adj_L,		// ����� 2 � 3 ���������, ������� � ����� (2)
			P2_P3_Adj_R,		// ����� 2 � 3 ���������, ������� � ������ (3)
			P2_P3_Adj_RL,		// ����� 2 � 3 ���������, ������� � ����� � ������ (2 � 3)
			P3_P4_Adj_None,		// ����� 3 � 4 ���������, ���������
			P3_P4_Adj_L,		// ����� 3 � 4 ���������, ������� � ����� (3)
			P3_P4_Adj_R,		// ����� 3 � 4 ���������, ������� � ������ (4)
			P3_P4_Adj_RL,		// ����� 3 � 4 ���������, ������� � ����� � ������ (3 � 4)
			P4_P5_Adj_None,		// ����� 4 � 5 ���������, ���������
			P4_P5_Adj_L,		// ����� 4 � 5 ���������, ������� � ����� (4)
			P4_P5_Adj_R,		// ����� 4 � 5 ���������, ������� � ������ (5)
			P4_P5_Adj_RL,		// ����� 4 � 5 ���������, ������� � ����� � ������ (4 � 5)
			P5_Finish_Adj_None, // � �����, ���������
			P5_Finish_Adj_L,	// � �����, �������
			P_InsertionTypes_Count
		};
		static const P_InsertionType P_InsertionTypes[P_InsertionTypes_Count];


		/* ��� �������� � S-List (������� �� ���� ������� � P-List) */
		enum S_ActionType
		{
			IR,			// ������� ������
			IL,			// ������� �����
			DR_IR,		// �������� ������, ������� ������
			DR_IL,		// �������� ������, ������� �����
			DL_IR,		// �������� �����, ������� ������
			DL_IL,		// �������� �����, ������� �����
			DL_DL_IR,	// 2 �������� �����, ������� ������
			DL_DL_IL,	// 2 �������� �����, ������� �����
			DL_DR_IR,	// �������� ����� � ������, ������� ������
			DL_DR_IL,	// �������� ����� � ������, ������� �����
			DR_DL_IR,	// �������� ������ � �����, ������� ������
			DR_DL_IL,	// �������� ������ � �����, ������� �����
			DR_DR_IR,	// 2 �������� ������, ������� ������
			DR_DR_IL,	// 2 �������� ������, ������� �����
			S_ActionTypes_Count
		};
		static const S_ActionType S_ActionTypes[S_ActionTypes_Count];


	   /*_____________________________________________________________________________________________
		*		�������  �������  ������,  ���  ��������  ���������� 5 ������������� �������� ������ |
		*	������,	 ��������  ����������������  �����������  RegionsList.  ���������� ����� �������� |
		*	�������� - 6 CELL. ������������ ������ �������, ��������������, 5 CELL.			  |
		*			| | | | |0|-|-|-|-| |1|-|-|-|-| |2|-|-|-|-| |3|-|-|-|-| |4|-|-|-|-| | | | |		  |
		*____________________________________________________________________________________________*/
		static CELL mem[37];


	   /*_____________________________________________________________________________________________
		*		��������� �� ������ 5 ������������� �������� + ��������� �� ����� ������			  |
		*____________________________________________________________________________________________*/
		static CELL* const regPtrs[Regions_Count];


	   /*_____________________________________________________________________________________________
	    *		���������  �����  ���������  P-List  �� � �����  �������  �  ����������� �� ��������� |
		*	��������� ����� � ���� ������� � P-List. �������� ������ ����������  �������������  ����� |
		*	"ComplexInsertions" (��� ���������).													  |
		*____________________________________________________________________________________________*/
		static const std::string P_StatesFlowDesc[FieldStates_Count][P_InsertionTypes_Count];


		/*____________________________________________________________________________________________
		*		���������  �����  ���������  S-List  �� � �����  �������  �  ����������� �� ��������� |
		*	��������� ����� � ���� �������� � S-List. �������� ������ ���������� �������������  ����� |
		*	"ComplexInsertions" (��� ���������).													  |
		*____________________________________________________________________________________________*/
		static const std::string S_StatesFlowDesc[FieldStates_Count][S_ActionTypes_Count];


        /*____________________________________________________________________________________________
        *		���������� ��� ����������� �������� � S-List.									      |
        *____________________________________________________________________________________________*/
        static S_ActionType SListActionDetermination( Side ins, Side del1, Side del2 );
	};
};
