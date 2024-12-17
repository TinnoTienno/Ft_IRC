/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:06:42 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/17 16:33:42 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"
#include "utils.hpp"

int serverExceptions::getErrorCode() const { return m_errorCode; }

serverExceptions::serverExceptions(const int errorCode) : m_errorCode(errorCode) { }