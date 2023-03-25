/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avillard <avillard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:40:52 by avillard          #+#    #+#             */
/*   Updated: 2023/02/22 16:40:52 by avillard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	*stash = NULL;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
	{
		free_stash(stash);
		stash = NULL;
		return (NULL);
	}
	line = NULL;
	read_and_stash(fd, &stash);
	if (stash == NULL)
		return (NULL);
	extract_line(stash, &line);
	clean_stash(&stash);
	if (line[0] == '\0')
	{
		free_stash(stash);
		stash = NULL;
		free(line);
		return (NULL);
	}
	return (line);
}

void	read_and_stash(int fd, t_list **stash)
{
	char	*buf;
	int		readed;

	readed = 1;
	while (!found_newline(*stash) && readed != 0)
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (buf == NULL)
			return ;
		readed = (int)read(fd, buf, BUFFER_SIZE);
		if ((*stash == NULL && readed == 0) || readed == -1)
		{
			free(buf);
			return ;
		}
		buf[readed] = '\0';
		add_to_stash(stash, buf, readed);
		free(buf);
	}
}

void	add_to_stash(t_list **stash, char *buf, int readed)
{
	int		i[0];
	t_list	*last;
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	new_node->next = NULL;
	new_node->content = malloc(sizeof(char) * (readed + 1));
	if (new_node->content == NULL)
		return ;
	i[0] = 0;
	while (buf[i[0]] && i[0] < readed)
	{
		new_node->content[i[0]] = buf[i[0]];
		i[0]++;
	}
	new_node->content[i[0]] = '\0';
	if (*stash == NULL)
	{
		*stash = new_node;
		return ;
	}
	last = ft_lst_get_last(*stash);
	last->next = new_node;
}

void	extract_line(t_list *stash, char **line)
{
	int	i[0];
	int	j;

	if (stash == NULL)
		return ;
	generate_line(line, stash);
	if (*line == NULL)
		return ;
	j = 0;
	while (stash)
	{
		i[0] = 0;
		while (stash->content[i[0]])
		{
			if (stash->content[i[0]] == '\n')
			{
				(*line)[j++] = stash->content[i[0]];
				break ;
			}
			(*line)[j++] = stash->content[i[0]++];
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

void	clean_stash(t_list **stash)
{
	t_list	*last;
	t_list	*clean_node;
	int		i[2];
	int		j;

	clean_node = malloc(sizeof(t_list));
	if (stash == NULL || clean_node == NULL)
		return ;
	clean_node->next = NULL;
	last = ft_lst_get_last(*stash);
	i[0] = 0;
	while (last->content[i[0]] && last->content[i[0]] != '\n')
		i[0]++;
	if (last->content && last->content[i[0]] == '\n')
		i[0]++;
	i[1] = (ft_strlen(last->content) - i[0]) + 1;
	clean_node->content = malloc(sizeof(char) * i[1]);
	if (clean_node->content == NULL)
		return ;
	j = 0;
	while (last->content[i[0]])
		clean_node->content[j++] = last->content[i[0]++];
	clean_node->content[j] = '\0';
	free_stash(*stash);
	*stash = clean_node;
}
