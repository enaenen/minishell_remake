#include "libft.h"

size_t	ft_putstr_fd(const char *str, int fd)
{
    return (write(fd, str, ft_strlen(str)));
}
