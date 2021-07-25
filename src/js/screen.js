
scr = {}
scr.w = 0
scr.h = 0
scr.c = 0
scr.i = 0
scr.t = 0 // last sync time
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
scr.draw_fun = function () {}

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
	
	// frame buffer
    image_data = ctx.getImageData(0, 0, ctx.canvas.width, ctx.canvas.height)
	frame_buffer = new Array(ctx.canvas.width * ctx.canvas.height)
	frame_buffer.fill(0)
	
	scr.t = clock()
	
	// touch (mouse)
	cnv.addEventListener('mouseup',on_mouse_up)
	cnv.addEventListener('mousedown',on_mouse_down)
	cnv.addEventListener('mousemove',on_mouse_move)
	window.setInterval(scr.draw_fun,1000.0/10) // TODO: freq
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

function flip() {
	ctx.putImageData(image_data, 0, 0)
}

function clock() {
	return new Date().getTime()
	//return performance.now()
}

// ----------------------------------------------------------------------------


// NOT USED - cls in morty using `fill`
function cls(c) {
	var n = ctx.canvas.width * ctx.canvas.height
	fill(0,n,c)
}

function pset(i,c) {
	frame_buffer[i] = c
	//
	var po = i*4 // pixel offset
	var co = c*4 // color offset
	rgba_frame_buffer = image_data.data
	rgba_frame_buffer[po+0] = scr.pal[co+0]
	rgba_frame_buffer[po+1] = scr.pal[co+1]
	rgba_frame_buffer[po+2] = scr.pal[co+2]
	rgba_frame_buffer[po+3] = scr.pal[co+3]
}

function pget(i) {
	return frame_buffer[i]
}

function fill(i0,n,c) {
	var co = c*4 // color offset
	var r = scr.pal[co+0]
	var g = scr.pal[co+1]
	var b = scr.pal[co+2]
	var a = scr.pal[co+3]
	rgba_frame_buffer = image_data.data
	for (var i=i0; i<i0+n; i++) {
		frame_buffer[i] = c
		var po = i*4 // pixel offset
		rgba_frame_buffer[po+0] = r
		rgba_frame_buffer[po+1] = g
		rgba_frame_buffer[po+2] = b
		rgba_frame_buffer[po+3] = a
	}
}

// ----------------------------------------------------------------------------

// this is from itsy0, its not that accurate :/
function scale_mouse_xy(cx,cy) {
	var bcr = cnv.getBoundingClientRect()
	var bcr_left = bcr.left
	var bcr_top = bcr.top
	var bcr_w = bcr.width
	var bcr_h = bcr.height
	
	var ratio = bcr_h/scr.h
	var width = scr.w * ratio
	var bcr_left = 0.5*(bcr_w-width)
	
	var x = (cx - bcr_left) / ratio
	var y = (cy - bcr_top) / ratio
	return [x,y]
}

function on_mouse_down(e) {
	if (e.button!=0) return;
	console.log('left mouse button DOWN',e)
}

function on_mouse_up(e) {
	if (e.button!=0) return;
	console.log('left mouse button UP',e)
}

function on_mouse_move(e) {
	var cx = e.clientX
	var cy = e.clientY
	var xy = scale_mouse_xy(cx,cy)
	var x = xy[0]
	var y = xy[1]
	console.log('mouse move',cx,cy,x,y,e)
}

// ----------------------------------------------------------------------------
