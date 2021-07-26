
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

// ---[ TOUCH ]----------------------------------------------------------------

scr.touch_pos = -1

function get_mouse_xy(e) {
	var ratio = cnv.clientWidth / scr.w

	var ox = e.offsetX
	var oy = e.offsetY
	var x = ox / ratio
	var y = oy / ratio
	return [x,y,ox,oy]
}

function on_mouse_down(e) {
	if (e.button!=0) return;
	//console.log('left mouse button DOWN',e)
	var xy = get_mouse_xy(e)
	var x = xy[0]
	var y = xy[1]
	var ox = xy[2]
	var oy = xy[3]	
	scr.touch_pos = Math.round(y*scr.w + x)
	console.log('mouse down',ox,oy,x,y,scr.touch_pos)
}

function on_mouse_up(e) {
	if (e.button!=0) return
	//console.log('left mouse button UP',scr.touch_pos)
	scr.touch_pos = -1
}

function on_mouse_move(e) {
	if (scr.touch_pos < 0) return
	var xy = get_mouse_xy(e)
	var x = xy[0]
	var y = xy[1]
	var ox = xy[2]
	var oy = xy[3]	
	scr.touch_pos = Math.round(y*scr.w + x)
	console.log('mouse move',ox,oy,x,y,scr.touch_pos)
}

// ----------------------------------------------------------------------------
