/**********************************************************
	��������������Ă����v���O����(�����t)
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define		BUF       		256		/*  ������̍ő���͕��̒�`  */
#define		RET_LEN(N)		if( N > 0 ){ printf("ilen(%d) = %d\n", N, ilen ); }
       		          		   		/*  ilen �̒l��\��  */

int main(void)
{
	char	szWords[BUF];			/*  ���͗p�̕�����    */
	char	szBun[BUF];  			/*  ������̕�����    */
	int 	ilen = 0;    			/*  ������S�̂̒���  */

	system("cls");	/*  ��ʂ̃N���A  */

	printf("1�ڂ̕��߂���͂��Ă��������B\n");
	printf("==> ");
	scanf("%s", szWords );

	/*  �ŏ�(1��)�̕�������i�[  */
	ilen = sprintf( szBun, "%s�A", szWords );

	RET_LEN(0); // ���̎���ilen�̒l������������ 1 ���w��

	printf("\n2�ڂ̕��߂���͂��Ă��������B\n");
	printf("==> ");
	scanf("%s", szWords );

	/*  2�ڂ̕����������  */
	ilen += sprintf( szBun + ilen, "%s�A", szWords );	// ������̕�����̒������i�[( strlen(szBun)�Ɠ��� )

	RET_LEN(0);	// ���̎���ilen�̒l������������ 2 ���w��

	printf("\n3�ڂ̕��߂���͂��Ă��������B\n");
	printf("==> ");
	scanf("%s", szWords );

	/*  3�ڂ̕����������  */
	ilen += sprintf( szBun + ilen, "%s�B", szWords );

	RET_LEN(0);	// ���̎���ilen�̒l������������ 3 ���w��

	sprintf( szWords, "\n���͂��ꂽ���߂ō\�����ꂽ���́A\n\n�y%s�z\n\n�ł��B\n", szBun );

	printf("%s\nPress any key to continue ", szWords ); getch();

	return 0;
}
