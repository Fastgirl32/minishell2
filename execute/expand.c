#include "../minishell.h"

/*
Helper function.
*/
t_status	expand_str_util(char *format, char *rstr, int i[3], t_vars *vars)
{
	char	*tmptmp;
	char	*var;

	i[0]++;
	while (format[i[0] + i[1]] && format[i[0] + i[1]] != ' ' && format[i[0] + i[1]] != '$')
		(i[1])++;
	if (i[1] == 0)
		return (1);
	tmptmp = ft_strndup(format + i[0], i[1]);
	if (!tmptmp)
		return (2);
	var = get_var(tmptmp, vars);
	free(tmptmp);
	if (var)
		rstr = ft_str_append(rstr, var);
	i[0] += (i[1] - 1);
	i[2] = 0;
	return (0);
}

/*
Expands a string once according to environment variables.
i[0] is the cursor in the format string.
i[i] is the cursor in the variable name in the format string.
i[2] is the cursor in the non-variable in the format string.
*/
char	*expand_str_call(char *format, t_vars *vars)
{
	char	*rstr;
	char	*tmp;
	int		i[3];

	rstr = ft_calloc(ft_strlen(format) + 1, 1);
	tmp = malloc(ft_strlen(format) + 1);
	ft_bzero(&i, sizeof(i));
	while (format[i[0]])
	{
		i[1] = 0;
		if (format[i[0]] != '$')
			tmp[(i[2])++] = format[i[0]];
		else
		{
			if (expand_str_util(format, rstr, i, vars) == 2)
				return (NULL);
		}
		i[0]++;
	}
	tmp[i[2]] = 0;
	rstr = ft_str_append(rstr, tmp);
	return (free(tmp), rstr);
}

/*
Recursively expands a string according to environment variables.
*/
char	*expand_str(char *format, t_vars *vars)
{
	char	*rstr;
	char	*tmp;

	rstr = expand_str_call(format, vars);
	while (ft_strchr(rstr, '$'))
	{
		tmp = rstr;
		rstr = expand_str_call(tmp, vars);
		free(tmp);
	}
	tmp = rstr;
	rstr = ft_strtrim(tmp, " ");
	free(tmp);
	return (rstr);
}
