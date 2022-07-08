#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char			**environ;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_buffer
{
	char	*str;
	size_t	len;
	size_t	size;
}	t_buffer;

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t index;

	index = -1;
	if (n && dst != src)
	{
		while (++index < n)
			((unsigned char *)dst)[index] = ((unsigned char *)src)[index];
	}
	return (dst);
}

char	*ft_substr(const char *str, size_t start, size_t len)
{
	size_t	i;
	char	*ret;

	ret = malloc(len + 1 * sizeof(char));
	i = 0;
	while (i < len)
	{
		ret[i] = str[i + start];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

t_buffer	*create_buf(void)
{
	t_buffer	*buf;
	char		*str;

	buf = malloc(sizeof(t_buffer));
	str = malloc(sizeof(char) * 1024);
	if (buf == NULL || str == NULL)
		return (NULL);
	buf->str = str;
	buf->size = 1;
	buf->len = 0;
	return (buf);
}

char	*add_char(t_buffer *buf, char c)
{
	char	*tmp;

	if (buf->len + 1 >= buf->size)
	{
		tmp = malloc(sizeof(char) * buf->size * 2);
		if (tmp == NULL)
			return (NULL);
		ft_memcpy(tmp, buf->str, buf->len);
		free(buf->str);
		buf->str = tmp;
		buf->size *= 2;
	}
	buf->str[buf->len] = c;
	buf->len++;
	return (buf->str);
}

char	*add_str(t_buffer *buf, char *str)
{
	const size_t	len = ft_strlen(str);
	char			*tmp;

	if (buf->len + len >= buf->size)
	{
		tmp = malloc(sizeof(char) * (buf->len + len + 1));
		if (tmp == NULL)
			return (NULL);
		ft_memcpy(tmp, buf->str, buf->len);
		free(buf->str);
		buf->str = tmp;
		buf->size = buf->len + len + 1;
	}
	ft_memcpy(&buf->str[buf->len], str, len);
	buf->len += len;
	return (buf->str);
}

char	*put_str(t_buffer *buf)
{
	char	*tmp;

	tmp = ft_substr(buf->str, 0, buf->len);
	if (tmp == NULL)
		return (NULL);
	buf->len = 0;
	return (tmp);
}

void	del_buf(t_buffer *buf)
{
	free(buf->str);
	free(buf);
}

static void	get_env_convert(t_env **env_list, char **env)
{
	t_env		*tmp;
	t_buffer	*buf;
	int			i;

	buf = create_buf();
	tmp = *env_list;
	i = 0;
	while (tmp)
	{
		if (tmp->value)
		{
			add_str(buf, tmp->key);
			add_char(buf, '=');
			add_str(buf, tmp->value);
			env[i++] = put_str(buf);
		}
		tmp = tmp->next;
	}
	del_buf(buf);
}

char **get_env_list(t_env **env_list)
{
	t_env	*tmp;
	char	**env;
	int		i;

	i = 0;
	tmp = *env_list;
	while (tmp)
	{
		if (tmp->value)
			i++;
		tmp = tmp->next;
	}
	env = malloc(sizeof(char *) * (i + 1));
	if (env == NULL)
		exit(1);
	get_env_convert(env_list, env);
	return (env);
}

void	print_strs(char **strs)
{
	while (*strs)
	{
		printf("%s\n", *strs++);
	}
}
int main(void)
{
    char **envp;
    char **tmp = environ;
    // envp = get_env_list(tmp);
    print_strs(environ);
}