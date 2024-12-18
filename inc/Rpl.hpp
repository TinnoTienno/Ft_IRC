/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:46:21 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/18 18:10:59 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

#define RPL_TOPIC "%h 332 %p %C :%t" // channel topic
#define RPL_NOTOPIC "%h 331 %p %C :No topic is set" // channel
#define RPL_NAMREPLY "%h 353 %p %s %C:%l" //symbol channel list of clients
#endif