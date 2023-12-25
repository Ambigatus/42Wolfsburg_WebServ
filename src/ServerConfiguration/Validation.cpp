#include "../../include/ServerConfiguration.hpp"
#include "../../include/Utils.hpp"

/*The function checks if the token ends with a semicolon (';'). The purpose of this 
function is to ensure that tokens are correctly formatted and terminate with a semicolon. 
If the token does not meet this criteria, it's considered invalid, and an exception is 
thrown.*/
void	ServerConfiguration::validationToken(STR &param)
{
	size_t buffer = param.rfind(';');
	if (buffer != param.size() - 1)
		throw ErrorExeption("This token is invalid.");
	param.erase(buffer);
}

/* This function is responsible for checking the validity of a host address. 
This function takes a host address as input and attempts to convert it to an IPv4 address 
using the inet_pton function. If the conversion is successful, it returns true, 
indicating that the host address is valid. If the conversion fails, it returns false, 
indicating that the host address is not a valid IPv4 address.*/
bool	ServerConfiguration::checkHostValid(STR host) const
{
	struct sockaddr_in	sockaddr;
	return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

/*This function iterates through a map of error pages, where the keys are HTTP status 
codes and the values are paths to error pages. The function is used to ensure that 
error pages are correctly configured and that the associated files exist and are 
accessible.*/
bool	ServerConfiguration::checkErrPagesValid()
{
	MAP<short, STR>::const_iterator iter;
	for (iter = this->_error_pages.begin(); iter != this->_error_pages.end(); iter++)
	{
		if (iter->first < 100 || iter->first > 599)
			return false;
		if (ConfigurationFile::checkConfigFile(getRoot() + iter->second, 0) < 0 || ConfigurationFile::checkConfigFile(getRoot() + iter->second, 4) < 0)
			return false;
	}
	return true;
}

int	ServerConfiguration::checkLocationValid(Location &location) const
{
	if (location.getPath() == "/cgi-bin")
	{
		if (location.getCGIPath().empty() || location.getCGIExtension().empty() || location.getIndexLocation().empty())
            return 1;
        if (ConfigurationFile::checkConfigFile(location.getIndexLocation(), 4) < 0)
        {
            STR path = location.getRootLocation() + location.getPath() + "/" + location.getIndexLocation();
            if (ConfigurationFile::getTypePath(path) != 1)
            {
                STR root = getcwd(NULL, 0);
                location.setRootLocation(root);
                path = root + location.getPath() + "/" + location.getIndexLocation();
            }
            if (path.empty() || ConfigurationFile::getTypePath(path) != 1 || ConfigurationFile::checkConfigFile(path, 4) < 0)
                return 1;
        }
        if (location.getCGIPath().size() != location.getCGIExtension().size())
            return 1;
        VECTOR<STR>::const_iterator iter;
        for (iter = location.getCGIPath().begin(); iter != location.getCGIPath().end(); iter++)
        {
            if (ConfigurationFile::getTypePath(*iter) < 0)
                return 1;
        }
        VECTOR<STR>::const_iterator path_iter;
        for (iter = location.getCGIExtension().begin(); iter != location.getCGIExtension().end(); ++iter)
        {
            STR temp = *iter;
            if (temp != ".py" && temp != ".sh" && temp != "*.py" && temp != "*.sh")
                return 1;
            for (path_iter = location.getCGIPath().begin(); path_iter != location.getCGIPath().end(); ++path_iter)
            {
                STR path_temp = *path_iter;
                if (temp == ".py" || temp == "*.py")
                {
                    if (path_temp.find("python") != STR::npos)
                        location._exten_path.insert(std::make_pair(".py", path_temp));
                }
                else if (temp == ".sh" || temp == "*.sh")
                {
                    if (path_temp.find("bash") != STR::npos)
                        location._exten_path[".sh"] = path_temp;
                }
            }
        }
        if (location.getCGIPath().size() != location.getExtensionPath().size())
            return 1;
	}
    else
    {
        if (location.getPath()[0] != '/')
            return 2;
        if (location.getRootLocation().empty())
            location.setRootLocation(this->_root);
        if (ConfigurationFile::checkIsFileReadable(location.getRootLocation() + location.getPath() + "/", location.getIndexLocation()))
            return 5;
        if (!location.getReturn().empty())
        {
            if (ConfigurationFile::checkIsFileReadable(location.getRootLocation(), location.getReturn()))
                return 3;
        }
        if (!location.getAlias().empty())
        {
            if (ConfigurationFile::checkIsFileReadable(location.getRootLocation(), location.getAlias()))
                return 4;
        }
    }
    return 0;
}

bool    ServerConfiguration::checkLocationDup(void)
{
    if (this->_locations.size() < 2)
        return (false);
    std::vector<Location>::const_iterator it1;
    std::vector<Location>::const_iterator it2;
    it1 = this->_locations.begin();
    while (it1 != this->_locations.end() - 1)
    {
        it2 = it1 + 1;
        while (it2 != this->_locations.end())
        {
            if (it1->getPath() == it2->getPath())
                return(true);
            it2++;
        }
        it1++;
    }
    return (false);
}
