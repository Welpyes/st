void
updatexy()
{
	Window child;
	XTranslateCoordinates(xw.dpy, xw.win, DefaultRootWindow(xw.dpy), 0, 0, &win.x, &win.y, &child);
}

/*
 * initialize background image
 */
void
bginit()
{
	XGCValues gcvalues;
	Imlib_Image img;
	char path[PATH_MAX];

	memset(&gcvalues, 0, sizeof(gcvalues));
	xw.bggc = XCreateGC(xw.dpy, xw.win, 0, &gcvalues);

	if (bgfile[0] == '~') {
		const char *home = getenv("HOME");
		if (home) {
			int ret = snprintf(path, sizeof(path), "%s%s", home, bgfile + 1);
			if (ret < 0 || (size_t)ret >= sizeof(path)) {
				fprintf(stderr, "path too long\n");
				return;
			}
		} else {
			strncpy(path, bgfile, sizeof(path));
			path[sizeof(path)-1] = '\0';
		}
	} else {
		strncpy(path, bgfile, sizeof(path));
		path[sizeof(path)-1] = '\0';
	}

	img = imlib_load_image(path);
	if (!img) {
		fprintf(stderr, "could not load background image: %s\n", path);
		return;
	}

	imlib_context_set_image(img);
	int w = imlib_image_get_width();
	int h = imlib_image_get_height();

	Pixmap bgimg = XCreatePixmap(xw.dpy, xw.win, w, h, DefaultDepth(xw.dpy, xw.scr));

	imlib_context_set_display(xw.dpy);
	imlib_context_set_visual(DefaultVisual(xw.dpy, xw.scr));
	imlib_context_set_colormap(DefaultColormap(xw.dpy, xw.scr));
	imlib_context_set_drawable(bgimg);

	imlib_render_image_on_drawable(0, 0);
	imlib_free_image();

	XSetTile(xw.dpy, xw.bggc, bgimg);
	XSetFillStyle(xw.dpy, xw.bggc, FillTiled);

	if (pseudotransparency) {
		updatexy();
		MODBIT(xw.attrs.event_mask, 1, PropertyChangeMask);
		XChangeWindowAttributes(xw.dpy, xw.win, CWEventMask, &xw.attrs);
	}
}

void
reload_image()
{
	XFreeGC(xw.dpy, xw.bggc);
	bginit();
	redraw();
}