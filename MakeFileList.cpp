/************************************************************************************
	指定されたフォルダ内の全ファイル名をファイルに出力するプログラム v006
*************************************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define		OUTPUT_FILE		"ファイル(フォルダ)リスト.txt"
#define		LIST(IDX)		mszLists+MAX_PATH*IDX
#define		BUFF_SIZE		512

void search_file(char *pszDir, char *pszExt );

FILE	*phfile;
char	szPath[MAX_PATH] = "";	// ファイルリストを取得したいフォルダ
char	achMode[4] = 			// 出力モード [0]:ファイル名出力形式
    	{ '0', '0', '0', '0' };	//            [1]:出力対象範囲
    	                    	//            [2]:ファイル/フォルダのリスト選択
    	                    	//            [3]:ソート有無選択
int 	iCnt = 0;				// 見つかったファイルのカウント用

int main(int argc, char **argv)
{
	char	szLine[BUFF_SIZE];	// 入力用TEMP
	char	szOutputDir[MAX_PATH];// リストの出力先
	char	szOutputFile[sizeof(szOutputDir)+sizeof(OUTPUT_FILE)];// 出力ファイル名
	char	szExt[MAX_PATH] = "";// 拡張子
	char	*pszExtPos;			// 拡張子指定時の処理用
	char	*mszLists;			// ソート用（リスト格納）
	char	szTemp[MAX_PATH];	// ソート用（入替退避）
	int 	iExtLen = 0;		// ワイルド文字を含んだ拡張子を指定された場合の処理用
	int 	i = 0, j = 0;

	puts(" ☆ ファイルリスト作成ソフト Ver.6.00 ☆");

	/* 入力ファイルをドラッグ又はコマンドラインに入力して渡された場合 */
	if( argc > 1 ) {
		/* 入力されたファイルまでのパスを取得 */
		for( i = 0; argv[1][i] != '\0'; i++ ) {
			if( argv[1][i] == '\\' || argv[1][i] == '/' ) {
				strncpy( szPath, argv[1], i+1 );
				szPath[i+1] = '\0';
			}
			/*  全角文字で半角カナ以外の場合  */
			if( argv[1][i] < 0 &&
			   ( (unsigned char)argv[1][i] < 0xa0 ||
			     (unsigned char)argv[1][i] > 0xdf ) )
			{	i++;	}
		}
		if( argc > 2 ) {
			putchar('\n');
			puts("<!>-------------------------------------------------------------------------･");
			puts("| 複数のファイルをドラッグされたので、渡されたファイルのリストを作成します。|");
			puts("･---------------------------------------------------------------------------･");
		}
	/* プログラムを引数なしで起動した場合 */
	}else {
		/* ファイル一覧を作成したいフォルダの指定 */
		printf("\nファイル(フォルダ)一覧を作成したいフォルダまでのパスを入力してください。\n> ");
		fgets( szPath, sizeof(szPath), stdin );
		szPath[strlen(szPath)-1] = '\0';	// 改行文字削除＆'\'文字分確保

		/* 入力文字数のチェック */
		if( strlen(szPath) < 2 ) {
			puts("\nERROR : ２文字以上のパスを入力してください。");
			printf("Press any key to continue "), getch();
			exit( 1 );
		/* 文字列の最後に'\'を付ける */
		}else if( szPath[strlen(szPath)-1] != '/' && szPath[strlen(szPath)-1] != '\\' ) {
			strcat( szPath, "\\" );
		}
	}

	/* ファイルリスト作成 OR フォルダリスト作成 の選択 */
	if( argc <= 2 ) {
		puts("\n作成したいリストの種類を選択してください。（省略可）");
		puts("( 0:ファイル，1:フォルダ, 2:ファイル＆フォルダ )");
		printf(" [ 省略時：ファイルリスト ]\n> ");
		fgets( szLine, BUFF_SIZE, stdin );
		sscanf( szLine, "%c", &achMode[2] );
		/* 入力チェック */
		if( achMode[2] != '1' && achMode[2] != '2' ) { achMode[2] = '0'; }
	}

	/* 一覧を作成したいファイルのタイプを指定 */
	if( achMode[2] != '1' ) {
		puts("\nリストを作成したいファイルの拡張子を入力してください。（省略可）");
		/* ファイルリスト作成モードにより切り替え */
		if( argc <= 2 ) {
			puts("( 例: * , txt , htm* , c*, jp*;gif | ※複数選択可 )");
			printf(" [ 省略時：*(全種類) ]\n> ");
		}else {
			puts("( 例: * , txt , htm* , c*, jp* | ※単一選択のみ )");
			printf("  [ 省略時：*(全種類) ]\n> ");
		}
		fgets( szExt, sizeof(szExt), stdin );
		szExt[strlen(szExt)-1] = '\0';	// 改行文字削除
	}
	/* 何も入力されなかった場合 */
	if( szExt[0] == '\0' ) { strcpy( szExt, "*" ); }

	/* カレントディレクトリをデフォルトの出力先として設定 */
	GetCurrentDirectory( MAX_PATH - 1, szOutputDir );

	/* 出力先のフォルダを指定 */
	puts("\n出力先のフォルダまでのパスを入力してください。（省略可）");
	printf("( ※ \" 1 \" 入力時：%s )\n", szPath );
	printf(" [ 省略時：%s ]\n> ", szOutputDir );
	fgets(szLine, sizeof(szOutputDir), stdin );
	szLine[strlen(szLine)-1] = '\0';	// 改行文字削除＆'\'文字分確保

	/* ファイルリスト取得先に出力する場合 */
	if( szLine[0] == '1' ) {
		putchar('\n');
		puts("<!>-----------------------------------------------------------------･");
		puts("| '1' を入力されたので、ファイルリスト取得先のフォルダに出力します。|");
		puts("･-------------------------------------------------------------------･");
		strcpy( szOutputDir, szPath );

	/* 通常選択 */
	}else {
		/* 入力された文字列から出力先を選択 */
		if( szLine[0] != '\0' ) {
			strcpy( szOutputDir, szLine );

		/* 何も入力されずにEnterを押された場合はカレントフォルダに作成 */
		}else {
			putchar('\n');
			puts("<!>--------------------------------------------------------------･");
			puts("| 省略されたので、上記 '省略時：'に書かれたフォルダに出力します。|");
			puts("･----------------------------------------------------------------･");
		}

		/* 文字列の最後に'\'を付ける */
		if( szOutputDir[strlen(szOutputDir)-1] != '/' &&
		    szOutputDir[strlen(szOutputDir)-1] != '\\' ) {
			strcat( szOutputDir, "\\" );
		}
	}

	/* ファイル名の出力形式（フルパス出力有無）を指定 */
	puts("\nファイル名の出力形式を指定してください。（省略可）");
	printf("( 1:フルパス，0:ファイル名のみ )\n [ 省略時：ファイル名のみ ]\n> ");
	fgets( szLine, BUFF_SIZE, stdin );
	sscanf( szLine, "%c", &achMode[0] );

	if( argc <= 2 ) {
		/* 出力対象フォルダの範囲（サブフォルダを含むかどうか）を指定 */
		puts("\nリストにサブフォルダのファイルも含みますか？（省略可）");
		printf("( 1:含む，0:含まない )\n [ 省略時：含まない ]\n> ");
		fgets( szLine, BUFF_SIZE, stdin );
		sscanf( szLine, "%c", &achMode[1] );
	}else {
		/* ソートの有無を指定 */
		puts("\nリストをソート〔ファイル名順に出力〕しますか？（省略可）");
		printf("( 1:する，0:しない )\n [ 省略時：する ]\n> ");
		fgets( szLine, BUFF_SIZE, stdin );
		sscanf( szLine, "%c", &achMode[3] );
	}

	/* 確認画面の出力 */
	system("cls");	/*  画面のクリア  */
	puts("\n\n  **************** 設定事項確認 ****************\n");

	if( argc <= 2 ) {
		puts("  <<作成するリストの種類>>");
		( achMode[2] == '2' ) ?
			puts("    ファイル＆フォルダリスト\n"):
			( achMode[2] == '1' ) ?
				puts("    フォルダリスト\n"):
				puts("    ファイルリスト\n");

		puts("  <<一覧作成対象フォルダ>>");
		printf("    %s\n\n", szPath );
	}
	if( achMode[2] != '1' ) {
		puts("  <<ファイルの種類>>");
		printf("    %s\n\n", szExt );
	}
	puts("  <<出力先フォルダ>>");
	printf("    %s\n\n", szOutputDir );
	puts("  <<出力形式>>");
	printf("    ファイル名出力形式：");
	( achMode[0] == '1' ) ? puts("フルパス"):
	                        puts("ファイル名のみ");
	if( argc <= 2 ) {
		printf("    リスト出力の範囲  ：");
		( achMode[1] == '1' ) ? puts("サブフォルダを含む"):
		                        puts("サブフォルダを含まない");
	}else {
		printf("    ソートの有無  ：");
		( achMode[3] != '0' ) ? puts("ソートする"):
		                        puts("ソートしない");
	}

	printf("\nこの設定でよろしいですか？(yes:y /no:n) ");
	fgets( szLine, BUFF_SIZE, stdin );
	if( szLine[0] != 'y' ) {
		puts("\n処理を中断します。");
		printf("Press any key to continue "), getch();
		return 1;
	}

	putchar( '\n' );

	/* 出力ファイル名の作成 */
	sprintf( szOutputFile, "%s%s", szOutputDir, OUTPUT_FILE );

	/* 出力先フォルダに出力ファイルと同じ名前のファイルが無いかチェック */
	if( fopen( szOutputFile, "r" ) != NULL ) {
		putchar('\n');
		if( strcmp( szPath, szOutputDir ) == 0 ) {
			puts("<!>-------------------------------------------------------------------------･");
			puts("| ファイルリスト取得先に出力ファイルと同名のファイルが存在します。          |");
			puts("| ( 出力先がファイルリスト取得先と同一のため処理続行すると上書きされます。) |");
			puts("･---------------------------------------------------------------------------･");
		}else{
			puts("<!>-----------------------------------------------------------･");
			puts("| 出力先のフォルダに出力ファイルと同名のファイルが存在します。|");
			puts("･-------------------------------------------------------------･");
		}
		printf("  [ 出力ファイルの名前:%s ]\n", OUTPUT_FILE );
		printf("\nどうしますか？( 1:上書き，0:中断 ) ");
		fgets( szLine, BUFF_SIZE, stdin );
		if( szLine[0] != '1' ) {
			puts("\n処理を中断します。");
			printf("Press any key to continue "), getch();
			return 2;
		}
	}

	/* 出力用ファイルを開く */
	if( (phfile = fopen( szOutputFile, "w" )) == NULL ) {
		puts("\nERROR : 出力用ファイルを開く事が出来ませんでした。");
		printf("Press any key to continue "), getch();
		exit( 3 );
	}

	/* -- ファイルリストの作成 -- */

	/* 複数のファイルをドラッグされた場合 */
	if( argc > 2 ) {
		if( achMode[3] != '0' ) {
			/* メモリ領域の確保 */
			if((mszLists = (char *)malloc((argc-1) * MAX_PATH)) == NULL) {
				puts("\nERROR : 処理に必要なメモリ領域を確保できませんでした。");
				printf("\nソートしないで出力しますか？(yes:y /no:n) ");
				fgets( szLine, BUFF_SIZE, stdin );
				if( szLine[0] != 'y' ) {
					fclose( phfile );
					remove( szOutputFile );
					puts("\n処理を中断します。");
					printf("Press any key to continue "), getch();
					exit( 4 );
				}
				achMode[3] = '0';	// ソート無しへ変更
			}
		}
		/* 拡張子指定にワイルド文字を使用されたかのチェック */
		if( szExt[0] != '*' && (pszExtPos = strstr( szExt, "*" )) != NULL ) {
			iExtLen = pszExtPos - szExt;
		}
		for( i = 1; i < argc; i++ ) {
			int 	iPos = 0, iExt = 0;

			/* 拡張子の位置およびファイル名の位置を取得 */
			for( j = 0; argv[i][j] != '\0'; j++ ) {
				if( argv[i][j] == '\\' || argv[i][j] == '/' ) {
					iPos = j + 1;
				}
				/* 拡張子の取得 */
				if( argv[i][j] == '.' ) {
					iExt = j + 1;
				}
				/*  全角文字で半角カナ以外の場合  */
				if( argv[i][j] < 0 &&
				   ( (unsigned char)argv[i][j] < 0xa0 ||
				     (unsigned char)argv[i][j] > 0xdf ) )
				{	j++;	}
			}

			/* 拡張子の一致チェック */
			if( strcmp( szExt, "*" ) == 0 ||
			       strcmp( szExt, &argv[i][iExt] ) == 0 ||
			      ( iExtLen > 0 && strncmp( szExt, &argv[i][iExt], iExtLen ) == 0 ) ) {

				/* リストを出力 */
				if( achMode[3] != '0' ) {		// ソート有りの場合
					/* ファイル名を配列に出力 */
					( achMode[0] == '1' ) ?
						strcpy( LIST(iCnt), argv[i] ):
						strcpy( LIST(iCnt), &argv[i][iPos] );

				}else {							// ソート無しの場合
					/* ファイル名をファイルへ出力 */
					( achMode[0] == '1' ) ?
						fprintf( phfile, "%s\n", argv[i] ):
						fprintf( phfile, "%s\n", &argv[i][iPos] );
				}
				iCnt++;
			}
		}
		/* リストのソート処理 */
		if( achMode[3] != '0' ) {
			for( i = 0; iCnt > 1 && i < iCnt - 1; i++ ) {
				for( j = i + 1; j < iCnt; j++ ) {
					if( strcmp( LIST(i), LIST(j) ) > 0 ) {
						strcpy( szTemp, LIST(i) );
						strcpy( LIST(i), LIST(j) );
						strcpy( LIST(j), szTemp );
					}
				}
			}
			/* ソート結果をファイルに出力 */
			for( i = 0; i < iCnt; i++ ) {
				fprintf( phfile, "%s\n", LIST(i) );
			}
			free( mszLists );
		}

	/* 拡張子を複数指定された場合 */
	}else if((pszExtPos = strtok( szExt, ";" )) != NULL) {
		/* ファイルの検索処理実行 */
		search_file( "", pszExtPos );
		while( (pszExtPos = strtok( NULL, ";" )) != NULL ) {
			/* ファイルの検索処理実行 */
			search_file( "", pszExtPos );
		}

	/* 拡張子を一つだけ指定された場合 */
	}else {
		/* ファイルの検索処理実行 */
		search_file( "", szExt );
	}
	fclose( phfile );

	putchar('\n');
	/* ファイルリスト作成結果の出力 */
	if( iCnt == 0 ) {	// ファイル(フォルダ)が存在しなかった場合
		remove( szOutputFile );
		puts("<!>---------------------------------------------------------------･");
		puts("| 入力されたパスに取得できるファイル(フォルダ)がありませんでした。|");
		puts("･-----------------------------------------------------------------･");
		printf("Press any key to continue "), getch();

	}else {		// 取得するファイル(フォルダ)が存在した場合
		puts("<!>---------------------------------------------･");
		puts("| 正常にファイル(フォルダ)リストを作成しました。|");
		puts("･-----------------------------------------------･");
		printf("Press any key to continue "), getch();
	}

	return 0;
}

/*******************************************************************************
	引数として渡されたフォルダ内のファイルを検索しファイルに出力する関数
********************************************************************************/
void search_file(char *pszDir, char *pszExt )
{
	WIN32_FIND_DATA fd;
	HANDLE	hFind;
	char	szKeyName[MAX_PATH] = "";		// 検索するファイル名
	char	szDirectory[MAX_PATH] = "";		// サブフォルダ名格納用
	BOOL	bRet = TRUE;

	// 検索対象パスの文字長チェック
	if( strlen(szPath) + strlen(pszDir) + strlen(pszExt) + 3 > MAX_PATH ) {
		printf("\n取得するフォルダ名が長すぎます。(%s)\n\n", pszDir);
		return;
	}else {
		sprintf( szKeyName, "%s\\%s\\*.%s", szPath, pszDir, pszExt );
	}
	/* 指定されたフォルダ内にある最初のファイルを検索 */
	hFind = FindFirstFile(szKeyName, &fd);

	while( hFind != INVALID_HANDLE_VALUE && bRet && achMode[2] != '1' ) {
		/* ファイル名の場合 */
		if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
		   !( (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0 &&
		      (fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0 ) )
		{
			if( achMode[2] == '2' ) {	fprintf( phfile, "[FILE] " );	}

			/* 出力ファイルにファイル名を出力する */
			if( achMode[0] == '1' ) {
				( pszDir[0] != '\0' ) ?
					fprintf( phfile, "%s%s\\%s\n", szPath, pszDir, fd.cFileName ):
					fprintf( phfile, "%s%s\n", szPath, fd.cFileName );
				iCnt++;
			}else {
				fprintf( phfile, "%s\n", fd.cFileName );
				iCnt++;
			}
		}
		bRet = FindNextFile( hFind, &fd );	// 次のファイルを検索する
	}
	FindClose( hFind );

	/* 出力対象がサブフォルダを含む場合またはフォルダ一覧を取得する場合 */
	if( achMode[1] == '1' || achMode[2] != '0' ) {
		bRet = TRUE;
		sprintf( szKeyName, "%s\\%s\\*.*", szPath, pszDir );
		/* 指定されたフォルダ内にある最初のファイルを検索 */
		hFind = FindFirstFile(szKeyName, &fd);

		while( hFind != INVALID_HANDLE_VALUE && bRet ) {
			/* 取得したデータがフォルダ名だった場合 */
			if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
			   ( strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0 ) )
			{
				/* 出力ファイルにフォルダ名を出力する */
				if( achMode[2] != '0' ) {
					if( achMode[2] == '2' ) {	fprintf( phfile, "[DIR] " ); }
					/* ファイル出力形式選択切り替え */
					if( achMode[0] == '1' ) {
						( pszDir[0] != '\0' ) ?
							fprintf( phfile, "%s%s\\%s\n", szPath, pszDir, fd.cFileName ):
							fprintf( phfile, "%s%s\n", szPath, fd.cFileName );
					}else {
						fprintf( phfile, "%s\n", fd.cFileName );
					}
					iCnt++;
				}
				/* サブフォルダ名の生成 */
				if( pszDir[0] == '\0' ) {
					sprintf( szDirectory, "%s", fd.cFileName );
				}else {
					sprintf( szDirectory, "%s\\%s", pszDir, fd.cFileName );
				}
				/* サブフォルダ内のファイルを検索(再帰処理) */
				if( achMode[1] == '1' ) {
					search_file( szDirectory, pszExt );
				}
			}
			bRet = FindNextFile( hFind, &fd );	// 次のファイルを検索する
		}
		FindClose( hFind );
	}

	return;
}
