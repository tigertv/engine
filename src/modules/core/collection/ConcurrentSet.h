/**
 * @file
 */

#pragma once

#include "core/concurrent/Lock.h"
#include "core/concurrent/ConditionVariable.h"
#include "core/Common.h"
#include <stdint.h>
#include <unordered_set>
#include <algorithm>

namespace collection {

template<class Data>
class ConcurrentSet {
public:
	using underlying_type = std::unordered_set<Data>;
private:
	underlying_type _data;
	mutable core::Lock _mutex;
	core::ConditionVariable _conditionVariable;
public:
	void swap(underlying_type& target) {
		core::ScopedLock lock(_mutex);
		std::swap(_data, target);
	}

	void clear() {
		core::ScopedLock lock(_mutex);
		_data.clear();
	}

	bool insert(Data const& data) {
		bool result;
		{
			core::ScopedLock lock(_mutex);
			result = _data.insert(data).second;
		}
		_conditionVariable.signalOne();
		return result;
	}

	bool insert(Data&& data) {
		bool result;
		{
			core::ScopedLock lock(_mutex);
			result = _data.insert(data).second;
		}
		_conditionVariable.signalOne();
		return result;
	}

	bool contains(Data const& data) const {
		core::ScopedLock lock(_mutex);
		return _data.find(data) != _data.end();
	}

	inline bool empty() const {
		core::ScopedLock lock(_mutex);
		return _data.empty();
	}

	inline uint32_t size() const {
		core::ScopedLock lock(_mutex);
		return _data.size();
	}

	template<class VISITOR>
	void visit(VISITOR&& visitor) const {
		core::ScopedLock lock(_mutex);
		for (const Data& d : _data) {
			visitor(d);
		}
	}
};

}
