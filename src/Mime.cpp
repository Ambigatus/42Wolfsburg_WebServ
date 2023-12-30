/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:44:16 by hboichuk          #+#    #+#             */
/*   Updated: 2023/12/08 17:40:50 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../include/Mime.hpp"

Mime::Mime()
{
	_types[".html"] = "text/html";
	_types[".htm"] = "text/html";
	_types[".css"] = "text/css";
	_types[".ico"] = "image/x-icon";
	_types[".avi"] = "video/x-msvideo";
	_types[".bmp"] = "image/bmp";
	_types[".doc"] = "application/msword";
	_types[".gif"] = "image/gif";
	_types[".gz"] = "application/x-gzip";
	_types[".ico"] = "image/x-icon";
	_types[".jpg"] = "image/jpeg";
	_types[".jpeg"] = "image/jpeg";
	_types[".png"] = "image/png";
	_types[".txt"] = "text/plain";
	_types[".mp3"] = "audio/mp3";
	_types[".pdf"] = "application/pdf";
	_types["default"] = "text/html"; // Default MIME type
}

std::string	Mime::getFileType(std::string extension)
{
	if (_types.count(extension))
		return (_types[extension]);
	return (_types["default"]);
}
