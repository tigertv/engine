/**
 * @file
 */

#pragma once

#include "commonlua/LUA.h"
#include "stock/StockDataProvider.h"
#include "stock/Shape.h"
#include "core/StringUtil.h"

namespace stock {

static StockDataProvider* luaGetStockDataProvider(lua_State * l) {
	return lua::LUA::globalData<StockDataProvider>(l, "Provider");
}

static int luaCreateContainer(lua_State * l) {
	StockDataProvider *stockDataProvider = luaGetStockDataProvider(l);
	const uint8_t containerId = (uint8_t)luaL_checkinteger(l, 1);
	const char* containerName = luaL_checkstring(l, 2);
	ContainerData* udata = lua::LUA::newUserdata(l, "Container", new ContainerData());
	udata->name = containerName;
	udata->id = containerId;
	if (!stockDataProvider->addContainerData(udata)) {
		const core::String& error = core::string::format("Could not add container with name: %s", containerName);
		return lua::LUA::returnError(l, error);
	}
	return 1;
}

static ContainerData* luaGetContainerData(lua_State * l, int n) {
	return lua::LUA::userData<ContainerData>(l, n, "Container");
}

static int luaContainerDataGetShape(lua_State * l) {
	ContainerData *containerData = luaGetContainerData(l, 1);
	lua::LUA::newUserdata(l, "ContainerShape", &containerData->shape);
	return 1;
}

static int luaContainerDataGC(lua_State * l) {
	// this is deleted in the StockDataProvider
	return 0;
}

static int luaContainerDataToString(lua_State * l) {
	const ContainerData *containerData = luaGetContainerData(l, 1);
	lua_pushfstring(l, "container: %d (%s)", (int)containerData->id, containerData->name.c_str());
	return 1;
}

static int luaContainerDataGetName(lua_State * l) {
	const ContainerData *ctx = luaGetContainerData(l, 1);
	lua_pushstring(l, ctx->name.c_str());
	return 1;
}

static int luaContainerDataGetId(lua_State * l) {
	const ContainerData *ctx = luaGetContainerData(l, 1);
	lua_pushinteger(l, ctx->id);
	return 1;
}

static ContainerShape* luaGetContainerDataShape(lua_State * l, int n) {
	return lua::LUA::userData<ContainerShape>(l, n, "ContainerShape");
}

static int luaContainerDataShapeAddRect(lua_State * l) {
	ContainerShape *containerShape = luaGetContainerDataShape(l, 1);
	const uint8_t x = luaL_checkinteger(l, 2);
	const uint8_t y = luaL_checkinteger(l, 3);
	const uint8_t w = luaL_checkinteger(l, 4);
	const uint8_t h = luaL_checkinteger(l, 5);
	containerShape->addRect(x, y, w, h);
	return 0;
}

static int luaContainerDataShapeGC(lua_State * l) {
	return 0;
}

static int luaContainerDataShapeToString(lua_State * l) {
	//const ContainerShape *containerShape = luaGetContainerDataShape(l, 1);
	lua_pushfstring(l, "container shape");
	return 1;
}

static int luaCreateItemData(lua_State * l) {
	StockDataProvider *stockDataProvider = luaGetStockDataProvider(l);
	const ItemId itemId = luaL_checkinteger(l, 1);
	const char *type = luaL_checkstring(l, 2);
	const char *name = luaL_optstring(l, 3, nullptr);
	const ItemType itemType = getItemType(type);
	if (itemType == ItemType::NONE) {
		const core::String& error = core::string::format("Unknown type given: %s", type);
		return lua::LUA::returnError(l, error);
	}

	ItemData* udata = lua::LUA::newUserdata(l, "Item", new ItemData(itemId, itemType));
	if (name != nullptr) {
		udata->setName(name);
	}
	stockDataProvider->addItemData(udata);
	return 1;
}

static ItemData* luaGetItemData(lua_State * l, int n) {
	return lua::LUA::userData<ItemData>(l, n, "Item");
}

static int luaItemDataGetShape(lua_State * l) {
	ItemData *itemData = luaGetItemData(l, 1);
	lua::LUA::newUserdata(l, "ItemShape", &itemData->shape());
	return 1;
}

static int luaItemDataGC(lua_State * l) {
	// this is deleted in the StockDataProvider
	return 0;
}

static int luaPushItem(lua_State * l, ItemData* item) {
	lua::LUA::newUserdata(l, "Item", item);
	return 1;
}

static int luaItemDataToString(lua_State * l) {
	const ItemData *itemData = luaGetItemData(l, 1);
	lua_pushfstring(l, "item: %d (%s)", (int)itemData->id(), itemData->name());
	return 1;
}

static int luaItemDataGetName(lua_State * l) {
	const ItemData *ctx = luaGetItemData(l, 1);
	lua_pushstring(l, ctx->name());
	return 1;
}

static int luaItemDataSetName(lua_State * l) {
	ItemData *ctx = luaGetItemData(l, 1);
	ctx->setName(luaL_checkstring(l, 2));
	return luaPushItem(l, ctx);
}

static int luaItemDataAddLabel(lua_State * l) {
	ItemData *ctx = luaGetItemData(l, 1);
	const char *key = luaL_checkstring(l, 2);
	const char *value = luaL_checkstring(l, 3);
	ctx->addLabel(key, value);
	return luaPushItem(l, ctx);
}

static int luaItemDataSetSize(lua_State * l) {
	ItemData *ctx = luaGetItemData(l, 1);
	const int w = luaL_checkinteger(l, 2);
	const int h = luaL_checkinteger(l, 3);
	ctx->setSize(w, h);
	return luaPushItem(l, ctx);
}

static int luaItemDataGetId(lua_State * l) {
	const ItemData *ctx = luaGetItemData(l, 1);
	lua_pushinteger(l, ctx->id());
	return 1;
}

static ItemShape* luaGetItemDataShape(lua_State * l, int n) {
	return lua::LUA::userData<ItemShape>(l, n, "ItemShape");
}

static int luaItemDataShapeAddRect(lua_State * l) {
	ItemShape *itemShape = luaGetItemDataShape(l, 1);
	const uint8_t x = luaL_checkinteger(l, 2);
	const uint8_t y = luaL_checkinteger(l, 3);
	const uint8_t w = luaL_checkinteger(l, 4);
	const uint8_t h = luaL_checkinteger(l, 5);
	itemShape->addRect(x, y, w, h);
	return 0;
}

static int luaItemDataShapeGC(lua_State * l) {
	return 0;
}

static int luaItemDataShapeToString(lua_State * l) {
	const ItemShape& itemShape = *luaGetItemDataShape(l, 1);
	const core::String& bitsStr = core::string::format("w:%i, h:%i", itemShape.width(), itemShape.height());
	lua_pushfstring(l, "item shape:\n%s", bitsStr.c_str());
	return 1;
}

}
