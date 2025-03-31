/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:45:00 by rcurty-g          #+#    #+#             */
/*   Updated: 2025/03/31 11:45:01 by rcurty-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *str, size_t n)
{
	size_t	i;
	size_t	len;
	char	*str2;

	len = 0;
	if (!str)
		return (NULL);
	while (len < n && str[len])
		len ++;
	str2 = ft_calloc(len + 1, sizeof(char));
	if (!str2)
		return (NULL);
	i = 0;
	while (str[i] && i < len)
	{
		str2[i] = str[i];
		i++;
	}
	return (str2);
}
