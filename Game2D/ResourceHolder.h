#pragma once

#include <map>
#include <string>
#include <cassert>
#include <iostream>

template<typename idT, typename resT>
class ResourceHolder {
public:
	void				load(idT id, const std::string& filename);
	resT&				get(idT id);
	const resT&			get(idT id) const;
private:
	std::map<idT, resT>	mResourceMap;
};

#include "ResourceHolder.inl"