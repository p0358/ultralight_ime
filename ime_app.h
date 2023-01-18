#pragma once
#include <AppCore/AppCore.h>
#include <Ultralight/platform/Platform.h>

using namespace ultralight;

class ULApp : public AppListener,
	public WindowListener,
	public LoadListener,
	public ViewListener {
public:
	ULApp();

	virtual ~ULApp();

	// Start the run loop.
	virtual void Run();

	// This is called continuously from the app's main loop.
	virtual void OnUpdate() override;

	// This is called when the window is closing.
	virtual void OnClose(ultralight::Window* window) override;

	// This is called whenever the window resizes.
	virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override;

	// This is called when the page finishes a load in one of its frames.
	virtual void OnFinishLoading(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const String& url) override;

	// This is called when the DOM has loaded in one of its frames.
	virtual void OnDOMReady(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const String& url) override;

	// This is called when the page requests to change the Cursor.
	virtual void OnChangeCursor(ultralight::View* caller,
		Cursor cursor) override;

	virtual void OnChangeTitle(ultralight::View* caller,
		const String& title) override;

	virtual RefPtr<View> OnCreateInspectorView(ultralight::View* caller, bool is_local,
		const String& inspected_url) override;

protected:
	RefPtr<App> app_;
	RefPtr<Window> window_;
	RefPtr<Overlay> overlay_;
	RefPtr<Window> inspector_window_;
	RefPtr<Overlay> inspector_overlay_;
};
