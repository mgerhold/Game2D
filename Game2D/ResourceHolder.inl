#include "ResourceHolder.h"
template<typename idT, typename resT>
inline void ResourceHolder<idT, resT>::load(idT id, const std::string& filename) {
	/*const auto find = mResourceMap.find(id);
	assert(find == mResourceMap.end());*/
	if (isLoaded(id))
		return;
	resT resource;
	bool success = resource.loadFromFile(filename);
	assert(success);
	mResourceMap.insert({ id, std::move(resource) });
}

template<typename idT, typename resT>
inline bool ResourceHolder<idT, resT>::isLoaded(idT id) const {
	const auto find = mResourceMap.find(id);
	return find != mResourceMap.end();
}

template<typename idT, typename resT>
inline void ResourceHolder<idT, resT>::unload(idT id) {
	const auto find = mResourceMap.find(id);
	assert(find != mResourceMap.end());
	mResourceMap.erase(find);
}

template<typename idT, typename resT>
inline resT& ResourceHolder<idT, resT>::get(idT id) {
	return const_cast<resT&>(std::as_const(*this).get(id));
}

template<typename idT, typename resT>
inline const resT& ResourceHolder<idT, resT>::get(idT id) const {
	const auto find = mResourceMap.find(id);
	assert(find != mResourceMap.end());
	return find->second;
}