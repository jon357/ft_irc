/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheca <jcheca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:17:59 by jcheca            #+#    #+#             */
/*   Updated: 2024/05/16 18:17:21 by jcheca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::vector<std::string> split(const std::string &input, const std::string &delimiters)
{
	std::vector<std::string> result;
	size_t startPos = 0;
	size_t foundPos = input.find_first_of(delimiters, startPos);

	while (foundPos != std::string::npos)
	{
		if (foundPos != startPos)
		{
			result.push_back(input.substr(startPos, foundPos - startPos));
		}
		startPos = foundPos + 1;
		foundPos = input.find_first_of(delimiters, startPos);
	}

	if (startPos < input.length())
	{
		result.push_back(input.substr(startPos));
	}

	return result;
}
