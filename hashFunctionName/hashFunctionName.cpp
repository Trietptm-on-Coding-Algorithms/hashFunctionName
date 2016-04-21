// hashFunctionName.cpp : Defines the entry pounsigned int for the console application.
//

#include "stdafx.h"
#include<windows.h>

unsigned int hash(unsigned int number, unsigned int key){
	key &= 0x1F;
   return (number >> key) | (number << (32-key));
}

unsigned int hashName(unsigned int key, char* APIName){
	unsigned int hashSum = 0;
	for(int i = 0; APIName[i]; i++){
		hashSum += hash(hashSum, key) + (int)APIName[i];
	}
	return hashSum;
}

void *getNames(void* dll, int&size){
	int imageBase = (int)dll;										//imageBase
	void* peheader =(void*) ((int)dll + 0x3c);						//peheadr->e_lfanew
	peheader = (void*)((int)dll + *(int*)peheader);					//
	dll = (void*)((int)peheader + 0x78);							//dll -> export directory
	void* ExportDir = (void*)(imageBase + *(int*)dll);				//ExportDir
	size = *(int*)((int)ExportDir + 0x18);
	return (void*)*(int*)((int)ExportDir + 0x20);
}

unsigned int main(int argc, char** argv){
	if(argc != 3){
		printf("Usage: %s <DllName.dll> <key>\n",argv[0]);
	}
	else{
		int size = 0;
		LPWSTR LibName = new wchar_t[50];
		memset(LibName, 0, 50);
		MultiByteToWideChar(CP_ACP,0,argv[1],-1, LibName,strlen(argv[1]));
		void* dll = (void*) LoadLibrary(LibName);
		int key = atoi(argv[2]);
		if(dll != NULL){
			void * APINames = (void*)((int)getNames(dll, size) + (int)dll);
			for(int i = 0; i < size; i++){
				char* name = (char*)(*(int*)((int)APINames+i*4)+(int)dll);
				printf("%-40s\t%x\n", name, hashName(key,name));
			}
		}
		else{
			printf("Can not load your DLL!");
		}
	}
	system("pause");
	return 0;
}
