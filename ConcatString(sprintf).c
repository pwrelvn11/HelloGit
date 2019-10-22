/**********************************************************
	文字列を結合していくプログラム(書式付)
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define		BUF       		256		/*  文字列の最大入力幅の定義  */
#define		RET_LEN(N)		if( N > 0 ){ printf("ilen(%d) = %d\n", N, ilen ); }
       		          		   		/*  ilen の値を表示  */

int main(void)
{
	char	szWords[BUF];			/*  入力用の文字列    */
	char	szBun[BUF];  			/*  結合先の文字列    */
	int 	ilen = 0;    			/*  文字列全体の長さ  */

	system("cls");	/*  画面のクリア  */

	printf("1つ目の文節を入力してください。\n");
	printf("==> ");
	scanf("%s", szWords );

	/*  最初(1つ目)の文字列を格納  */
	ilen = sprintf( szBun, "%s、", szWords );

	RET_LEN(0); // この時のilenの値を見たい時は 1 を指定

	printf("\n2つ目の文節を入力してください。\n");
	printf("==> ");
	scanf("%s", szWords );

	/*  2つ目の文字列を結合  */
	ilen += sprintf( szBun + ilen, "%s、", szWords );	// 結合後の文字列の長さを格納( strlen(szBun)と同じ )

	RET_LEN(0);	// この時のilenの値を見たい時は 2 を指定

	printf("\n3つ目の文節を入力してください。\n");
	printf("==> ");
	scanf("%s", szWords );

	/*  3つ目の文字列を結合  */
	ilen += sprintf( szBun + ilen, "%s。", szWords );

	RET_LEN(0);	// この時のilenの値を見たい時は 3 を指定

	sprintf( szWords, "\n入力された文節で構成された文は、\n\n【%s】\n\nです。\n", szBun );

	printf("%s\nPress any key to continue ", szWords ); getch();

	return 0;
}
