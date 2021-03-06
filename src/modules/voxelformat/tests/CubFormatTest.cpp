/**
 * @file
 */

#include "AbstractVoxFormatTest.h"
#include "voxelformat/CubFormat.h"

namespace voxel {

class CubFormatTest: public AbstractVoxFormatTest {
};

TEST_F(CubFormatTest, testLoad) {
	CubFormat f;
	std::unique_ptr<RawVolume> volume(load("cw.cub", f));
}

}
