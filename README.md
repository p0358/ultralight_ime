# ultralight_ime

App that implements IME (Input Method Editors) with Ultralight, suitable for fullscreen games. Sample project for MSVC/Visual Studio on Windows.

![image](https://user-images.githubusercontent.com/5182588/213150167-b9bf08e4-4075-4bc3-be3d-295bd2d1a0ab.png)

## Fullscreen vs non-fullscreen

By default IME is drawn by Windows as a window on top of your app.

If you use Ultralight in non-fullscreen mode, you could easily change `m_fullscreen` to `FALSE` in `IMM.h` in this sample app and have IME editor be drawn this way. However in such case you'd need to implement `m_sigGetTextExt` callback, which would have to get appropriate position from JS and convert it to real pixels to let Windows know where to show the editor, without doing this the window will be shown at position (0,0).

However manual drawing allows you to use IME even in full-screen mode, which is very useful for games. In this case you are responsible yourself for drawing the window with propositions. The extra perk of this is that you can style such window to match your custom app's style.

## Input handling

During the process of composition, events are passed into the page's JS: composition, commit, list of candidates, alpha mode.

JS handles inputting the text into your textual element (such as <input>) and all the logic around such (see `html_string.h` file).

## To-do

As this is a sample reference implementation, certain things aren't implemented/could be improved:

* Clicking inside or outside of input elements in HTML should cause the `g_ingameIME.setState` method to be called, in order to disable IME composition while you're not in any textual field.

* Callback `IME_OnAlphaMode` is received when alpha mode changes (alpha mode = entering regular latin characters from QWERTY, non-alpha = IME cpomposition is in use). You should offer some kind of visual cue to the user of which mode is in use, for example by displaying "A"/"小" to the right of the text field, so that they know which kind of characters they should expect to input now (and know to switch mode if needed).

## Building

1. `git clone --recursive https://github.com/p0358/ultralight_ime.git`

2. Copy `ultralight` dir from Ultralight's SDK (has to have `include` and `lib` dirs inside).

3. Build the app.

4. Copy `resources` folders next to your built app's exe (for example into `x64/Release`), from your Ultralight SDK to the same location. You will need to distribute it with your app too, you can optionally adjust paths in source code to use the same location as your main client app, if they differ from defaults.

5. Also copy the `inspector` folder if you want to poke around, otherwise edit `ime_app.cpp` and comment out `overlay_->view()->CreateLocalInspectorView();` line.

6. Copy all your Ultralight DLLs next to the app's exe too.

7. Launch `imetest.exe` and connect, assuming your client is running and its inspector server is also running (see below).
