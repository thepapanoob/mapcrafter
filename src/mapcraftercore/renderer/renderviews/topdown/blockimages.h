/*
 * Copyright 2012-2015 Moritz Hilscher
 *
 * This file is part of Mapcrafter.
 *
 * Mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOPDOWN_BLOCKIMAGES_H_
#define TOPDOWN_BLOCKIMAGES_H_

#include "../../blockimages.h"

namespace mapcrafter {
namespace renderer {

class TopdownBlockImages : public AbstractBlockImages {
public:
	TopdownBlockImages();
	virtual ~TopdownBlockImages();

	virtual const RGBAImage& getOpaqueWater(bool south, bool west) const;

	virtual int getBlockSize() const;

protected:
	void createItemStyleBlock(uint16_t id, uint16_t data, const RGBAImage& texture);
	void createRotatedBlock(uint16_t id, uint16_t extra_data, const RGBAImage& texture);

	void createWood(uint16_t id, uint16_t extra_data, const RGBAImage& side, const RGBAImage& top); // id 17, 162
	void createDispenserDropper(uint16_t id, const RGBAImage& front); // id 23, 158
	void createBed(); // id 26
	void createStraightRails(uint16_t id, uint16_t extra_data, const RGBAImage& texture); // id 27, 28
	void createChest(uint16_t id, const ChestTextures& textures); // 54, 130
	void createDoubleChest(uint16_t id, const DoubleChestTextures& textures); // 54
	void createRails(); // id 66
	void createVines(); // id 106
	void createLargePlant(uint16_t data, const RGBAImage& texture, const RGBAImage& top_texture); // id 175

	virtual uint16_t filterBlockData(uint16_t id, uint16_t data) const;
	virtual bool isImageTransparent(const RGBAImage& block) const;
	virtual void setBlockImage(uint16_t id, uint16_t data, const RGBAImage& block);

	virtual RGBAImage createUnknownBlock() const;
	virtual RGBAImage createBiomeBlock(uint16_t id, uint16_t data, const Biome& biome) const;

	virtual void createBlocks();
	virtual int createOpaqueWater();
};

} /* namespace renderer */
} /* namespace mapcrafter */

#endif /* TOPDOWN_BLOCKIMAGES_H_ */