/************************************************************************************
	�w�肳�ꂽ�t�H���_���̑S�t�@�C�������t�@�C���ɏo�͂���v���O���� v006
*************************************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define		OUTPUT_FILE		"�t�@�C��(�t�H���_)���X�g.txt"
#define		LIST(IDX)		mszLists+MAX_PATH*IDX
#define		BUFF_SIZE		512

void search_file(char *pszDir, char *pszExt );

FILE	*phfile;
char	szPath[MAX_PATH] = "";	// �t�@�C�����X�g���擾�������t�H���_
char	achMode[4] = 			// �o�̓��[�h [0]:�t�@�C�����o�͌`��
    	{ '0', '0', '0', '0' };	//            [1]:�o�͑Ώ۔͈�
    	                    	//            [2]:�t�@�C��/�t�H���_�̃��X�g�I��
    	                    	//            [3]:�\�[�g�L���I��
int 	iCnt = 0;				// ���������t�@�C���̃J�E���g�p

int main(int argc, char **argv)
{
	char	szLine[BUFF_SIZE];	// ���͗pTEMP
	char	szOutputDir[MAX_PATH];// ���X�g�̏o�͐�
	char	szOutputFile[sizeof(szOutputDir)+sizeof(OUTPUT_FILE)];// �o�̓t�@�C����
	char	szExt[MAX_PATH] = "";// �g���q
	char	*pszExtPos;			// �g���q�w�莞�̏����p
	char	*mszLists;			// �\�[�g�p�i���X�g�i�[�j
	char	szTemp[MAX_PATH];	// �\�[�g�p�i���֑ޔ��j
	int 	iExtLen = 0;		// ���C���h�������܂񂾊g���q���w�肳�ꂽ�ꍇ�̏����p
	int 	i = 0, j = 0;

	puts(" �� �t�@�C�����X�g�쐬�\�t�g Ver.6.00 ��");

	/* ���̓t�@�C�����h���b�O���̓R�}���h���C���ɓ��͂��ēn���ꂽ�ꍇ */
	if( argc > 1 ) {
		/* ���͂��ꂽ�t�@�C���܂ł̃p�X���擾 */
		for( i = 0; argv[1][i] != '\0'; i++ ) {
			if( argv[1][i] == '\\' || argv[1][i] == '/' ) {
				strncpy( szPath, argv[1], i+1 );
				szPath[i+1] = '\0';
			}
			/*  �S�p�����Ŕ��p�J�i�ȊO�̏ꍇ  */
			if( argv[1][i] < 0 &&
			   ( (unsigned char)argv[1][i] < 0xa0 ||
			     (unsigned char)argv[1][i] > 0xdf ) )
			{	i++;	}
		}
		if( argc > 2 ) {
			putchar('\n');
			puts("<!>-------------------------------------------------------------------------�");
			puts("| �����̃t�@�C�����h���b�O���ꂽ�̂ŁA�n���ꂽ�t�@�C���̃��X�g���쐬���܂��B|");
			puts("�---------------------------------------------------------------------------�");
		}
	/* �v���O�����������Ȃ��ŋN�������ꍇ */
	}else {
		/* �t�@�C���ꗗ���쐬�������t�H���_�̎w�� */
		printf("\n�t�@�C��(�t�H���_)�ꗗ���쐬�������t�H���_�܂ł̃p�X����͂��Ă��������B\n> ");
		fgets( szPath, sizeof(szPath), stdin );
		szPath[strlen(szPath)-1] = '\0';	// ���s�����폜��'\'�������m��

		/* ���͕������̃`�F�b�N */
		if( strlen(szPath) < 2 ) {
			puts("\nERROR : �Q�����ȏ�̃p�X����͂��Ă��������B");
			printf("Press any key to continue "), getch();
			exit( 1 );
		/* ������̍Ō��'\'��t���� */
		}else if( szPath[strlen(szPath)-1] != '/' && szPath[strlen(szPath)-1] != '\\' ) {
			strcat( szPath, "\\" );
		}
	}

	/* �t�@�C�����X�g�쐬 OR �t�H���_���X�g�쐬 �̑I�� */
	if( argc <= 2 ) {
		puts("\n�쐬���������X�g�̎�ނ�I�����Ă��������B�i�ȗ��j");
		puts("( 0:�t�@�C���C1:�t�H���_, 2:�t�@�C�����t�H���_ )");
		printf(" [ �ȗ����F�t�@�C�����X�g ]\n> ");
		fgets( szLine, BUFF_SIZE, stdin );
		sscanf( szLine, "%c", &achMode[2] );
		/* ���̓`�F�b�N */
		if( achMode[2] != '1' && achMode[2] != '2' ) { achMode[2] = '0'; }
	}

	/* �ꗗ���쐬�������t�@�C���̃^�C�v���w�� */
	if( achMode[2] != '1' ) {
		puts("\n���X�g���쐬�������t�@�C���̊g���q����͂��Ă��������B�i�ȗ��j");
		/* �t�@�C�����X�g�쐬���[�h�ɂ��؂�ւ� */
		if( argc <= 2 ) {
			puts("( ��: * , txt , htm* , c*, jp*;gif | �������I���� )");
			printf(" [ �ȗ����F*(�S���) ]\n> ");
		}else {
			puts("( ��: * , txt , htm* , c*, jp* | ���P��I���̂� )");
			printf("  [ �ȗ����F*(�S���) ]\n> ");
		}
		fgets( szExt, sizeof(szExt), stdin );
		szExt[strlen(szExt)-1] = '\0';	// ���s�����폜
	}
	/* �������͂���Ȃ������ꍇ */
	if( szExt[0] == '\0' ) { strcpy( szExt, "*" ); }

	/* �J�����g�f�B���N�g�����f�t�H���g�̏o�͐�Ƃ��Đݒ� */
	GetCurrentDirectory( MAX_PATH - 1, szOutputDir );

	/* �o�͐�̃t�H���_���w�� */
	puts("\n�o�͐�̃t�H���_�܂ł̃p�X����͂��Ă��������B�i�ȗ��j");
	printf("( �� \" 1 \" ���͎��F%s )\n", szPath );
	printf(" [ �ȗ����F%s ]\n> ", szOutputDir );
	fgets(szLine, sizeof(szOutputDir), stdin );
	szLine[strlen(szLine)-1] = '\0';	// ���s�����폜��'\'�������m��

	/* �t�@�C�����X�g�擾��ɏo�͂���ꍇ */
	if( szLine[0] == '1' ) {
		putchar('\n');
		puts("<!>-----------------------------------------------------------------�");
		puts("| '1' ����͂��ꂽ�̂ŁA�t�@�C�����X�g�擾��̃t�H���_�ɏo�͂��܂��B|");
		puts("�-------------------------------------------------------------------�");
		strcpy( szOutputDir, szPath );

	/* �ʏ�I�� */
	}else {
		/* ���͂��ꂽ�����񂩂�o�͐��I�� */
		if( szLine[0] != '\0' ) {
			strcpy( szOutputDir, szLine );

		/* �������͂��ꂸ��Enter�������ꂽ�ꍇ�̓J�����g�t�H���_�ɍ쐬 */
		}else {
			putchar('\n');
			puts("<!>--------------------------------------------------------------�");
			puts("| �ȗ����ꂽ�̂ŁA��L '�ȗ����F'�ɏ����ꂽ�t�H���_�ɏo�͂��܂��B|");
			puts("�----------------------------------------------------------------�");
		}

		/* ������̍Ō��'\'��t���� */
		if( szOutputDir[strlen(szOutputDir)-1] != '/' &&
		    szOutputDir[strlen(szOutputDir)-1] != '\\' ) {
			strcat( szOutputDir, "\\" );
		}
	}

	/* �t�@�C�����̏o�͌`���i�t���p�X�o�͗L���j���w�� */
	puts("\n�t�@�C�����̏o�͌`�����w�肵�Ă��������B�i�ȗ��j");
	printf("( 1:�t���p�X�C0:�t�@�C�����̂� )\n [ �ȗ����F�t�@�C�����̂� ]\n> ");
	fgets( szLine, BUFF_SIZE, stdin );
	sscanf( szLine, "%c", &achMode[0] );

	if( argc <= 2 ) {
		/* �o�͑Ώۃt�H���_�͈̔́i�T�u�t�H���_���܂ނ��ǂ����j���w�� */
		puts("\n���X�g�ɃT�u�t�H���_�̃t�@�C�����܂݂܂����H�i�ȗ��j");
		printf("( 1:�܂ށC0:�܂܂Ȃ� )\n [ �ȗ����F�܂܂Ȃ� ]\n> ");
		fgets( szLine, BUFF_SIZE, stdin );
		sscanf( szLine, "%c", &achMode[1] );
	}else {
		/* �\�[�g�̗L�����w�� */
		puts("\n���X�g���\�[�g�k�t�@�C�������ɏo�́l���܂����H�i�ȗ��j");
		printf("( 1:����C0:���Ȃ� )\n [ �ȗ����F���� ]\n> ");
		fgets( szLine, BUFF_SIZE, stdin );
		sscanf( szLine, "%c", &achMode[3] );
	}

	/* �m�F��ʂ̏o�� */
	system("cls");	/*  ��ʂ̃N���A  */
	puts("\n\n  **************** �ݒ莖���m�F ****************\n");

	if( argc <= 2 ) {
		puts("  <<�쐬���郊�X�g�̎��>>");
		( achMode[2] == '2' ) ?
			puts("    �t�@�C�����t�H���_���X�g\n"):
			( achMode[2] == '1' ) ?
				puts("    �t�H���_���X�g\n"):
				puts("    �t�@�C�����X�g\n");

		puts("  <<�ꗗ�쐬�Ώۃt�H���_>>");
		printf("    %s\n\n", szPath );
	}
	if( achMode[2] != '1' ) {
		puts("  <<�t�@�C���̎��>>");
		printf("    %s\n\n", szExt );
	}
	puts("  <<�o�͐�t�H���_>>");
	printf("    %s\n\n", szOutputDir );
	puts("  <<�o�͌`��>>");
	printf("    �t�@�C�����o�͌`���F");
	( achMode[0] == '1' ) ? puts("�t���p�X"):
	                        puts("�t�@�C�����̂�");
	if( argc <= 2 ) {
		printf("    ���X�g�o�͈͂̔�  �F");
		( achMode[1] == '1' ) ? puts("�T�u�t�H���_���܂�"):
		                        puts("�T�u�t�H���_���܂܂Ȃ�");
	}else {
		printf("    �\�[�g�̗L��  �F");
		( achMode[3] != '0' ) ? puts("�\�[�g����"):
		                        puts("�\�[�g���Ȃ�");
	}

	printf("\n���̐ݒ�ł�낵���ł����H(yes:y /no:n) ");
	fgets( szLine, BUFF_SIZE, stdin );
	if( szLine[0] != 'y' ) {
		puts("\n�����𒆒f���܂��B");
		printf("Press any key to continue "), getch();
		return 1;
	}

	putchar( '\n' );

	/* �o�̓t�@�C�����̍쐬 */
	sprintf( szOutputFile, "%s%s", szOutputDir, OUTPUT_FILE );

	/* �o�͐�t�H���_�ɏo�̓t�@�C���Ɠ������O�̃t�@�C�����������`�F�b�N */
	if( fopen( szOutputFile, "r" ) != NULL ) {
		putchar('\n');
		if( strcmp( szPath, szOutputDir ) == 0 ) {
			puts("<!>-------------------------------------------------------------------------�");
			puts("| �t�@�C�����X�g�擾��ɏo�̓t�@�C���Ɠ����̃t�@�C�������݂��܂��B          |");
			puts("| ( �o�͐悪�t�@�C�����X�g�擾��Ɠ���̂��ߏ������s����Ə㏑������܂��B) |");
			puts("�---------------------------------------------------------------------------�");
		}else{
			puts("<!>-----------------------------------------------------------�");
			puts("| �o�͐�̃t�H���_�ɏo�̓t�@�C���Ɠ����̃t�@�C�������݂��܂��B|");
			puts("�-------------------------------------------------------------�");
		}
		printf("  [ �o�̓t�@�C���̖��O:%s ]\n", OUTPUT_FILE );
		printf("\n�ǂ����܂����H( 1:�㏑���C0:���f ) ");
		fgets( szLine, BUFF_SIZE, stdin );
		if( szLine[0] != '1' ) {
			puts("\n�����𒆒f���܂��B");
			printf("Press any key to continue "), getch();
			return 2;
		}
	}

	/* �o�͗p�t�@�C�����J�� */
	if( (phfile = fopen( szOutputFile, "w" )) == NULL ) {
		puts("\nERROR : �o�͗p�t�@�C�����J�������o���܂���ł����B");
		printf("Press any key to continue "), getch();
		exit( 3 );
	}

	/* -- �t�@�C�����X�g�̍쐬 -- */

	/* �����̃t�@�C�����h���b�O���ꂽ�ꍇ */
	if( argc > 2 ) {
		if( achMode[3] != '0' ) {
			/* �������̈�̊m�� */
			if((mszLists = (char *)malloc((argc-1) * MAX_PATH)) == NULL) {
				puts("\nERROR : �����ɕK�v�ȃ������̈���m�ۂł��܂���ł����B");
				printf("\n�\�[�g���Ȃ��ŏo�͂��܂����H(yes:y /no:n) ");
				fgets( szLine, BUFF_SIZE, stdin );
				if( szLine[0] != 'y' ) {
					fclose( phfile );
					remove( szOutputFile );
					puts("\n�����𒆒f���܂��B");
					printf("Press any key to continue "), getch();
					exit( 4 );
				}
				achMode[3] = '0';	// �\�[�g�����֕ύX
			}
		}
		/* �g���q�w��Ƀ��C���h�������g�p���ꂽ���̃`�F�b�N */
		if( szExt[0] != '*' && (pszExtPos = strstr( szExt, "*" )) != NULL ) {
			iExtLen = pszExtPos - szExt;
		}
		for( i = 1; i < argc; i++ ) {
			int 	iPos = 0, iExt = 0;

			/* �g���q�̈ʒu����уt�@�C�����̈ʒu���擾 */
			for( j = 0; argv[i][j] != '\0'; j++ ) {
				if( argv[i][j] == '\\' || argv[i][j] == '/' ) {
					iPos = j + 1;
				}
				/* �g���q�̎擾 */
				if( argv[i][j] == '.' ) {
					iExt = j + 1;
				}
				/*  �S�p�����Ŕ��p�J�i�ȊO�̏ꍇ  */
				if( argv[i][j] < 0 &&
				   ( (unsigned char)argv[i][j] < 0xa0 ||
				     (unsigned char)argv[i][j] > 0xdf ) )
				{	j++;	}
			}

			/* �g���q�̈�v�`�F�b�N */
			if( strcmp( szExt, "*" ) == 0 ||
			       strcmp( szExt, &argv[i][iExt] ) == 0 ||
			      ( iExtLen > 0 && strncmp( szExt, &argv[i][iExt], iExtLen ) == 0 ) ) {

				/* ���X�g���o�� */
				if( achMode[3] != '0' ) {		// �\�[�g�L��̏ꍇ
					/* �t�@�C������z��ɏo�� */
					( achMode[0] == '1' ) ?
						strcpy( LIST(iCnt), argv[i] ):
						strcpy( LIST(iCnt), &argv[i][iPos] );

				}else {							// �\�[�g�����̏ꍇ
					/* �t�@�C�������t�@�C���֏o�� */
					( achMode[0] == '1' ) ?
						fprintf( phfile, "%s\n", argv[i] ):
						fprintf( phfile, "%s\n", &argv[i][iPos] );
				}
				iCnt++;
			}
		}
		/* ���X�g�̃\�[�g���� */
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
			/* �\�[�g���ʂ��t�@�C���ɏo�� */
			for( i = 0; i < iCnt; i++ ) {
				fprintf( phfile, "%s\n", LIST(i) );
			}
			free( mszLists );
		}

	/* �g���q�𕡐��w�肳�ꂽ�ꍇ */
	}else if((pszExtPos = strtok( szExt, ";" )) != NULL) {
		/* �t�@�C���̌����������s */
		search_file( "", pszExtPos );
		while( (pszExtPos = strtok( NULL, ";" )) != NULL ) {
			/* �t�@�C���̌����������s */
			search_file( "", pszExtPos );
		}

	/* �g���q��������w�肳�ꂽ�ꍇ */
	}else {
		/* �t�@�C���̌����������s */
		search_file( "", szExt );
	}
	fclose( phfile );

	putchar('\n');
	/* �t�@�C�����X�g�쐬���ʂ̏o�� */
	if( iCnt == 0 ) {	// �t�@�C��(�t�H���_)�����݂��Ȃ������ꍇ
		remove( szOutputFile );
		puts("<!>---------------------------------------------------------------�");
		puts("| ���͂��ꂽ�p�X�Ɏ擾�ł���t�@�C��(�t�H���_)������܂���ł����B|");
		puts("�-----------------------------------------------------------------�");
		printf("Press any key to continue "), getch();

	}else {		// �擾����t�@�C��(�t�H���_)�����݂����ꍇ
		puts("<!>---------------------------------------------�");
		puts("| ����Ƀt�@�C��(�t�H���_)���X�g���쐬���܂����B|");
		puts("�-----------------------------------------------�");
		printf("Press any key to continue "), getch();
	}

	return 0;
}

/*******************************************************************************
	�����Ƃ��ēn���ꂽ�t�H���_���̃t�@�C�����������t�@�C���ɏo�͂���֐�
********************************************************************************/
void search_file(char *pszDir, char *pszExt )
{
	WIN32_FIND_DATA fd;
	HANDLE	hFind;
	char	szKeyName[MAX_PATH] = "";		// ��������t�@�C����
	char	szDirectory[MAX_PATH] = "";		// �T�u�t�H���_���i�[�p
	BOOL	bRet = TRUE;

	// �����Ώۃp�X�̕������`�F�b�N
	if( strlen(szPath) + strlen(pszDir) + strlen(pszExt) + 3 > MAX_PATH ) {
		printf("\n�擾����t�H���_�����������܂��B(%s)\n\n", pszDir);
		return;
	}else {
		sprintf( szKeyName, "%s\\%s\\*.%s", szPath, pszDir, pszExt );
	}
	/* �w�肳�ꂽ�t�H���_���ɂ���ŏ��̃t�@�C�������� */
	hFind = FindFirstFile(szKeyName, &fd);

	while( hFind != INVALID_HANDLE_VALUE && bRet && achMode[2] != '1' ) {
		/* �t�@�C�����̏ꍇ */
		if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
		   !( (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0 &&
		      (fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0 ) )
		{
			if( achMode[2] == '2' ) {	fprintf( phfile, "[FILE] " );	}

			/* �o�̓t�@�C���Ƀt�@�C�������o�͂��� */
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
		bRet = FindNextFile( hFind, &fd );	// ���̃t�@�C������������
	}
	FindClose( hFind );

	/* �o�͑Ώۂ��T�u�t�H���_���܂ޏꍇ�܂��̓t�H���_�ꗗ���擾����ꍇ */
	if( achMode[1] == '1' || achMode[2] != '0' ) {
		bRet = TRUE;
		sprintf( szKeyName, "%s\\%s\\*.*", szPath, pszDir );
		/* �w�肳�ꂽ�t�H���_���ɂ���ŏ��̃t�@�C�������� */
		hFind = FindFirstFile(szKeyName, &fd);

		while( hFind != INVALID_HANDLE_VALUE && bRet ) {
			/* �擾�����f�[�^���t�H���_���������ꍇ */
			if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
			   ( strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0 ) )
			{
				/* �o�̓t�@�C���Ƀt�H���_�����o�͂��� */
				if( achMode[2] != '0' ) {
					if( achMode[2] == '2' ) {	fprintf( phfile, "[DIR] " ); }
					/* �t�@�C���o�͌`���I��؂�ւ� */
					if( achMode[0] == '1' ) {
						( pszDir[0] != '\0' ) ?
							fprintf( phfile, "%s%s\\%s\n", szPath, pszDir, fd.cFileName ):
							fprintf( phfile, "%s%s\n", szPath, fd.cFileName );
					}else {
						fprintf( phfile, "%s\n", fd.cFileName );
					}
					iCnt++;
				}
				/* �T�u�t�H���_���̐��� */
				if( pszDir[0] == '\0' ) {
					sprintf( szDirectory, "%s", fd.cFileName );
				}else {
					sprintf( szDirectory, "%s\\%s", pszDir, fd.cFileName );
				}
				/* �T�u�t�H���_���̃t�@�C��������(�ċA����) */
				if( achMode[1] == '1' ) {
					search_file( szDirectory, pszExt );
				}
			}
			bRet = FindNextFile( hFind, &fd );	// ���̃t�@�C������������
		}
		FindClose( hFind );
	}

	return;
}
