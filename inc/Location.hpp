#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Webserv.hpp"

class Location
{
	private:
		STR					        _path;
		STR					        _root;
		bool				        _autoindex;
		STR					        _index;
		VECTOR<short>	            _methods;
		STR					        _return;
		STR					        _alias;
		VECTOR<STR>	                _cgi_path;
		VECTOR<STR>	                _cgi_ext;
		unsigned long				_client_max_body_size;

	public:
		MAP<STR, STR> _ext_path;

		Location();
		Location(const Location &other);
		Location &operator=(const Location &rhs);
		~Location();

		void setPath(STR param);
		void setRootLocation(STR param);
		void setMethods(std::vector<STR> methods);
		void setAutoindex(STR param);
		void setIndexLocation(STR param);
		void setReturn(STR param);
		void setAlias(STR param);
		void setCgiPath(std::vector<STR> path);
		void setCgiExtension(std::vector<STR> extension);
		void setMaxBodySize(STR param);
		void setMaxBodySize(unsigned long param);

		const STR &getPath() const;
		const STR &getRootLocation() const;
		const VECTOR<short> &getMethods() const;
		const bool &getAutoindex() const;
		const STR &getIndexLocation() const;
		const STR &getReturn() const;
		const STR &getAlias() const;
		const VECTOR<STR> &getCgiPath() const;
		const VECTOR<STR> &getCgiExtension() const;
		const MAP<STR, STR> &getExtensionPath() const;
		const unsigned long &getMaxBodySize() const;

		STR getPrintMethods() const;

};

#endif
