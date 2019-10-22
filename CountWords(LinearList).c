/**************************************************************************************
	入力されたソースファイル上の単語数を数えるプログラム（線形リストサンプル）
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define	OUTPUT_FILE	"解析結果.txt"

enum { Buff = 128, MAX_LENGTH = 25 };

struct Word_Data{
	char	szWord[MAX_LENGTH+1];
	int 	iCount;
	struct Word_Data *next;
};

void Analysis_Word(struct Word_Data **node, char *pszWord );
struct Word_Data *Get_Memory(void);
void Output_List(struct Word_Data *node, char chMode);
void DeleteNode(struct Word_Data **node);
void ClearList(struct Word_Data **node);
void Stop_Prg(void);

FILE	*phfile;

int main(void)
{
	struct Word_Data	*node = NULL;
	char	szSrcName[Buff];
	char	szMode[Buff/2];
	char	szWork[Buff];
	char	chDat;
	int 	i = 0;

	printf("解析したいソースのファイル名を入力してください。\n> ");
	gets( szSrcName );

	if( (phfile = fopen(szSrcName, "r")) == NULL ) {
		puts("入力された名前のソースファイルを開く事が出来ませんでした。");
		Stop_Prg();
		exit( 1 );
	}

	/* 処理結果一覧の出力形式 */
	printf("\n出力形式を指定してください。\n( 0:画面に出力，1:ファイルに出力 ) > ");
	scanf("%s", szMode );

	while( (chDat = fgetc( phfile )) != EOF ) {
		if( chDat < 0 ) {	/* 全角文字の場合 */
			fgetc( phfile );
			continue;
		}
		if( isalpha( chDat ) || chDat == '#' || chDat == '_' ) {
			if( i < MAX_LENGTH ) {	/* 最大文字数チェック */
				szWork[i++] = chDat;
			}
		}else if( i > 0 ) {
			szWork[i] = '\0';
			i = 0;
			/* 出現した単語を解析する */
			Analysis_Word( &node, szWork );
		}
	}
	fclose( phfile );

	/* 処理結果を出力 */
	Output_List(node, szMode[0]);

	/* 全てのリストを削除する */
	ClearList( &node );

	puts("\n正常に処理を終了しました。");
	Stop_Prg();
	return 0;
}

/*-- 渡された単語を解析してリストを作成する関数 --*/
void Analysis_Word(struct Word_Data **node, char *pszWord)
{
	if( *node == NULL ) {	/* 新規追加の場合 */
		/* 単語リストに登録 */
		if( (*node = Get_Memory()) == NULL ) {	/* メモリ領域の確保 */
			puts("\n処理に必要なメモリを確保できませんでした。");
			fclose( phfile );
			ClearList( &(*node) );	/* リストを削除する */
			Stop_Prg();
			exit( 3 );
		}
		strcpy( (*node)->szWord, pszWord );
		(*node)->iCount = 1;
		/* 新しいノードを初期化 */
		(*node)->next = NULL;
		return;
	}

	if( strcmp( (*node)->szWord, pszWord ) == 0 ) {	/* 既出の単語の場合 */
		(*node)->iCount++;
		return;
	}else {
		/* 次のノードへのアドレスを渡して再帰する */
		Analysis_Word( &(*node)->next, pszWord );
	}
}

/*-- メモリ領域を確保する関数 --*/
struct Word_Data *Get_Memory(void)
{
     return( (struct Word_Data *)malloc(sizeof(struct Word_Data)) );
}

/*-- 処理結果一覧を出力する関数 --*/
void Output_List(struct Word_Data *node, char chMode)
{
	int 	iCnt = 0;

	/* ファイルへの出力する場合 */
	if( chMode == '1' ) {
		if( (phfile = fopen(OUTPUT_FILE, "w")) == NULL ) {
			puts("\n※ 出力用ファイルを開く事が出来なかったので、画面に出力します。");
			Stop_Prg();
			chMode = '0';
		}else {
			fputs( "単語\t個数\n", phfile );
		}
	}

	putchar('\n');
	/* 全てのリストを出力 */
	while( node != NULL )
	{
		( chMode == '1' ) ?
			fprintf(phfile, "%s\t%d個\n", node->szWord, node->iCount ):
			printf("%s : %d個\n", node->szWord, node->iCount );

		node = node->next;
		iCnt++;
	}
	if( iCnt == 0 ) {
		( chMode == '1' ) ?
			fprintf( phfile, "単語は一つもありません。\n" ):
			puts( "単語は一つもありません。" );
	}
	if( chMode == '1' ) {
		fclose( phfile );
	}

	return;
}

/*--- 先頭のノードを削除（参考） ---*/
void DeleteNode(struct Word_Data **node)
{
	struct Word_Data	*ptr;

	if( *node != NULL ) {
		ptr = (*node)->next;
		free( *node );
		*node = ptr;
	}
	return;
}

/*--- 全てのノードを削除 ---*/
void ClearList(struct Word_Data **node)
{
	struct Word_Data	*ptr;	/* 次ノードへのアドレスの退避用 */

	if( *node != NULL ) {
		ptr = (*node)->next;
		free( *node );
		*node = ptr;
		/* リストが無くなるまで再帰する */
		ClearList( &(*node) );
	}else {
		return;
	}
}

/*-- プログラムを一時停止させる関数 --*/
void Stop_Prg(void)
{
	printf("Press any key to continue "), getch();
	return;
}
