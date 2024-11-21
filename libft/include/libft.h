/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   libft.h											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2021/12/13 18:00:40 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/09/06 17:52:54 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>

# define MAL_ERR "your malloc failed! D:"

int			ft_isalpha(int c);
int			ft_isalnum(int c);
int			ft_isdigit(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
size_t		ft_strlen(char const *str);
void		*ft_memset(void *dest, int c, size_t	len);
void		*ft_memmove(void	*dst, const void	*src, size_t	len);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memcpy(void	*dest, const void	*src, size_t	n);
int			ft_memcmp(const void	*s1, const void	*s2, size_t	n);
void		ft_bzero(void	*s, size_t	n);
size_t		ft_strlcpy(char *dst, const char	*src, size_t	size);
size_t		ft_strlcat(char	*dst, const char	*src, size_t	dstsize);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_atoi(const char	*str);
char		*ft_strdup(const char	*s1);
void		*ft_calloc(size_t	count, size_t	size);
char		*ft_strnstr(const char	*haystack, const char
				*needle, size_t	len);
int			ft_strncmp(const char	*s1, const char	*s2, size_t	n);
char		*ft_substr(char const	*s, unsigned int start, size_t	len);
char		*ft_strjoin(char const	*s1, char const	*s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		str_reverse(char *str);
bool		ft_strrcmp(char *str1, char *str2, size_t n);
int			ft_printf(const char *str, ...);
char		*get_next_line(int fd);
void		print_2d_charray(char **map);
void		free_map(char **map);
int			ptrarr_len(void **arr);
void		error_exit(char *str, int err_nbr);
void		*safe_alloc(int size, int count);
void		free_array(void **array);
char		*protected_str_iple_join(char *s1, char *s2, char *s3);
int			ft_strcmp(const char *s1, const char *s2);
bool		str_is_fully_alnum(char *str);
void		str_switch(char **str1, char **str2);
bool		is_valid_nbr_str(char *str);
bool		ft_isdigit_str(char *s);
int			skip_space(char *str, int i);
int			skip_whitespace(char *str, int i);

typedef struct s_list
{
	struct s_list	*next;
	void			*content;
}				t_list;

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new_node);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new_node);
void		ft_lstdelone(t_list *lst, void (*del)(void*));
void		ft_lstclear(t_list **lst, void (*del)(void*));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *),
				void (*del)(void *));
#endif
