/************************************************
	������̕���(strtok�֐�)
*************************************************/
#include <stdio.h>
#include <string.h>

void strtoken2();

/*** �T���v�� -���̂P- ***/
int main(void)
{
	char *pStr1, *pStr2, *pStr3;
    char szBuff[50+1] = "aTESTaaDATAaaSAMPLE"; // ���̕�����
	char seps[10];		// ��؂蕶��
    
	printf("\n");
	printf("== �v���O�������̂P ==\n\n");

	strcpy( seps,"a" );		// ��؂镶�����w��

	printf(" %s\n\n", szBuff );

	printf("��\"%s\"�ŋ�؂�ƁA\n\n", seps );

	pStr1 = strtok(szBuff, seps);// szBuff��,��؂�Ő擪������擾
    pStr2 = strtok(NULL, seps);  // ����������ő��s����ꍇNULL��
    pStr3 = strtok(NULL, seps);  // �w��

	printf(" %s �� ", pStr1 );
	printf("%s �� ", pStr2 );
	printf("%s\n", pStr3 );

	printf("\n�ɂȂ�܂��B\n\n");

    /* �T���v�� Part.2 */
	strtoken2();

	return (0);
}

/*************************************************
	 �T���v�� -���̂Q-
	(�����ł��Ȃ��Ȃ�܂ŌJ��Ԃ��Ď擾)
**************************************************/
void strtoken2()
{
	char	szStr[70+1] = "London,Hawaii,Tokyo,Paris,NewYork,HongKong";
	char	*token;
	char	seps[3] = ",";
	int		n = 0;

	printf("\n== �v���O�������̂Q ==\n\n");
	printf("%s\n\n", szStr );
	token = strtok( szStr, seps );	/* �ŏ��̃g�[�N�����擾 */

	/* szStr �Ƀg�[�N�����Ȃ��Ȃ�܂ŌJ��Ԃ� */
	while( token != NULL )
	{
		printf(" %d�ڂ́A", 1 + n++ );
		printf("%s�B\n", token );
		token = strtok( NULL, seps );	/* ���̃g�[�N�����擾 */
	}
	putchar('\n');

	return;
}
