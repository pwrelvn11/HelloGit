/********************************************************************
	������̌��� (strstr�֐�)
*********************************************************************/
#include <stdio.h>
#include <string.h>

#define		BUFF	256		/*  ���͍ő包  */

int main(void)
{
	char	szStr[BUFF] = "This is a pen.";		/*  �������镶����  */
	char	szKey[BUFF] = "pen";				/*  �����Ώۂ̕�����  */
	char	*pszDat;
	int		result = 0;

	printf("���̃f�[�^�@�@�F%s\n", szStr );
	printf("�����Ώە�����F%s\n", szKey );

	pszDat = strstr( szStr, szKey );		/*  strstr�֐����g���Č���  */

	if( pszDat != NULL )		/*  ���������ꍇ  */
	{
		printf("\n�����������ʁA%d�����ڂɂ���܂����B\n", pszDat - szStr + 1 );

	}else						/*  ������Ȃ������ꍇ  */
	{
		printf("\n\"%s\" ��������܂���ł����B\n", szKey );
	}

	return 0;
}