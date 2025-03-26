/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raissacurty <raissacurty@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:44:52 by raissacurty       #+#    #+#             */
/*   Updated: 2024/12/14 23:45:12 by raissacurty      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Return the last node of the list
// lst    -> The beginning of the list
// return -> Success (last node)

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}
