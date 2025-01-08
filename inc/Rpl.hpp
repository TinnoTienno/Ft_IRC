/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:46:21 by eschussl          #+#    #+#             */
/*   Updated: 2025/01/08 18:58:41 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

#define RPL_TOPIC "%h 332 %p %C :%T" // channel topic
#define RPL_NOTOPIC ":%h 331 %p %C :No topic is set" // channel
#define RPL_NAMREPLY ":%p 353 %n%s %C :%l" //symbol channel list of clients
#define RPL_ENDOFNAMES ":%p 366 %n %C :End of /NAMES list." // nick channel
#endif