/**
 * @file
 */
#include "TestTexture.h"
#include "testcore/TestAppMain.h"
#include "video/Camera.h"
#include "video/ScopedViewPort.h"
#include "core/Color.h"

TestTexture::TestTexture(const metric::MetricPtr& metric, const io::FilesystemPtr& filesystem, const core::EventBusPtr& eventBus, const core::TimeProviderPtr& timeProvider) :
		Super(metric, filesystem, eventBus, timeProvider) {
	init(ORGANISATION, "testtexture");
}

core::AppState TestTexture::onInit() {
	const core::AppState state = Super::onInit();
	if (state != core::AppState::Running) {
		return state;
	}
	setUICamera();

	if (!_renderer.init(frameBufferDimension())) {
		Log::error("Failed to init the texture renderer");
		return core::AppState::InitFailure;
	}

	_texture = video::createTextureFromImage("texture.png");
	if (!_texture) {
		Log::error("Failed to load texture");
		return core::AppState::InitFailure;
	}

	video::clearColor(::core::Color::White);

	return state;
}

void TestTexture::doRender() {
	video::ScopedViewPort viewPort(0, 0, frameBufferDimension().x, frameBufferDimension().y);
	video::ScopedTexture texture(_texture, video::TextureUnit::Zero);
	_renderer.render(camera().projectionMatrix());
}

core::AppState TestTexture::onCleanup() {
	if (_texture) {
		_texture->shutdown();
	}
	_renderer.shutdown();
	return Super::onCleanup();
}

TEST_APP(TestTexture)
