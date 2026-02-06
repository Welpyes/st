void
updatexy()
{
	Window child;
	XTranslateCoordinates(xw.dpy, xw.win, DefaultRootWindow(xw.dpy), 0, 0, &win.x, &win.y, &child);
}

/*
 * initialize background image
 */

static Pixmap bgpix = 0;
static int last_w = -1, last_h = -1;

void
bginit()
{
	XGCValues gcvalues;
	Imlib_Image img;
	int w, h, tw, th;
	int nw, nh, x_off, y_off;
	double scale;

	if (!bgfile)
		return;

	if (pseudotransparency) {
		tw = DisplayWidth(xw.dpy, xw.scr);
		th = DisplayHeight(xw.dpy, xw.scr);
	} else {
		tw = win.w;
		th = win.h;
	}

	if (tw == last_w && th == last_h && bgpix != 0)
		return;

	img = imlib_load_image(bgfile);
	if (!img) {
		fprintf(stderr, "could not load background image: %s\n", bgfile);
		return;
	}

	imlib_context_set_image(img);
	w = imlib_image_get_width();
	h = imlib_image_get_height();
	
	if (tw <= 0 || th <= 0) {
		imlib_free_image();
		return;
	}

	if (w > 0 && h > 0) {
		scale = MAX((double)tw / w, (double)th / h);
		nw = (int)(w * scale);
		nh = (int)(h * scale);
		x_off = (tw - nw) / 2;
		y_off = (th - nh) / 2;
	} else {
		nw = tw;
		nh = th;
		x_off = 0;
		y_off = 0;
	}

	if (bgpix)
		XFreePixmap(xw.dpy, bgpix);

	bgpix = XCreatePixmap(xw.dpy, xw.win, tw, th, DefaultDepth(xw.dpy, xw.scr));

	imlib_context_set_display(xw.dpy);
	imlib_context_set_visual(xw.vis);
	imlib_context_set_colormap(xw.cmap);
	imlib_context_set_drawable(bgpix);

	imlib_render_image_on_drawable_at_size(x_off, y_off, nw, nh);
	imlib_free_image();

	if (xw.bggc)
		XFreeGC(xw.dpy, xw.bggc);

	memset(&gcvalues, 0, sizeof(gcvalues));
	xw.bggc = XCreateGC(xw.dpy, xw.win, 0, &gcvalues);
	XSetTile(xw.dpy, xw.bggc, bgpix);
	XSetFillStyle(xw.dpy, xw.bggc, FillTiled);
	
	last_w = tw;
	last_h = th;

	if (pseudotransparency) {
		updatexy();
		MODBIT(xw.attrs.event_mask, 1, PropertyChangeMask);
		XChangeWindowAttributes(xw.dpy, xw.win, CWEventMask, &xw.attrs);
	}
}

void
reload_image()
{
	bginit();
	redraw();
}
