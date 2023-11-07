#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "Webserv.hpp"

class Location
{
	private:
		STR						_path;
		STR						_root;
		STR						_index;
		STR						_return;
		STR						_alias;
		VECTOR<short>			_methods;
		VECTOR<STR>				_cgi_path;
		VECTOR<STR>				_cgi_exten;
		bool					_autoindex;
		size_t					_client_max_body_size;

	public:
		Location();
		Location(const Location &copy);
		Location &operator=(const Location &copy);
		~Location();

		MAP<STR, STR>			_exten_path;

		void					setPath(STR param);
		void					setRootLocation(STR param);
		void					setAutoIndex(STR param);
		void					setIndexLocation(STR param);
		void					setReturn(STR param);
		void					setAlias(STR param);
		void					setMaxBodySize(STR param); //idk, do we need both of this functions? 
		void					setMaxBodySize(size_t param);
		void					setMethods(VECTOR<STR> methods);
		void					setCGIExtension(VECTOR<STR> extension);
		void					setCGIPath(VECTOR<STR> path);

		const STR				&getPath() const;
		const STR				&getRootLocation() const;
		const bool				&getAutoIndex() const;
		const STR				&getIndexLocation() const;
		const STR				&getReturn() const;
		const STR				&getAlias() const;
		const VECTOR<short>		&getMethods() const;
		const VECTOR<STR>		&getCGIPath() const;
		const VECTOR<STR>		&getCGIExtension() const;
		const MAP<STR, STR>		&getExtensionPath() const;
		const size_t			&getMaxBodySize() const;

		STR						methodsOutput() const; //delete after finishing
};

#endif
