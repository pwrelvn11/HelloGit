/************************************************
	文字列の分割(strtok関数)
*************************************************/
#include <stdio.h>
#include <string.h>

void strtoken2();

/*** サンプル -その１- ***/
int main(void)
{
	char *pStr1, *pStr2, *pStr3;
    char szBuff[50+1] = "aTESTaaDATAaaSAMPLE"; // 元の文字列
	char seps[10];		// 区切り文字
    
	printf("\n");
	printf("== プログラムその１ ==\n\n");

	strcpy( seps,"a" );		// 区切る文字を指定

	printf(" %s\n\n", szBuff );

	printf("を\"%s\"で区切ると、\n\n", seps );

	pStr1 = strtok(szBuff, seps);// szBuffを,区切りで先頭文字列取得
    pStr2 = strtok(NULL, seps);  // 同じ文字列で続行する場合NULLを
    pStr3 = strtok(NULL, seps);  // 指定

	printf(" %s と ", pStr1 );
	printf("%s と ", pStr2 );
	printf("%s\n", pStr3 );

	printf("\nになります。\n\n");

    /* サンプル Part.2 */
	strtoken2();

	return (0);
}

/*************************************************
	 サンプル -その２-
	(分割できなくなるまで繰り返して取得)
**************************************************/
void strtoken2()
{
	char	szStr[70+1] = "London,Hawaii,Tokyo,Paris,NewYork,HongKong";
	char	*token;
	char	seps[3] = ",";
	int		n = 0;

	printf("\n== プログラムその２ ==\n\n");
	printf("%s\n\n", szStr );
	token = strtok( szStr, seps );	/* 最初のトークンを取得 */

	/* szStr にトークンがなくなるまで繰り返す */
	while( token != NULL )
	{
		printf(" %d個目は、", 1 + n++ );
		printf("%s。\n", token );
		token = strtok( NULL, seps );	/* 次のトークンを取得 */
	}
	putchar('\n');

	return;
}
