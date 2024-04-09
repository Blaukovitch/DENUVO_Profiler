/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

//êëàññ îáðàáîò÷èêà ñîîáùåíèé
#include "../../stdafx.h"
#include "3__EA_Origin_Message_class.h"
//******************************************//
//ÂÈÐÒÓÀËÜÍÛÅ ÔÓÍÊÖÈÈ ÊËÀÑÑÀ ÏÎ ÎÁÐÀÁÎÒÊÅ ÑÎÎÁÙÅÍÈÉ ÎÒ Origin Core(OriginSDK)
//******************************************//

ULONG __fastcall EA_Message_ops::EA_digital_watermark_signature(__in char* Signature_string)
{
	size_t signature_len = strlen(Signature_string);


	signed __int64 v2; // r9
	char *v3; // r10
	unsigned long v4; // eax
	unsigned long v5; // r11
	int v6; // ecx
	signed long v7; // r9
	signed long v8; // r9

	v2 = signature_len;
	v3 = Signature_string;
	v4 = signature_len ^ EA_CRYPTO_SIGNATURE_1;
	if (signature_len >= 4)
	{
		v5 = signature_len >> 2;
		v2 = -4 * (signature_len >> 2) + signature_len;
		do
		{
			v6 = *v3;
			v3 += 4;
			v4 = EA_CRYPTO_SIGNATURE_2 * v4 ^ EA_CRYPTO_SIGNATURE_2 * (EA_CRYPTO_SIGNATURE_2 * v6 ^ ((EA_CRYPTO_SIGNATURE_2 * v6) >> 24));
			--v5;
		} while (v5);
	}
	v7 = v2 - 1;
	if (!v7)
	{
		v4 = EA_CRYPTO_SIGNATURE_2 * (*v3 ^ v4);
		return (EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) ^ (EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) >> 15))+DENUVO_HIDDEN_CONST_1;
	}
	v8 = v7 - 1;
	if (!v8)
	{
		//LABEL_10:
		v4 ^= v3[1] << 8;
		//LABEL_11:
		v4 = EA_CRYPTO_SIGNATURE_2 * (*v3 ^ v4);
		return (EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) ^ (EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) >> 15))+DENUVO_HIDDEN_CONST_1;
	}
	if (v8 == 1)
	{
		v4 ^= v3[2] << 16;
		v4 ^= v3[1] << 8;
		//LABEL_11:
		v4 = EA_CRYPTO_SIGNATURE_2 * (*v3 ^ v4);
		return EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) ^ (EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) >> 15);
	}
	return (EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) ^ (EA_CRYPTO_SIGNATURE_2 * (v4 ^ (v4 >> 13)) >> 15)) + DENUVO_HIDDEN_CONST_1;
}