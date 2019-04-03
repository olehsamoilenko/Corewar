
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
	// asm_parsing->asm_code = ft_strjoin(".name ", str);
	// asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
	ft_putstr_fd(".name \"", asm_parsing->fd_dis);
	ft_putstr_fd(str, asm_parsing->fd_dis);
	ft_putendl_fd("\"", asm_parsing->fd_dis);
	
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
	// asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, ".comment ");
	// asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, str);
	// asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
	// ft_putstr_fd("comment. %")
}

int		parse_bytecode(t_asm *asm_parsing, unsigned char *all_code, int size)
{
	int res;
	int shift;
	int i;

	i = asm_parsing->position + size;
	i--;
	shift = 0;
	res = 0;
	while (size != 0)
	{
		res = res | (all_code[i] << shift);
		shift += 8;
		i--;
		asm_parsing->position++;
		size--;
	}
	return (res);
}

int		size_arg(int argument, int instruction)
{
	if (argument == REG_CODE)
		return (1);
	else if (argument == DIR_CODE)
	{
		if (g_op_tab[instruction].label_size == 1)
			return (2);
		else
			return (4);
	}
	else if (argument == IND_CODE)
		return (2);
	return (-1);
}

void	get_args_codage(t_asm *asm_parsing, unsigned char *all_code,  int instruction, unsigned char codage)
{
	int args[3];
	int i;
	int arg_size;
	int arg_content;
	char *tmp;
	char *buf;

	args[0] = (codage >> 6) & 3;
	args[1] = (codage >> 4) & 3;
	args[2] = (codage >> 2) & 3;
	i = 0;
	tmp = asm_parsing->asm_code;
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, " ");
	ft_strdel(&tmp);
	while (i < 3)
	{
		arg_size = size_arg(args[i], instruction);
		if (arg_size == -1)
			break ;
		arg_content = parse_bytecode(asm_parsing, all_code, arg_size);
		if (args[i] == REG_CODE)
		{
			buf = asm_parsing->asm_code;
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "r");
			ft_strdel(&buf);
			tmp = ft_itoa(arg_content);
			buf = asm_parsing->asm_code;
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, tmp);
			ft_strdel(&buf);
			if (i + 1 != g_op_tab[instruction - 1].count_args)
			{
				buf = asm_parsing->asm_code;
				asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, ", ");
				ft_strdel(&buf);
			}
			else
			{
				buf = asm_parsing->asm_code;
				asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
				ft_strdel(&buf);
			}
		}
		else if (args[i] == DIR_CODE)
		{
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "%");
			if (g_op_tab[instruction - 1].label_size == 1)
				tmp = ft_itoa((short)arg_content);
			else
				tmp = ft_itoa(arg_content);
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, tmp);
			if (i + 1 != g_op_tab[instruction - 1].count_args)
			{
				asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, ", ");
			}
			else
			{
				asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
			}
		}
		else if (args[i] == IND_CODE)
		{
			tmp = ft_itoa((short)arg_content);
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, tmp);
			if (i + 1 != g_op_tab[instruction - 1].count_args)
			{
				asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, ", ");
			}
			else
			{
				asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
			}
		}
		ft_strdel(&tmp);
		if (arg_size == -1)
			break;
		i++;
	}
}

void	get_args(t_asm *asm_parsing, unsigned char *all_code,  int instruction, unsigned char codage)
{
	int arg;
	int arg_size;
	int arg_content;
	char *tmp;

	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, " ");
	if (g_op_tab[instruction - 1].label_size == 0)
		arg_size = 4;
	else
		arg_size = 2;
	arg_content = parse_bytecode(asm_parsing, all_code, arg_size);
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "%");
	if (g_op_tab[instruction - 1].label_size == 0)
		tmp = ft_itoa(arg_content);
	else
		tmp = ft_itoa((short)arg_content);
	
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, tmp);
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
}

void	read_code(t_asm *asm_parsing, int size_code)
{
	unsigned char *all_code;
	int instruction;
	int ret;
	unsigned char codage;

	all_code = (unsigned char *)malloc(sizeof(char) * size_code);
	ret = read(asm_parsing->fd, all_code, size_code);
	while (asm_parsing->position != size_code)
	{
		instruction = parse_bytecode(asm_parsing, all_code, 1);
		if (instruction > 16)
			error_dis("Unknown instruction");
		if (g_op_tab[instruction - 1].codage_octal == 1)
		{
			codage = parse_bytecode(asm_parsing, all_code, 1);
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code,
								g_op_tab[instruction - 1].name);
			get_args_codage(asm_parsing, all_code, instruction, codage);
		}
		else
		{
			asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code,
								g_op_tab[instruction - 1].name);
			get_args(asm_parsing, all_code, instruction, codage);
		}
	}
}

void	asm_to_file(t_asm *asm_parsing, const char *filename)
{
	int fd;
	char *file_to_write;
	char *temp;

	filename = ft_strsub(filename, 0, ft_strlen(filename) - 4);
	file_to_write = ft_strjoin(filename, ".s");
	fd = open(file_to_write, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(fd, asm_parsing->asm_code, ft_strlen(asm_parsing->asm_code));
}

void	disassembler(const char *filename)
{
	int fd;
	t_asm	*asm_parsing;
	int ret;
	int size_code;

	char *file_to_write;
	

	if ((fd = open(filename, O_RDONLY)) == -1)
		ft_arg_error("Can't open this file");
	asm_parsing = init_asm(fd, filename);

	filename = ft_strsub(filename, 0, ft_strlen(filename) - 4);
	file_to_write = ft_strjoin(filename, ".s");
	asm_parsing->fd_dis = open(file_to_write, O_CREAT | O_WRONLY | O_TRUNC, 0644);

	ret = help_dis(asm_parsing, 4);
	if (ret != COREWAR_EXEC_MAGIC)
		error_dis("MAGIC is wrong");
	read_name(asm_parsing);
	ret = help_dis(asm_parsing, 4);
	if (ret != 0)
		error_dis("NULL is wrong");
	size_code = help_dis(asm_parsing, 4);
	read_comment(asm_parsing);
	ret = help_dis(asm_parsing, 4);
	if (ret != 0)
		error_dis("NULL is wrong");
	asm_parsing->asm_code = ft_strjoin(asm_parsing->asm_code, "\n");
	read_code(asm_parsing, size_code);

	// asm_to_file(asm_parsing, filename);
}
