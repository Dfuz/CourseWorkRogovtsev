#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define stringsize 14 // длина относительного имени выходного файла

/*БЛОК ПРОТОТИПОВ ФУНКЦИЙ*/
int fileProcessing(FILE*, char*); // подпрограмма обработки текстового файла
int fileCheck(FILE*); // подпрограмма проверки текстового файла

/*ОСНОВНАЯ ПРОГРАММА*/
int main(int argc, char* argv[])
{
	int i = 1; // вспомогательная переменная-счётчик, необходимая для формирования имени выходного файла
	system("color F0"); // белый цвет консоли
	setlocale(LC_ALL, "Rus"); // подключение русского языка
	char filenameout[stringsize]; // строка для хранения выходного относительного имени файла
	FILE* ifp;
	if (argc == 1) // если нет аргументов
	{
		fputs("Не заданы параметры запуска (не написаны имена файлов).\n", stderr);
		system("PAUSE");
		exit(1);
	}
	while (--argc > 0)
	{
		ifp = fopen(*++argv, "r"); // открытие файла для чтения
		int error = fileCheck(ifp); // флаг ошибок
		if (error == 1)
		{
			// проверка на открытие файла
			fprintf(stderr, "\nНевозможно открыть файл №%d для чтения.\n", i);
			exit(2);
		}
		if (error == 2)
		{
			// проверка на пустоту файла
			fprintf(stderr, "\nФайл №%d пуст.\n", i);
			exit(3);
		}
		sprintf(filenameout, "fileout%02d.txt", i); // генерация имени выходного файла
		error = fileProcessing(ifp, filenameout); // вызов подпрограммы
		if (error == 1)
		{
			// проверка на ошибки обработки файла
			fprintf(stderr, "\nПри обработке файла №%d возникла непредвиденная ошибка.\n", i);
			exit(4);
		}
		printf("\nФайл №%d обработан.\n", i);
		fclose(ifp); // закрытие файла для чтения
		i++;
	}
	printf("\nПрограмма завершилась успешно.\n");
	return 0;
}

/*БЛОК РЕАЛИЗАЦИЙ ФУНКЦИЙ*/
int fileCheck(FILE* filein)
{
	if (filein == NULL) // проверка на открытие файла
		return 1; // код ошибки открытия файла
	fseek(filein, 0, SEEK_END); // перенос курсора в конец файла
	long pos = ftell(filein); // получения значения позиции курсора
	if (pos == 0)
		return 2; // проверка на пустоту файла
	rewind(filein); // установить значение указателя на начало потока
	return 0;
}

int fileProcessing(FILE* ifp, char* fileNameOut)
{
	FILE* ofp = fopen(fileNameOut, "w"); // открытие файла для записи
	long position;
	char c;
	int i = 0;
	while ((c = getc(ifp)) != EOF)
	{
		if (ferror(ifp)) return 1; // если произошла ошибка
		if (c == '\t')
			for (i = 0; i <= 2; i++)
				putc(' ', ofp);
		else if ((c == '.') || (c == '!') || (c == '?') || (c == '…'))
		{
			putc(c, ofp);
			c = getc(ifp);
			if (ferror(ifp)) return 1; // если произошла ошибка
			if (c == '.')
				while (((c == '.') || (c == '!') || (c == '?') || (c == '…')) && (c != EOF))
				{
					putc(c, ofp);
					c = getc(ifp);
					if (ferror(ifp))
					{
						fclose(ofp);
						return 1;
					} // если произошла ошибка
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
				} // если произошла ошибка
			}
			fseek(ifp, position, SEEK_SET);
		}
		else putc(c, ofp);
	}
	fclose(ofp); // закрытие файла для записи
	return 0;
}
