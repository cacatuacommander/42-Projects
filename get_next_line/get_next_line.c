/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-ross <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 18:18:18 by ade-ross          #+#    #+#             */
/*   Updated: 2024/12/18 21:27:15 by ade-ross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*check_for_word(char *fin_buffer, int	*i)
{
	char	*buffer;

	*i = 0;
	if (fin_buffer == NULL)
		return (NULL);
	while (fin_buffer[*i] != '\0')
	{
		if (fin_buffer[*i] == '\n')
		{
			*i = *i + 1;
			buffer = ft_substr(fin_buffer, 0, (*i));
			return (buffer);
		}
		else
			*i = *i + 1;
	}
	return (NULL);
}

char	*new_read(char *fin_buffer, int *buffercheck, int fd)
{
	char	*buffer;
	char	*temp;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (buffer == NULL)
	{
		free(fin_buffer);
		return (NULL);
	}
	*buffercheck = read(fd, buffer, BUFFER_SIZE);
	if (*buffercheck == -1)
	{
		free(fin_buffer);
		free(buffer);
		return (NULL);
	}
	temp = fin_buffer;
	fin_buffer = ft_strjoin(fin_buffer, buffer);
	free(temp);
	free(buffer);
	return (fin_buffer);
}

char	*final_checks(char **fin_buffer)
{
	char			*buffer;

	if (ft_strlen(*fin_buffer) != 0)
	{
		buffer = ft_strdup(*fin_buffer);
		free(*fin_buffer);
		*fin_buffer = NULL;
		return (buffer);
	}
	else
	{
		free(fin_buffer);
		return (NULL);
	}
}

// questa funzione controlla sia il caso in cui il malloc di ritorno da 
// check_for_wordfallisca sia aggiorna il fin buffer se check_for_word 
//trova \n
char	*check_buf_or_upd_fin_buf(char **fin_buffer, int i, char *buffer)
{
	char	*temp;

	if (!buffer)
	{
		free(*fin_buffer);
		return (NULL);
	}
	temp = ft_substr(*fin_buffer, i, ft_strlen(*fin_buffer) - i);
	free(*fin_buffer);
	return (temp);
}

char	*get_next_line(int fd)
{
	static char	*fin_buffer ;
	char		*buffer;
	int			i;
	int			buffercheck;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = check_for_word(fin_buffer, &i);
	if (buffer == NULL && fin_buffer != NULL && (fin_buffer[i - 1] == '\n'))
		return (check_buf_or_upd_fin_buf(&fin_buffer, i, buffer));
	if (buffer != NULL && fin_buffer[i - 1] == '\n')
	{
		fin_buffer = check_buf_or_upd_fin_buf(&fin_buffer, i, (char *) 1);
		return (buffer);
	}
	fin_buffer = new_read(fin_buffer, &buffercheck, fd);
	if (!fin_buffer)
		return (NULL);
	else if (buffercheck > 0 && ft_strlen(fin_buffer) != 0)
		buffer = get_next_line(fd);
	else
		buffer = final_checks(&fin_buffer);
	return (buffer);
}
/*
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char *argv[])
{
	int fd = open("read_error.txt", O_RDONLY);
	int i = 0;

	char *s = "a";


	//read(fd, s, 4);
	while (s)
	{
		//s = try(fd);
		s = (get_next_line(fd));
		printf("'%s'", s);
		//s++;
		//if (s)
		//s[1] = 0;
		free(s);
	}

	return (0);
}*/
