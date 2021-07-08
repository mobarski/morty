function init_screen(width=800, height=400) {	
	var screen = document.getElementById("screen")
	screen.innerHTML = `<canvas id="main_canvas" width="${width}" height="${height}""></canvas>`
	
	out = document.getElementById("output")
	cnv = document.getElementById("main_canvas")
	
	ctx = cnv.getContext("2d")
	ctx.imageSmoothingEnabled = false
	
	get_frame_buffer()
}

function fullscreen() {
	var elem = cnv
	if (elem.requestFullscreen) {
		elem.requestFullscreen()
	} else if (elem.mozRequestFullScreen) { /* Firefox */
		elem.mozRequestFullScreen()
	} else if (elem.webkitRequestFullscreen) { /* Chrome, Safari and Opera */
		elem.webkitRequestFullscreen()
	} else if (elem.msRequestFullscreen) { /* IE/Edge */
		elem.msRequestFullscreen()
	}
}

// ---[ canvas api based ]-----------------------------------------------------

function color(r,g,b,a=1) {
	ctx.fillStyle = `rgb(${r},${g},${b},${a})`
	ctx.strokeStyle = ctx.fillStyle
}

function rect(x,y,w,h) {
	ctx.fillRect(x,y,w,h)
}

function cls() {
	rect(0, 0, ctx.canvas.width, ctx.canvas.height)
}

function pset(x,y) {
	rect(x,y,1,1)
}

// ---[ frame buffer based ]---------------------------------------------------

function get_frame_buffer() {
    image_data = ctx.getImageData(0, 0, ctx.canvas.width, ctx.canvas.height)
    frame_buffer = image_data.data
}

function pseti(i,r,g,b,a=255) {
	frame_buffer[i+0] = r
	frame_buffer[i+1] = g
	frame_buffer[i+2] = b
	frame_buffer[i+3] = a
}

function psetxy(x,y,r,g,b,a=255) {
	i = (y*ctx.canvas.width+x)*4
	frame_buffer[i+0] = r
	frame_buffer[i+1] = g
	frame_buffer[i+2] = b
	frame_buffer[i+3] = a
}

function flip() {
	ctx.putImageData(image_data, 0, 0)
	get_frame_buffer()
}

// ----------------------------------------------------------------------------

init_screen() // XXX
