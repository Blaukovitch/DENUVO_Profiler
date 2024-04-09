
#include "EA_AES.h"
#include "../AES_def/aes.h"

pAES_CONTEXT __fastcall RAND_GEN(__in pAES_CONTEXT pAES_ctx, __in size_t seed)
{
	LPBYTE lpointer; // rbx
	size_t i = 0;

	pAES_ctx->nr = 1;
	lpointer = (LPBYTE)&pAES_ctx->buf[0];//AES_CTX + 0x110;
	if (seed)
	{
		srand(7u);
		srand(seed + rand());
		i = CBC_AES_BLOCK_SIZE;
		do
		{
			*(++lpointer - 1) = rand();
		} while (--i);
		return pAES_ctx;
	}
	else
	{
		i = 0;
		do
			*lpointer++ = i++;
		while (i < CBC_AES_BLOCK_SIZE);
		return pAES_ctx;
	}
	return pAES_ctx;
}

char *__cdecl AES_crypt(__in pAES_CONTEXT pAES_ctx, __out pS_EA_M_UNIT pOut, __in pS_EA_M_UNIT pKey)
{/*
	struct AES_ctx ctx;
	uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	uint8_t key[] = { 
	//		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		//	0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61
	//	 }; //aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

		0x65, 0x38, 0x64, 0x66, 0x39, 0x66, 0x66, 0x37, 0x62, 0x33, 0x37, 0x63, 0x34, 0x64, 0x63, 0x65,
			0x65, 0x61, 0x64, 0x35, 0x62, 0x64, 0x38, 0x38, 0x34, 0x33, 0x30, 0x37, 0x33, 0x39, 0x34, 0x36
	};

//ГЛАВНЫЙ!!!
	uint8_t in[] = {
	//	0xB8, 0x40, 0xFB, 0xA0, 0xC5, 0x53, 0x4B, 0x7C, 0x13, 0x3B, 0xEC, 0xFB, 0x1B, 0x67, 0x4D, 0xF3,
		//0xB8, 0x40, 0xFB, 0xA0, 0xC5, 0x53, 0x4B, 0x7C, 0x13, 0x3B, 0xEC, 0xFB, 0x1B, 0x67, 0x4D, 0xF3,
	//	0x95, 0x4F, 0x64, 0xF2, 0xE4, 0xE8, 0x6E, 0x9E, 0xEE, 0x82, 0xD2, 0x02, 0x16, 0x68, 0x48, 0x99
//	};

		0x9F, 0xFC, 0x9A, 0x5B, 0xD2, 0x9B, 0xD0, 0x7B, 0xF5, 0x5E, 0xC9, 0x08, 0x20, 0x78, 0x9C, 0x97,
			0x99, 0xAF, 0x81, 0xD1, 0xC0, 0x66, 0xB8, 0x42, 0xCF, 0xC9, 0x5F, 0x32, 0xE7, 0x3E, 0x55, 0x4F,
			0x95, 0x4F, 0x64, 0xF2, 0xE4, 0xE8, 0x6E, 0x9E, 0xEE, 0x82, 0xD2, 0x02, 0x16, 0x68, 0x48, 0x99
	};
	//AES_CBC_decrypt_buffer
//	uint8_t in[] = {
//0xFA, 0x15, 0x79, 0x3C, 0xA6, 0x62, 0x44, 0x89, 0xFC, 0x40, 0xEB, 0xE8, 0x68, 0xFC, 0x7A, 0xE6,
//0x42, 0x54, 0x80, 0x9F, 0x67, 0x34, 0x09, 0xF2, 0xE7, 0x72, 0x0D, 0x18, 0x7F, 0x96, 0x39, 0x1A,
//0x4D, 0x46, 0x14, 0xDD, 0xD4, 0x72, 0x64, 0xC5, 0x48, 0x7F, 0xEF, 0xC4, 0xD1, 0xDE, 0x46, 0x85
//};

	//AES_init_ctx_iv(&ctx, key, iv);
	//AES_CBC_decrypt_buffer(&ctx, in, 48);
	//AES_init_ctx_iv(&ctx, key, iv);
	//AES_CBC_encrypt_buffer(&ctx, in, 48);
	*/
	//выеделение памяти под выходную строку
	
	size_t out_len = CBC_AES_BLOCK_SIZE * ((pKey->len >> 4) + sizeof('\0'));
	byte* pAES_out = (byte*)malloc(out_len);
	memset(pAES_out, 0, out_len);
	AES_ops_inside((__int64)pAES_ctx, 0, 0, (BYTE *)&pAES_ctx->buf[0], 0, 0);

	char* pCharKey_input = pKey->string;
	if(pKey->len > MAX_BUILD_CHARS_with_ZERO_LEN)
		pCharKey_input = *(char**)pCharKey_input;
	AES_ops_inside2((DWORD*)pAES_ctx, pCharKey_input, pKey->len, pAES_out);

	char* pOut_aes_string = (char*)malloc((pKey->len * sizeof(char)) + 1);
	char* fpOt_aes_string = pOut_aes_string;
	char* fpAES_out = (char*) pAES_out;
	size_t i = pKey->len;
	do
	{
		sprintf(fpOt_aes_string++, SPRINT_F_HEX_TO_STR, *fpAES_out++);
	} while (i--);
	free(pAES_out);

	size_t out_aes_len = strlen(pOut_aes_string);
	if (pOut->MAX_strlen < out_aes_len)
	{
		strcpy(&pOut->string[0], pOut_aes_string);
		free(pOut_aes_string);
	}
	else
	{
		*(char**)pOut->string = (char*)pAES_out;
		pOut->len = out_aes_len;
	}

	return &pOut->string[0];
}

char *__cdecl AES_decrypt(__in pAES_CONTEXT pAES_ctx, __out pS_EA_M_UNIT pOut, __in pS_EA_M_UNIT pKey)
{
	return 0;
}

//AES ops №1
size_t __fastcall AES_ops_inside(__int64 Src, unsigned int a2, unsigned __int64 a3_af, _BYTE *a4, int a5, __int64 a6) //CBCEncrypt
{
	_DWORD *v6; // rbx
	size_t v8; // r10
	__int64 v9; // rcx
	signed __int64 v10; // rdx
	char v11; // al
	unsigned __int64 v12; // r8
	__int64 v13; // rax
	unsigned __int64 v14; // rdx
	char *v15; // rcx
	char v16[32]; // [rsp+20h] [rbp-38h]

	v6 = (_DWORD *)Src;
	*(_DWORD *)Src = 1;
	if (a2 > 2)
		return 0xFFFFFFFF;
	*(_DWORD *)(Src + 4) = a2;
	if ((unsigned int)a3_af > 1)
		return 0xFFFFFFFE;
	*(_DWORD *)(Src + 8) = a3_af;
	v8 = 16;
	if (a6)
	{
		v9 = Src + 0xC;
		v10 = 16;
		do
		{
			v11 = *(_BYTE *)(a6 - (_QWORD)v6 - 12 + v9++);
			*(_BYTE *)(v9 - 1) = v11;
			--v10;
		} while (v10);
	}
	else
	{
		*(_QWORD *)(Src + 0xC) = 0;
		*(_QWORD *)(Src + 0x14) = 0;
	}
	if (a5)
	{
		if (a5 == 1)
		{
			v8 = 24;
			v6[7] = 12;
		}
		else
		{
			if (a5 != 2)
				return 0xFFFFFFFD;
			v8 = 32;
			v6[7] = 14;
		}
	}
	else
	{
		v6[7] = 10;
	}
	if (!a4)
		return 0xFFFFFFFC;
	if (v8)
	{
		v12 = 0;
		do
		{
			v13 = v12 & 3;
			v14 = v12++ >> 2;
			v15 = &v16[v13];
			v13 = *a4++;
			v15[4 * v14] = v13;
			--v8;
		} while (v8);
	}
	crypto_AES_XORBYTE((DWORD*)v6, (BYTE*)&v16[0]);
	if (v6[2] == 1)
		crypto_AES_XOR_dword((size_t)v6);
	*v6 = 0;
	return 0;
}

size_t __fastcall crypto_AES_XORBYTE(DWORD *CTX, const BYTE *round)
{

		DWORD *v2; // rdi
		signed int buf_len; // esi
		size_t jj; // r10
		size_t result; // rax
		signed __int64 gg; // r9
		//bool v7; // zf
		DWORD v8; // ecx
		size_t v9; // r8
		const UINT8 *v10; // r11
		signed __int64 ii; // rdx
		DWORD v12; // ecx
		DWORD v13; // ecx
		signed __int64 v14; // rdx
		DWORD v15; // ecx
		signed __int64 hh; // r9
		//bool v17; // zf
		DWORD v18; // ecx
		size_t v19; // r8
		DWORD TempKey[9]; // [rsp+1Ch] [rbp-34h]

		v2 = CTX;
		buf_len = CTX[7] - 6;
		if (buf_len > 0)
			memcpy(&TempKey[1], round, sizeof(DWORD) * buf_len);
		jj = 0; result = 0; gg = 0;
		if (buf_len <= 0)
		{
		LABEL_12:
			if ((unsigned int)jj <= v2[7])
			{
				v10 = &rcon[0];
				while (1)
				{
					v10 += sizeof(DWORD);
					byte ca = BYTE1(TempKey[buf_len]);
					byte fafg = SBox[BYTE1(TempKey[buf_len])];
					byte ta1 = LOBYTE(TempKey[1]);

					LOBYTE(TempKey[1]) ^= SBox[BYTE1(TempKey[buf_len])];
					ii = 1;
					BYTE1(TempKey[1]) ^= SBox[BYTE2(TempKey[buf_len])];
					BYTE2(TempKey[1]) ^= SBox[BYTE3(TempKey[buf_len])];
					BYTE3(TempKey[1]) ^= SBox[LOBYTE(TempKey[buf_len])];
					LOBYTE(TempKey[1]) ^= *(const UINT8*)((size_t)v10 - sizeof(DWORD));
					if (buf_len == 8)
					{
						do
						{
							v13 = TempKey[ii++];
							TempKey[ii] ^= v13;
						} while (ii < 4);
						v14 = 5;
						LOBYTE(TempKey[5]) ^= SBox[LOBYTE(TempKey[4])];
						BYTE1(TempKey[5]) ^= SBox[BYTE1(TempKey[4])];
						BYTE2(TempKey[5]) ^= SBox[BYTE2(TempKey[4])];
						HIBYTE(TempKey[5]) ^= SBox[HIBYTE(TempKey[4])];
						if (buf_len > 5)
						{
							do
							{
								v15 = TempKey[v14++];
								TempKey[v14] ^= v15;
							} while (v14 < buf_len);
						}
					}
					else if (buf_len > 1)
					{
						do
						{
							v12 = TempKey[ii++];
							TempKey[ii] ^= v12;
						} while (ii < buf_len);
					}
					hh = 0;
					if (buf_len > 0)
						break;
				LABEL_30:
					if ((unsigned long)jj > v2[7])
						return result;
				}
				while ((unsigned long)jj <= v2[7])
				{
					if (hh >= buf_len)
					{
				
					//	v17 = (_DWORD)result == 4;
					}
					else
					{
						while (1)
						{
						//	v17 = (_DWORD)result == 4;
							if ((unsigned int)result >= 4)
								break;
							v18 = TempKey[hh + 1];
							v19 = result + (4 * jj);
							++hh;
							result++;
							v2[v19 + 8] = v18;
							if (hh >= buf_len)
								goto LABEL_26;
						}
					}
				LABEL_26:
					if (result == 4)
					{
						jj++;
						result = 0;
					}
					if (hh >= buf_len)
						goto LABEL_30;
				}
			}
		}
		else
		{
			while ((unsigned int)jj <= v2[7])
			{
				if (gg >= buf_len)
				{
				//	v7 = (_DWORD)result == 4;
				}
				else
				{
					while (1)
					{
						//v7 = (_DWORD)result == 4;
						if ((unsigned int)result >= 4)
							break;
						v8 = TempKey[gg + 1];
						v9 = result + (4 * jj);
						++gg;
						result++;
						v2[v9 + 8] = v8;
						if (gg >= buf_len)
							goto LABEL_8;
					}
				}
			LABEL_8:
				if (result == 4)
				{
					jj++;
					result = 0;
				}
				if (gg >= buf_len)
					goto LABEL_12;
			}
		}
		return result;
	}
	

size_t __fastcall crypto_AES_XOR_dword(size_t a1)
{
	size_t i; // er11
	size_t v2; // r9
	size_t v3; // r8
	unsigned __int8 *v4; // r8
	size_t result; // rax

	for (i = 1;
		i < *(DWORD *)(a1 + 28);
		*(DWORD *)(a1 + 8 * v2 + 44) = UL1[result] ^ UL2[*(unsigned __int8 *)(a1 + 8 * v2 + 45)] ^ UL3[*(unsigned __int8 *)(a1 + 8 * v2 + 46)] ^ UL4[*(unsigned __int8 *)(a1 + 8 * v2 + 47)])
	{
		v2 = i++;
		v3 = v2 + 2;
		v2 *= 2;
		v4 = (unsigned __int8 *)(a1 + 16 * v3);
		*(DWORD *)v4 = UL1[*v4] ^ UL2[v4[1]] ^ UL3[v4[2]] ^ UL4[v4[3]];
		*(DWORD *)(a1 + 8 * v2 + 36) = UL1[*(unsigned __int8 *)(a1 + 8 * v2 + 36)] ^ UL2[*(unsigned __int8 *)(a1 + 8 * v2 + 37)] ^ UL3[*(unsigned __int8 *)(a1 + 8 * v2 + 38)] ^ UL4[*(unsigned __int8 *)(a1 + 8 * v2 + 39)];
		*(DWORD *)(a1 + 8 * v2 + 40) = UL1[*(unsigned __int8 *)(a1 + 8 * v2 + 40)] ^ UL2[*(unsigned __int8 *)(a1 + 8 * v2 + 41)] ^ UL3[*(unsigned __int8 *)(a1 + 8 * v2 + 42)] ^ UL4[*(unsigned __int8 *)(a1 + 8 * v2 + 43)];
		result = *(unsigned __int8 *)(a1 + 8 * v2 + 44);
	}
	return result;
}

size_t __fastcall AES_ops_inside2(DWORD *pAES_ctx, char *pKey_string, int input_buf_len, byte *out_chipper)
{
		DWORD *pOut; // rsi
		DWORD v5; // er15
		DWORD *pKey_DWORD; // rbx
		DWORD *v7; // rbp
		DWORD pCtx_sboxcomplete; // eax
		int buflen_div_16_1; // er14
		DWORD *pCTX; // rcx
		int ii; // edi
		DWORD v13; // er14
		size_t count_leave; // r8
		DWORD *v15; // rbx
		DWORD *v16; // rdx
		__int64 ff; // r10
		char v18; // al
		signed __int64 v19; // rcx
		_BYTE *v20; // rdx
		int buflen_div_16; // edi
		DWORD Dst[10]; // [rsp+20h] [rbp-58h]

		pOut = (DWORD *)out_chipper;
		v5 = input_buf_len;
		pKey_DWORD = (DWORD *)pKey_string;
		v7 = pAES_ctx;
		if (*pAES_ctx || pAES_ctx[2])
			return 0xFFFFFFFB;
		if (!pKey_string || input_buf_len <= 0)
			return 0i64;
		pCtx_sboxcomplete = pAES_ctx[1];
		buflen_div_16_1 = input_buf_len / 16;
		if (pCtx_sboxcomplete)
		{
			if (pCtx_sboxcomplete != 1)
				return 0xFFFFFFFF;
			pCTX = pAES_ctx + 3;
			ii = input_buf_len / 16;
			if (buflen_div_16_1 > 0)
			{
				do
				{
					Dst[0] = *pCTX ^ *pKey_DWORD;
					Dst[1] = pCTX[1] ^ pKey_DWORD[1];
					Dst[2] = pCTX[2] ^ pKey_DWORD[2];
					Dst[3] = pCTX[3] ^ pKey_DWORD[3];
					aes_round_assembly(v7, Dst, pOut);
					--ii;
					pCTX = pOut;
					pKey_DWORD += 4;
					pOut += 4;
				} while (ii > 0);
			}
			v13 = 16 * buflen_div_16_1;
			count_leave = (signed int)(v5 - v13);
			if ((signed int)(v5 - v13) > 0)
			{
				v15 = (DWORD *)((char *)pKey_DWORD - (char *)pCTX);
				v16 = pCTX;
				ff = (signed int)(v5 - v13);
				do
				{
					v18 = *(char*)((size_t)v16 + (size_t)v15);
					v16 = (DWORD *)((size_t)v16 + 1); //inc
					*((BYTE *)v16 + (size_t)&Dst[0] - ((size_t)pCTX - 1)) = *((BYTE *)v16 - 1) ^ v18;
					--ff;
				} while (ff);
			}
			if ((signed __int64)count_leave < 16)
			{
				v19 = (char *)pCTX - (char *)Dst;
				do
				{
					v20 = (uint8*)((size_t)&Dst[0] + count_leave++);
					*v20 = (v13 - v5 + 16) ^ v20[v19];
				} while ((signed __int64)count_leave < 16);
			}
		}
		else
		{
			buflen_div_16 = input_buf_len / 16;
			if (buflen_div_16_1 > 0)
			{
				do
				{
					aes_round_assembly(v7, pKey_DWORD, pOut);
					--buflen_div_16;
					pKey_DWORD += 4;
					pOut += 4;
				} while (buflen_div_16 > 0);
			}
			v13 = 16 * buflen_div_16_1;
			memcpy(Dst, pKey_DWORD, (signed int)(v5 - v13));
			memset((char *)&Dst[4] - (signed int)(v13 - v5 + 16), v13 - v5 + 16, (signed int)(v13 - v5 + 16));
		}
		aes_round_assembly(v7, Dst, pOut);
		return v13 + 16;
	}

size_t __fastcall aes_round(DWORD *CTX, DWORD *a2, DWORD *a3)
{
	DWORD v3; // er9
	BYTE *v4; // rdi
	DWORD *v5; // rsi
	DWORD v6; // ST0C_4
	DWORD v7; // ST08_4
	DWORD v8; // ST04_4
	DWORD v9; // ST00_4
	unsigned int v10; // er14
	int v11; // er8
	int v12; // er9
	int v13; // eax
	size_t v14; // rdx
	signed __int64 v15; // rcx
	int v16; // ST08_4
	int v17; // er9
	int v18; // ST04_4
	int v19; // ST00_4
	unsigned __int8 v20; // ST0C_1
	int v21; // er8
	int v22; // er9
	int v23; // er10
	int v24; // edx
	int v25; // ST0C_4
	size_t result; // rax

	v4 = (BYTE *)a3;
	v5 = CTX;
	v6 = a2[3] ^ CTX[11];
	v3 = a2[3] ^ CTX[11];
	v7 = a2[2] ^ CTX[10];
	v8 = a2[1] ^ CTX[9];
	v9 = *a2 ^ CTX[8];
	v10 = 1;
	*a3 = Te0[(unsigned __int8)v9] ^ Te1[BYTE1(v8)] ^ Te2[BYTE2(v7)] ^ Te3[v6 >> 24];
	v11 = Te0[(unsigned __int8)v8] ^ Te3[HIBYTE(v9)] ^ Te1[BYTE1(v7)] ^ Te2[BYTE2(v3)];
	*((_DWORD *)v4 + 1) = v11;
	v12 = Te0[(unsigned __int8)v7] ^ Te2[BYTE2(v9)] ^ Te3[HIBYTE(v8)] ^ Te1[BYTE1(v3)];
	*((_DWORD *)v4 + 2) = v12;
	v13 = Te0[(unsigned __int8)v6] ^ Te1[BYTE1(v9)] ^ Te2[BYTE2(v8)] ^ Te3[HIBYTE(v7)];
	for (*((_DWORD *)v4 + 3) = v13; v10 < v5[7] - 1; *((_DWORD *)v4 + 3) = v13)
	{
		v14 = v10++;
		v15 = v14 + 2;
		v14 *= 2i64;
		v16 = v12 ^ v5[2 * v14 + 10];
		v17 = v13 ^ v5[2 * v14 + 11];
		v18 = v11 ^ v5[2 * v14 + 9];
		v19 = *(_DWORD *)v4 ^ v5[4 * v15];
		v20 = v13 ^ LOBYTE(v5[2 * v14 + 11]);
		*(_DWORD *)v4 = Te0[(unsigned __int8)v19] ^ Te1[BYTE1(v18)] ^ Te2[BYTE2(v16)] ^ Te3[HIBYTE(v17)];
		v11 = Te0[(unsigned __int8)v18] ^ Te3[HIBYTE(v19)] ^ Te1[BYTE1(v16)] ^ Te2[BYTE2(v17)];
		*((_DWORD *)v4 + 1) = v11;
		v12 = Te0[(unsigned __int8)v16] ^ Te2[BYTE2(v19)] ^ Te3[HIBYTE(v18)] ^ Te1[BYTE1(v17)];
		*((_DWORD *)v4 + 2) = v12;
		v13 = Te0[v20] ^ Te1[BYTE1(v19)] ^ Te2[BYTE2(v18)] ^ Te3[HIBYTE(v16)];
	}
	v21 = *(_DWORD *)v4 ^ v5[4 * ((unsigned int)(v5[7] - 1) + 2i64)];
	v22 = *((_DWORD *)v4 + 1) ^ v5[4 * (v5[7] - 1) + 9];
	v23 = *((_DWORD *)v4 + 2) ^ v5[4 * (v5[7] - 1) + 10];
	v25 = *((_DWORD *)v4 + 3) ^ v5[4 * (v5[7] - 1) + 11];
	v24 = *((_DWORD *)v4 + 3) ^ v5[4 * (v5[7] - 1) + 11];
	*v4 = BYTE1(Te0[(unsigned __int8)v21]);
	v4[1] = BYTE1(Te0[BYTE1(v22)]);
	v4[2] = BYTE1(Te0[BYTE2(v23)]);
	v4[3] = BYTE1(Te0[HIBYTE(v24)]);
	v4[4] = BYTE1(Te0[(unsigned __int8)v22]);
	v4[5] = BYTE1(Te0[BYTE1(v23)]);
	v4[6] = BYTE1(Te0[BYTE2(v24)]);
	v4[7] = BYTE1(Te0[HIBYTE(v21)]);
	v4[8] = BYTE1(Te0[(unsigned __int8)v23]);
	v4[9] = BYTE1(Te0[BYTE1(v24)]);
	v4[10] = BYTE1(Te0[BYTE2(v21)]);
	v4[11] = BYTE1(Te0[HIBYTE(v22)]);
	v4[12] = BYTE1(Te0[(unsigned __int8)v25]);
	v4[13] = BYTE1(Te0[BYTE1(v21)]);
	v4[14] = BYTE1(Te0[BYTE2(v22)]);
	v4[15] = BYTE1(Te0[HIBYTE(v23)]);
	*(_DWORD *)v4 ^= v5[4 * ((unsigned int)v5[7] + 2i64)];
	*((_DWORD *)v4 + 1) ^= v5[4 * v5[7] + 9];
	*((_DWORD *)v4 + 2) ^= v5[4 * v5[7] + 10];
	result = (unsigned int)v5[4 * v5[7] + 11];
	*((_DWORD *)v4 + 3) ^= result;
	return result;
}

__declspec(naked) size_t aes_round_assembly(DWORD *CTX, DWORD *a2, DWORD *a3)
{
	__asm
	{
		mov qword ptr[rsp + 8], rbx
		mov qword ptr[rsp + 0x10], rsi
		mov qword ptr[rsp + 0x18], rdi
		push rbp
		push r14
		push r15
		mov rbp, rsp
		sub rsp, 0x10
		mov r9d, dword ptr[rcx + 0x2c]
		mov r10d, dword ptr[rcx + 0x28]
		mov ebx, dword ptr[rcx + 0x20]
		xor r9d, dword ptr[rdx + 0xc]
		mov r11d, dword ptr[rcx + 0x24]
		xor r10d, dword ptr[rdx + 8]
		xor ebx, dword ptr[rdx]
		xor r11d, dword ptr[rdx + 4]
			xor r15, r15 //lea r15, qword ptr ds:[0x140000000] //not req
		mov rdi, r8
		mov rsi, rcx
		mov eax, r9d
		shr eax, 0x18
		mov dword ptr[rbp - 4], r9d
		mov dword ptr[rbp - 8], r10d
		movzx edx, al
		mov eax, r10d
		mov dword ptr[rbp - 0xc], r11d
			lea r15, [Te3]
		mov r8d, dword ptr[rdx * 4 + r15]
		shr eax, 0x10
		mov dword ptr[rbp - 0x10], ebx
		movzx ecx, al
		mov eax, r11d
		//mov r14d, 0xc390a20f
		lea r15, [Te2]
		xor r8d, dword ptr[rcx * 4 + r15]
		mov r14d, 1//lea r14d, [r14 + 0x3c6f5df2]
		shr eax, 8
		movzx ecx, al
		movzx eax, bl
		lea r15, [Te1]
		xor r8d, dword ptr[rcx * 4 + r15]
		lea r15, [Te0]
		xor r8d, dword ptr[rax * 4 + r15]
		mov eax, r9d
		shr r9d, 8
		shr eax, 0x10
		mov dword ptr[rdi], r8d
		movzx edx, al
		mov eax, r10d
		lea r15, [Te2]
		mov r8d, dword ptr[rdx * 4 + r15]
		shr eax, 8
		movzx edx, r9b
		movzx ecx, al
		mov eax, ebx
		lea r15, [Te1]
		xor r8d, dword ptr[rcx * 4 + r15]
		shr eax, 0x18
		movzx ecx, al
		movzx eax, r11b
		lea r15, [Te3]
		xor r8d, dword ptr[rcx * 4 + r15]
		lea r15, [Te0]
		xor r8d, dword ptr[rax * 4 + r15]
		mov eax, r11d
		shr r11d, 0x10
		shr eax, 0x18
		mov dword ptr[rdi + 4], r8d
		lea r15, [Te1]
		mov r9d, dword ptr[rdx * 4 + r15]
		movzx ecx, al
		mov eax, ebx
		shr ebx, 8
		lea r15, [Te3]
		xor r9d, dword ptr[rcx * 4 + r15]
		shr eax, 0x10
		movzx ecx, al
		movzx eax, r10b
		shr r10d, 0x18
		lea r15, [Te2]
		xor r9d, dword ptr[rcx * 4 + r15]
		movzx ecx, r11b
		lea r15, [Te0]
		xor r9d, dword ptr[rax * 4 + r15]
		movzx eax, r10b
		mov dword ptr[rdi + 8], r9d
		lea r15, [Te3]
		mov eax, dword ptr[rax * 4 + r15]
		lea r15, [Te2]
		xor eax, dword ptr[rcx * 4 + r15]
		movzx ecx, bl
		lea r15, [Te1]
		xor eax, dword ptr[rcx * 4 + r15]
		movzx ecx, byte ptr[rbp - 4]
		lea r15, [Te0]
		xor eax, dword ptr[rcx * 4 + r15]
		mov dword ptr[rdi + 0xc], eax
		mov ecx, dword ptr[rsi + 0x1c]
		dec ecx
		cmp ecx, r14d
		jbe L00000002

	L00000001:
		mov edx, r14d
		inc r14d
		lea rcx, [rdx + 2]
		add rdx, rdx
		mov r10d, dword ptr[rsi + rdx * 8 + 0x28]
		mov r11d, dword ptr[rsi + rdx * 8 + 0x24]
		add rcx, rcx
		mov ebx, dword ptr[rsi + rcx * 8]
		xor r11d, r8d
		xor r10d, r9d
		mov r9d, dword ptr[rsi + rdx * 8 + 0x2c]
		xor ebx, dword ptr[rdi]
		mov dword ptr[rbp - 8], r10d
		xor r9d, eax
		mov dword ptr[rbp - 0xc], r11d
		mov dword ptr[rbp - 0x10], ebx
		mov eax, r9d
		mov dword ptr[rbp - 4], r9d
		shr eax, 0x18
		movzx edx, al
		mov eax, r10d
		lea r15, [Te3]
		mov r8d, dword ptr[rdx * 4 + r15]
		shr eax, 0x10
		movzx ecx, al
		mov eax, r11d
		lea r15, [Te2]
		xor r8d, dword ptr[rcx * 4 + r15]
		shr eax, 8
		movzx ecx, al
		movzx eax, bl
		lea r15, [Te1]
		xor r8d, dword ptr[rcx * 4 + r15]
		lea r15, [Te0]
		xor r8d, dword ptr[rax * 4 + r15]
		mov eax, r9d
		shr r9d, 8
		shr eax, 0x10
		mov dword ptr[rdi], r8d
		movzx edx, al
		mov eax, r10d
		lea r15, [Te2]
		mov r8d, dword ptr[rdx * 4 + r15]
		shr eax, 8
		movzx edx, r9b
		movzx ecx, al
		mov eax, ebx
		lea r15, [Te1]
		xor r8d, dword ptr[rcx * 4 + r15]
		shr eax, 0x18
		movzx ecx, al
		movzx eax, r11b
		lea r15, [Te3]
		xor r8d, dword ptr[rcx * 4 + r15]
		lea r15, [Te0]
		xor r8d, dword ptr[rax * 4 + r15]
		mov eax, r11d
		shr r11d, 0x10
		shr eax, 0x18
		mov dword ptr[rdi + 4], r8d
		lea r15, [Te1]
		mov r9d, dword ptr[rdx * 4 + r15]
		movzx ecx, al
		mov eax, ebx
		shr ebx, 8
		lea r15, [Te3]
		xor r9d, dword ptr[rcx * 4 + r15]
		shr eax, 0x10
		movzx ecx, al
		movzx eax, r10b
		shr r10d, 0x18
		lea r15, [Te2]
		xor r9d, dword ptr[rcx * 4 + r15]
		movzx ecx, r11b
		lea r15, [Te0]
		xor r9d, dword ptr[rax * 4 + r15]
		movzx eax, r10b
		mov dword ptr[rdi + 8], r9d
		lea r15, [Te3]
		mov eax, dword ptr[rax * 4 + r15]
		lea r15, [Te2]
		xor eax, dword ptr[rcx * 4 + r15]
		movzx ecx, bl
			lea r15, [Te1]
		xor eax, dword ptr[rcx * 4 + r15]
		movzx ecx, byte ptr[rbp - 4]
			lea r15, [Te0]
		xor eax, dword ptr[rcx * 4 + r15]
		mov dword ptr[rdi + 0xc], eax
		mov ecx, dword ptr[rsi + 0x1c]
		dec ecx
		cmp r14d, ecx
		jb L00000001

	L00000002:
		mov eax, dword ptr[rsi + 0x1c]
		dec eax
		mov ecx, eax
		add rax, 2
		add rax, rax
		add rcx, rcx
		mov r8d, dword ptr[rsi + rax * 8]
		mov r9d, dword ptr[rsi + rcx * 8 + 0x24]
		mov r10d, dword ptr[rsi + rcx * 8 + 0x28]
		mov edx, dword ptr[rsi + rcx * 8 + 0x2c]
		xor r8d, dword ptr[rdi]
		xor r9d, dword ptr[rdi + 4]
		xor r10d, dword ptr[rdi + 8]
		xor edx, dword ptr[rdi + 0xc]
		movzx eax, r8b
			lea r15, [Te0+1]
		movzx eax, byte ptr[rax * 4 + r15]
		mov dword ptr[rbp - 4], edx
		mov dword ptr[rbp - 0x10], r8d
		mov byte ptr[rdi], al
		mov eax, r9d
		mov dword ptr[rbp - 0xc], r9d
		shr eax, 8
		mov dword ptr[rbp - 8], r10d
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 1], al
		mov eax, r10d
		shr eax, 0x10
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 2], al
		mov eax, edx
		shr eax, 0x18
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 3], al
		movzx eax, r9b
		movzx eax, byte ptr[rax * 4 + r15]
		mov byte ptr[rdi + 4], al
		mov eax, r10d
		shr eax, 8
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 5], al
		mov eax, edx
		shr edx, 8
		shr eax, 0x10
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 6], al
		mov eax, r8d
		shr eax, 0x18
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 7], al
		movzx eax, r10b
		movzx eax, byte ptr[rax * 4 + r15]
		mov byte ptr[rdi + 8], al
		movzx eax, dl
		movzx eax, byte ptr[rax * 4 + r15]
		mov byte ptr[rdi + 9], al
		mov eax, r8d
		shr r8d, 8
		shr eax, 0x10
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 0xa], al
		mov eax, r9d
		shr eax, 0x18
		shr r9d, 0x10
		movzx ecx, al
		movzx eax, byte ptr[rcx * 4 + r15]
		mov byte ptr[rdi + 0xb], al
		movzx eax, byte ptr[rbp - 4]
		movzx eax, byte ptr[rax * 4 + r15]
		mov byte ptr[rdi + 0xc], al
		movzx eax, r8b
		movzx eax, byte ptr[rax * 4 + r15]
		mov byte ptr[rdi + 0xd], al
		movzx eax, r9b
		movzx eax, byte ptr[rax * 4 + r15]
		mov rbx, qword ptr[rsp + 0x30]
		mov byte ptr[rdi + 0xe], al
		shr r10d, 0x18
		movzx eax, r10b
		movzx eax, byte ptr[rax * 4 + r15]
		mov byte ptr[rdi + 0xf], al
		mov eax, dword ptr[rsi + 0x1c]
		add rax, 2
		add rax, rax
		mov eax, dword ptr[rsi + rax * 8]
		xor dword ptr[rdi], eax
		mov eax, dword ptr[rsi + 0x1c]
		add rax, rax
		mov eax, dword ptr[rsi + rax * 8 + 0x24]
		xor dword ptr[rdi + 4], eax
		mov eax, dword ptr[rsi + 0x1c]
		add rax, rax
		mov eax, dword ptr[rsi + rax * 8 + 0x28]
		xor dword ptr[rdi + 8], eax
		mov eax, dword ptr[rsi + 0x1c]
		add rax, rax
		mov eax, dword ptr[rsi + rax * 8 + 0x2c]
		mov rsi, qword ptr[rsp + 0x38]
		xor dword ptr[rdi + 0xc], eax
		mov rdi, qword ptr[rsp + 0x40]
		add rsp, 0x10
		pop r15
		pop r14
		pop rbp
		ret
	}

}
////////////////////