/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:21:44 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 10:10:21 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>

/* GNL ASSETS */
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

# define SUCCESS 1
# define END_OF_FILE 0
# define ERROR -1

typedef struct s_list
{
	char			*data;
	struct s_list	*next;
}					t_list;

typedef struct s_buffer
{
	char	*str;
	size_t	len;
	size_t	size;
}	t_buffer;

size_t		ft_strlen(const char *str);
size_t		ft_putstr_fd(const char *str, int fd);
char		**ft_split(char const *str, char c);
void		*ft_memset(void *b, int c, size_t len);
void		*ft_memcpy(void *dst, const void *src, size_t n);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strntrim(char *str, char *set, int n);
char		*ft_strtrim(char const *str, char const *set);
char		*ft_substr(const char *str, size_t start, size_t len);
void		*ft_free(char *p);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strdup(const char *s1);
char		*ft_strndup(char *s, int n);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
char		*ft_itoa(int n);
void		ft_putstr(char *str);
int			ft_atoi(const char *str);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_strchr(const char *s, int c);
void		ft_free_split(char **arr);

/* LIST & LIST UTILS */
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
t_list		*ft_lstnew(void *data);
void		ft_lstclear(t_list **lst, void (*del)(void *));
int			ft_lstsize(t_list *lst);

/* STRING BUFFER */
t_buffer	*create_buf(void);
char		*add_char(t_buffer *buf, char c);
char		*add_str(t_buffer *buf, char *str);
char		*put_str(t_buffer *buf);
void		del_buf(t_buffer *buf);

/* GET_NEXT_LINE */
char		*get_next_line(int fd);
#endif
