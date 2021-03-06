/**
 * @file
 */

#pragma once

#include "math/Octree.h"
#include "WorldMeshExtractor.h"
#include "video/Camera.h"
#include "voxel/VoxelVertex.h"
#include "WorldShader.h"
#include "video/Buffer.h"
#include <future>

namespace voxelrender {

class WorldChunkMgr {
protected:
	struct ChunkBuffer {
		bool inuse = false;
		float birthSeconds = 0.0f;
		math::AABB<int> _aabb = {glm::zero<glm::ivec3>(), glm::zero<glm::ivec3>()};
		voxel::Mesh mesh;

		video::Buffer _buffer;
		int32_t _vbo = -1;
		int32_t _ibo = -1;

		~ChunkBuffer() {
			reset();
		}

		void reset() {
			_buffer.shutdown();
			_vbo = -1;
			_ibo = -1;
			inuse = false;
		}

		/**
		 * This is the world position. Not the render positions. There is no scale
		 * applied here.
		 */
		inline const glm::ivec3 &translation() const {
			return mesh.getOffset();
		}

		/**
		 * This is the render aabb. There might be a scale applied here. So the mins of
		 * the AABB might not be at the position given by @c translation()
		 */
		inline const math::AABB<int> &aabb() const {
			return _aabb;
		}
	};


	using Tree = math::Octree<ChunkBuffer *>;
	Tree _octree;
	static constexpr int MAX_CHUNKBUFFERS = 512;
	ChunkBuffer _chunkBuffers[MAX_CHUNKBUFFERS];
	int _maxAllowedDistance = -1;

	struct VisibleBuffers {
		int size = 0;
		ChunkBuffer* visible[MAX_CHUNKBUFFERS];
	};
	VisibleBuffers _visibleBuffers;

	shader::WorldShader* _worldShader;

	WorldMeshExtractor _meshExtractor;
	core::ThreadPool &_threadPool;

	int distance2(const glm::ivec3 &pos, const glm::ivec3 &pos2) const;

	void cull(const video::Camera &camera);
	void handleMeshQueue(float nowSeconds);
	bool initTerrainBuffer(ChunkBuffer* chunk);
public:
	WorldChunkMgr(core::ThreadPool& threadPool);

	int renderTerrain(float nowSeconds);

	void extractMesh(const glm::ivec3 &pos);
	void extractMeshes(const video::Camera &camera);
	void extractScheduledMesh();

	void update(float nowSeconds, const video::Camera &camera, const glm::vec3& focusPos);

	void updateViewDistance(float viewDistance);
	bool init(shader::WorldShader* worldShader, voxel::PagedVolume* volume);
	void shutdown();
	void reset();
};

}
