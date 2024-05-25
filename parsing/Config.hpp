/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otuyishi <otuyishi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 10:37:17 by otuyishi          #+#    #+#             */
/*   Updated: 2024/05/25 10:41:01 by otuyishi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <map>

class Config {
	private:
		std::string	_FileStream;
		std::string	_LineBuffer;
		std::string	_CurrToken;
		int	_PortN;
	public:
};

#endif
