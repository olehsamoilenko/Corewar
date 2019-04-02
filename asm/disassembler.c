
#include "asm.h"

void		error_dis(char *message)
{
	ft_putendl_fd(message, 2);
	exit(1);
}

unsigned int	from_bytes_to_int(unsigned char *buf, int size)
{
	int res;
	int shift;
	int i;

	i = size;
	i--;
	shift = 0;
	res = 0;
	while (size != 0)
	{
		res = res | (buf[i] << shift);
		i--;
		shift += 8;
		size--;
	}
	printf("res = %x\n", res);
	return (res);
}

int help_dis(t_asm *asm_parsing,int size)
{
	unsigned char *buf;
	int  size_ret;
	int res;

	buf = (unsigned char *)malloc(sizeof(unsigned char) * size);
	size_ret = read(asm_parsing->fd, buf, size);
	if (size_ret == 0)
		error_dis("Error with reading from file");
	res = from_bytes_to_int(buf, size);
	// printf("%2x\n", buf[1]);
	return (res);
}

void	read_name(t_asm *asm_parsing)
{
	char *str;
	int size;

	str = (char *)malloc(sizeof(char ) * 128);
	size = read(asm_parsing->fd, str, 128);
	if (size == 0)
		error_dis("Error with reading from file");
	// printf("name = %s\n", str);
	asm_parsing->asm_code = ft_strjoin(".name ", str);
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
}

void	read_comment(t_asm *asm_parsing)
{
	char *str;
	int size;

	str = (char *)malloc(sizeof(char ) * 2048);
	size = read(asm_parsing->fd, str, 2048);
	if (size == 0)
		error_dis("Error with reading from file");
	// printf("name = %s\n", str);
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, ".comment ");
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, str);
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
}

int		parse_bytecode(t_asm *asm_parsing, unsigned char *all_code, int size)
{
	int res;
	int shift;

	shift = 0;
	res = 0;
	printf("check %x\n", all_code[asm_parsing->position]);
	while (size != 0)
	{
		res = res | (all_code[asm_parsing->position] << shift);
		shift += 8;
		asm_parsing->position++;
		size--;
	}
	printf("res = %x\n", res);
	return (res);
}

int		size_arg(int argument, int instruction)
{
	if (argument == 1)
		return (1);
	else if (argument == 2)
	{
		if (g_op_tab[instruction].label_size == 1)
			return (2);
		else
			return (4);
	}
	else if (argument == 3)
		return (4);
	return (-1);
}

void	get_args(t_asm *asm_parsing, unsigned char *all_code,  int instruction, unsigned char codage)
{
	int args[3];
	int i;
	int arg_size;
	int arg_content;

	args[0] = (codage >> 6) & 3;
	args[1] = (codage >> 4) & 3;
	args[2] = (codage >> 2) & 3;
	i = 0;
	printf("=====================================\n");
	while (i < 3)
	{
		arg_size = size_arg(args[i], instruction);
		printf("arg_size = %d\n", arg_size);
		arg_content = parse_bytecode(asm_parsing, all_code, arg_size);
		printf("arg_content = %x\n", arg_content);
		if (args[i] == T_REG)
		{
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, " ");
			
		}
		if (arg_size == -1)
			break;
		i++;
	}
	// printf("first = %d\n", first);
	// printf("second = %d\n", second);
	// printf("third = %d\n", third);


}

void	read_code(t_asm *asm_parsing, int size_code)
{
	unsigned char *all_code;
	int instruction;
	int ret;
	unsigned char codage;

	all_code = (unsigned char *)malloc(sizeof(char) * size_code);
	ret = read(asm_parsing->fd, all_code, size_code);
	printf("+++++++++++++++++++++++++++++++++++\n");
	// while (asm_parsing->position < size_code)
	// {
		instruction = parse_bytecode(asm_parsing, all_code, 1);
		if (instruction > 16)
			error_dis("Unknown instruction");
		printf("ret = %d\n", instruction);
		if (g_op_tab[instruction - 1].codage_octal == 1)
		{
			codage = parse_bytecode(asm_parsing, all_code, 1);
			printf("codage = %x\n", codage);
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code,
								g_op_tab[instruction - 1].name);
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
			get_args(asm_parsing, all_code, instruction, codage);

		}

	// }
}

void	disassembler(const char *filename)
{
	int fd;
	t_asm	*asm_parsing;
	int ret;
	int size_code;

	if ((fd = open(filename, O_RDONLY)) == -1)
		ft_arg_error("Can't open this file");
	asm_parsing = init_asm(fd, filename);
	ret = help_dis(asm_parsing, 4);
	if (ret != COREWAR_EXEC_MAGIC)
		error_dis("MAGIC is wrong");
	read_name(asm_parsing);
	ret = help_dis(asm_parsing, 4);
	if (ret != 0)
		error_dis("NULL is wrong");
	size_code = help_dis(asm_parsing, 4);
	// printf("ret == %d\n", ret);
	read_comment(asm_parsing);
	ret = help_dis(asm_parsing, 4);
	if (ret != 0)
		error_dis("NULL is wrong");
	// printf("GOOOD\n");
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
	read_code(asm_parsing, size_code);
	printf("asm_code = \n%s\n", asm_parsing->asm_code);

	// print_asm_structure(asm_parsing);
}
