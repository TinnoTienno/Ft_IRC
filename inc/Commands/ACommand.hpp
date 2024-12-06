/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:05:41 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/06 18:07:42 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include <vector>

class ACommand
{
	protected :
		std::vector <std::string>	m_args;
	public :
		ACommand(std::vector<std::string>);
		virtual void execute() = 0;
}	;