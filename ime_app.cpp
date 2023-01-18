#include "ime_app.h"
#include "html_string.h"

#define BASE_WINDOW_WIDTH  600
#define BASE_WINDOW_HEIGHT 400

#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 600

ULApp::ULApp() {
	ultralight::Settings settings;
	settings.app_name = "inspector";
	settings.file_system_path = ".";

	///
	/// Create our main App instance.
	///
	app_ = App::Create(settings);

	///
	/// Create a resizable window by passing by OR'ing our window flags with
	/// kWindowFlags_Resizable.
	///
	window_ = Window::Create(app_->main_monitor(), BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT,
		false, kWindowFlags_Titled | kWindowFlags_Resizable);

	///
	/// Create our HTML overlay-- we don't care about its initial size and
	/// position because it'll be calculated when we call OnResize() below.
	///
	overlay_ = Overlay::Create(window_, 1, 1, 0, 0);

	///
	/// Force a call to OnResize to perform size/layout of our overlay.
	///
	OnResize(window_.get(), window_->width(), window_->height());

	///
	/// Load a page into our overlay's View
	///
	//overlay_->view()->LoadURL("inspector://127.0.0.1:19998");
	overlay_->view()->LoadHTML(htmlString());

	///
	/// Register our InspectorApp instance as an AppListener so we can handle the
	/// App's OnUpdate event below.
	///
	app_->set_listener(this);

	///
	/// Register our InspectorApp instance as a WindowListener so we can handle the
	/// Window's OnResize event below.
	///
	window_->set_listener(this);

	///
	/// Register our InspectorApp instance as a LoadListener so we can handle the
	/// View's OnFinishLoading and OnDOMReady events below.
	///
	overlay_->view()->set_load_listener(this);

	///
	/// Register our InspectorApp instance as a ViewListener so we can handle the
	/// View's OnChangeCursor and OnChangeTitle events below.
	///
	overlay_->view()->set_view_listener(this);

	extern void IME_Init(void* hwnd);
	IME_Init(window_->native_handle());

	overlay_->view()->CreateLocalInspectorView();
}

RefPtr<View> ULApp::OnCreateInspectorView(ultralight::View* caller, bool is_local,
	const String& inspected_url) {
	if (inspector_overlay_ || inspector_window_)
		return nullptr;

	inspector_window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
		false, kWindowFlags_Titled | kWindowFlags_Resizable | kWindowFlags_Maximizable);
	inspector_window_->set_listener(this);

	inspector_overlay_ = Overlay::Create(inspector_window_, inspector_window_->width(), inspector_window_->height(), 0, 0);

	inspector_overlay_->Show();

	return inspector_overlay_->view();
}

ULApp::~ULApp() {
}

void ULApp::Run() {
	app_->Run();
}

void ULApp::OnUpdate() {
	///
	/// This is called repeatedly from the application's update loop.
	///
	/// You should update any app logic here.
	///
}

void ULApp::OnClose(ultralight::Window* window) {
	bool dont_quit = window == inspector_window_.get();
	// Invalidate the inspector either way
	inspector_overlay_ = nullptr;
	inspector_window_ = nullptr;
	if (dont_quit)
		return;

	app_->Quit();
}

void ULApp::OnResize(ultralight::Window* window, uint32_t width, uint32_t height) {
	///
	/// This is called whenever the window changes size (values in pixels).
	///
	/// We resize our overlay here to take up the entire window.
	///
	if (window == window_)
		overlay_->Resize(width, height);
	else if (window == inspector_window_)
		inspector_overlay_->Resize(width, height);
}

void ULApp::OnFinishLoading(ultralight::View* caller,
	uint64_t frame_id,
	bool is_main_frame,
	const String& url) {
	///
	/// This is called when a frame finishes loading on the page.
	///
}

ultralight::RefPtr<ultralight::JSContext> jsctx;

void ULApp::OnDOMReady(ultralight::View* caller,
	uint64_t frame_id,
	bool is_main_frame,
	const String& url) {
	///
	/// This is called when a frame's DOM has finished loading on the page.
	///
	/// This is the best time to setup any JavaScript bindings.
	///
	
	jsctx = caller->LockJSContext();
}

ultralight::RefPtr<ultralight::JSContext> JSCtx()
{
	return jsctx;
}

void ULApp::OnChangeCursor(ultralight::View* caller,
	Cursor cursor) {
	///
	/// This is called whenever the page requests to change the cursor.
	///
	/// We update the main window's cursor here.
	///
	window_->SetCursor(cursor);
}

void ULApp::OnChangeTitle(ultralight::View* caller,
	const String& title) {
	///
	/// This is called whenever the page requests to change the title.
	///
	/// We update the main window's title here.
	///
	window_->SetTitle(title.utf8().data());
}
