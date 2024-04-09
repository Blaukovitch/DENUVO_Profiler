/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#include "../stdafx.h"
#include "ENGINE_PIRATEMAP.h"

pPIRATEMAP_FILELIST_INFO DH_PIRATEMAP_Create_main_info_stack_cache(void)
{
	pDH_ENUM_FILES_of_PROFILES pPMAP_list = (pDH_ENUM_FILES_of_PROFILES)malloc(sizeof(DH_ENUM_FILES_of_PROFILES)*MAX_CONTAINTER_FILES);
	register size_t i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_PIRATE_MAP, pPMAP_list);

	if(i == NULL) //filter #1
		{free(pPMAP_list); return NULL;}

	pPIRATEMAP_FILELIST_INFO pPirateFililest = (pPIRATEMAP_FILELIST_INFO)malloc(sizeof(PIRATEMAP_FILELIST_INFO));
		if(pPirateFililest == NULL) //filter #2
			{free(pPMAP_list); return NULL;}

		pPirateFililest->FileList_count = NULL;
		register pDH_ENUM_FILES_of_PROFILES fpPMAP_list = pPMAP_list; //float_pointer - filenames
		register pPIRATEMAP_PROFILE_MAIN_INFO fpPMAP_MAIN_INFO_stack = &pPirateFililest->PMAP_Main_info_stack[0]; //float_pointer - PMAP_Main_info_stack
		ULONG reta = NULL;
		register pPIRATEMAP_FILELIST_INFO pRetStack = NULL;
		fpPMAP_list--;
		do
		{
		fpPMAP_list++;
			pRetStack = (pPIRATEMAP_FILELIST_INFO)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PIRATE_MAP, &fpPMAP_list->FileName[0],PROFILE_READ_ALL,&reta);
				if(!pRetStack)
					continue;

						memcpy(fpPMAP_MAIN_INFO_stack, pRetStack->PMAP_Main_info_stack, sizeof(PIRATEMAP_PROFILE_MAIN_INFO)); //copy
						pPirateFililest->FileList_count++; //inc
						free(pRetStack); //free returned

						if(fpPMAP_MAIN_INFO_stack->is_dll_addres_type == true)
							fpPMAP_MAIN_INFO_stack->PIRATEMAPPING_Region_start_addres += (SERVICE_ADDRES)HUNT_HIVE.Process_HIVE.Main_HMODULE; //OFFSET + HMODULE

						fpPMAP_MAIN_INFO_stack++;
		}while(--i);//main cycle

free(pPMAP_list);
return pPirateFililest;
}

pPIRATEMAP_PROFILE_MAIN_INFO __fastcall DH_PIRATEMAP_Found_target_addres_in_stack_cache(SERVICE_ADDRES Target_Addres, pPIRATEMAP_FILELIST_INFO pFill_main_info_stack_cache)
{

	register size_t i = pFill_main_info_stack_cache->FileList_count; //get i

	if(i == NULL) //filter #1
		return NULL;

	register pPIRATEMAP_PROFILE_MAIN_INFO fpPMAP_MAIN_INFO_stack = &pFill_main_info_stack_cache->PMAP_Main_info_stack[0]; 
	fpPMAP_MAIN_INFO_stack--;
	do
	{
		fpPMAP_MAIN_INFO_stack++;
		
		if((Target_Addres >= fpPMAP_MAIN_INFO_stack->PIRATEMAPPING_Region_start_addres) && (Target_Addres <= fpPMAP_MAIN_INFO_stack->PIRATEMAPPING_Region_start_addres+fpPMAP_MAIN_INFO_stack->PIRATEMAPPING_Region_size))
			return fpPMAP_MAIN_INFO_stack;

	}while(--i); ///main cycle

return NULL;
}

