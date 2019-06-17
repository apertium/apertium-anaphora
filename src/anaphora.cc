#include<cstdio>
#include<fstream>
#include<string>

int main()
{
	char input_char;

	input_char = fgetc(stdin);

	char input_stream[100] = "";
	char output_stream[100] = "";

	int flag_LU = 0;

	while(input_char!=EOF)
	{
		fprintf(stdout, "%c",input_char);

		if(input_char == '^')
			flag_LU = 1;

		if(flag_LU == 1) //Part of an LU
			strcat(input_stream, string(input_char))

		if(flag_LU == 0) //Not Part of an LU
			fprintf(stdout, "%c", input_char);

		if(input_char == '$')
		{
			flag_LU = 0;

			LU = parse(input_stream);

		}




		input_char = fgetc(stdin);
	}

	//fclose(fin);

	return 0;
}

