/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:46:21 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/21 09:50:35 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

#define RPL_TOPIC "%h 332 %p %C :%t" // channel topic
#define RPL_NOTOPIC "%h 331 %p %C :No topic is set" // channel
#define RPL_NAMREPLY ":%p 353 %n%s %C :%l" //symbol channel list of clients
#define RPL_ENDOFNAMES ":%p 366 %n %C :End of /NAMES list." // nick channel
#endif