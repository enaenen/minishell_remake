#include "minishell.h"

char	*expand_in_quot_uitls(t_proc *proc, char *data, char **new)
{
	int	i;

	i = -1;
	while (data[++i])
	{
		if (data[i] == '$')
		{
			data = expand_env_var(proc, data, i , new);
			if (!data)
				return(ft_free(*new));
			i = -1;
		}
	}
	return (data);
}


char	*ms_strtrim(char *data, int start, int end)
{
	int		i;
	int		j;
	char	*ret;

	ret = (char *)malloc(end * sizeof(char));
	if (!ret)
		return (NULL);
	i = -1;
	j = 0;
	while (data[++i] && i < end)
	{
		if (i != start)
			ret[j++] = data[i];
	}
	ret[j] = '\0';
	return (ret);
}

// data를 받아 data +1 부터 $ 토큰을 찾음
int	find_env_var_token(char *data, int start, int end)
{
	int	find;

	find = start + 1;
	while (data[find] && find < end && data[find] != '$')
		find++;
	if (find == end)
		return (FALSE);
	return (TRUE);
}


char	*parse_pre_env_var(char *data, int start, char *new_data)
{
	char	*org_data;
	char	*tmp;

	org_data = new_data;
	tmp = ft_strndup(data, start);
	if (!tmp)
		return (ft_free(org_data));
	new_data = ft_strjoin(new_data, tmp);
	ft_free(tmp);
	ft_free(org_data);
	return (new_data);

}


int		find_valid_env_var_point(char *data)
{
	int	index;
	if (data[0] == '?')
		return (1);
	index = 0;
	while (data[index] && (ft_isalnum(data[index]) || data[index] == '_'))
		index++;
	return (index);
}


char	*expand_env_var(t_proc *proc, char *data, int start, char **new_data)
{
	char	*tmp;
	char	*get_env;

	*new_data = parse_pre_env_var(data, start, *new_data);
	if (!(*new_data))
		return (NULL);
	data = &data[start + 1];
	//$ 이후에 num, alnum or _ 일때
	get_env = ft_strndup(data, find_valid_env_var_point(data));
	data = &data[find_valid_env_var_point(data)];
	if (!get_env)
		return (ft_free(*new_data));
	tmp = get_env;
	if (get_env[0] == '?')
		get_env = ft_itoa(g_status);
	else
		get_env = read_key(proc->env_list, get_env);
	ft_free(tmp);
	tmp = *new_data;
	*new_data = ft_strjoin(*new_data, get_env);
	ft_free(tmp);
	ft_free(get_env);
	if (!(*new_data))
		return (NULL);
	return (data);
}

char	*expand_int_quot_utils(t_proc *proc, char *data, char **new_data)
{
	int i;
	i = -1;
	while (data[++i])
	{
		if (data[i]== '$')
		{
			data = expand_env_var(proc, data, i ,new_data);
			if (!data)
				return (ft_free(*new_data));
			i = -1;
		}
	}
	return (data);
}

char	*expand_in_quot_utils(t_proc *proc, char *data, char **new_data)
{
	int i;

	i = -1;
	while (data[++i])
	{
		if (data[i] == '$')
		{
			data = expand_env_var(proc, data, i, new_data);
			if (!data)
				return (ft_free(*new_data));
			i = -1;
		}
	}
	return (data);
}

char	*expand_in_quot_env_var(t_proc *proc, char *data, int start, int end)
{
	char	*new_data;
	char	*tmp;
	char	*tmp2;

	new_data = ft_strndup(data, start);
	if (!new_data)
		return (NULL);
	data = &data[start + 1];
	data = ft_strndup(data, end - start - 1);
	if (!data)
		return (ft_free(new_data));
	tmp2 = data;
	data = expand_in_quot_utils(proc, data, &new_data);
	if (!data)
		return (ft_free(tmp2));
	tmp = new_data;
	new_data = ft_strjoin(new_data, data);
	ft_free(tmp2);
	ft_free(tmp);
	return (new_data);
}
/*QUOT 처리*/
//data 는 quot 뒤엣부분
char	*del_big_quot(t_proc *proc, char *data, int start, char **new_data)
{
	int		end;
	char	*org_data;
	char	*tmp;

	org_data = *new_data;
	tmp = NULL;
	end = find_valid_quot_point(data, start);
	if (find_env_var_token(data, start, end) == TRUE)
		tmp = expand_in_quot_env_var(proc, data, start, end);
	else
		tmp = ms_strtrim(data, start, end);
	if (!tmp)
		return (ft_free(org_data));
	*new_data = ft_strjoin(*new_data, tmp);
	ft_free(org_data);
	ft_free(tmp);
	if (!(*new_data))
		return (NULL);
	data = &data[end + 1];
	return (data);
}

char	*del_small_quot_token(char *data, int start, char **new_data)
{
	int		end;
	char	*tmp;
	char	*org_data;

	org_data = *new_data;
	end = find_valid_quot_point(data, start);
	tmp = ms_strtrim(data, start, end);
	if (!tmp)
		return (NULL);
	*new_data = ft_strjoin(*new_data, tmp);
	ft_free(org_data);
	ft_free(tmp);
	if (!(*new_data))
		return (NULL);
	data = &data[end + 1];
	return (data);
}

