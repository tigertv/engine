/**
 * @file
 */

#pragma once

#include "Map.h"
#include "core/IComponent.h"
#include "backend/ForwardDecl.h"
#include "ai/server/Server.h"
#include <unordered_map>

namespace backend {

/**
 * @brief The world is the whole universe of all @c Map instances.
 */
class World : public core::IComponent {
private:
	MapProviderPtr _mapProvider;
	AIRegistryPtr _registry;
	core::EventBusPtr _eventBus;
	io::FilesystemPtr _filesystem;
	ai::Server* _aiServer = nullptr;
	std::unordered_map<MapId, MapPtr> _maps;
public:
	World(const MapProviderPtr& mapProvider, const AIRegistryPtr& registry,
			const core::EventBusPtr& eventBus, const io::FilesystemPtr& filesystem);
	~World();

	void update(long dt);

	MapPtr map(MapId id) const;

	void construct() override;
	bool init() override;
	void shutdown() override;
};

inline MapPtr World::map(MapId id) const {
	auto i = _maps.find(id);
	if (i == _maps.end()) {
		return MapPtr();
	}
	return i->second;
}

}
