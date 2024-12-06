/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:55:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/12/06 13:59:56 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <iostream>


Client::Client() {
	m_authentified = false;
}

const int& Client::getFD() const { return m_fd; }

void Client::setFD(const int &fd) {	m_fd = fd; }

void Client::setIPadd(const std::string &ipadd) { m_ipAdd = ipadd; }

const bool& Client::getAuth() const { return m_authentified; }

void Client::setAuth(const bool &is)
{
	this->m_authentified = is;
}

Client::~Client()
{
	// close(m_fd);
	// std::cout << "client is dead" << std::endl;
};