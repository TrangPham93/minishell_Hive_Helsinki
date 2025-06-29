/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 15:34:40 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 13:03:53 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	is_pure_variable(const char *str)
{
	if (!str || *str != '$')
		return (TRUE);
	str++;
	if (!ft_isalpha(*str) && *str != '_')
		return (TRUE);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (TRUE);
		str++;
	}
	return (FALSE);
}

static int	remove_token_empty(t_token **token_list,
	t_token **temp, t_token **prev)
{
	t_token	*to_free;

	if ((ft_strcmp((*temp)->value, "") == 0)
		&& !is_fully_quoted((*temp)->ori_value)
		&& is_pure_variable((*temp)->ori_value) && (*temp)->next
		&& (!*prev || is_redirection(*prev) == FALSE))
	{
		to_free = *temp;
		*temp = (*temp)->next;
		free_string(to_free->value);
		free_string(to_free->ori_value);
		free(to_free);
		if (*prev)
			(*prev)->next = *temp;
		else
			*token_list = *temp;
		return (FALSE);
	}
	return (TRUE);
}

void	skip_middle_empty_vars(t_token **token_list)
{
	t_token	*temp;
	t_token	*prev;

	temp = *token_list;
	prev = NULL;
	while (temp)
	{
		if (remove_token_empty(token_list, &temp, &prev) == TRUE)
		{
			if (prev == NULL)
				prev = temp;
			else
				prev = prev->next;
			temp = temp->next;
		}
	}
}

int	skip_expanded_empty_var(t_token **token_list)
{
	if (!ft_strchr((*token_list)->ori_value, '"')
		&& !ft_strchr((*token_list)->ori_value, '\''))
	{
		skip_first_empty_vars(token_list);
		skip_middle_empty_vars(token_list);
	}
	return (TRUE);
}

void	skip_first_empty_vars(t_token **token_list)

{
	t_token	*to_free;

	while (*token_list && ft_strcmp((*token_list)->value, "") == 0)
	{
		to_free = *token_list;
		(*token_list) = (*token_list)->next;
		free_string(to_free->value);
		free_string(to_free->ori_value);
		free(to_free);
	}
}
