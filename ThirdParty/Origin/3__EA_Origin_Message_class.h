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
//КЛАСС ОБРАБОТЧИКА/ОТПРАВЛЕНИЯ СООБЩЕНИЙ ОТ Origin CORE
//******************************************//

#include "3__EA_Origin_STUB.h"
//crypto include
#include "../../CRYPTO/MD5/md5.h"
#include "../../CRYPTO/EA_AES/EA_AES.h"

class EA_Message_ops
{
public:
	ULONG __fastcall EA_digital_watermark_signature(__in char* Signature_string); //строковая сигнатура -> цифровая

	// Overrides
public:
	

	// Implementation

//	DECLARE_MESSAGE_MAP()
};
