/**************************************************************************************
	���͂��ꂽ�\�[�X�t�@�C����̒P�ꐔ�𐔂���v���O�����i���`���X�g�T���v���j
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define	OUTPUT_FILE	"��͌���.txt"

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

	printf("��͂������\�[�X�̃t�@�C��������͂��Ă��������B\n> ");
	gets( szSrcName );

	if( (phfile = fopen(szSrcName, "r")) == NULL ) {
		puts("���͂��ꂽ���O�̃\�[�X�t�@�C�����J�������o���܂���ł����B");
		Stop_Prg();
		exit( 1 );
	}

	/* �������ʈꗗ�̏o�͌`�� */
	printf("\n�o�͌`�����w�肵�Ă��������B\n( 0:��ʂɏo�́C1:�t�@�C���ɏo�� ) > ");
	scanf("%s", szMode );

	while( (chDat = fgetc( phfile )) != EOF ) {
		if( chDat < 0 ) {	/* �S�p�����̏ꍇ */
			fgetc( phfile );
			continue;
		}
		if( isalpha( chDat ) || chDat == '#' || chDat == '_' ) {
			if( i < MAX_LENGTH ) {	/* �ő啶�����`�F�b�N */
				szWork[i++] = chDat;
			}
		}else if( i > 0 ) {
			szWork[i] = '\0';
			i = 0;
			/* �o�������P�����͂��� */
			Analysis_Word( &node, szWork );
		}
	}
	fclose( phfile );

	/* �������ʂ��o�� */
	Output_List(node, szMode[0]);

	/* �S�Ẵ��X�g���폜���� */
	ClearList( &node );

	puts("\n����ɏ������I�����܂����B");
	Stop_Prg();
	return 0;
}

/*-- �n���ꂽ�P�����͂��ă��X�g���쐬����֐� --*/
void Analysis_Word(struct Word_Data **node, char *pszWord)
{
	if( *node == NULL ) {	/* �V�K�ǉ��̏ꍇ */
		/* �P�ꃊ�X�g�ɓo�^ */
		if( (*node = Get_Memory()) == NULL ) {	/* �������̈�̊m�� */
			puts("\n�����ɕK�v�ȃ��������m�ۂł��܂���ł����B");
			fclose( phfile );
			ClearList( &(*node) );	/* ���X�g���폜���� */
			Stop_Prg();
			exit( 3 );
		}
		strcpy( (*node)->szWord, pszWord );
		(*node)->iCount = 1;
		/* �V�����m�[�h�������� */
		(*node)->next = NULL;
		return;
	}

	if( strcmp( (*node)->szWord, pszWord ) == 0 ) {	/* ���o�̒P��̏ꍇ */
		(*node)->iCount++;
		return;
	}else {
		/* ���̃m�[�h�ւ̃A�h���X��n���čċA���� */
		Analysis_Word( &(*node)->next, pszWord );
	}
}

/*-- �������̈���m�ۂ���֐� --*/
struct Word_Data *Get_Memory(void)
{
     return( (struct Word_Data *)malloc(sizeof(struct Word_Data)) );
}

/*-- �������ʈꗗ���o�͂���֐� --*/
void Output_List(struct Word_Data *node, char chMode)
{
	int 	iCnt = 0;

	/* �t�@�C���ւ̏o�͂���ꍇ */
	if( chMode == '1' ) {
		if( (phfile = fopen(OUTPUT_FILE, "w")) == NULL ) {
			puts("\n�� �o�͗p�t�@�C�����J�������o���Ȃ������̂ŁA��ʂɏo�͂��܂��B");
			Stop_Prg();
			chMode = '0';
		}else {
			fputs( "�P��\t��\n", phfile );
		}
	}

	putchar('\n');
	/* �S�Ẵ��X�g���o�� */
	while( node != NULL )
	{
		( chMode == '1' ) ?
			fprintf(phfile, "%s\t%d��\n", node->szWord, node->iCount ):
			printf("%s : %d��\n", node->szWord, node->iCount );

		node = node->next;
		iCnt++;
	}
	if( iCnt == 0 ) {
		( chMode == '1' ) ?
			fprintf( phfile, "�P��͈������܂���B\n" ):
			puts( "�P��͈������܂���B" );
	}
	if( chMode == '1' ) {
		fclose( phfile );
	}

	return;
}

/*--- �擪�̃m�[�h���폜�i�Q�l�j ---*/
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

/*--- �S�Ẵm�[�h���폜 ---*/
void ClearList(struct Word_Data **node)
{
	struct Word_Data	*ptr;	/* ���m�[�h�ւ̃A�h���X�̑ޔ�p */

	if( *node != NULL ) {
		ptr = (*node)->next;
		free( *node );
		*node = ptr;
		/* ���X�g�������Ȃ�܂ōċA���� */
		ClearList( &(*node) );
	}else {
		return;
	}
}

/*-- �v���O�������ꎞ��~������֐� --*/
void Stop_Prg(void)
{
	printf("Press any key to continue "), getch();
	return;
}
