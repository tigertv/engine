/**
 * @file
 */

#pragma once

#include "util/Console.h"
#include "console/Input.h"
#include "core/Var.h"
#include <uv.h>

namespace console {

class CursesConsole : public util::Console {
private:
	using Super = util::Console;
	core::VarPtr _cursesVar;
	console::Input _input;
	bool _enableCurses = false;
	bool _cursesActive = false;
	uv_loop_t _loop;
protected:
	void initCurses();
	void shutdownCurses();

	void drawString(int x, int y, const glm::ivec4& color, int, const char* str, int len) override;
	int lineHeight() override;
	glm::ivec2 stringSize(const char* s, int length) override;
	void afterRender(const math::Rect<int> &rect) override;
	void beforeRender(const math::Rect<int> &rect) override;
public:
	CursesConsole();
	virtual ~CursesConsole();

	bool init() override;
	void construct() override;
	void update(uint32_t deltaTime);
	void shutdown() override;
};

}
