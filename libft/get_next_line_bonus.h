/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstarek <lstarek@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 12:42:41 by lstarek           #+#    #+#             */
/*   Updated: 2025/11/20 12:59:03 by lstarek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# ifndef BUFFER_SIZE 
#  define BUFFER_SIZE 10
# endif
# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif
# ifndef SEP
#  define SEP '\n'
# endif

char	*get_next_line(int fd);
void	clear(char *stash);
char	*ft_strdup(char *src);
char	*join_and_free(char *s1, char *s2);
size_t	safe_strlen(char const *str);
void	*ft_calloc(size_t nmemb, size_t size);

#endif
