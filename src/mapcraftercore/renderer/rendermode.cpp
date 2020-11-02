/*
 * Copyright 2012-2016 Moritz Hilscher
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

#include "rendermode.h"

#include "blockimages.h"
#include "image.h"
#include "rendermodes/cave.h"
#include "rendermodes/nether.h"
#include "rendermodes/lighting.h"
#include "rendermodes/heightoverlay.h"
#include "rendermodes/slimeoverlay.h"
#include "rendermodes/spawnoverlay.h"
#include "../config/configsections/map.h"
#include "../config/configsections/world.h"
#include "../mc/chunk.h"
#include "../mc/pos.h"
#include "../mc/world.h"
#include "../util.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace mapcrafter {
namespace renderer {

BaseRenderMode::BaseRenderMode()
	: images(nullptr), block_images(nullptr), world(nullptr),
	current_chunk(nullptr) {
}

BaseRenderMode::~BaseRenderMode() {
}

void BaseRenderMode::initialize(const RenderView* render_view, 
		BlockImages* images, mc::WorldCache* world, mc::Chunk** current_chunk) {
	this->images = images;
	this->block_images = dynamic_cast<RenderedBlockImages*>(images);
	assert(this->block_images != nullptr);
	this->world = world;
	this->current_chunk = current_chunk;
}

bool BaseRenderMode::isHidden(const mc::BlockPos& pos, uint16_t id,
		uint16_t data) {
	return false;
}

void BaseRenderMode::draw(RGBAImage& image, const mc::BlockPos& pos,
		uint16_t id, uint16_t data) {
}

mc::Block BaseRenderMode::getBlock(const mc::BlockPos& pos, int get) {
	return world->getBlock(pos, *current_chunk, get);
}

MultiplexingRenderMode::~MultiplexingRenderMode() {
	for (auto it = render_modes.begin(); it != render_modes.end(); ++it)
		delete *it;
}

void MultiplexingRenderMode::addRenderMode(RenderMode* render_mode) {
	render_modes.push_back(render_mode);
}

void MultiplexingRenderMode::initialize(const RenderView* render_view, 
		BlockImages* images, mc::WorldCache* world, mc::Chunk** current_chunk) {
	for (auto it = render_modes.begin(); it != render_modes.end(); ++it)
		(*it)->initialize(render_view, images, world, current_chunk);
}

bool MultiplexingRenderMode::isHidden(const mc::BlockPos& pos, uint16_t id,
		uint16_t data) {
	for (auto it = render_modes.begin(); it != render_modes.end(); ++it)
		if ((*it)->isHidden(pos, id, data))
			return true;
	return false;
}

bool MultiplexingRenderMode::isHidden(const mc::BlockPos& pos, const BlockImage& block_image) {
	for (auto it = render_modes.begin(); it != render_modes.end(); ++it)
		if ((*it)->isHidden(pos, block_image))
			return true;
	return false;
}

void MultiplexingRenderMode::draw(RGBAImage& image, const mc::BlockPos& pos,
		uint16_t id, uint16_t data) {
	for (auto it = render_modes.begin(); it != render_modes.end(); ++it)
		(*it)->draw(image, pos, id, data);
}


void MultiplexingRenderMode::draw(RGBAImage& image, const BlockImage& block_image,
		const mc::BlockPos& pos, uint16_t id) {
	for (auto it = render_modes.begin(); it != render_modes.end(); ++it)
		(*it)->draw(image, block_image, pos, id);
}

std::ostream& operator<<(std::ostream& out, RenderModeType render_mode) {
	switch (render_mode) {
	case RenderModeType::PLAIN: return out << "plain";
	case RenderModeType::DAYLIGHT: return out << "daylight";
	case RenderModeType::NIGHTLIGHT: return out << "nightlight";
	case RenderModeType::CAVE: return out << "cave";
	case RenderModeType::CAVELIGHT: return out << "cavelight";
	case RenderModeType::NETHER: return out << "nether";
	default: return out << "unknown";
	}
}

std::ostream& operator<<(std::ostream& out, OverlayType overlay) {
	switch (overlay) {
	case OverlayType::SLIME: return out << "slime";
	case OverlayType::SPAWNDAY: return out << "spawnday";
	case OverlayType::SPAWNNIGHT: return out << "spawnnight";
	default: return out << "unknown";
	}
}

RenderMode* createRenderMode(const config::WorldSection& world_config,
		const config::MapSection& map_config, int rotation) {
	RenderModeType type = map_config.getRenderMode();
	OverlayType overlay = map_config.getOverlay();
	MultiplexingRenderMode* render_mode = new MultiplexingRenderMode();
	
	// create render mode
	if (type == RenderModeType::PLAIN) {
		// nothing
	} else if (type == RenderModeType::CAVE || type == RenderModeType::CAVELIGHT) {
		// hide some walls of caves which would cover the view into the caves
		if (map_config.getRenderView() == RenderViewType::ISOMETRIC)
			render_mode->addRenderMode(new CaveRenderMode({mc::DIR_SOUTH, mc::DIR_WEST, mc::DIR_TOP}));
		else
			render_mode->addRenderMode(new CaveRenderMode({mc::DIR_TOP}));
		// if we want some shadows, then simulate the sun light because it's dark in caves
		if (type == RenderModeType::CAVELIGHT)
			render_mode->addRenderMode(new LightingRenderMode(true, map_config.getLightingIntensity(),
						map_config.getLightingWaterIntensity(), true));
		render_mode->addRenderMode(new HeightOverlay());
	} else if (type == RenderModeType::NETHER) {
		// hide some walls of caves which would cover the view into the caves
		if (map_config.getRenderView() == RenderViewType::ISOMETRIC)
			render_mode->addRenderMode(new CaveRenderMode({mc::DIR_SOUTH, mc::DIR_WEST, mc::DIR_TOP}));
		else
			render_mode->addRenderMode(new CaveRenderMode({mc::DIR_TOP}));
	} else if (type == RenderModeType::DAYLIGHT) {
		render_mode->addRenderMode(new LightingRenderMode(true,
					map_config.getLightingIntensity(), map_config.getLightingWaterIntensity(),
					world_config.getDimension() == mc::Dimension::END));
	} else if (type == RenderModeType::NIGHTLIGHT) {
		render_mode->addRenderMode(new LightingRenderMode(false,
					map_config.getLightingIntensity(), map_config.getLightingWaterIntensity(),
					world_config.getDimension() == mc::Dimension::END));
	} else {
		// this shouldn't happen
		delete render_mode;
		assert(false);
		return nullptr;
	}
	
	// create overlay
	if (overlay == OverlayType::NONE) {
		// nothing
	} else if (overlay == OverlayType::SLIME) {
		mc::World world(world_config.getInputDir().string(), world_config.getDimension());
		render_mode->addRenderMode(new SlimeOverlay(world.getWorldDir(), rotation));
	} else if (overlay == OverlayType::SPAWNDAY) {
		render_mode->addRenderMode(new SpawnOverlay(true));
	} else if (overlay == OverlayType::SPAWNNIGHT) {
		render_mode->addRenderMode(new SpawnOverlay(false));
	} else {
		// this shouldn't happen
		delete render_mode;
		assert(false);
		return nullptr;
	}

	return render_mode;
}

} /* namespace render */
} /* namespace mapcrafter */
