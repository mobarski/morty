
scr = {}
scr.w = 0
scr.h = 0
scr.c = 0
scr.x = 0 // TODO
scr.y = 0 // TODO
scr.i = 0
scr.pal = [
	0x1a,0x18,0x28,0xff,
	0x4d,0x5a,0x6c,0xff,
	0x6d,0xa9,0xe3,0xff,
	0xff,0xff,0xff,0xff,
	0xee,0xb3,0x33,0xff,
	0x25,0x93,0x22,0xff,
	0xb0,0x48,0x48,0xff,
	0x5b,0x2e,0x33,0xff,
] // https://lospec.com/palette-list/primal8


// ----------------------------------------------------------------------------

function init_screen(width=800, height=400) {	
	var screen = document.getElementById("screen")
	screen.innerHTML = `<canvas id="main_canvas" width="${width}" height="${height}""></canvas>`
	scr.w = width
	scr.h = height
	
	out = document.getElementById("output")
	cnv = document.getElementById("main_canvas")
	
	ctx = cnv.getContext("2d")
	ctx.imageSmoothingEnabled = false
	
	get_frame_buffer()
}

function fullscreen() {
	if (cnv.requestFullscreen) {
		cnv.requestFullscreen()
	} else if (cnv.mozRequestFullScreen) { /* Firefox */
		cnv.mozRequestFullScreen()
	} else if (cnv.webkitRequestFullscreen) { /* Chrome, Safari and Opera */
		cnv.webkitRequestFullscreen()
	} else if (cnv.msRequestFullscreen) { /* IE/Edge */
		cnv.msRequestFullscreen()
	}
}

function get_frame_buffer() {
    image_data = ctx.getImageData(0, 0, ctx.canvas.width, ctx.canvas.height)
    frame_buffer = image_data.data
}

function flip() {
	ctx.putImageData(image_data, 0, 0)
}

// ----------------------------------------------------------------------------

function cls(c) {
	i_max = ctx.canvas.width * ctx.canvas.height
	for (i=0; i<i_max; i++) {
		pset(i,c)
	}
}

function pset(i,c) {
	po = i*4 // pixel offset
	co = c*4 // color offset
	frame_buffer[po+0] = scr.pal[co+0]
	frame_buffer[po+1] = scr.pal[co+1]
	frame_buffer[po+2] = scr.pal[co+2]
	frame_buffer[po+3] = scr.pal[co+3]
}

// ----------------------------------------------------------------------------

init_screen() // XXX
