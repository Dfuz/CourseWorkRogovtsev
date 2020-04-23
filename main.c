#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define stringsize 14 // ����� �������������� ����� ��������� �����

/*���� ���������� �������*/
int fileProcessing(FILE*, char*); // ������������ ��������� ���������� �����
int fileCheck(FILE*); // ������������ �������� ���������� �����

/*�������� ���������*/
int main(int argc, char* argv[])
{
	int i = 1; // ��������������� ����������-�������, ����������� ��� ������������ ����� ��������� �����
	system("color F0"); // ����� ���� �������
	setlocale(LC_ALL, "Rus"); // ����������� �������� �����
	char filenameout[stringsize]; // ������ ��� �������� ��������� �������������� ����� �����
	FILE* ifp;
	if (argc == 1) // ���� ��� ����������
	{
		fputs("�� ������ ��������� ������� (�� �������� ����� ������).\n", stderr);
		system("PAUSE");
		exit(1);
	}
	while (--argc > 0)
	{
		ifp = fopen(*++argv, "r"); // �������� ����� ��� ������
		int error = fileCheck(ifp); // ���� ������
		if (error == 1)
		{
			// �������� �� �������� �����
			fprintf(stderr, "\n���������� ������� ���� �%d ��� ������.\n", i);
			exit(2);
		}
		if (error == 2)
		{
			// �������� �� ������� �����
			fprintf(stderr, "\n���� �%d ����.\n", i);
			exit(3);
		}
		sprintf(filenameout, "fileout%02d.txt", i); // ��������� ����� ��������� �����
		error = fileProcessing(ifp, filenameout); // ����� ������������
		if (error == 1)
		{
			// �������� �� ������ ��������� �����
			fprintf(stderr, "\n��� ��������� ����� �%d �������� �������������� ������.\n", i);
			exit(4);
		}
		printf("\n���� �%d ���������.\n", i);
		fclose(ifp); // �������� ����� ��� ������
		i++;
	}
	printf("\n��������� ����������� �������.\n");
	return 0;
}

/*���� ���������� �������*/
int fileCheck(FILE* filein)
{
	if (filein == NULL) // �������� �� �������� �����
		return 1; // ��� ������ �������� �����
	fseek(filein, 0, SEEK_END); // ������� ������� � ����� �����
	long pos = ftell(filein); // ��������� �������� ������� �������
	if (pos == 0)
		return 2; // �������� �� ������� �����
	rewind(filein); // ���������� �������� ��������� �� ������ ������
	return 0;
}

int fileProcessing(FILE* ifp, char* fileNameOut)
{
	FILE* ofp = fopen(fileNameOut, "w"); // �������� ����� ��� ������
	long position;
	char c;
	int i = 0;
	while ((c = getc(ifp)) != EOF)
	{
		if (ferror(ifp)) return 1; // ���� ��������� ������
		if (c == '\t')
			for (i = 0; i <= 2; i++)
				putc(' ', ofp);
		else if ((c == '.') || (c == '!') || (c == '?') || (c == '�'))
		{
			putc(c, ofp);
			c = getc(ifp);
			if (ferror(ifp)) return 1; // ���� ��������� ������
			if (c == '.')
				while (((c == '.') || (c == '!') || (c == '?') || (c == '�')) && (c != EOF))
				{
					putc(c, ofp);
					c = getc(ifp);
					if (ferror(ifp))
					{
						fclose(ofp);
						return 1;
					} // ���� ��������� ������
				}
			if (c != EOF)
			{
				fprintf(ofp, "\n\r");
				if ((c != '\n') && (c != ' ')) putc(c, ofp);
			}
			else if (feof(ifp)) break;
		}
		else if (c == ' ')
		{
			putc(c, ofp);
			while ((c == ' ') && (!feof(ifp)))
			{
				position = ftell(ifp);
				c = getc(ifp);
				if (ferror(ifp))
				{
					fclose(ofp);
					return 1;
				} // ���� ��������� ������
			}
			fseek(ifp, position, SEEK_SET);
		}
		else putc(c, ofp);
	}
	fclose(ofp); // �������� ����� ��� ������
	return 0;
}
