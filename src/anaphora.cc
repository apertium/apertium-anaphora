#include<cstdio>
#include<fstream>

int main()
{
	char input_char;

	input_char = fgetc(stdin);

	while(input_char!=EOF)
	{
		printf("%c",input_char);

		input_char = fgetc(stdin);
	}

	//fclose(fin);

	return 0;
}

