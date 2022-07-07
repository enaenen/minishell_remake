#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>

typedef struct s_buffer
{
	char	*str;
	size_t	len;
	size_t	size;
}	t_buffer;

size_t	ft_strlen(const char *str);
size_t	ft_putstr_fd(const char *str, int fd);
char	**ft_split(char const *str, char c);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strntrim(char *str, char *set, int n);
char	*ft_strtrim(char const *str, char const *set);
char	*ft_substr(const char *str, size_t start, size_t len);
void	*ft_free(char *p);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s1);
char	*ft_strndup(char *s, int n);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
char	*ft_itoa(int n);
void	ft_putstr(char *str);
int		ft_atoi(const char *str);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strchr(const char *s, int c);
void	ft_free_split(char **arr);

/* BUFFER*/
t_buffer	*create_buf(void);
char	*add_char(t_buffer *buf, char c);
char	*add_str(t_buffer *buf, char *str);
char	*put_str(t_buffer *buf);
void	del_buf(t_buffer *buf);
#endif