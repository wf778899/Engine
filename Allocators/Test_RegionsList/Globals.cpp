#include "Globals.h"


typedef Globals::Ins_complex Ins_complex;


/* ����� ������� */
const Ins_complex::InsertionPlace Ins_complex::InsertionPlaces[Regions_Count] = { Start_P1, P1_P2, P2_P3, P3_P4, P4_P5, P5_Finish };


/* ��������� ����� (���������� ��� P-List � S-List. ������ ���� - 1 RegionP ��� RegionS) */
const Ins_complex::FieldState Ins_complex::FieldStates[FieldStates_Count] = { L0_R0, L0_R1, L1_R0, L1_R1 };


/* ���� ������� � P-List */
const Ins_complex::P_InsertionType Ins_complex::P_InsertionTypes[P_InsertionTypes_Count] =
{
	Start_P1_Adj_None, Start_P1_Adj_R, 
	P1_P2_Adj_None, P1_P2_Adj_L, P1_P2_Adj_R, P1_P2_Adj_RL, P2_P3_Adj_None, P2_P3_Adj_L, P2_P3_Adj_R, P2_P3_Adj_RL,
	P3_P4_Adj_None, P3_P4_Adj_L, P3_P4_Adj_R, P3_P4_Adj_RL, P4_P5_Adj_None, P4_P5_Adj_L, P4_P5_Adj_R, P4_P5_Adj_RL, 
	P5_Finish_Adj_None, P5_Finish_Adj_L
};


/* ��� �������� � S-List (������� �� ���� ������� � P-List) */
const Ins_complex::S_ActionType Ins_complex::S_ActionTypes[S_ActionTypes_Count] =
{
	IR, IL, DR_IR, DR_IL, DL_IR, DL_IL, DL_DL_IR, DL_DL_IL, DL_DR_IR, DL_DR_IL, DR_DL_IR, DR_DL_IL, DR_DR_IR, DR_DR_IL,
};


/*_____________________________________________________________________________________________
*		�������  �������  ������,  ���  ��������  ���������� 5 ������������� �������� ������ |
*	������,	 ��������  ����������������  �����������  RegionsList.  ���������� ����� �������� |
*	�������� - 6 CELL. ������������ ������ �������, ��������������, 5 CELL.					  |
*			| | | | |0|-|-|-|-| |1|-|-|-|-| |2|-|-|-|-| |3|-|-|-|-| |4|-|-|-|-| | | | |		  |
*____________________________________________________________________________________________*/
CELL Ins_complex::mem[37];


/* ��������� �� ������ 5 ������������� �������� + ��������� �� ����� ������ */
CELL* const Ins_complex::regPtrs[Regions_Count] = { mem + 4, mem + 10, mem + 16, mem + 22, mem + 28, mem + 37 };


/*_____________________________________________________________________________________________
*		���������  �����  ���������  P-List  �� � �����  �������  �  ����������� �� ��������� |
*	��������� ����� � ���� ������� � P-List. �������� ������ ����������  �������������  ����� |
*	"ComplexInsertions" (��� ���������).													  |
*____________________________________________________________________________________________*/
const std::string Ins_complex::P_StatesFlowDesc[FieldStates_Count][P_InsertionTypes_Count] =
{
	{
		"P-State: |0|# [0]   [1]   [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C10,S06,L01,R03,B01,E07 ",
		"P-State: |0| #[0]   [1]   [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0] # [1]   [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C10,S06,L01,R03,B01,E07 ",
		"P-State: |0|  [0]#  [1]   [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]  #[1]   [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]###[1]   [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L01,R00,B01,E05 ",
		"P-State: |0|  [0]   [1] # [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C10,S06,L02,R02,B02,E08 ",
		"P-State: |0|  [0]   [1]#  [2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]   [1]  #[2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]   [1]###[2]   [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L00,R01,B00,E04 ",
		"P-State: |0|  [0]   [1]   [2] # [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C10,S06,L02,R02,B02,E08 ",
		"P-State: |0|  [0]   [1]   [2]#  [3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]  #[3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]###[3]   [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L00,R01,B00,E04 ",
		"P-State: |0|  [0]   [1]   [2]   [3] # [4]  |0| C05,S05,L00,R00,B00,E05 -> C10,S06,L02,R02,B02,E08 ",
		"P-State: |0|  [0]   [1]   [2]   [3]#  [4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]   [3]  #[4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]   [3]###[4]  |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L00,R01,B00,E04 ",
		"P-State: |0|  [0]   [1]   [2]   [3]   [4] #|0| C05,S05,L00,R00,B00,E05 -> C10,S06,L02,R02,B02,E08 ",
		"P-State: |0|  [0]   [1]   [2]   [3]   [4]# |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 "
	},
	{
		"P-State: |0|# [0]   [1]   [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C12,S06,L02,R04,B02,E08 ",
		"P-State: |0| #[0]   [1]   [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0] # [1]   [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C12,S06,L02,R04,B02,E08 ",
		"P-State: |0|  [0]#  [1]   [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]  #[1]   [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]###[1]   [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L01,R01,B01,E05 ",
		"P-State: |0|  [0]   [1] # [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S06,L00,R00,B00,E06 ",
		"P-State: |0|  [0]   [1]#  [2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]   [1]  #[2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]   [1]###[2]   [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L00,R02,B00,E04 ",
		"P-State: |0|  [0]   [1]   [2] # [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S06,L00,R00,B00,E06 ",
		"P-State: |0|  [0]   [1]   [2]#  [3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]  #[3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]###[3]   [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L00,R02,B00,E04 ",
		"P-State: |0|  [0]   [1]   [2]   [3] # [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S06,L00,R00,B00,E06 ",
		"P-State: |0|  [0]   [1]   [2]   [3]#  [4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]   [3]  #[4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"P-State: |0|  [0]   [1]   [2]   [3]###[4]  |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L00,R02,B00,E04 ",
		"P-State: |0|  [0]   [1]   [2]   [3]   [4] #|1| C06,S05,L00,R01,B00,E05 -> C06,S06,L00,R00,B00,E06 ",
		"P-State: |0|  [0]   [1]   [2]   [3]   [4]# |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 "
	},
	{
		"P-State: |1|# [0]   [1]   [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S06,L00,R00,B00,E06 ",
		"P-State: |1| #[0]   [1]   [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0] # [1]   [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S06,L00,R00,B00,E06 ",
		"P-State: |1|  [0]#  [1]   [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]  #[1]   [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]###[1]   [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L02,R00,B02,E06 ",
		"P-State: |1|  [0]   [1] # [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C12,S06,L03,R03,B03,E09 ",
		"P-State: |1|  [0]   [1]#  [2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]   [1]  #[2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]   [1]###[2]   [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L01,R01,B01,E05 ",
		"P-State: |1|  [0]   [1]   [2] # [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C12,S06,L03,R03,B03,E09 ",
		"P-State: |1|  [0]   [1]   [2]#  [3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]  #[3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]###[3]   [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L01,R01,B01,E05 ",
		"P-State: |1|  [0]   [1]   [2]   [3] # [4]  |0| C06,S05,L01,R00,B01,E06 -> C12,S06,L03,R03,B03,E09 ",
		"P-State: |1|  [0]   [1]   [2]   [3]#  [4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]   [3]  #[4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]   [3]###[4]  |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L01,R01,B01,E05 ",
		"P-State: |1|  [0]   [1]   [2]   [3]   [4] #|0| C06,S05,L01,R00,B01,E06 -> C12,S06,L03,R03,B03,E09 ",
		"P-State: |1|  [0]   [1]   [2]   [3]   [4]# |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 "
	},
	{
		"P-State: |1|# [0]   [1]   [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S06,L00,R01,B00,E06 ",
		"P-State: |1| #[0]   [1]   [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0] # [1]   [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S06,L00,R01,B00,E06 ",
		"P-State: |1|  [0]#  [1]   [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]  #[1]   [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]###[1]   [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L02,R01,B02,E06 ",
		"P-State: |1|  [0]   [1] # [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S06,L01,R00,B01,E07 ",
		"P-State: |1|  [0]   [1]#  [2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]   [1]  #[2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]   [1]###[2]   [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L01,R02,B01,E05 ",
		"P-State: |1|  [0]   [1]   [2] # [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S06,L01,R00,B01,E07 ",
		"P-State: |1|  [0]   [1]   [2]#  [3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]  #[3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]###[3]   [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L01,R02,B01,E05 ",
		"P-State: |1|  [0]   [1]   [2]   [3] # [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S06,L01,R00,B01,E07 ",
		"P-State: |1|  [0]   [1]   [2]   [3]#  [4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]   [3]  #[4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"P-State: |1|  [0]   [1]   [2]   [3]###[4]  |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L01,R02,B01,E05 ",
		"P-State: |1|  [0]   [1]   [2]   [3]   [4] #|1| C07,S05,L01,R01,B01,E06 -> C07,S06,L01,R00,B01,E07 ",
		"P-State: |1|  [0]   [1]   [2]   [3]   [4]# |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 "
	}
};


/*____________________________________________________________________________________________
*		���������  �����  ���������  S-List  �� � �����  �������  �  ����������� �� ��������� |
*	��������� ����� � ���� �������� � S-List. �������� ������ ���������� �������������  ����� |
*	"ComplexInsertions" (��� ���������).													  |
*____________________________________________________________________________________________*/
const std::string Ins_complex::S_StatesFlowDesc[FieldStates_Count][S_ActionTypes_Count] =
{
	{
		"S-State: |0|           I1-> |0| C05,S05,L00,R00,B00,E05 -> C10,S06,L02,R02,B02,E08 ",
		"S-State: |0|           I1<- |0| C05,S05,L00,R00,B00,E05 -> C10,S06,L01,R03,B01,E07 ",
		"S-State: |0| D1->      I1-> |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"S-State: |0| D1->      I1<- |0| C05,S05,L00,R00,B00,E05 -> C10,S05,L02,R03,B02,E07 ",
		"S-State: |0| D1<-      I1-> |0| C05,S05,L00,R00,B00,E05 -> C10,S05,L03,R02,B03,E08 ",
		"S-State: |0| D1<-      I1<- |0| C05,S05,L00,R00,B00,E05 -> C05,S05,L00,R00,B00,E05 ",
		"S-State: |0| D1<- D2<- I1-> |0| C05,S05,L00,R00,B00,E05 -> C10,S04,L03,R03,B03,E07 ",
		"S-State: |0| D1<- D2<- I1<- |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L01,R00,B01,E05 ",
		"S-State: |0| D1<- D2-> I1-> |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L01,R00,B01,E05 ",
		"S-State: |0| D1<- D2-> I1<- |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L00,R01,B00,E04 ",
		"S-State: |0| D1-> D2<- I1-> |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L01,R00,B01,E05 ",
		"S-State: |0| D1-> D2<- I1<- |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L00,R01,B00,E04 ",
		"S-State: |0| D1-> D2-> I1-> |0| C05,S05,L00,R00,B00,E05 -> C05,S04,L00,R01,B00,E04 ",
		"S-State: |0| D1-> D2-> I1<- |0| C05,S05,L00,R00,B00,E05 -> C10,S04,L02,R04,B02,E06 "
	},
	{
		"S-State: |0|           I1-> |1| C06,S05,L00,R01,B00,E05 -> C06,S06,L00,R00,B00,E06 ",
		"S-State: |0|           I1<- |1| C06,S05,L00,R01,B00,E05 -> C12,S06,L02,R04,B02,E08 ",
		"S-State: |0| D1->      I1-> |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"S-State: |0| D1->      I1<- |1| C06,S05,L00,R01,B00,E05 -> C12,S05,L03,R04,B03,E08 ",
		"S-State: |0| D1<-      I1-> |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L01,R00,B01,E06 ",
		"S-State: |0| D1<-      I1<- |1| C06,S05,L00,R01,B00,E05 -> C06,S05,L00,R01,B00,E05 ",
		"S-State: |0| D1<- D2<- I1-> |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L02,R00,B02,E06 ",
		"S-State: |0| D1<- D2<- I1<- |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L01,R01,B01,E05 ",
		"S-State: |0| D1<- D2-> I1-> |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L01,R01,B01,E05 ",
		"S-State: |0| D1<- D2-> I1<- |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L00,R02,B00,E04 ",
		"S-State: |0| D1-> D2<- I1-> |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L01,R01,B01,E05 ",
		"S-State: |0| D1-> D2<- I1<- |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L00,R02,B00,E04 ",
		"S-State: |0| D1-> D2-> I1-> |1| C06,S05,L00,R01,B00,E05 -> C06,S04,L00,R02,B00,E04 ",
		"S-State: |0| D1-> D2-> I1<- |1| C06,S05,L00,R01,B00,E05 -> C12,S04,L03,R05,B03,E07 "
	},
	{
		"S-State: |1|           I1-> |0| C06,S05,L01,R00,B01,E06 -> C12,S06,L03,R03,B03,E09 ",
		"S-State: |1|           I1<- |0| C06,S05,L01,R00,B01,E06 -> C06,S06,L00,R00,B00,E06 ",
		"S-State: |1| D1->      I1-> |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"S-State: |1| D1->      I1<- |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L00,R01,B00,E05 ",
		"S-State: |1| D1<-      I1-> |0| C06,S05,L01,R00,B01,E06 -> C12,S05,L04,R03,B04,E09 ",
		"S-State: |1| D1<-      I1<- |0| C06,S05,L01,R00,B01,E06 -> C06,S05,L01,R00,B01,E06 ",
		"S-State: |1| D1<- D2<- I1-> |0| C06,S05,L01,R00,B01,E06 -> C12,S04,L04,R04,B04,E08 ",
		"S-State: |1| D1<- D2<- I1<- |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L02,R00,B04,E06 ",
		"S-State: |1| D1<- D2-> I1-> |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L02,R00,B02,E06 ",
		"S-State: |1| D1<- D2-> I1<- |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L01,R01,B01,E05 ",
		"S-State: |1| D1-> D2<- I1-> |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L02,R00,B02,E06 ",
		"S-State: |1| D1-> D2<- I1<- |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L01,R01,B01,E05 ",
		"S-State: |1| D1-> D2-> I1-> |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L01,R01,B01,E05 ",
		"S-State: |1| D1-> D2-> I1<- |0| C06,S05,L01,R00,B01,E06 -> C06,S04,L00,R02,B00,E04 "
	},
	{
		"S-State: |1|           I1-> |1| C07,S05,L01,R01,B01,E06 -> C07,S06,L01,R00,B01,E07 ",
		"S-State: |1|           I1<- |1| C07,S05,L01,R01,B01,E06 -> C07,S06,L00,R01,B00,E06 ",
		"S-State: |1| D1->      I1-> |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"S-State: |1| D1->      I1<- |1| C07,S05,L01,R00,B01,E06 -> C07,S05,L00,R02,B00,E05 ",
		"S-State: |1| D1<-      I1-> |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L02,R00,B02,E07 ",
		"S-State: |1| D1<-      I1<- |1| C07,S05,L01,R01,B01,E06 -> C07,S05,L01,R01,B01,E06 ",
		"S-State: |1| D1<- D2<- I1-> |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L03,R00,B03,E07 ",
		"S-State: |1| D1<- D2<- I1<- |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L02,R01,B02,E06 ",
		"S-State: |1| D1<- D2-> I1-> |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L02,R01,B02,E06 ",
		"S-State: |1| D1<- D2-> I1<- |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L01,R02,B01,E05 ",
		"S-State: |1| D1-> D2<- I1-> |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L02,R01,B02,E06 ",
		"S-State: |1| D1-> D2<- I1<- |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L01,R02,B01,E05 ",
		"S-State: |1| D1-> D2-> I1-> |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L01,R02,B01,E05 ",
		"S-State: |1| D1-> D2-> I1<- |1| C07,S05,L01,R01,B01,E06 -> C07,S04,L00,R03,B00,E04 "
	}
};

/*_________________________________________________________________________________________________
 *		���������� ��� ����������� �������� � S-List.											   |
 *________________________________________________________________________________________________*/
Ins_complex::S_ActionType Ins_complex::SListActionDetermination( Side ins, Side del1, Side del2 )
{
    if (del1 == Side_NONE && del2 == Side_NONE && ins == Side_RIGHT)
        return Ins_complex::IR;
    else if (del1 == Side_NONE && del2 == Side_NONE && ins == Side_LEFT)
        return Ins_complex::IL;
    else if (del1 == Side_RIGHT && del2 == Side_NONE && ins == Side_RIGHT)
        return Ins_complex::DR_IR;
    else if (del1 == Side_RIGHT && del2 == Side_NONE && ins == Side_LEFT)
        return Ins_complex::DR_IL;
    else if (del1 == Side_LEFT && del2 == Side_NONE && ins == Side_RIGHT)
        return Ins_complex::DL_IR;
    else if (del1 == Side_LEFT && del2 == Side_NONE && ins == Side_LEFT)
        return Ins_complex::DL_IL;
    else if (del1 == Side_LEFT && del2 == Side_LEFT && ins == Side_RIGHT)
        return Ins_complex::DL_DL_IR;
    else if (del1 == Side_LEFT && del2 == Side_LEFT && ins == Side_LEFT)
        return Ins_complex::DL_DL_IL;
    else if (del1 == Side_LEFT && del2 == Side_RIGHT && ins == Side_RIGHT)
        return Ins_complex::DL_DR_IR;
    else if (del1 == Side_LEFT && del2 == Side_RIGHT && ins == Side_LEFT)
        return Ins_complex::DL_DR_IL;
    else if (del1 == Side_RIGHT && del2 == Side_LEFT && ins == Side_RIGHT)
        return Ins_complex::DR_DL_IR;
    else if (del1 == Side_RIGHT && del2 == Side_LEFT && ins == Side_LEFT)
        return Ins_complex::DR_DL_IL;
    else if (del1 == Side_RIGHT && del2 == Side_RIGHT && ins == Side_RIGHT)
        return Ins_complex::DR_DR_IR;
    else if (del1 == Side_RIGHT && del2 == Side_RIGHT && ins == Side_LEFT)
        return Ins_complex::DR_DR_IL;
    else 
        return Ins_complex::S_ActionTypes_Count;
}