
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboichuk <hboichuk@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:44:13 by hboichuk          #+#    #+#             */
/*   Updated: 2023/11/15 18:23:37 by hboichuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
#define	MIME_HPP

#include "Webserv.hpp"

/*MIME stands for Multipurpose Internet Mail Extensions. 
It's a standard way of identifying files on the Internet according to their nature and format.
In the context of web servers, the MIME type is a label used to identify a type of data. 
It's a part of the HTTP header sent by a server to a client indicating the media type of the resource being sent.

Content-Type HTTP Header:

The Content-Type HTTP header is used to indicate the media type (MIME type) of the resource.
For example, when a web server sends an image, the Content-Type might be set to image/jpeg or 
image/png to tell the browser how to interpret and display the data.

Class Purpose:

The Mime class appears to provide a mechanism for looking up the MIME type based on the file extension.
When the server needs to send a file to a client, it often needs to include the correct Content-Type in
 the HTTP header.
This class likely maintains a mapping between file extensions (like ".html" or ".jpg") and their corresponding
 MIME types ("text/html" or "image/jpeg").
The getMimeType function is likely used to retrieve the MIME type for a given file extension.*/

class	Mime
{
	private:
	/*stores associations between file extensions and their corresponding MIME types*/
		std::map<std::string, std::string>	_types;
		
	public:
		Mime();
		std::string	getFileType(std::string extension);
	
};
