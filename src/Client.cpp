/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/05 14:46:48 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <iostream>


Client::Client() { }

const int& Client::getFD() const { return m_fd; }

void Client::setFD(const int &fd) {	m_fd = fd; }

void Client::setIPadd(const std::string &ipadd) { m_ipAdd = ipadd; }

Client::~Client()
{
	// close(m_fd);
	std::cout << "client is dead" << std::endl;
};