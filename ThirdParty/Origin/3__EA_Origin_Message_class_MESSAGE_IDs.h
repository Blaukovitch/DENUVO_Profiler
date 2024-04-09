/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#pragma once
//******************************************//
//ID (�����) ��������� �����������/����������� ��������� �� Origin CORE
//******************************************//

#include "3__EA_Origin_Message_class.h"
#include "3__EA_Origin_STUB.h"

#define defs_ElectronicArts_LosAngeles_watermark "EALS" //ID ���������� - Eletronic Arts Los Angeles
#define defs_Message_event "Event" //Message 'Event'
#define defs_Request_event "Request" //Message 'Request'
#define defs_Response_event "Response" //Message 'Response'

#define EA_CRYPTO_SIGNATURE_1 0x34123549 //��������� XOR �1
#define EA_CRYPTO_SIGNATURE_2 0x5BD1E995 //��������� XOR �2

#define DENUVO_HIDDEN_CONST_1 0x5A2566BE //������� ��������� 1

#define MD5_BYTE_LEN 32 //����� ������ MD5
#define MD5_STR_LEN MD5_BYTE_LEN/2 //����� ������ MD5

#define SPRINT_F_HEX_TO_STR "%02x"

#define MAX_BUILD_CHARS_LEN 15 //���������� ����� ������, ��� ���������� � ������� ����� ������ ��� ��������� ��������������
#define MAX_BUILD_CHARS_with_ZERO_LEN MAX_BUILD_CHARS_LEN+sizeof('\0')

#define MAX_WAITING_TIME 15000u

/*

(ID)___(EALS)-----------------------------------------------------------------__---------------------------------------------------------------(Origin CORE)
        wsapi.connect													      >> 
									                                          << [Event sender] '�����������' (��������� ����, ������ Origin CORE, ����)
01     [ChallengeResponse]����� �� �����������(response,IDs, ����, ver.SDK *) >>
01                                                                            << [ChallengeAccepted] �������� ������ (response)
------------------------------------------------------------------------------__---------------------------------------------------------------------
	* Origin CORE != ������ SDK
*/

union ucount
{
	volatile unsigned __int32 HI;
	volatile unsigned __int32 LO;
};

typedef struct struct_EA_MT_SIGNAL
{
	/*
	hSem            dq ? ; offset
	00000008 count_HI        dd ?
	0000000C count_LO        dd ?
	00000010 tick_byte1      db ?
	00000011 -      db ?
	00000012 field_12        db ?
	00000013 field_13        db ?
	00000014 initial         db ?
	00000015 -        db ?
	00000016 -        db ?
	00000017 -        db ?
	00000018 CRITICAL        CRITICAL_SECTION ?
	00000040 sema            ends
	*/

	//����� ��������
	HANDLE hSem;
	//���������� ���������?
	ucount count;
	//����� ��������
	size_t ticks;
	//���� �������������
	bool initial;
	//����������� ������
	CRITICAL_SECTION Crit_section_signal;
}EA_MT_SIGNAL, *pEA_MT_SIGNAL; //��������� ������������ ��������������

typedef struct EA_MESSAGE_UNIT_FULL
{
	size_t MAX_strlen; //�����������-���������� ����� ��������� ��� ���������� � ��������� MAX_BUILD_CHARS_LEN (�����-��������� ������ malloc)
	char string[MAX_BUILD_CHARS_with_ZERO_LEN]; //������
	size_t len; //����� ������ (strlen)
}S_EA_M_UNIT, *pS_EA_M_UNIT;

//std::string ��������
//����� �1 (�����������)
typedef struct EA_MESSAGE_ANSWER0_EVENT_HEAD_POOL
{
	LPCVOID Virtual_proc_1; //���� ����������� ������� EA_Message_ops

								        //���� ���������	<< (��������)//
	char seance_key_2[MAX_BUILD_CHARS_with_ZERO_LEN]; //������ �� ���������� ���������� "key" (�������� ������ head - ��������� ���� �� Origin CORE)
	//LPVOID heap_alloc_3; //������ �� ������ (EA_Probe)
	size_t key_len_4; //����� ����������� char* key_1;

	size_t max_version_size_5; //������������ ������ ������ ��� "version" = MAX_BUILD_CHARS_LEN
	char version_6[MAX_BUILD_CHARS_with_ZERO_LEN]; //���������� ���������� "version" (�������� ������ head - ������ Origin CORE)
	size_t version_len_8; //����� ����������� char version_6[16]

	size_t max_build_size_9; //������������ ������ ������ ��� "build" = MAX_BUILD_CHARS_LEN
	char build_10[MAX_BUILD_CHARS_with_ZERO_LEN]; //���������� ���������� "build" (�������� ������ head - ���� Origin CORE)
	size_t build_len_12; //����� ����������� char build_9[16]

	size_t max_Event_size_13; //������������ ������ ������ ��� "Event" = MAX_BUILD_CHARS_LEN
	char event_14[MAX_BUILD_CHARS_with_ZERO_LEN]; //XML-tag Event
	size_t event_len_16; //����� ����������� char event_12[16]. �� ������� ��������

	size_t max_EALS_size_17; //������������ ������ ������ ��� ���������� "EALS" = MAX_BUILD_CHARS_LEN
	char EALS_18[MAX_BUILD_CHARS_with_ZERO_LEN]; //���������� ���������� EALS
	size_t EALS_len_20; //����� ����������� char EALS_15[16]. �� ������� ��������

	size_t max_reserved1_size_21; //������������ ������ ������ reserved1
	char reserved1_22[MAX_BUILD_CHARS_with_ZERO_LEN]; //���������� reserved1
	size_t reserved1_len_24; //����� ����������� char reserved1[16]. �� ������� ��������

	size_t max_reserved2_size_25; //������������ ������ ������ reserved2
	size_t digital_signature1_26;//���� 26
	size_t watermark_digital_signature_27;//�������� ��������� ���������� (EALS)
	size_t digital_signature2_28;//���� 28

								 //>>>> ����������� �������������� <<<<//
	EA_MT_SIGNAL MT_Signal_stack; //���� �����������

}S_EA_MESSAGE_ANSWER0_EVENT, *pEA_MESSAGE_ANSWER0_EVENT; //��� ��� ������������ ��������� (����� ������ �� connect)

//������ �1 (����� �� �����������)
typedef struct EA_MESSAGE_SEND1_HEAD_POOL
{
	LPCVOID Virtual_proc_1; //���� ����������� ������� EA_Message_ops
	
								   //���� ���������	multi >> (���������) � << (��������)//
	//��������� >>
		char response[MAX_BUILD_CHARS_with_ZERO_LEN]; //������ �� ���������� ���������� "response" (������������ ������ � Origin CORE). ChallengeResponse
		size_t response_len; //����� response;

		S_EA_M_UNIT seance_key; //������ �� ���������� ���������� "key" (�������� ������ head - ��������� ���� �� Origin CORE)

		//������ �� EA_ACCESS_request
		S_EA_M_UNIT ContentId; //ContentId (ex: 1031469)
		S_EA_M_UNIT Title; //Title (ex: Unravel)
		S_EA_M_UNIT MultiplayerId; //MultiplayerId (ex: 1031469)
		S_EA_M_UNIT Language; //Language (ex: en_US)

		S_EA_M_UNIT SDK_version; //������ SDK (9.10.1.7)

		//������ �����������
		S_EA_M_UNIT Request_send; //��� ��������� - ������ ("Request")
		S_EA_M_UNIT EALS_send; //��� ��������� - ��������� ("EALS")

	//�������� <<
		S_EA_M_UNIT response_accepted; // �������� ������ "response". ChallengeAccepted
		S_EA_M_UNIT Response_recv; //��� ��������� - ����� ("Response")
		S_EA_M_UNIT EALS_recv; //��� ��������� - ��������� ("EALS")
		S_EA_M_UNIT reserved1; //reserved?

								 //>>>> ����������� �������������� <<<<//
		EA_MT_SIGNAL MT_Signal_stack;
	
								  //>>>> ���� ���������� <<<<//
	bool is_Challenge_accepted; //���� ��������� ������ �� Origin CORE
	size_t waiting_time; //����� �������� ������ �� MessageThread
	pEA_HANDLE eah0; //lp EA_HANDLE_MAIN_STACK
	size_t var_01; //?
	size_t var_02; //?
	size_t var_03; //?
	size_t var_04; //?

}S_EA_MESSAGE_SENDER1_HEAD, *pEA_MESSAGE_SENDER1_HEAD; //����� �� �����������(����� ������ �� connect)