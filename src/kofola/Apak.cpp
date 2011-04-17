#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Apak.h"

#define APAK_VERSION_HI		1
#define APAK_VERSION_LOW	1

void apakCheck(void)
{
  assert(sizeof(APAKFILEHEADER) == 428);
  assert(sizeof(APAKFILE) == 284);
  assert(sizeof(APAKDIRECTORY) == 272);
  assert(sizeof(APAKNODE) == 280);
  assert(sizeof(APAKFILENODE) == 8);
  assert(sizeof(APAK_HANDLE) == 700);
  assert(sizeof(APAK_FILE_HANDLE) == 28);
  assert(sizeof(APAK_STREAM_TYPE) == 8);
  assert(sizeof(APAK_FIND) == 272);
  assert(sizeof(APAK_FIND_SWITCH) == 8);
}

inline void ZeroMemory(void *mem, int size)
{
  memset(mem,0,size);
}

void apakError(APAK_HANDLE *pHandle, char *cError)
{
	pHandle->bError = 1;
	strcpy(pHandle->cError, cError);
  fprintf(stderr,"apakError: %s\n", cError);
}

int apakReadError(int iError, APAK_HANDLE	*pHandle)
{
	if(!iError)
	{
		apakError(pHandle, "Read error");
		return 1;
	}

	if(iError == -1)
	{
		apakError(pHandle, "Unknown Error");
		return 1;
	}

	return 0;
}

void apakGetVersion(int *iLow, int *iHi)
{
	*iLow = APAK_VERSION_LOW;
	*iHi  = APAK_VERSION_HI;
}

void apakCreateNode(APAK_HANDLE	*pHandle, char bFirst, APAKNODE *pPrevNode, 
					APAKNODE **pCpyNode, char bConnect, char *cDir, char bSetcwDir)
{
	char			buffer[256];
	int				iRet, 
					iLastFile, 
					iLastDir, 
					i;
	APAKFILENODE	FileNode;
	APAKNODE		*pNode;

	if(!bFirst)
	{
		iRet = fread(&FileNode, sizeof(APAKFILENODE),1,pHandle->pFILE);
		if(apakReadError(iRet, pHandle))
			return;

		if(FileNode.iNextItem != APAK_NODE)
		{
			apakError(pHandle, "AFAT corrupted");
			return;
		}
	}

	pNode = (APAKNODE *)malloc(sizeof(APAKNODE));

	if(!pNode)
	{
		apakError(pHandle, "Unable allocate memory for node");
		return;
	}

	if(pCpyNode)
		*pCpyNode = pNode;

	iRet = fread(pNode, sizeof(APAKNODE),1,pHandle->pFILE);

	if(apakReadError(iRet, pHandle))
		return;

	if(bFirst)
	{
		pHandle->pRootNode = pNode;
		pHandle->pActualNode = pNode;
	}

	if(bConnect)
	{
		pPrevNode->pNextNode = pNode;

		if(bSetcwDir && !strcmp(pNode->cWDir, "{APPDIR}"))
			strcpy(pNode->cWDir, cDir);
	}
	else
		if(!bFirst)
		{
			strcpy(buffer, pNode->cWDir);
			strcpy(pNode->cWDir, cDir);
			strcat(pNode->cWDir, "\\");
			strcat(pNode->cWDir, buffer);
		}

	pNode->apakDirectory = NULL;
	pNode->apakFile = NULL;
	pNode->pPreviousNode = pPrevNode;
	pNode->pNextNode = NULL;
	
	iLastFile = 0;
	iLastDir = 0;

	if(pNode->apuLSizeofFile)
	{
		pNode->apakFile = (APAKFILE *)malloc(pNode->apuLSizeofFile * sizeof(APAKFILE));

		if(!pNode->apakFile)
		{	
			apakError(pHandle, "Unable allocate memory for file table");
			return;
		}
	}

	for(i=0;(unsigned int)i<pNode->apuLSizeofFile;i++)
		ZeroMemory(&pNode->apakFile[i], sizeof(APAKFILE));

	if(pNode->apuLSizeofDirectory)
	{
		pNode->apakDirectory = (APAKDIRECTORY *)malloc(pNode->apuLSizeofDirectory * sizeof(APAKDIRECTORY));

		if(!pNode->apakDirectory)
		{
			apakError(pHandle, "Unable allocate memory for directory table");
			return;
		}
	}

	for(i=0;(unsigned int)i<pNode->apuLSizeofDirectory;i++)
		ZeroMemory(&pNode->apakDirectory[i], sizeof(APAKDIRECTORY));

	for(i=0;(unsigned int)i<pNode->apuLSizeofFile;i++)
	{
		iRet = fread(&FileNode, sizeof(APAKFILENODE),1,pHandle->pFILE);

		if(apakReadError(iRet, pHandle))
			return;
		
		if(FileNode.iNextItem != APAK_FILE)
		{
			apakError(pHandle, "AFAT corrupted");
			return;
		}

		iRet = fread(&pNode->apakFile[iLastFile], sizeof(APAKFILE),1,pHandle->pFILE);

		if(apakReadError(iRet, pHandle))
			return;

		pNode->apakFile[iLastFile].pNext = NULL;

		iLastFile++;
	}

	for(i=0;(unsigned int)i<pNode->apuLSizeofDirectory;i++)
	{
		iRet = fread(&FileNode, sizeof(APAKFILENODE),1,pHandle->pFILE);

		if(apakReadError(iRet, pHandle))
			return;
		
		if(FileNode.iNextItem != APAK_DIRECTORY)
		{
			apakError(pHandle, "AFAT corrupted");
			return;
		}

		iRet = fread(&pNode->apakDirectory[iLastDir], sizeof(APAKDIRECTORY),1,pHandle->pFILE);

		if(apakReadError(iRet, pHandle))
			return;

		pNode->apakDirectory[iLastDir].papakNode = NULL;

		apakCreateNode(pHandle, 0, pNode, 
					   (APAKNODE **)&pNode->apakDirectory[iLastDir].papakNode, 0, 
					   pNode->cWDir, bSetcwDir);
		
		pNode->apakDirectory[iLastDir].pNext = NULL;

		iLastDir++;
	}

	for(i=1;(unsigned int)i<pNode->apuLSizeofFile;i++)
		pNode->apakFile[i-1].pNext = &pNode->apakFile[i];

	for(i=1;(unsigned int)i<pNode->apuLSizeofDirectory;i++)
		pNode->apakDirectory[i-1].pNext = &pNode->apakDirectory[i];

	iRet = fread(&FileNode, sizeof(APAKFILENODE),1,pHandle->pFILE);

	if(apakReadError(iRet, pHandle))
		return;

	if(FileNode.iNextItem == APAK_ALLOCNODE)
		apakCreateNode(pHandle, 0, pNode, NULL, 1, cDir, bSetcwDir);
}

int apakLoadFAT(APAK_HANDLE	*pHandle, char *cDir)
{
	int				iRet;
	APAKFILENODE	FileNode;

	FileNode.iNextItem = -1;

	iRet = fread(&FileNode, sizeof(APAKFILENODE),1,pHandle->pFILE);

	if(apakReadError(iRet, pHandle))
		return 0;

	if(FileNode.iNextItem == APAK_ENDOFFAT)
		return 1;
	else
		if(FileNode.iNextItem == APAK_NODE)
			if(cDir)
				if(cDir[1] == ':')
					apakCreateNode(pHandle, 1, NULL, NULL, 0, &cDir[3], 1);
				else
					apakCreateNode(pHandle, 1, NULL, NULL, 0, cDir, 1);
			else
				apakCreateNode(pHandle, 1, NULL, NULL, 0, NULL, 0);
		else
			return 0;

	return 1;
}

APAK_HANDLE *apakopen(char *cArchive, char *cDir, int *pError)
{
	int				iRet;
	APAK_HANDLE		*pHandle;
	FILE			*pFile;

  apakCheck();

	pFile = fopen(cArchive, "rb");

	if(!pFile)
	{
		*pError =  APAK_FILE_NOT_FOUND;
		return NULL;
	}

	pHandle = (APAK_HANDLE *)malloc(sizeof(APAK_HANDLE));

	if(!pHandle)
	{
		*pError = APAK_OUT_OF_MEMORY;
		return NULL;
	}

	ZeroMemory(pHandle, sizeof(APAK_HANDLE));

	pHandle->pFILE = pFile;

	iRet = fread(&pHandle->FileHeader, sizeof(APAKFILEHEADER),1,pFile);

	if(apakReadError(iRet, pHandle))
	{
		fclose(pHandle->pFILE);
		free((void *) pHandle);
		*pError = APAK_UNABLE_TO_READ;
		return NULL;
	}

	if(pHandle->FileHeader.apBFileHiVersion != APAK_VERSION_HI ||
	   pHandle->FileHeader.apBFileLoVersion != APAK_VERSION_LOW)
	{
		fclose(pHandle->pFILE);
		free((void *) pHandle);
		*pError = APAK_VERSION_MISMATCH;
		return NULL;
	}

	if(!apakLoadFAT(pHandle, cDir))
		apakError(pHandle,"AFAT corrupted");

	return pHandle;
}

void apakReleaseNode(APAKNODE *pNode)
{
	int i;

	if(!pNode)
		return;

	if(pNode->apakFile)
		free((void *) pNode->apakFile);


	if(pNode->apuLSizeofDirectory)
	{
		for(i=0;(unsigned)i<pNode->apuLSizeofDirectory;i++)
			apakReleaseNode(pNode->apakDirectory[i].papakNode);

		free((void *) pNode->apakDirectory);
	}


	if(pNode->pNextNode)
		apakReleaseNode(pNode->pNextNode);
	
	free((void *) pNode);
}

void apakclose(APAK_HANDLE *pHandle)
{
	if(!pHandle)
		return;

	apakReleaseNode(pHandle->pRootNode);

	fclose(pHandle->pFILE);
	free((void *) pHandle);
}
