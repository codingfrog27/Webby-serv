void	Request::checkLocations(std::string _filePath)
{
	location	reqRules;
	location	nestRules;
	std::cout << "current req _filePath == (b4 loc-check) " << _filePath << std::endl \
	<< "FD == " << _clientFD << std::endl;
	std::vector<location> &locVec = this->_config->_locations;
	for (size_t matchCount = 1; matchCount != 0;)
	{
		matchCount = checkLocationMatch(locVec, reqRules);
		if (matchCount && !reqRules._nestedLocations.empty())
			matchCount = checkLocationMatch(reqRules._nestedLocations, nestRules);
		if (matchCount)
			setLocRules(nestRules, reqRules);
		if (!nestRules._nestedLocations.empty())
			locVec = nestRules._nestedLocations;
	}
	

	while nested locs exist //check for any match and if match uset setlocrules to overwrite
	size_t	matchCount = checkLocationMatch(locVec, reqRules);
	location &current = reqRules; 
	while (matchCount != 0)
	{
		if (!matchCount || current._nestedLocations.empty())
			return;
		locVec = current._nestedLocations;
		matchCount = checkLocationMatch(locVec, nestRules);
		if (matchCount)
			setLocRules(reqRules, nestRules);

		if (matchCount && !reqRules._nestedLocations.empty())
		if (!nestRules._nestedLocations.empty())
	}
}

size_t	Request::checkLocationMatch(std::vector<location> &locs, location &ruleblock)
{ 
	// 1: Exact Match (=)
	// 2: Longest Prefix Match
	// 3: Regex Match
	// 4: Default Location
	size_t	bestMatchSize = 0;
	size_t	newSize;
	for (std::vector<location>::iterator it = locs.begin(); it != locs.end(); ++it)
	{
		newSize = countPathMatch(_filePath, it->getName());
		if (newSize > bestMatchSize)
		{
			bestMatchSize = newSize;
			ruleblock = *it;
			if (newSize == std::string::npos)
				break;
		}
	}
	return (bestMatchSize);
}
