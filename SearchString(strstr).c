/********************************************************************
	文字列の検索 (strstr関数)
*********************************************************************/
#include <stdio.h>
#include <string.h>

#define		BUFF	256		/*  入力最大桁  */

int main(void)
{
	char	szStr[BUFF] = "This is a pen.";		/*  検索する文字列  */
	char	szKey[BUFF] = "pen";				/*  検索対象の文字列  */
	char	*pszDat;
	int		result = 0;

	printf("元のデータ　　：%s\n", szStr );
	printf("検索対象文字列：%s\n", szKey );

	pszDat = strstr( szStr, szKey );		/*  strstr関数を使って検索  */

	if( pszDat != NULL )		/*  見つかった場合  */
	{
		printf("\n検索した結果、%d文字目にありました。\n", pszDat - szStr + 1 );

	}else						/*  見つからなかった場合  */
	{
		printf("\n\"%s\" が見つかりませんでした。\n", szKey );
	}

	return 0;
}